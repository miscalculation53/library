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

from submit_code import preprocessor_lines


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
        if arg in ('-I', '-iquote', '-isystem') and index + 1 < len(flags):
            roots.append(Path(flags[index + 1]).resolve())
            index += 2
            continue
        if arg.startswith('-I') and len(arg) > 2:
            roots.append(Path(arg[2:]).resolve())
        index += 1
    return roots


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
                prefix = (f'#line 1 {json.dumps(str(source), ensure_ascii=False)}\n'
                          + ''.join(lines[:index])
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


def run_checked(command: list[str]) -> None:
    result = subprocess.run(command, check=False)
    if result.returncode:
        raise PrecompileError('プリコンパイルに失敗しました。ヘッダを修正して再実行してください。')


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
    executable = Path(shutil.which(command[0]) or command[0]).resolve()
    compiler_stat = executable.stat()
    context = {
        'compiler': str(executable),
        'compiler_stamp': [compiler_stat.st_size, compiler_stat.st_mtime_ns],
        'flags': flags,
        'header': str(header),
        'environment': {key: os.environ.get(key) for key in (
            'CPATH', 'CPLUS_INCLUDE_PATH', 'GCC_EXEC_PREFIX', 'COMPILER_PATH',
            'SDKROOT', 'MACOSX_DEPLOYMENT_TARGET', 'SOURCE_DATE_EPOCH',
        )},
    }
    key = hashlib.sha256(json.dumps(context, sort_keys=True).encode()).hexdigest()[:20]
    directory = CACHE_ROOT / key
    directory.mkdir(parents=True, exist_ok=True)
    wrapper = directory / 'template.hpp'
    output = directory / 'template.hpp.gch'
    state_path = directory / 'state.json'
    base = [command[0], *flags, '-iquote', str(source.parent)]
    inputs = {**context, 'prefix': prefix, 'cwd': str(Path.cwd())}
    with (directory / 'build.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        try:
            state = json.loads(state_path.read_text())
            stat = output.stat()
            valid = (state['inputs'] == inputs and wrapper.read_text() == prefix
                     and state['output'] == [stat.st_size, stat.st_mtime_ns]
                     and PreparedTemplate(command, state['files']).unchanged())
        except (OSError, ValueError, KeyError, TypeError):
            valid = False
        if not valid:
            print(f'[precompile] {header.name} を更新します', file=sys.stderr)
            output.unlink(missing_ok=True)
            wrapper.write_text(prefix)
            with tempfile.TemporaryDirectory(prefix='build-', dir=directory) as temporary:
                temp = Path(temporary)
                depfile = temp / 'template.d'
                artifact = temp / 'template.gch'
                run_checked([*base, '-x', 'c++', '-M', '-MT', 'pch', '-MF', str(depfile), str(wrapper)])
                files = snapshot(dependencies(depfile))
                run_checked([*base, '-x', 'c++-header',
                             '-MD', '-MT', 'pch', '-MF', str(depfile), str(wrapper), '-o', str(artifact)])
                after = snapshot(dependencies(depfile))
                if files != after:
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
            prepared = PreparedTemplate([command[0], '-iquote', str(source.parent), *replaced], state['files'])
            yield prepared
