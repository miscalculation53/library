"""Keep Code Runner's template precompiled header consistent with its inputs."""
from __future__ import annotations

from contextlib import contextmanager
import fcntl
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

from submit_code import preprocessor_lines, strip_comments


TEMPLATE_DIR = Path(__file__).resolve().parent.parent / 'template'
CACHE_ROOT = TEMPLATE_DIR / '.precompiled'
ENTRY_HEADERS = {'template_all.hpp', 'template_all_but_modint.hpp'}
INCLUDE = re.compile(r'^\s*#\s*include\s*([<"])([^>"\n]+)[>"]\s*$')


class PrecompileError(Exception):
    pass


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def compiler_flags(command: list[str], source: Path) -> list[str]:
    flags: list[str] = []
    index = 1
    while index < len(command):
        arg = command[index]
        if arg in ('-o', '-L', '-l', '-Xlinker', '-MF', '-MT', '-MQ'):
            index += 2
            continue
        if (arg in ('-c', '-S', '-E', '-MD', '-MMD', '-MP')
                or arg.startswith(('-o', '-l', '-L', '-Wl,'))):
            index += 1
            continue
        if not arg.startswith('-') and Path(arg).resolve() == source:
            index += 1
            continue
        flags.append(arg)
        index += 1
    return flags


def include_roots(flags: list[str], source: Path) -> list[Path]:
    roots = [source.parent]
    index = 0
    while index < len(flags):
        arg = flags[index]
        options = ('-I', '-iquote', '-isystem', '-idirafter')
        if arg in options and index + 1 < len(flags):
            roots.append(Path(flags[index + 1]).resolve())
            index += 2
            continue
        for option in options:
            if arg.startswith(option) and len(arg) > len(option):
                roots.append(Path(arg[len(option):]).resolve())
                break
        index += 1
    return roots


def preamble(source: Path, text: str, flags: list[str] | None = None) -> str:
    clean = strip_comments(text)
    lines = [line.strip() for line in clean.splitlines() if line.strip()]
    # Simple macro directives are independent of the source location. Keep the
    # original text and location for includes, continuations and other C++ code.
    if (not re.search(r'\\|__|##', clean)
            and not any('__' in flag or flag.startswith(('-include', '-imacros', '@'))
                        for flag in flags or ())
            and all(re.match(r'^#\s*(define|undef|if|ifdef|ifndef|elif|else|endif)\b', line)
                    for line in lines)):
        return ''.join(line + '\n' for line in lines)
    return f'#line 1 {json.dumps(str(source), ensure_ascii=False)}\n' + text


def template_prefix(source: Path, text: str, flags: list[str]) -> tuple[Path, str, str] | None:
    lines = text.splitlines(keepends=True)
    depth = 0
    continued = False
    for index, line in enumerate(preprocessor_lines(text)):
        if continued:
            continued = line.rstrip().endswith('\\')
            continue
        continued = line.rstrip().endswith('\\')
        if re.match(r'^\s*#\s*(if|ifdef|ifndef)\b', line):
            depth += 1
        elif re.match(r'^\s*#\s*endif\b', line):
            depth -= 1
        match = INCLUDE.match(line.rstrip('\r\n'))
        if depth or not match or Path(match.group(2)).name not in ENTRY_HEADERS:
            continue
        for root in include_roots(flags, source):
            header = (root / match.group(2)).resolve()
            if header.is_file():
                prefix = (preamble(source, ''.join(lines[:index]), flags)
                          + f'#include {json.dumps(str(header), ensure_ascii=False)}\n')
                suffix = (f'#line {index + 2} {json.dumps(str(source), ensure_ascii=False)}\n'
                          + ''.join(lines[index + 1:]))
                return header, prefix, suffix
    return None


def dependencies(path: Path) -> list[Path]:
    rule = path.read_text().replace('\\\n', ' ')
    _, _, names = rule.partition(':')
    words = re.findall(r'(?:\\.|[^\s\\])+', names)
    return sorted({Path(re.sub(r'\\(.)', r'\1', name).replace('$$', '$')).resolve()
                   for name in words})


def snapshot(paths: list[Path]) -> dict[str, dict]:
    return {str(path): {'hash': digest(path), 'mtime': path.stat().st_mtime_ns}
            for path in paths}


class PreparedTemplate:
    def __init__(self, command: list[str], files: dict[str, dict] | None = None):
        self.command = command
        self.files = files or {}

    def unchanged(self) -> bool:
        try:
            return snapshot([Path(name) for name in self.files]) == self.files
        except OSError:
            return False


def run_checked(command: list[str], *, input: str | None = None) -> None:
    result = subprocess.run(command, check=False, input=input, text=True)
    if result.returncode:
        raise PrecompileError('プリコンパイルに失敗しました。ヘッダを修正して再実行してください。')


def rebuild_reason(state_path: Path, wrapper: Path, output: Path,
                   inputs: dict, files: dict[str, dict]) -> str | None:
    if not state_path.exists():
        return '設定に対応するキャッシュ未作成'
    try:
        state = json.loads(state_path.read_text())
        if state['inputs'] != inputs or wrapper.read_text() != inputs['prefix']:
            return 'キャッシュの設定が変更'
        for name, current in files.items():
            if state['files'].get(name, {}).get('hash') != current['hash']:
                return f'参照ヘッダの内容が変更: {Path(name).name}'
        stat = output.stat()
        if state['output'] != [stat.st_size, stat.st_mtime_ns]:
            return 'プリコンパイル生成物が変更'
    except FileNotFoundError:
        return 'キャッシュのファイルが欠落'
    except (OSError, ValueError, KeyError, TypeError, AttributeError):
        return 'キャッシュ情報の再作成'
    return None


@contextmanager
def prepare(source: Path, command: list[str]):
    source = source.resolve()
    # Retire unmanaged artifacts: GCC does not detect edits to their dependencies.
    for name in ENTRY_HEADERS:
        legacy = TEMPLATE_DIR / (name + '.gch')
        if legacy.is_file():
            legacy.unlink()
    flags = compiler_flags(command, source)
    selected = template_prefix(source, source.read_text(), flags)
    if selected is None:
        yield PreparedTemplate(command)
        return
    header, prefix, suffix = selected
    base = [command[0], *flags, '-iquote', str(source.parent)]
    executable = Path(shutil.which(command[0]) or command[0]).resolve()
    compiler_stat = executable.stat()
    context = {
        'compiler': str(executable),
        'compiler_stamp': [compiler_stat.st_size, compiler_stat.st_mtime_ns],
        'flags': flags,
        'header': str(header),
        'prefix': prefix,
        'environment': {key: os.environ.get(key) for key in (
            'CPATH', 'CPLUS_INCLUDE_PATH', 'GCC_EXEC_PREFIX', 'COMPILER_PATH',
            'SDKROOT', 'MACOSX_DEPLOYMENT_TARGET', 'SOURCE_DATE_EPOCH',
        )},
    }
    if prefix.startswith('#line '):
        context['cwd'] = str(Path.cwd())
    # Resolve includes in the current problem, including newly added headers.
    with tempfile.TemporaryDirectory(prefix='pch-dependencies-') as temporary:
        depfile = Path(temporary) / 'template.d'
        run_checked([*base, '-x', 'c++', '-M', '-MT', 'pch', '-MF', str(depfile), '-'],
                    input=prefix)
        paths = dependencies(depfile)
    context['dependencies'] = [str(path) for path in paths]
    key = hashlib.sha256(json.dumps(context, sort_keys=True).encode()).hexdigest()[:20]
    directory = CACHE_ROOT / key
    directory.mkdir(parents=True, exist_ok=True)
    wrapper = directory / 'template.hpp'
    output = directory / 'template.hpp.gch'
    state_path = directory / 'state.json'
    inputs = context
    with (directory / 'build.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        files = snapshot(paths)
        reason = rebuild_reason(state_path, wrapper, output, inputs, files)
        if reason:
            print(f'[precompile] {header.name} を更新します（{reason}）', file=sys.stderr)
            output.unlink(missing_ok=True)
            wrapper.write_text(prefix)
            with tempfile.TemporaryDirectory(prefix='build-', dir=directory) as temporary:
                temp = Path(temporary)
                depfile = temp / 'template.d'
                artifact = temp / 'template.gch'
                run_checked([*base, '-x', 'c++-header',
                             '-MD', '-MT', 'pch', '-MF', str(depfile), str(wrapper), '-o', str(artifact)])
                after = snapshot([path for path in dependencies(depfile) if path != wrapper])
                if files != after or wrapper.read_text() != prefix:
                    raise PrecompileError('プリコンパイル中にヘッダが変更されました。もう一度実行してください。')
                os.replace(artifact, output)
                stat = output.stat()
                state = {'inputs': inputs, 'files': files, 'output': [stat.st_size, stat.st_mtime_ns]}
                metadata = temp / 'state.json'
                metadata.write_text(json.dumps(state, ensure_ascii=False, indent=2) + '\n')
                os.replace(metadata, state_path)
        with tempfile.TemporaryDirectory(prefix='run-cpp-') as temporary:
            candidate = Path(temporary) / source.name
            candidate.write_text(f'#include {json.dumps(str(wrapper), ensure_ascii=False)}\n' + suffix)
            replaced = [str(candidate) if not arg.startswith('-') and Path(arg).resolve() == source else arg
                        for arg in command[1:]]
            prepared = PreparedTemplate([command[0], '-iquote', str(source.parent), *replaced],
                                        {**files, **snapshot([wrapper])})
            yield prepared
