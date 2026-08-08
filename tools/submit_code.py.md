---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: py
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "Traceback (most recent call last):\n  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/opt/hostedtoolcache/Python/3.11.15/x64/lib/python3.11/site-packages/onlinejudge_verify/languages/python.py\"\
    , line 96, in bundle\n    raise NotImplementedError\nNotImplementedError\n"
  code: "#!/usr/bin/env python3\nfrom __future__ import annotations\n\nimport argparse\n\
    from collections import Counter\nfrom concurrent.futures import ThreadPoolExecutor\n\
    from dataclasses import dataclass\nimport fnmatch\nimport hashlib\nimport json\n\
    import os\nfrom pathlib import Path\nimport re\nimport shutil\nimport subprocess\n\
    import sys\nimport tempfile\nimport time\nfrom typing import Iterable, Sequence\n\
    \n\nLINE_DIRECTIVE = re.compile(\n    r'^\\s*#\\s*line\\s+(\\d+)\\s+(\"(?:\\\\\
    .|[^\"\\\\])*\")(?:\\s+.*)?$'\n)\nGNU_LINE_MARKER = re.compile(\n    r'^\\s*#\\\
    s+(\\d+)\\s+(\"(?:\\\\.|[^\"\\\\])*\")(?:\\s+.*)?$'\n)\nINCLUDE_DIRECTIVE = re.compile(r\"\
    ^\\s*#\\s*include\\b\")\nLOCAL_INCLUDE_DIRECTIVE = re.compile(r'^\\s*#\\s*include\\\
    s*\"([^\"]+)\"\\s*$')\nSYSTEM_INCLUDE_DIRECTIVE = re.compile(r\"^\\s*#\\s*include\\\
    s*<([^>]+)>\\s*$\")\nPRAGMA_ONCE_DIRECTIVE = re.compile(r\"^\\s*#\\s*pragma\\\
    s+once\\s*$\")\nCONDITIONAL_OPEN_DIRECTIVE = re.compile(r\"^\\s*#\\s*(?:if|ifdef|ifndef)\\\
    b\")\nCONDITIONAL_BRANCH_DIRECTIVE = re.compile(r\"^\\s*#\\s*(?:else|elif)\\b\"\
    )\nCONDITIONAL_CLOSE_DIRECTIVE = re.compile(r\"^\\s*#\\s*endif\\b\")\nINCLUDE_GUARD_OPEN_DIRECTIVE\
    \ = re.compile(\n    r\"^\\s*#\\s*ifndef\\s+([_A-Za-z]\\w*)\\s*$\"\n)\nIDENTIFIER\
    \ = re.compile(r\"\\b[_A-Za-z]\\w*\\b\")\nDEFAULT_KEEP_FUNCTIONS = {\"read\",\
    \ \"write\", \"print\", \"init\", \"main2\", \"test\"}\nDEFAULT_BYTE_LIMIT = 65536\n\
    \n\nclass ToolError(RuntimeError):\n    pass\n\n\nclass FastBundleUnsupported(ToolError):\n\
    \    pass\n\n\nclass Reporter:\n    COLORS = {\n        \"bold\": \"\\033[1m\"\
    ,\n        \"cyan\": \"\\033[36m\",\n        \"green\": \"\\033[32m\",\n     \
    \   \"yellow\": \"\\033[33m\",\n        \"red\": \"\\033[31m\",\n        \"dim\"\
    : \"\\033[2m\",\n    }\n    RESET = \"\\033[0m\"\n\n    def __init__(\n      \
    \  self,\n        verbose: bool = False,\n        color: str = \"auto\",\n   \
    \     stream=None,\n    ) -> None:\n        self.verbose = verbose\n        self.stream\
    \ = sys.stderr if stream is None else stream\n        if color not in {\"auto\"\
    , \"always\", \"never\"}:\n            raise ValueError(f\"invalid color mode:\
    \ {color}\")\n        self.use_color = color == \"always\" or (\n            color\
    \ == \"auto\"\n            and \"NO_COLOR\" not in os.environ\n            and\
    \ bool(getattr(self.stream, \"isatty\", lambda: False)())\n        )\n\n    def\
    \ _emit(self, tag: str, message: str, color: str | None = None) -> None:\n   \
    \     label = f\"[{tag}]\"\n        if self.use_color and color is not None:\n\
    \            label = f\"{self.COLORS[color]}{label}{self.RESET}\"\n        print(f\"\
    {label} {message}\", file=self.stream, flush=True)\n\n    def byte_count(\n  \
    \      self,\n        value: int,\n        color: str | None = None,\n       \
    \ *,\n        signed: bool = False,\n    ) -> str:\n        text = f\"{value:+,}\
    \ B\" if signed and value != 0 else f\"{value:,} B\"\n        if not self.use_color:\n\
    \            return text\n        color_code = self.COLORS[color] if color is\
    \ not None else \"\"\n        return f\"{self.COLORS['bold']}{color_code}{text}{self.RESET}\"\
    \n\n    def info(self, message: str) -> None:\n        self._emit(\"info\", message)\n\
    \n    def phase(self, stage: str, message: str) -> None:\n        self._emit(stage,\
    \ message, \"cyan\")\n\n    def done(self, stage: str, message: str, started:\
    \ float) -> None:\n        elapsed = time.monotonic() - started\n        self._emit(stage,\
    \ f\"{message} ({elapsed:.2f}s)\", \"green\")\n\n    def warning(self, message:\
    \ str) -> None:\n        self._emit(\"warning\", message, \"yellow\")\n\n    def\
    \ checkpoint(self, message: str) -> None:\n        self._emit(\"checkpoint\",\
    \ message, \"green\")\n\n    def error(self, message: str) -> None:\n        self._emit(\"\
    error\", message, \"red\")\n\n    def detail(self, message: str) -> None:\n  \
    \      if self.verbose:\n            self._emit(\"detail\", message, \"dim\")\n\
    \n    def size(self, stage: str, before: int, after: int) -> None:\n        saved\
    \ = before - after\n        color = \"green\" if saved >= 0 else \"yellow\"\n\
    \        self._emit(\n            \"SIZE\",\n            f\"{stage}: {self.byte_count(before)}\
    \  ->  \"\n            f\"{self.byte_count(after, color)}  \"\n            f\"\
    ({self.byte_count(after - before, color, signed=True)})\",\n            color,\n\
    \        )\n\n\ndef atomic_write(path: Path, text: str) -> None:\n    path.parent.mkdir(parents=True,\
    \ exist_ok=True)\n    mode: int | None\n    try:\n        mode = path.stat().st_mode\n\
    \    except FileNotFoundError:\n        mode = None\n\n    temporary_path: Path\
    \ | None = None\n    try:\n        with tempfile.NamedTemporaryFile(\n       \
    \     mode=\"w\",\n            encoding=\"utf-8\",\n            newline=\"\",\n\
    \            dir=path.parent,\n            prefix=f\".{path.name}.\",\n      \
    \      suffix=\".tmp\",\n            delete=False,\n        ) as temporary:\n\
    \            temporary_path = Path(temporary.name)\n            temporary.write(text)\n\
    \            temporary.flush()\n            os.fsync(temporary.fileno())\n   \
    \     if mode is not None:\n            os.chmod(temporary_path, mode)\n     \
    \   os.replace(temporary_path, path)\n        temporary_path = None\n    finally:\n\
    \        if temporary_path is not None:\n            try:\n                temporary_path.unlink()\n\
    \            except FileNotFoundError:\n                pass\n\n\nclass CheckpointWriter:\n\
    \    def __init__(self, path: Path, reporter: Reporter) -> None:\n        self.path\
    \ = path\n        self.reporter = reporter\n        self.current: str | None =\
    \ None\n        self.preserve_below: int | None = None\n\n    def save(self, stage:\
    \ str, text: str, *, force: bool = False) -> bool:\n        if not force and text\
    \ == self.current:\n            return False\n        if (\n            self.preserve_below\
    \ is not None\n            and self.current is not None\n            and len(self.current.encode())\
    \ < self.preserve_below\n            and len(text.encode()) >= self.preserve_below\n\
    \        ):\n            return False\n        before = len(self.current.encode())\
    \ if self.current is not None else len(text.encode())\n        atomic_write(self.path,\
    \ text)\n        self.current = text\n        self.reporter.size(stage, before,\
    \ len(text.encode()))\n        return True\n\n\ndef safe_decode(data: bytes) ->\
    \ str:\n    return data.decode(\"utf-8\", errors=\"replace\")\n\n\ndef encoded_size(text:\
    \ str) -> int:\n    return len(text.encode())\n\n\ndef below_byte_limit(text:\
    \ str, limit: int | None) -> bool:\n    return limit is not None and encoded_size(text)\
    \ < limit\n\n\ndef redact_home(text: str) -> str:\n    home = str(Path.home())\n\
    \    return text.replace(home, \"~\")\n\n\ndef run_command(\n    command: Sequence[str],\n\
    \    *,\n    cwd: Path | None = None,\n    env: dict[str, str] | None = None,\n\
    \    input_text: str | None = None,\n    timeout: float | None = None,\n) -> subprocess.CompletedProcess[str]:\n\
    \    try:\n        return subprocess.run(\n            list(command),\n      \
    \      cwd=cwd,\n            env=env,\n            input=input_text,\n       \
    \     capture_output=True,\n            text=True,\n            errors=\"replace\"\
    ,\n            timeout=timeout,\n            check=False,\n        )\n    except\
    \ FileNotFoundError as exc:\n        raise ToolError(f\"executable not found:\
    \ {command[0]}\") from exc\n    except subprocess.TimeoutExpired as exc:\n   \
    \     raise ToolError(f\"command timed out: {command[0]}\") from exc\n    except\
    \ OSError as exc:\n        raise ToolError(f\"failed to run {command[0]}: {exc}\"\
    ) from exc\n\n\ndef resolve_executable(value: str | None, candidates: Sequence[str])\
    \ -> str:\n    if value:\n        resolved = shutil.which(value)\n        if resolved\
    \ is None and Path(value).is_file():\n            resolved = str(Path(value).resolve())\n\
    \        if resolved is None:\n            raise ToolError(f\"executable not found:\
    \ {value}\")\n        return resolved\n\n    for candidate in candidates:\n  \
    \      resolved = shutil.which(candidate)\n        if resolved is not None:\n\
    \            return resolved\n    raise ToolError(f\"none of these executables\
    \ were found: {', '.join(candidates)}\")\n\n\ndef discover_oj_bundle(repo_root:\
    \ Path, value: str | None) -> str:\n    if value:\n        return resolve_executable(value,\
    \ ())\n\n    on_path = shutil.which(\"oj-bundle\")\n    if on_path is not None:\n\
    \        return on_path\n\n    candidates = [\n        repo_root / \".venv/bin/oj-bundle\"\
    ,\n        repo_root / \"venv/bin/oj-bundle\",\n        repo_root.parent / \"\
    venv/bin/oj-bundle\",\n    ]\n    for candidate in candidates:\n        if candidate.is_file()\
    \ and os.access(candidate, os.X_OK):\n            return str(candidate.resolve())\n\
    \    raise ToolError(\"oj-bundle was not found; pass --oj-bundle PATH\")\n\n\n\
    def normalize_standard(value: str) -> str:\n    value = value.strip()\n    if\
    \ value.startswith(\"-std=\"):\n        value = value[5:]\n    if value.isdigit():\n\
    \        return f\"c++{value}\"\n    if re.fullmatch(r\"(?:c|gnu)\\+\\+\\d+\"\
    , value):\n        return value\n    raise ToolError(f\"invalid C++ standard:\
    \ {value}\")\n\n\n@dataclass(frozen=True)\nclass Origin:\n    key: str\n    display:\
    \ str\n    resolved: Path | None\n\n\nclass OriginResolver:\n    def __init__(\n\
    \        self,\n        *,\n        base_dir: Path,\n        source: Path,\n \
    \       include_paths: Sequence[Path],\n    ) -> None:\n        self.base_dir\
    \ = base_dir.resolve()\n        self.source = source.resolve()\n        self.home_name\
    \ = Path.home().name\n        self.external_ids: dict[str, int] = {}\n\n     \
    \   roots: list[tuple[str, Path]] = [(\"source\", self.source.parent)]\n     \
    \   seen = {self.source.parent}\n        for include_path in include_paths:\n\
    \            root = include_path.resolve()\n            if root in seen:\n   \
    \             continue\n            seen.add(root)\n            roots.append((f\"\
    include-{len(roots) - 1}\", root))\n        self.roots = roots\n\n    @property\n\
    \    def source_key(self) -> str:\n        return str(self.source)\n\n    def\
    \ _clean_component(self, value: str) -> str:\n        if value == self.home_name:\n\
    \            return \"_user_\"\n        cleaned = re.sub(r\"[^-._A-Za-z0-9]+\"\
    , \"_\", value)\n        return cleaned or \"_\"\n\n    def _clean_relative(self,\
    \ path: Path) -> str:\n        return \"/\".join(self._clean_component(part) for\
    \ part in path.parts)\n\n    def resolve(self, raw_path: str) -> Origin:\n   \
    \     if raw_path.startswith(\"bundle/\"):\n            return Origin(f\"virtual:{raw_path}\"\
    , raw_path, None)\n        if raw_path.startswith(\"<\") and raw_path.endswith(\"\
    >\"):\n            key = raw_path\n            label = self._clean_component(raw_path[1:-1])\n\
    \            return Origin(key, f\"bundle/special/{label}\", None)\n\n       \
    \ path = Path(raw_path).expanduser()\n        if not path.is_absolute():\n   \
    \         path = self.base_dir / path\n        resolved = path.resolve()\n   \
    \     key = str(resolved)\n\n        if resolved == self.source:\n           \
    \ return Origin(key, f\"bundle/source/{self._clean_component(resolved.name)}\"\
    , resolved)\n\n        for label, root in self.roots:\n            try:\n    \
    \            relative = resolved.relative_to(root)\n            except ValueError:\n\
    \                continue\n            return Origin(key, f\"bundle/{label}/{self._clean_relative(relative)}\"\
    , resolved)\n\n        external_id = self.external_ids.setdefault(key, len(self.external_ids))\n\
    \        basename = self._clean_component(resolved.name)\n        return Origin(key,\
    \ f\"bundle/external-{external_id}/{basename}\", resolved)\n\n\n@dataclass(frozen=True)\n\
    class BundledLine:\n    text: str\n    origin_key: str | None\n    origin_display:\
    \ str | None\n    source_line: int\n\n\nclass BundleDocument:\n    def __init__(\n\
    \        self,\n        lines: Sequence[BundledLine],\n        origins: dict[str,\
    \ Origin],\n        protected: Iterable[str],\n    ) -> None:\n        self.lines\
    \ = list(lines)\n        self.origins = origins\n        self.protected = set(protected)\n\
    \n    @classmethod\n    def parse(\n        cls,\n        text: str,\n       \
    \ *,\n        resolver: OriginResolver,\n        protected: Iterable[str] = (),\n\
    \    ) -> \"BundleDocument\":\n        lines: list[BundledLine] = []\n       \
    \ origins: dict[str, Origin] = {}\n        current: Origin | None = None\n   \
    \     source_line = 1\n\n        for line in text.splitlines(keepends=True):\n\
    \            match = LINE_DIRECTIVE.match(line.rstrip(\"\\r\\n\"))\n         \
    \   if match:\n                try:\n                    raw_path = json.loads(match.group(2))\n\
    \                except json.JSONDecodeError as exc:\n                    raise\
    \ ToolError(f\"invalid #line path: {line.rstrip()}\") from exc\n             \
    \   current = resolver.resolve(raw_path)\n                origins[current.key]\
    \ = current\n                source_line = int(match.group(1))\n             \
    \   continue\n\n            lines.append(\n                BundledLine(\n    \
    \                text=line,\n                    origin_key=current.key if current\
    \ else None,\n                    origin_display=current.display if current else\
    \ None,\n                    source_line=source_line,\n                )\n   \
    \         )\n            source_line += 1\n\n        protected_keys = set(protected)\n\
    \        protected_keys.add(resolver.source_key)\n        return cls(lines, origins,\
    \ protected_keys)\n\n    def origin_sizes(self) -> dict[str, int]:\n        sizes:\
    \ dict[str, int] = Counter()\n        for line in self.lines:\n            if\
    \ line.origin_key is not None:\n                sizes[line.origin_key] += len(line.text.encode())\n\
    \        return dict(sizes)\n\n    def origin_first_positions(self) -> dict[str,\
    \ int]:\n        result: dict[str, int] = {}\n        for index, line in enumerate(self.lines):\n\
    \            if line.origin_key is not None:\n                result.setdefault(line.origin_key,\
    \ index)\n        return result\n\n    def render(self, removed: set[str], *,\
    \ keep_line: bool) -> str:\n        output: list[str] = []\n        previous_key:\
    \ str | None = None\n        expected_line: int | None = None\n\n        for line\
    \ in self.lines:\n            if line.origin_key in removed:\n               \
    \ continue\n\n            if keep_line and line.origin_key is not None:\n    \
    \            if line.origin_key != previous_key or line.source_line != expected_line:\n\
    \                    assert line.origin_display is not None\n                \
    \    quoted = json.dumps(line.origin_display, ensure_ascii=True)\n           \
    \         output.append(f\"#line {line.source_line} {quoted}\\n\")\n         \
    \       previous_key = line.origin_key\n                expected_line = line.source_line\
    \ + 1\n            else:\n                previous_key = line.origin_key\n   \
    \             expected_line = line.source_line + 1\n            output.append(line.text)\n\
    \n        return \"\".join(output)\n\n\ndef read_direct_local_includes(\n    source:\
    \ Path,\n    *,\n    include_paths: Sequence[Path],\n    resolver: OriginResolver,\n\
    ) -> set[str]:\n    try:\n        text = strip_comments(source.read_text(encoding=\"\
    utf-8\"))\n    except OSError:\n        return set()\n\n    result: set[str] =\
    \ set()\n    pattern = re.compile(r'^\\s*#\\s*include\\s*([<\"])([^>\"]+)[>\"\
    ]', re.MULTILINE)\n    for match in pattern.finditer(text):\n        opening,\
    \ name = match.groups()\n        search_roots: list[Path] = []\n        if opening\
    \ == '\"':\n            search_roots.append(source.parent)\n        search_roots.extend(include_paths)\n\
    \        for root in search_roots:\n            candidate = (root / name).resolve()\n\
    \            if candidate.is_file():\n                result.add(str(candidate))\n\
    \                break\n    return result\n\n\nclass CompilerWorkspace:\n    def\
    \ __init__(\n        self,\n        *,\n        cxx: str,\n        standard: str,\n\
    \        include_paths: Sequence[Path],\n        extra_args: Sequence[str],\n\
    \        timeout: float | None,\n        reporter: Reporter,\n    ) -> None:\n\
    \        self.cxx = cxx\n        self.standard = standard\n        self.include_paths\
    \ = list(include_paths)\n        self.extra_args = list(extra_args)\n        self.timeout\
    \ = timeout\n        self.reporter = reporter\n        self.temporary = tempfile.TemporaryDirectory(prefix=\"\
    submit-code-\")\n        self.directory = Path(self.temporary.name)\n        self.source\
    \ = self.directory / \"candidate.cpp\"\n        self.analysis_source = self.directory\
    \ / \"analysis.cpp\"\n        self.object = self.directory / \"candidate.o\"\n\
    \        self.executable = self.directory / \"candidate.out\"\n        self.pch_header\
    \ = self.directory / \"standard_prelude.hpp\"\n        self.pch_output = self.directory\
    \ / \"standard_prelude.hpp.gch\"\n        self.pch_available: bool | None = None\n\
    \        self.pch_key: str | None = None\n        self.pch_prefix_end: int | None\
    \ = None\n        self.syntax_cache: dict[str, tuple[bool, str]] = {}\n      \
    \  self.link_cache: dict[str, tuple[bool, str]] = {}\n\n    def close(self) ->\
    \ None:\n        self.temporary.cleanup()\n\n    def __enter__(self) -> \"CompilerWorkspace\"\
    :\n        return self\n\n    def __exit__(self, exc_type, exc_value, traceback)\
    \ -> None:\n        self.close()\n\n    def write(self, text: str) -> None:\n\
    \        self.source.write_text(text, encoding=\"utf-8\", newline=\"\")\n\n  \
    \  def write_analysis_source(self, text: str) -> None:\n        self.analysis_source.write_text(text,\
    \ encoding=\"utf-8\", newline=\"\")\n\n    def _base_command(self) -> list[str]:\n\
    \        command = [self.cxx, f\"-std={self.standard}\", \"-w\"]\n        for\
    \ include_path in self.include_paths:\n            command.extend([\"-I\", str(include_path)])\n\
    \        command.extend(self.extra_args)\n        return command\n\n    def _pch_prefix(self,\
    \ text: str) -> tuple[str, int] | None:\n        include = re.search(r\"^\\s*#\\\
    s*include\\s*<bits/stdc\\+\\+\\.h>\\s*$\", text, re.MULTILINE)\n        if include\
    \ is None:\n            return None\n        prefix = strip_comments(text[: include.start()])\n\
    \        continued = False\n        allowed_directives = {\n            \"define\"\
    ,\n            \"undef\",\n            \"if\",\n            \"ifdef\",\n     \
    \       \"ifndef\",\n            \"elif\",\n            \"else\",\n          \
    \  \"endif\",\n            \"pragma\",\n            \"line\",\n        }\n   \
    \     for line in prefix.splitlines():\n            stripped = line.strip()\n\
    \            if not stripped:\n                continue\n            if continued:\n\
    \                continued = line.rstrip().endswith(\"\\\\\")\n              \
    \  continue\n            match = re.match(r\"^#\\s*([A-Za-z_]\\w*)\", stripped)\n\
    \            if match is None or match.group(1) not in allowed_directives:\n \
    \               return None\n            continued = line.rstrip().endswith(\"\
    \\\\\")\n        if continued:\n            return None\n        return text[:\
    \ include.end()] + \"\\n\", include.end()\n\n    def _ensure_pch(self, text: str)\
    \ -> bool:\n        prefix = self._pch_prefix(text)\n        if prefix is None:\n\
    \            self.pch_prefix_end = None\n            return False\n        if\
    \ any((path / \"bits/stdc++.h\").is_file() for path in self.include_paths):\n\
    \            self.pch_prefix_end = None\n            return False\n        header_text,\
    \ prefix_end = prefix\n\n        try:\n            compiler_stat = Path(self.cxx).stat()\n\
    \            compiler_stamp = [compiler_stat.st_size, compiler_stat.st_mtime_ns]\n\
    \        except OSError:\n            compiler_stamp = [0, 0]\n        cache_key\
    \ = hashlib.sha256(\n            json.dumps(\n                {\n            \
    \        \"compiler\": str(Path(self.cxx).resolve()),\n                    \"\
    compiler_stamp\": compiler_stamp,\n                    \"standard\": self.standard,\n\
    \                    \"extra_args\": self.extra_args,\n                    \"\
    header\": header_text,\n                },\n                sort_keys=True,\n\
    \            ).encode()\n        ).hexdigest()[:20]\n        if self.pch_key ==\
    \ cache_key and self.pch_available is not None:\n            self.pch_prefix_end\
    \ = prefix_end if self.pch_available else None\n            return self.pch_available\n\
    \n        self.pch_key = cache_key\n        self.pch_available = None\n      \
    \  self.pch_prefix_end = None\n        cache_root = Path(\n            os.environ.get(\n\
    \                \"SUBMIT_CODE_CACHE_DIR\",\n                str(Path(tempfile.gettempdir())\
    \ / \"submit-code-cache\"),\n            )\n        )\n        cache_dir = cache_root\
    \ / cache_key\n        cache_dir.mkdir(parents=True, exist_ok=True)\n        self.pch_header\
    \ = cache_dir / \"standard_prelude.hpp\"\n        self.pch_output = cache_dir\
    \ / \"standard_prelude.hpp.gch\"\n        if self.pch_header.is_file() and self.pch_output.is_file():\n\
    \            try:\n                if self.pch_header.read_text(encoding=\"utf-8\"\
    ) == header_text:\n                    self.pch_available = True\n           \
    \         self.pch_prefix_end = prefix_end\n                    return True\n\
    \            except OSError:\n                pass\n\n        atomic_write(self.pch_header,\
    \ header_text)\n        temporary_output = cache_dir / f\".{self.pch_output.name}.{os.getpid()}.tmp\"\
    \n        result = self._run(\n            [\n                self.cxx,\n    \
    \            f\"-std={self.standard}\",\n                \"-w\",\n           \
    \     *self.extra_args,\n                \"-x\",\n                \"c++-header\"\
    ,\n                \"-c\",\n                str(self.pch_header),\n          \
    \      \"-o\",\n                str(temporary_output),\n            ]\n      \
    \  )\n        self.pch_available = result.returncode == 0\n        if self.pch_available:\n\
    \            os.replace(temporary_output, self.pch_output)\n            self.pch_prefix_end\
    \ = prefix_end\n        else:\n            try:\n                temporary_output.unlink()\n\
    \            except FileNotFoundError:\n                pass\n        if not self.pch_available:\n\
    \            self.reporter.detail(\n                f\"standard-library PCH is\
    \ unavailable: {first_error(result.stderr)}\"\n            )\n        return self.pch_available\n\
    \n    def _prepare_candidate_source(self, text: str) -> bool:\n        use_pch\
    \ = self._ensure_pch(text)\n        if use_pch and self.pch_prefix_end is not\
    \ None:\n            prefix = text[: self.pch_prefix_end]\n            masked_prefix\
    \ = \"\".join(\"\\n\" if char == \"\\n\" else \" \" for char in prefix)\n    \
    \        self.write(masked_prefix + text[self.pch_prefix_end :])\n        else:\n\
    \            self.write(text)\n        return use_pch\n\n    def _candidate_command(self,\
    \ use_pch: bool) -> list[str]:\n        command = self._base_command()\n     \
    \   if use_pch:\n            command.extend([\"-include\", str(self.pch_header)])\n\
    \        return command\n\n    def _run(self, command: Sequence[str]) -> subprocess.CompletedProcess[str]:\n\
    \        return run_command(command, timeout=self.timeout)\n\n    def syntax(self,\
    \ text: str) -> tuple[bool, str]:\n        digest = hashlib.sha256(text.encode()).hexdigest()\n\
    \        cached = self.syntax_cache.get(digest)\n        if cached is not None:\n\
    \            return cached\n        use_pch = self._prepare_candidate_source(text)\n\
    \        result = self._run([*self._candidate_command(use_pch), \"-fsyntax-only\"\
    , str(self.source)])\n        value = result.returncode == 0, result.stderr\n\
    \        self.syntax_cache[digest] = value\n        return value\n\n    def link(self,\
    \ text: str) -> tuple[bool, str]:\n        digest = hashlib.sha256(text.encode()).hexdigest()\n\
    \        cached = self.link_cache.get(digest)\n        if cached is not None:\n\
    \            return cached\n        use_pch = self._prepare_candidate_source(text)\n\
    \        for artifact in (self.object, self.executable):\n            try:\n \
    \               artifact.unlink()\n            except FileNotFoundError:\n   \
    \             pass\n\n        if not self.extra_args:\n            result = self._run(\n\
    \                [\n                    *self._candidate_command(use_pch),\n \
    \                   \"-O0\",\n                    str(self.source),\n        \
    \            \"-o\",\n                    str(self.executable),\n            \
    \    ]\n            )\n            value = result.returncode == 0, result.stderr\n\
    \            self.link_cache[digest] = value\n            return value\n\n   \
    \     compile_result = self._run(\n            [\n                *self._candidate_command(use_pch),\n\
    \                \"-O0\",\n                \"-c\",\n                str(self.source),\n\
    \                \"-o\",\n                str(self.object),\n            ]\n \
    \       )\n        if compile_result.returncode != 0:\n            value = False,\
    \ compile_result.stderr\n            self.link_cache[digest] = value\n       \
    \     return value\n\n        link_result = self._run(\n            [\n      \
    \          self.cxx,\n                \"-w\",\n                str(self.object),\n\
    \                \"-o\",\n                str(self.executable),\n            \
    \    *self.extra_args,\n            ]\n        )\n        value = link_result.returncode\
    \ == 0, link_result.stderr\n        self.link_cache[digest] = value\n        return\
    \ value\n\n    def validate(self, text: str) -> tuple[bool, str]:\n        return\
    \ self.link(text)\n\n    def compiler_warnings(self, text: str, *, allow_pch:\
    \ bool = True) -> str:\n        if allow_pch:\n            use_pch = self._prepare_candidate_source(text)\n\
    \        else:\n            self.write(text)\n            use_pch = False\n  \
    \      command = [self.cxx, f\"-std={self.standard}\"]\n        for include_path\
    \ in self.include_paths:\n            command.extend([\"-I\", str(include_path)])\n\
    \        command.extend(self.extra_args)\n        if use_pch:\n            command.extend([\"\
    -include\", str(self.pch_header)])\n        result = self._run(\n            [\n\
    \                *command,\n                \"-Wall\",\n                \"-Wextra\"\
    ,\n                \"-Wunused\",\n                \"-Wunused-macros\",\n     \
    \           \"-c\",\n                str(self.source),\n                \"-o\"\
    ,\n                os.devnull,\n            ]\n        )\n        if result.returncode\
    \ != 0:\n            self.reporter.detail(redact_home(result.stderr))\n      \
    \      return \"\"\n        return result.stderr\n\n    def ipa_unused_functions(self,\
    \ text: str) -> list[tuple[int, str]]:\n        for path in self.directory.glob(\"\
    ipa.*\"):\n            if path.is_file():\n                path.unlink()\n\n \
    \       use_pch = self._prepare_candidate_source(text)\n        started = time.monotonic()\n\
    \        result = self._run(\n            [\n                *self._candidate_command(use_pch),\n\
    \                \"-Og\",\n                \"-fdump-ipa-cgraph\",\n          \
    \      \"-fdump-tree-original-raw\",\n                \"-dumpdir\",\n        \
    \        str(self.directory) + os.sep,\n                \"-dumpbase\",\n     \
    \           \"ipa.\",\n                \"-c\",\n                str(self.source),\n\
    \                \"-o\",\n                str(self.directory / \"ipa.o\"),\n \
    \           ]\n        )\n        if result.returncode != 0:\n            self.reporter.detail(\n\
    \                \"IPA analysis failed: \" + first_error(result.stderr)\n    \
    \        )\n            return []\n\n        cgraph = next(self.directory.glob(\"\
    ipa.*.cgraph\"), None)\n        original = next(self.directory.glob(\"ipa.*.original\"\
    ), None)\n        if cgraph is None or original is None:\n            self.reporter.detail(\"\
    IPA analysis did not produce GCC dump files\")\n            return []\n\n    \
    \    signatures = ipa_removed_signatures(\n            cgraph.read_text(encoding=\"\
    utf-8\", errors=\"replace\")\n        )\n        functions = ipa_function_locations(\n\
    \            original,\n            signatures=signatures,\n            source_name=self.source.name,\n\
    \        )\n        self.reporter.detail(\n            f\"IPA analysis: {len(functions)}\
    \ mapped function(s), \"\n            f\"{time.monotonic() - started:.2f}s\"\n\
    \        )\n        return functions\n\n    def preprocess_directives(self, text:\
    \ str) -> tuple[bool, str, str]:\n        self.write(text)\n        result = self._run(\n\
    \            [\n                self.cxx,\n                f\"-std={self.standard}\"\
    ,\n                \"-E\",\n                \"-fdirectives-only\",\n         \
    \       \"-x\",\n                \"c++\",\n                *(\n              \
    \      argument\n                    for include_path in self.include_paths\n\
    \                    for argument in (\"-I\", str(include_path))\n           \
    \     ),\n                *self.extra_args,\n                str(self.source),\n\
    \            ]\n        )\n        return result.returncode == 0, result.stdout,\
    \ result.stderr\n\n\ndef first_error(stderr: str) -> str:\n    for line in stderr.splitlines():\n\
    \        if \"error:\" in line:\n            return redact_home(line.strip())\n\
    \    for line in stderr.splitlines():\n        if line.strip():\n            return\
    \ redact_home(line.strip())\n    return \"validation failed\"\n\n\ndef split_weighted(items:\
    \ Sequence[str], sizes: dict[str, int]) -> tuple[list[str], list[str]]:\n    if\
    \ len(items) < 2:\n        return list(items), []\n    total = sum(max(1, sizes.get(item,\
    \ 1)) for item in items)\n    left: list[str] = []\n    weight = 0\n    for item\
    \ in items:\n        if left and weight >= total / 2:\n            break\n   \
    \     left.append(item)\n        weight += max(1, sizes.get(item, 1))\n    if\
    \ len(left) == len(items):\n        left = list(items[: len(items) // 2])\n  \
    \  return left, list(items[len(left) :])\n\n\ndef prune_header_origins(\n    document:\
    \ BundleDocument,\n    *,\n    compiler: CompilerWorkspace,\n    writer: CheckpointWriter,\n\
    \    keep_patterns: Sequence[str],\n    keep_line: bool,\n    max_passes: int,\n\
    \    reporter: Reporter,\n) -> set[str]:\n    sizes = document.origin_sizes()\n\
    \    first_positions = document.origin_first_positions()\n    removed: set[str]\
    \ = set()\n    validation_cache: dict[frozenset[str], tuple[bool, str]] = {}\n\
    \n    candidates = []\n    for key, origin in document.origins.items():\n    \
    \    if key in document.protected:\n            continue\n        if any(fnmatch.fnmatch(origin.display,\
    \ pattern) for pattern in keep_patterns):\n            continue\n        candidates.append(key)\n\
    \n    candidates.sort(key=lambda key: first_positions.get(key, -1), reverse=True)\n\
    \n    def try_remove(batch: Sequence[str]) -> bool:\n        nonlocal removed\n\
    \        proposal = frozenset(removed.union(batch))\n        if proposal == frozenset(removed):\n\
    \            return True\n        text = document.render(set(proposal), keep_line=keep_line)\n\
    \        cached = validation_cache.get(proposal)\n        if cached is None:\n\
    \            started = time.monotonic()\n            valid, stderr = compiler.validate(text)\n\
    \            validation_cache[proposal] = valid, stderr\n            reporter.detail(\n\
    \                f\"header trial: remove {len(batch)} file(s), \"\n          \
    \      f\"{time.monotonic() - started:.2f}s, {'accepted' if valid else 'rejected'}\"\
    \n            )\n        else:\n            valid, stderr = cached\n        if\
    \ not valid:\n            reporter.detail(first_error(stderr))\n            return\
    \ False\n\n        removed = set(proposal)\n        writer.save(f\"removed {len(batch)}\
    \ header group(s)\", text)\n        if reporter.verbose:\n            for key\
    \ in batch:\n                reporter.detail(f\"  {document.origins[key].display}\"\
    )\n        return True\n\n    def reduce_batch(items: Sequence[str]) -> None:\n\
    \        active = [item for item in items if item not in removed]\n        if\
    \ not active:\n            return\n        if try_remove(active):\n          \
    \  return\n        if len(active) == 1:\n            return\n        left, right\
    \ = split_weighted(active, sizes)\n        reduce_batch(left)\n        reduce_batch(right)\n\
    \n    for pass_index in range(1, max_passes + 1):\n        before = len(removed)\n\
    \        reduce_batch([candidate for candidate in candidates if candidate not\
    \ in removed])\n        if len(removed) == before:\n            reporter.detail(f\"\
    header pruning converged after pass {pass_index}\")\n            break\n\n   \
    \ return removed\n\n\ndef strip_comments(text: str) -> str:\n    output: list[str]\
    \ = []\n    index = 0\n    size = len(text)\n\n    while index < size:\n     \
    \   if text.startswith(\"//\", index):\n            index += 2\n            while\
    \ index < size:\n                if text[index] == \"\\n\":\n                \
    \    output.append(\"\\n\")\n                    index += 1\n                \
    \    break\n                if text[index] == \"\\\\\" and index + 1 < size and\
    \ text[index + 1] == \"\\n\":\n                    output.append(\"\\n\")\n  \
    \                  index += 2\n                    continue\n                index\
    \ += 1\n            continue\n\n        if text.startswith(\"/*\", index):\n \
    \           output.append(\" \")\n            index += 2\n            while index\
    \ < size and not text.startswith(\"*/\", index):\n                if text[index]\
    \ == \"\\n\":\n                    output.append(\"\\n\")\n                index\
    \ += 1\n            if index < size:\n                index += 2\n           \
    \ continue\n\n        raw_match = re.match(r'(?:u8|u|U|L)?R\"([^ ()\\\\\\t\\r\\\
    n]{0,16})\\(', text[index:])\n        if raw_match:\n            opener = raw_match.group(0)\n\
    \            delimiter = raw_match.group(1)\n            closer = f'){delimiter}\"\
    '\n            end = text.find(closer, index + len(opener))\n            if end\
    \ < 0:\n                output.append(text[index:])\n                break\n \
    \           end += len(closer)\n            output.append(text[index:end])\n \
    \           index = end\n            continue\n\n        ch = text[index]\n  \
    \      if ch in {'\"', \"'\"}:\n            quote = ch\n            start = index\n\
    \            index += 1\n            escaped = False\n            while index\
    \ < size:\n                current = text[index]\n                index += 1\n\
    \                if escaped:\n                    escaped = False\n          \
    \      elif current == \"\\\\\":\n                    escaped = True\n       \
    \         elif current == quote:\n                    break\n            output.append(text[start:index])\n\
    \            continue\n\n        output.append(ch)\n        index += 1\n\n   \
    \ return \"\".join(output)\n\n\ndef preprocessor_lines(text: str) -> list[str]:\n\
    \    \"\"\"Return comment-free lines, masking lines continued from a literal or\
    \ macro.\"\"\"\n\n    lines = strip_comments(text).splitlines(keepends=True)\n\
    \    result: list[str] = []\n    raw_closer: str | None = None\n    quote: str\
    \ | None = None\n    previous_continues = False\n\n    for line in lines:\n  \
    \      protected_at_start = (\n            raw_closer is not None or quote is\
    \ not None or previous_continues\n        )\n        if protected_at_start:\n\
    \            result.append(\"\\n\" if line.endswith(\"\\n\") else \"\")\n    \
    \    else:\n            result.append(line)\n\n        index = 0\n        escaped\
    \ = False\n        while index < len(line):\n            if raw_closer is not\
    \ None:\n                end = line.find(raw_closer, index)\n                if\
    \ end < 0:\n                    index = len(line)\n                    continue\n\
    \                index = end + len(raw_closer)\n                raw_closer = None\n\
    \                continue\n\n            if quote is not None:\n             \
    \   ch = line[index]\n                index += 1\n                if escaped:\n\
    \                    escaped = False\n                elif ch == \"\\\\\":\n \
    \                   escaped = True\n                elif ch == quote:\n      \
    \              quote = None\n                continue\n\n            raw_match\
    \ = re.match(\n                r'(?:u8|u|U|L)?R\"([^ ()\\\\\\t\\r\\n]{0,16})\\\
    (', line[index:]\n            )\n            if raw_match:\n                raw_closer\
    \ = f'){raw_match.group(1)}\"'\n                index += len(raw_match.group(0))\n\
    \                continue\n\n            ch = line[index]\n            if ch in\
    \ {'\"', \"'\"}:\n                quote = ch\n            index += 1\n\n     \
    \   previous_continues = line.rstrip(\"\\r\\n\").endswith(\"\\\\\")\n        if\
    \ quote is not None and not previous_continues:\n            quote = None\n\n\
    \    return result\n\n\ndef collapse_blank_lines(text: str) -> str:\n    output:\
    \ list[str] = []\n    raw_closer: str | None = None\n    quote: str | None = None\n\
    \    previous_continues = False\n    previous_blank = False\n\n    for line in\
    \ text.splitlines(keepends=True):\n        preserve_indentation = (\n        \
    \    raw_closer is not None or quote is not None or previous_continues\n     \
    \   )\n        protected = preserve_indentation\n        index = 0\n        while\
    \ index < len(line):\n            if raw_closer is not None:\n               \
    \ protected = True\n                end = line.find(raw_closer, index)\n     \
    \           if end < 0:\n                    index = len(line)\n             \
    \       continue\n                index = end + len(raw_closer)\n            \
    \    raw_closer = None\n                continue\n\n            if quote is not\
    \ None:\n                protected = True\n                escaped = False\n \
    \               while index < len(line):\n                    ch = line[index]\n\
    \                    index += 1\n                    if escaped:\n           \
    \             escaped = False\n                    elif ch == \"\\\\\":\n    \
    \                    escaped = True\n                    elif ch == quote:\n \
    \                       quote = None\n                        break\n        \
    \        continue\n\n            raw_match = re.match(\n                r'(?:u8|u|U|L)?R\"\
    ([^ ()\\\\\\t\\r\\n]{0,16})\\(',\n                line[index:],\n            )\n\
    \            if raw_match:\n                protected = True\n               \
    \ delimiter = raw_match.group(1)\n                raw_closer = f'){delimiter}\"\
    '\n                index += len(raw_match.group(0))\n                continue\n\
    \n            ch = line[index]\n            if ch in {'\"', \"'\"}:\n        \
    \        protected = True\n                quote = ch\n                index +=\
    \ 1\n                continue\n            index += 1\n\n        previous_continues\
    \ = line.rstrip(\"\\r\\n\").endswith(\"\\\\\")\n        if protected:\n      \
    \      output.append(line)\n            previous_blank = False\n            continue\n\
    \        if line.strip():\n            output.append(line)\n            previous_blank\
    \ = False\n            continue\n        if not previous_blank:\n            output.append(line)\n\
    \            previous_blank = True\n\n    return \"\".join(output)\n\n\ndef wrap_include_directives(text:\
    \ str) -> tuple[str, dict[int, str]]:\n    lines = text.splitlines(keepends=True)\n\
    \    output: list[str] = []\n    includes: dict[int, str] = {}\n    index = 0\n\
    \n    while index < len(lines):\n        line = lines[index]\n        is_line_directive\
    \ = LINE_DIRECTIVE.match(line.rstrip(\"\\r\\n\")) is not None\n        if not\
    \ INCLUDE_DIRECTIVE.match(line) and not is_line_directive:\n            output.append(line)\n\
    \            index += 1\n            continue\n\n        include_lines = [line]\n\
    \        index += 1\n        while include_lines[-1].rstrip(\"\\r\\n\").endswith(\"\
    \\\\\") and index < len(lines):\n            include_lines.append(lines[index])\n\
    \            index += 1\n        include_id = len(includes)\n        includes[include_id]\
    \ = \"\".join(include_lines)\n        output.append(f\"#pragma submit_code_include_begin\
    \ {include_id}\\n\")\n        if not is_line_directive:\n            output.extend(include_lines)\n\
    \        output.append(f\"#pragma submit_code_include_end {include_id}\\n\")\n\
    \n    return \"\".join(output), includes\n\n\ndef filter_preprocessor_output(\n\
    \    output: str,\n    *,\n    source_path: Path,\n    includes: dict[int, str],\n\
    ) -> str:\n    source_resolved = source_path.resolve()\n    current_file: Path\
    \ | None = None\n    result: list[str] = []\n    begin_pattern = re.compile(r\"\
    ^\\s*#pragma\\s+submit_code_include_begin\\s+(\\d+)\\s*$\")\n    end_pattern =\
    \ re.compile(r\"^\\s*#pragma\\s+submit_code_include_end\\s+(\\d+)\\s*$\")\n\n\
    \    for line in output.splitlines(keepends=True):\n        marker = GNU_LINE_MARKER.match(line.rstrip(\"\
    \\r\\n\"))\n        if marker:\n            try:\n                raw_path = json.loads(marker.group(2))\n\
    \            except json.JSONDecodeError:\n                current_file = None\n\
    \                continue\n            if raw_path.startswith(\"<\"):\n      \
    \          current_file = None\n            else:\n                current_file\
    \ = Path(raw_path).resolve()\n            continue\n\n        if current_file\
    \ != source_resolved:\n            continue\n        begin = begin_pattern.match(line)\n\
    \        if begin:\n            include_id = int(begin.group(1))\n           \
    \ result.append(includes[include_id])\n            continue\n        if end_pattern.match(line):\n\
    \            continue\n        result.append(line)\n\n    return \"\".join(result)\n\
    \n\ndef safe_cleanup_candidate(text: str, compiler: CompilerWorkspace) -> str:\n\
    \    uncommented = strip_comments(text)\n    wrapped, includes = wrap_include_directives(uncommented)\n\
    \    valid, output, stderr = compiler.preprocess_directives(wrapped)\n    if not\
    \ valid:\n        raise ToolError(f\"preprocessor failed: {first_error(stderr)}\"\
    )\n    filtered = filter_preprocessor_output(\n        output,\n        source_path=compiler.source,\n\
    \        includes=includes,\n    )\n    return collapse_blank_lines(filtered)\n\
    \n\ndef insert_anonymous_namespace(text: str) -> str:\n    \"\"\"Put bundled declarations\
    \ after the initial include block in an anonymous namespace.\"\"\"\n\n    lines\
    \ = text.splitlines(keepends=True)\n    if not lines:\n        return text\n\n\
    \    first_include = next(\n        (index for index, line in enumerate(lines)\
    \ if INCLUDE_DIRECTIVE.match(line)),\n        None,\n    )\n    include_end =\
    \ 0\n    if first_include is not None:\n        uncommented = strip_comments(text).splitlines()\n\
    \        include_end = first_include\n        continued = False\n        while\
    \ include_end < len(lines):\n            stripped = uncommented[include_end].strip()\n\
    \            if not continued and stripped and not stripped.startswith(\"#\"):\n\
    \                break\n            continued = lines[include_end].rstrip(\"\\\
    r\\n\").endswith(\"\\\\\")\n            include_end += 1\n\n    tokens = structural_tokens(lines)\n\
    \    brace_depth = 0\n    main_line: int | None = None\n    main_pattern = re.compile(r\"\
    ^\\s*(?:[_A-Za-z]\\w*(?:::\\w+)*[\\s*&]+)*main\\s*\\(\")\n    for line_index,\
    \ line in enumerate(lines):\n        if brace_depth == 0 and main_pattern.match(line):\n\
    \            main_line = line_index\n            break\n        if line.lstrip().startswith(\"\
    #\"):\n            continue\n        for token in tokens[line_index]:\n      \
    \      if token == \"{\":\n                brace_depth += 1\n            elif\
    \ token == \"}\":\n                brace_depth = max(0, brace_depth - 1)\n\n \
    \   if main_line is None or include_end > main_line:\n        return text\n\n\
    \    previous = main_line - 1\n    while previous >= 0 and not lines[previous].strip():\n\
    \        previous -= 1\n    if (\n        include_end < len(lines)\n        and\
    \ lines[include_end].strip() == \"namespace {\"\n        and previous >= 0\n \
    \       and lines[previous].strip() == \"}\"\n    ):\n        return text\n\n\
    \    return \"\".join(\n        [\n            *lines[:include_end],\n       \
    \     \"namespace {\\n\",\n            *lines[include_end:main_line],\n      \
    \      \"}\\n\",\n            *lines[main_line:],\n        ]\n    )\n\n\ndef structural_tokens(lines:\
    \ Sequence[str]) -> list[list[str]]:\n    result: list[list[str]] = []\n    in_block_comment\
    \ = False\n    quote: str | None = None\n    escaped = False\n    raw_closer:\
    \ str | None = None\n\n    for line in lines:\n        tokens: list[str] = []\n\
    \        index = 0\n        while index < len(line):\n            if raw_closer\
    \ is not None:\n                end = line.find(raw_closer, index)\n         \
    \       if end < 0:\n                    index = len(line)\n                 \
    \   continue\n                index = end + len(raw_closer)\n                raw_closer\
    \ = None\n                continue\n            if in_block_comment:\n       \
    \         end = line.find(\"*/\", index)\n                if end < 0:\n      \
    \              index = len(line)\n                    continue\n             \
    \   index = end + 2\n                in_block_comment = False\n              \
    \  continue\n\n            ch = line[index]\n            next_ch = line[index\
    \ + 1] if index + 1 < len(line) else \"\"\n            if quote is not None:\n\
    \                if escaped:\n                    escaped = False\n          \
    \      elif ch == \"\\\\\":\n                    escaped = True\n            \
    \    elif ch == quote:\n                    quote = None\n                index\
    \ += 1\n                continue\n            if ch == \"/\" and next_ch == \"\
    /\":\n                break\n            if ch == \"/\" and next_ch == \"*\":\n\
    \                in_block_comment = True\n                index += 2\n       \
    \         continue\n            if ch in {'\"', \"'\"}:\n                quote\
    \ = ch\n                index += 1\n                continue\n            if ch\
    \ == \"R\" and next_ch == '\"':\n                open_paren = line.find(\"(\"\
    , index + 2, index + 19)\n                if open_paren >= 0:\n              \
    \      delimiter = line[index + 2 : open_paren]\n                    if all(c\
    \ not in \" ()\\\\\\t\\r\\n\" for c in delimiter):\n                        raw_closer\
    \ = \")\" + delimiter + '\"'\n                        index = open_paren + 1\n\
    \                        continue\n            if ch in \"()[]{};\":\n       \
    \         tokens.append(ch)\n            index += 1\n        if quote is not None\
    \ and not line.rstrip().endswith(\"\\\\\"):\n            quote = None\n      \
    \      escaped = False\n        result.append(tokens)\n    return result\n\n\n\
    def code_identifiers(text: str) -> Iterable[str]:\n    in_block_comment = False\n\
    \    quote: str | None = None\n    escaped = False\n    raw_closer: str | None\
    \ = None\n    index = 0\n\n    while index < len(text):\n        if raw_closer\
    \ is not None:\n            end = text.find(raw_closer, index)\n            if\
    \ end < 0:\n                return\n            index = end + len(raw_closer)\n\
    \            raw_closer = None\n            continue\n        if in_block_comment:\n\
    \            end = text.find(\"*/\", index)\n            if end < 0:\n       \
    \         return\n            index = end + 2\n            in_block_comment =\
    \ False\n            continue\n\n        ch = text[index]\n        next_ch = text[index\
    \ + 1] if index + 1 < len(text) else \"\"\n        if quote is not None:\n   \
    \         if escaped:\n                escaped = False\n            elif ch ==\
    \ \"\\\\\":\n                escaped = True\n            elif ch == quote:\n \
    \               quote = None\n            index += 1\n            continue\n \
    \       if ch == \"/\" and next_ch == \"/\":\n            newline = text.find(\"\
    \\n\", index + 2)\n            if newline < 0:\n                return\n     \
    \       index = newline + 1\n            continue\n        if ch == \"/\" and\
    \ next_ch == \"*\":\n            in_block_comment = True\n            index +=\
    \ 2\n            continue\n        if ch == \"R\" and next_ch == '\"':\n     \
    \       open_paren = text.find(\"(\", index + 2, index + 19)\n            if open_paren\
    \ >= 0:\n                delimiter = text[index + 2 : open_paren]\n          \
    \      if all(c not in \" ()\\\\\\t\\r\\n\" for c in delimiter):\n           \
    \         raw_closer = \")\" + delimiter + '\"'\n                    index = open_paren\
    \ + 1\n                    continue\n        if ch in {'\"', \"'\"}:\n       \
    \     quote = ch\n            index += 1\n            continue\n        if ch\
    \ == \"_\" or ch.isalpha():\n            end = index + 1\n            while end\
    \ < len(text) and (text[end] == \"_\" or text[end].isalnum()):\n             \
    \   end += 1\n            yield text[index:end]\n            index = end\n   \
    \         continue\n        index += 1\n\n\ndef find_braced_entity_end(tokens:\
    \ Sequence[Sequence[str]], start: int) -> int | None:\n    paren_depth = 0\n \
    \   bracket_depth = 0\n    body_depth = 0\n    body_opened = False\n    for line_index\
    \ in range(start, len(tokens)):\n        for token in tokens[line_index]:\n  \
    \          if body_opened:\n                if token == \"{\":\n             \
    \       body_depth += 1\n                elif token == \"}\":\n              \
    \      body_depth -= 1\n                    if body_depth == 0:\n            \
    \            return line_index\n                continue\n            if token\
    \ == \"(\":\n                paren_depth += 1\n            elif token == \")\"\
    :\n                paren_depth = max(0, paren_depth - 1)\n            elif token\
    \ == \"[\":\n                bracket_depth += 1\n            elif token == \"\
    ]\":\n                bracket_depth = max(0, bracket_depth - 1)\n            elif\
    \ token == \"{\" and paren_depth == 0 and bracket_depth == 0:\n              \
    \  body_opened = True\n                body_depth = 1\n            elif token\
    \ == \";\" and paren_depth == 0 and bracket_depth == 0:\n                return\
    \ None\n    return None\n\n\ndef find_statement_end(tokens: Sequence[Sequence[str]],\
    \ start: int) -> int | None:\n    paren_depth = bracket_depth = brace_depth =\
    \ 0\n    for line_index in range(start, len(tokens)):\n        for token in tokens[line_index]:\n\
    \            if token == \"(\":\n                paren_depth += 1\n          \
    \  elif token == \")\":\n                paren_depth = max(0, paren_depth - 1)\n\
    \            elif token == \"[\":\n                bracket_depth += 1\n      \
    \      elif token == \"]\":\n                bracket_depth = max(0, bracket_depth\
    \ - 1)\n            elif token == \"{\":\n                brace_depth += 1\n \
    \           elif token == \"}\":\n                brace_depth = max(0, brace_depth\
    \ - 1)\n            elif token == \";\" and paren_depth == bracket_depth == brace_depth\
    \ == 0:\n                return line_index\n    return None\n\n\ndef template_prefix_start(lines:\
    \ Sequence[str], start: int) -> int:\n    if start <= 0 or not lines[start - 1].strip().endswith(\"\
    >\"):\n        return start\n    for line_index in range(start - 1, max(-1, start\
    \ - 65), -1):\n        stripped = lines[line_index].strip()\n        if stripped.startswith(\"\
    template\"):\n            return line_index\n        if stripped.endswith(\";\"\
    ) or stripped in {\"{\", \"}\"}:\n            break\n    return start\n\n\ndef\
    \ ipa_removed_signatures(cgraph: str) -> set[str]:\n    if \"Initial Symbol table:\"\
    \ not in cgraph or \"Removing unused symbols:\" not in cgraph:\n        return\
    \ set()\n    initial = cgraph.split(\"Initial Symbol table:\", 1)[1].split(\n\
    \        \"Removing unused symbols:\", 1\n    )[0]\n    signatures: dict[str,\
    \ str] = {}\n    for line in initial.splitlines():\n        match = re.match(r\"\
    ^.+/(\\d+) \\((.*)\\)$\", line)\n        if match is not None:\n            signatures[match.group(1)]\
    \ = match.group(2)\n\n    removed = cgraph.split(\"Removing unused symbols:\"\
    , 1)[1].split(\n        \"Reclaimed Symbol table:\", 1\n    )[0]\n    return {\n\
    \        signatures[symbol_id]\n        for symbol_id in re.findall(r\"/(\\d+)(?=\\\
    s|$)\", removed)\n        if symbol_id in signatures\n    }\n\n\ndef ipa_function_locations(\n\
    \    original: Path,\n    *,\n    signatures: set[str],\n    source_name: str,\n\
    ) -> list[tuple[int, str]]:\n    result: set[tuple[int, str]] = set()\n    signature:\
    \ str | None = None\n    in_function_decl = False\n    saw_function_decl = False\n\
    \    found_decl = False\n    location_pattern = re.compile(\n        rf\"\\bsrcp:\\\
    s+(?:.*[/\\\\])?{re.escape(source_name)}:(\\d+)\"\n    )\n\n    with original.open(encoding=\"\
    utf-8\", errors=\"replace\") as dump:\n        for line in dump:\n           \
    \ if line.startswith(\";; Function \"):\n                header = line.rstrip()[len(\"\
    ;; Function \") :]\n                candidate = header.rsplit(\" (\", 1)[0]\n\
    \                signature = candidate if candidate in signatures else None\n\
    \                in_function_decl = False\n                saw_function_decl =\
    \ False\n                found_decl = False\n                continue\n      \
    \      if signature is None or found_decl:\n                continue\n       \
    \     if line.startswith(\"@\"):\n                is_function_decl = re.match(\n\
    \                    r\"^@\\d+\\s+function_decl\\b\", line\n                )\
    \ is not None\n                in_function_decl = is_function_decl and not saw_function_decl\n\
    \                saw_function_decl = saw_function_decl or is_function_decl\n \
    \           if not in_function_decl:\n                continue\n            match\
    \ = location_pattern.search(line)\n            if match is not None:\n       \
    \         result.add((int(match.group(1)), signature))\n                found_decl\
    \ = True\n    return sorted(result)\n\n\n@dataclass(frozen=True, order=True)\n\
    class Removal:\n    start: int\n    end: int\n    label: str\n\n    @property\n\
    \    def size(self) -> int:\n        return self.end - self.start + 1\n\n\ndef\
    \ removal_from_function_line(\n    lines: Sequence[str], tokens: Sequence[Sequence[str]],\
    \ line_index: int, label: str\n) -> Removal | None:\n    if not 0 <= line_index\
    \ < len(lines):\n        return None\n    end = find_braced_entity_end(tokens,\
    \ line_index)\n    if end is None:\n        return None\n    start = template_prefix_start(lines,\
    \ line_index)\n    return Removal(start, end, label)\n\n\ndef ipa_unused_candidates(\n\
    \    text: str,\n    *,\n    compiler: CompilerWorkspace,\n    keep_functions:\
    \ set[str],\n) -> list[Removal]:\n    functions = compiler.ipa_unused_functions(text)\n\
    \    if not functions:\n        return []\n\n    lines = text.splitlines(keepends=True)\n\
    \    tokens = structural_tokens(lines)\n    names_by_range: dict[tuple[int, int],\
    \ set[str]] = {}\n    for line_number, signature in functions:\n        if \"\
    <lambda\" in signature or \"operator\" in signature:\n            continue\n \
    \       names = re.findall(\n            r\"([_A-Za-z]\\w*)\\s*\\(\", signature.split(\"\
    \ [with \", 1)[0]\n        )\n        if not names:\n            continue\n  \
    \      removal = removal_from_function_line(\n            lines,\n           \
    \ tokens,\n            line_number - 1,\n            \"unused function (IPA)\"\
    ,\n        )\n        if removal is None:\n            continue\n        name\
    \ = names[-1]\n        source = \"\".join(lines[removal.start : removal.end +\
    \ 1])\n        if name not in set(code_identifiers(source)):\n            continue\n\
    \        names_by_range.setdefault((removal.start, removal.end), set()).add(name)\n\
    \n    ranges_by_name: dict[str, set[tuple[int, int]]] = {}\n    for source_range,\
    \ names in names_by_range.items():\n        for name in names:\n            ranges_by_name.setdefault(name,\
    \ set()).add(source_range)\n\n    declarations_by_name: dict[str, set[tuple[int,\
    \ int]]] = {}\n    for line_index, line in enumerate(lines):\n        if any(start\
    \ <= line_index <= end for start, end in names_by_range):\n            continue\n\
    \        for name in ranges_by_name:\n            if re.match(\n             \
    \   rf\"^\\s*(?:template\\s*<[^;{{}}]*>\\s*)?\"\n                rf\"(?:[_A-Za-z]\\\
    w*(?:::[_A-Za-z]\\w*|<[^;{{}}]*>)?[\\s*&]+)+\"\n                rf\"{re.escape(name)}\\\
    s*\\([^;{{}}]*\\)\\s*\"\n                rf\"(?:const\\s*)?(?:noexcept(?:\\([^;{{}}]*\\\
    ))?\\s*)?;\\s*$\",\n                line,\n            ):\n                declarations_by_name.setdefault(name,\
    \ set()).add(\n                    (line_index, line_index)\n                )\n\
    \n    covered = {\n        line_index\n        for start, end in names_by_range\n\
    \        for line_index in range(start, end + 1)\n    }\n    covered.update(\n\
    \        line_index\n        for declarations in declarations_by_name.values()\n\
    \        for start, end in declarations\n        for line_index in range(start,\
    \ end + 1)\n    )\n    outside_names = set(\n        code_identifiers(\n     \
    \       \"\".join(\n                line\n                for line_index, line\
    \ in enumerate(lines)\n                if line_index not in covered\n        \
    \    )\n        )\n    )\n    outside_names.update(keep_functions)\n\n    graph:\
    \ dict[tuple[int, int], set[tuple[int, int]]] = {\n        source_range: set()\
    \ for source_range in names_by_range\n    }\n    for source_range in graph:\n\
    \        start, end = source_range\n        identifiers = set(code_identifiers(\"\
    \".join(lines[start : end + 1])))\n        for name in identifiers:\n        \
    \    graph[source_range].update(ranges_by_name.get(name, ()))\n\n    reachable\
    \ = {\n        source_range\n        for name in outside_names\n        for source_range\
    \ in ranges_by_name.get(name, ())\n    }\n    stack = list(reachable)\n    while\
    \ stack:\n        source_range = stack.pop()\n        for next_range in graph[source_range]:\n\
    \            if next_range not in reachable:\n                reachable.add(next_range)\n\
    \                stack.append(next_range)\n\n    unreachable = graph.keys() -\
    \ reachable\n    candidates = [\n        Removal(start, end, \"unused function\
    \ group (IPA)\")\n        for start, end in unreachable\n    ]\n    for name,\
    \ declarations in declarations_by_name.items():\n        if ranges_by_name[name].issubset(unreachable):\n\
    \            candidates.extend(\n                Removal(start, end, \"unused\
    \ function declaration (IPA)\")\n                for start, end in declarations\n\
    \            )\n    deduplicated: list[Removal] = []\n    for candidate in sorted(candidates,\
    \ key=lambda value: (value.start, -value.end)):\n        if any(\n           \
    \ existing.start <= candidate.start and candidate.end <= existing.end\n      \
    \      for existing in deduplicated\n        ):\n            continue\n      \
    \  deduplicated.append(candidate)\n    return deduplicated\n\n\ndef unused_candidates(\n\
    \    text: str,\n    *,\n    compiler: CompilerWorkspace,\n    cppcheck: str |\
    \ None,\n    keep_functions: set[str],\n    allow_pch: bool = True,\n    reporter:\
    \ Reporter,\n) -> list[Removal]:\n    lines = text.splitlines(keepends=True)\n\
    \    tokens = structural_tokens(lines)\n    removals: set[Removal] = set()\n\n\
    \    cppcheck_result: subprocess.CompletedProcess[str] | None = None\n    if cppcheck\
    \ is None:\n        compiler_output = compiler.compiler_warnings(text, allow_pch=allow_pch)\n\
    \    else:\n        compiler.write_analysis_source(text)\n\n        def run_cppcheck()\
    \ -> subprocess.CompletedProcess[str]:\n            return run_command(\n    \
    \            [\n                    cppcheck,\n                    f\"--std={compiler.standard}\"\
    ,\n                    \"--enable=unusedFunction\",\n                    \"--suppress=missingIncludeSystem\"\
    ,\n                    \"--template=gcc\",\n                    str(compiler.analysis_source),\n\
    \                ],\n                timeout=compiler.timeout,\n            )\n\
    \n        with ThreadPoolExecutor(max_workers=2) as executor:\n            compiler_future\
    \ = executor.submit(\n                compiler.compiler_warnings, text, allow_pch=allow_pch\n\
    \            )\n            cppcheck_future = executor.submit(run_cppcheck)\n\
    \            compiler_output = compiler_future.result()\n            cppcheck_result\
    \ = cppcheck_future.result()\n\n    warning_pattern = re.compile(\n        rf\"\
    ^{re.escape(str(compiler.source))}:(\\d+):\\d+: warning:.*\"\n        r\"\\[-Wunused-(function|macros)\\\
    ]$\"\n    )\n    for warning in compiler_output.splitlines():\n        match =\
    \ warning_pattern.match(warning)\n        if not match:\n            continue\n\
    \        line_index = int(match.group(1)) - 1\n        if match.group(2) == \"\
    macros\":\n            end = line_index\n            while end + 1 < len(lines)\
    \ and lines[end].rstrip().endswith(\"\\\\\"):\n                end += 1\n    \
    \        removals.add(Removal(line_index, end, \"unused macro\"))\n        else:\n\
    \            removal = removal_from_function_line(\n                lines, tokens,\
    \ line_index, \"unused function (compiler)\"\n            )\n            if removal\
    \ is not None:\n                removals.add(removal)\n\n    if cppcheck_result\
    \ is not None:\n        cppcheck_pattern = re.compile(\n            rf\"^{re.escape(str(compiler.analysis_source))}:(\\\
    d+):\\d+:.*\\[unusedFunction\\]$\"\n        )\n        function_name = re.compile(r\"\
    (?:function|Function) ['\\\"]([^'\\\"]+)['\\\"]\")\n        for warning in cppcheck_result.stderr.splitlines():\n\
    \            match = cppcheck_pattern.match(warning)\n            if not match:\n\
    \                continue\n            name_match = function_name.search(warning)\n\
    \            if name_match and name_match.group(1) in keep_functions:\n      \
    \          continue\n            line_index = int(match.group(1)) - 1\n      \
    \      removal = removal_from_function_line(\n                lines, tokens, line_index,\
    \ \"unused function (cppcheck)\"\n            )\n            if removal is not\
    \ None:\n                removals.add(removal)\n        if cppcheck_result.returncode\
    \ not in {0, 1}:\n            reporter.detail(redact_home(cppcheck_result.stderr))\n\
    \n    counts = Counter(code_identifiers(text))\n    macro_pattern = re.compile(r\"\
    ^\\s*#\\s*define\\s+([_A-Za-z]\\w*)\\b\")\n    struct_pattern = re.compile(r\"\
    ^\\s*struct\\s+([_A-Za-z]\\w*)\\b\")\n    using_pattern = re.compile(r\"^\\s*using\\\
    s+([_A-Za-z]\\w*)\\s*=\")\n    for line_index, line in enumerate(lines):\n   \
    \     macro_match = macro_pattern.match(line)\n        if macro_match and counts[macro_match.group(1)]\
    \ == 1:\n            end = line_index\n            while end + 1 < len(lines)\
    \ and lines[end].rstrip().endswith(\"\\\\\"):\n                end += 1\n    \
    \        removals.add(Removal(line_index, end, \"unused macro (text scan)\"))\n\
    \        struct_match = struct_pattern.match(line)\n        if struct_match and\
    \ counts[struct_match.group(1)] == 1:\n            end = find_braced_entity_end(tokens,\
    \ line_index)\n            if end is not None:\n                removals.add(\n\
    \                    Removal(\n                        template_prefix_start(lines,\
    \ line_index),\n                        end,\n                        f\"unused\
    \ struct {struct_match.group(1)}\",\n                    )\n                )\n\
    \        using_match = using_pattern.match(line)\n        if using_match and counts[using_match.group(1)]\
    \ == 1:\n            end = find_statement_end(tokens, line_index)\n          \
    \  if end is not None:\n                removals.add(\n                    Removal(\n\
    \                        template_prefix_start(lines, line_index),\n         \
    \               end,\n                        f\"unused using {using_match.group(1)}\"\
    ,\n                    )\n                )\n\n    ordered = sorted(removals)\n\
    \    deduplicated: list[Removal] = []\n    for removal in ordered:\n        if\
    \ any(\n            existing.start <= removal.start and removal.end <= existing.end\n\
    \            for existing in deduplicated\n        ):\n            continue\n\
    \        deduplicated.append(removal)\n    return deduplicated\n\n\ndef apply_removals(text:\
    \ str, removals: Iterable[Removal]) -> str:\n    lines = text.splitlines(keepends=True)\n\
    \    use = [True] * len(lines)\n    for removal in removals:\n        for line_index\
    \ in range(max(0, removal.start), min(len(lines), removal.end + 1)):\n       \
    \     use[line_index] = False\n    return \"\".join(line for line_index, line\
    \ in enumerate(lines) if use[line_index])\n\n\ndef reduce_unused_candidates(\n\
    \    text: str,\n    candidates: Sequence[Removal],\n    *,\n    compiler: CompilerWorkspace,\n\
    \    writer: CheckpointWriter,\n    reporter: Reporter,\n    target_bytes: int\
    \ | None = None,\n    stage: str = \"cleanup\",\n) -> tuple[str, int]:\n    started\
    \ = time.monotonic()\n    accepted: set[Removal] = set()\n    cache: dict[frozenset[Removal],\
    \ tuple[bool, str]] = {}\n    validations = 0\n    target_checkpointed = below_byte_limit(text,\
    \ target_bytes) or (\n        writer.current is not None\n        and below_byte_limit(writer.current,\
    \ target_bytes)\n    )\n\n    byte_sizes = {\n        candidate: len(\n      \
    \      \"\".join(text.splitlines(keepends=True)[candidate.start : candidate.end\
    \ + 1]).encode()\n        )\n        for candidate in candidates\n    }\n\n  \
    \  def split(items: Sequence[Removal]) -> tuple[list[Removal], list[Removal]]:\n\
    \        keys = {str(index): byte_sizes[item] for index, item in enumerate(items)}\n\
    \        left_keys, _ = split_weighted(list(keys), keys)\n        count = len(left_keys)\n\
    \        return list(items[:count]), list(items[count:])\n\n    def try_accept(batch:\
    \ Sequence[Removal]) -> bool:\n        nonlocal text, target_checkpointed, validations\n\
    \        proposal = frozenset(accepted.union(batch))\n        if proposal == frozenset(accepted):\n\
    \            return True\n        candidate_text = collapse_blank_lines(\n   \
    \         apply_removals(original_text, proposal)\n        )\n        cached =\
    \ cache.get(proposal)\n        if cached is None:\n            validations +=\
    \ 1\n            reporter.phase(\n                stage,\n                f\"\
    validation {validations}: testing {len(batch)} candidate(s)\",\n            )\n\
    \            valid, stderr = compiler.validate(candidate_text)\n            cache[proposal]\
    \ = valid, stderr\n        else:\n            valid, stderr = cached\n       \
    \ if not valid:\n            reporter.detail(first_error(stderr))\n          \
    \  return False\n        accepted.update(batch)\n        text = candidate_text\n\
    \        writer.save(f\"removed {len(batch)} unused declaration(s)\", text)\n\
    \        if not target_checkpointed and below_byte_limit(text, target_bytes):\n\
    \            target_checkpointed = True\n            reporter.checkpoint(\n  \
    \              f\"submission ready: {reporter.byte_count(encoded_size(text), 'green')}\
    \ \"\n                f\"< {reporter.byte_count(target_bytes, 'yellow')}; cleanup\
    \ continues\"\n            )\n        if reporter.verbose:\n            for candidate\
    \ in batch:\n                reporter.detail(\n                    f\"  lines\
    \ {candidate.start + 1}-{candidate.end + 1}: {candidate.label}\"\n           \
    \     )\n        return True\n\n    def reduce_batch(items: Sequence[Removal])\
    \ -> None:\n        active = [item for item in items if item not in accepted]\n\
    \        if not active:\n            return\n        if try_accept(active):\n\
    \            return\n        if len(active) == 1:\n            return\n      \
    \  left, right = split(active)\n        reduce_batch(left)\n        reduce_batch(right)\n\
    \n    original_text = text\n    reduce_batch(candidates)\n    reporter.done(\n\
    \        stage,\n        f\"accepted {len(accepted)}/{len(candidates)} candidate(s)\"\
    ,\n        started,\n    )\n    return text, len(accepted)\n\n\ndef cleanup_text(\n\
    \    text: str,\n    *,\n    compiler: CompilerWorkspace,\n    writer: CheckpointWriter,\n\
    \    cppcheck: str | None,\n    keep_functions: set[str],\n    max_passes: int,\n\
    \    safe_only: bool,\n    require_validation: bool,\n    reporter: Reporter,\n\
    \    target_bytes: int | None = None,\n) -> str:\n    target_checkpointed = below_byte_limit(text,\
    \ target_bytes) or (\n        writer.current is not None\n        and below_byte_limit(writer.current,\
    \ target_bytes)\n    )\n\n    def report_target_checkpoint() -> None:\n      \
    \  nonlocal target_checkpointed\n        if target_checkpointed or not below_byte_limit(text,\
    \ target_bytes):\n            return\n        target_checkpointed = True\n   \
    \     reporter.checkpoint(\n            f\"submission ready: {reporter.byte_count(encoded_size(text),\
    \ 'green')} \"\n            f\"< {reporter.byte_count(target_bytes, 'yellow')};\
    \ cleanup continues\"\n        )\n\n    if target_bytes is not None:\n       \
    \ quick_candidate = collapse_blank_lines(strip_comments(text))\n        if below_byte_limit(quick_candidate,\
    \ target_bytes):\n            started = time.monotonic()\n            reporter.phase(\"\
    cleanup\", \"validating a quick comment-only checkpoint\")\n            valid,\
    \ stderr = compiler.syntax(quick_candidate)\n            if valid:\n         \
    \       if quick_candidate != text:\n                    text = quick_candidate\n\
    \                    writer.save(\"comments and blank lines\", text)\n       \
    \         report_target_checkpoint()\n            else:\n                reporter.detail(\n\
    \                    \"quick cleanup was rejected: \" + first_error(stderr)\n\
    \                )\n            reporter.done(\"cleanup\", \"quick checkpoint\
    \ checked\", started)\n\n    started = time.monotonic()\n    reporter.phase(\"\
    cleanup\", \"processing comments and conditional branches\")\n    safe_candidate\
    \ = safe_cleanup_candidate(text, compiler)\n    if below_byte_limit(safe_candidate,\
    \ target_bytes):\n        if safe_candidate != text or require_validation:\n \
    \           valid, stderr = compiler.syntax(safe_candidate)\n            if not\
    \ valid:\n                raise ToolError(\n                    f\"safe cleanup\
    \ did not validate: {first_error(stderr)}\"\n                )\n        if safe_candidate\
    \ != text:\n            text = safe_candidate\n            writer.save(\"comments,\
    \ inactive branches, and blank lines\", text)\n        report_target_checkpoint()\n\
    \    reporter.done(\n        \"cleanup\",\n        f\"preprocessing complete:\
    \ \"\n        f\"{reporter.byte_count(encoded_size(safe_candidate), 'cyan')}\"\
    ,\n        started,\n    )\n\n    if safe_only:\n        if safe_candidate !=\
    \ text:\n            valid, stderr = compiler.validate(safe_candidate)\n     \
    \       if not valid:\n                raise ToolError(f\"safe cleanup did not\
    \ validate: {first_error(stderr)}\")\n            text = safe_candidate\n    \
    \        writer.save(\"comments, inactive branches, and blank lines\", text)\n\
    \        elif require_validation:\n            valid, stderr = compiler.validate(text)\n\
    \            if not valid:\n                raise ToolError(f\"the input file\
    \ does not compile: {first_error(stderr)}\")\n        return text\n\n    safe_changed\
    \ = safe_candidate != text\n    text = safe_candidate\n    pending_validation\
    \ = safe_changed or require_validation\n\n    started = time.monotonic()\n   \
    \ reporter.phase(\"namespace\", \"checking anonymous namespace insertion\")\n\
    \    namespace_candidate = insert_anonymous_namespace(text)\n    if namespace_candidate\
    \ != text:\n        valid, stderr = compiler.validate(namespace_candidate)\n \
    \       if valid:\n            text = namespace_candidate\n            writer.save(\"\
    anonymous namespace\", text)\n            pending_validation = False\n       \
    \ else:\n            reporter.detail(\n                \"anonymous namespace was\
    \ not inserted: \" + first_error(stderr)\n            )\n    reporter.done(\"\
    namespace\", \"anonymous namespace check complete\", started)\n\n    def validate_pending()\
    \ -> None:\n        nonlocal pending_validation\n        if not pending_validation:\n\
    \            return\n        valid, stderr = compiler.validate(text)\n       \
    \ if not valid:\n            label = \"safe cleanup\" if safe_changed else \"\
    the input file\"\n            raise ToolError(f\"{label} does not compile: {first_error(stderr)}\"\
    )\n        if safe_changed:\n            writer.save(\"comments, inactive branches,\
    \ and blank lines\", text)\n        pending_validation = False\n\n    def run_unused_pass(\n\
    \        analysis_cppcheck: str | None,\n        stage: str,\n    ) -> tuple[bool,\
    \ int]:\n        nonlocal text, pending_validation\n        started = time.monotonic()\n\
    \        analyzers = \"GCC\" + (\" + cppcheck\" if analysis_cppcheck else \"\"\
    )\n        reporter.phase(stage, f\"finding unused declarations with {analyzers}\"\
    )\n        candidates = unused_candidates(\n            text,\n            compiler=compiler,\n\
    \            cppcheck=analysis_cppcheck,\n            keep_functions=keep_functions,\n\
    \            allow_pch=not pending_validation,\n            reporter=reporter,\n\
    \        )\n        reporter.done(\n            stage,\n            f\"found {len(candidates)}\
    \ candidate(s)\",\n            started,\n        )\n        if not candidates:\n\
    \            validate_pending()\n            return False, 0\n        text, accepted\
    \ = reduce_unused_candidates(\n            text,\n            candidates,\n  \
    \          compiler=compiler,\n            writer=writer,\n            reporter=reporter,\n\
    \            target_bytes=target_bytes,\n            stage=stage,\n        )\n\
    \        if accepted == 0:\n            validate_pending()\n        else:\n  \
    \          pending_validation = False\n        return True, accepted\n\n    for\
    \ pass_index in range(1, max_passes + 1):\n        stage = f\"pass {pass_index}/{max_passes}\"\
    \n        had_candidates, accepted = run_unused_pass(cppcheck, stage)\n      \
    \  if not had_candidates:\n            reporter.detail(f\"unused cleanup converged\
    \ after pass {pass_index}\")\n            break\n        if accepted == 0:\n \
    \           reporter.detail(f\"no unused candidate was accepted in pass {pass_index}\"\
    )\n            break\n    validate_pending()\n\n    started = time.monotonic()\n\
    \    reporter.phase(\"IPA\", \"finding unreachable recursive function groups\"\
    )\n    ipa_candidates = ipa_unused_candidates(\n        text,\n        compiler=compiler,\n\
    \        keep_functions=keep_functions,\n    )\n    reporter.done(\n        \"\
    IPA\",\n        f\"found {len(ipa_candidates)} candidate(s)\",\n        started,\n\
    \    )\n    if ipa_candidates:\n        text, accepted = reduce_unused_candidates(\n\
    \            text,\n            ipa_candidates,\n            compiler=compiler,\n\
    \            writer=writer,\n            reporter=reporter,\n            target_bytes=target_bytes,\n\
    \            stage=\"IPA\",\n        )\n        reporter.detail(\n           \
    \ f\"IPA cleanup accepted {accepted}/{len(ipa_candidates)} candidate(s)\"\n  \
    \      )\n    return text\n\n\ndef default_include_paths(repo_root: Path, source:\
    \ Path) -> list[Path]:\n    candidates = [\n        source.parent,\n        repo_root,\n\
    \        repo_root.parent / \"cpp-dump-main\",\n        repo_root.parent.parent\
    \ / \"ac-library\",\n    ]\n    result: list[Path] = []\n    seen: set[Path] =\
    \ set()\n    for candidate in candidates:\n        resolved = candidate.resolve()\n\
    \        if resolved in seen or not resolved.is_dir():\n            continue\n\
    \        seen.add(resolved)\n        result.append(resolved)\n    return result\n\
    \n\ndef merge_include_paths(defaults: Sequence[Path], provided: Sequence[str])\
    \ -> list[Path]:\n    result: list[Path] = []\n    seen: set[Path] = set()\n \
    \   for path in [*defaults, *(Path(value) for value in provided)]:\n        resolved\
    \ = path.expanduser().resolve()\n        if resolved in seen:\n            continue\n\
    \        if not resolved.is_dir():\n            raise ToolError(f\"include directory\
    \ does not exist: {path}\")\n        seen.add(resolved)\n        result.append(resolved)\n\
    \    return result\n\n\nclass FastBundler:\n    \"\"\"Expand ordinary local includes\
    \ without invoking a compiler for every file.\"\"\"\n\n    def __init__(self,\
    \ include_paths: Sequence[Path]) -> None:\n        self.include_paths = [path.resolve()\
    \ for path in include_paths]\n        self.once: set[Path] = set()\n        self.system_includes:\
    \ set[str] = set()\n        self.stack: list[Path] = []\n\n    def _resolve(self,\
    \ name: str, included_from: Path) -> Path:\n        for root in [included_from.parent,\
    \ *self.include_paths]:\n            candidate = (root / name).resolve()\n   \
    \         if candidate.is_file():\n                return candidate\n        raise\
    \ FastBundleUnsupported(f'local include was not found: \"{name}\"')\n\n    @staticmethod\n\
    \    def _include_guard_lines(lines: Sequence[str]) -> set[int]:\n        meaningful\
    \ = [index for index, line in enumerate(lines) if line.strip()]\n        if len(meaningful)\
    \ < 3:\n            return set()\n\n        first, define, last = meaningful[0],\
    \ meaningful[1], meaningful[-1]\n        match = INCLUDE_GUARD_OPEN_DIRECTIVE.match(lines[first].rstrip(\"\
    \\r\\n\"))\n        if match is None:\n            return set()\n        name\
    \ = match.group(1)\n        if re.match(\n            rf\"^\\s*#\\s*define\\s+{re.escape(name)}(?:\\\
    s+.*)?$\",\n            lines[define].rstrip(\"\\r\\n\"),\n        ) is None:\n\
    \            return set()\n        if CONDITIONAL_CLOSE_DIRECTIVE.match(lines[last].rstrip(\"\
    \\r\\n\")) is None:\n            return set()\n\n        depth = 0\n        for\
    \ index in range(first, last + 1):\n            directive = lines[index].rstrip(\"\
    \\r\\n\")\n            if CONDITIONAL_OPEN_DIRECTIVE.match(directive):\n     \
    \           depth += 1\n            elif CONDITIONAL_BRANCH_DIRECTIVE.match(directive)\
    \ and depth == 1:\n                return set()\n            elif CONDITIONAL_CLOSE_DIRECTIVE.match(directive):\n\
    \                depth -= 1\n                if depth < 0 or (depth == 0 and index\
    \ != last):\n                    return set()\n        if depth != 0:\n      \
    \      return set()\n        return {first, define, last}\n\n    def _expand(self,\
    \ path: Path) -> str:\n        path = path.resolve()\n        try:\n         \
    \   text = path.read_text(encoding=\"utf-8\")\n        except (OSError, UnicodeError)\
    \ as exc:\n            raise FastBundleUnsupported(f\"could not read {path.name}:\
    \ {exc}\") from exc\n        if not text.endswith(\"\\n\"):\n            text\
    \ += \"\\n\"\n\n        lines = text.splitlines(keepends=True)\n        uncommented_lines\
    \ = preprocessor_lines(text)\n        if len(lines) != len(uncommented_lines):\n\
    \            raise FastBundleUnsupported(\n                f\"comment parsing\
    \ changed the line count in {path.name}\"\n            )\n\n        has_pragma_once\
    \ = any(\n            PRAGMA_ONCE_DIRECTIVE.match(line.rstrip(\"\\r\\n\"))\n \
    \           for line in uncommented_lines\n        )\n        include_guard_lines\
    \ = self._include_guard_lines(uncommented_lines)\n        guarded_once = has_pragma_once\
    \ or bool(include_guard_lines)\n        if guarded_once and path in self.once:\n\
    \            return \"\"\n        if guarded_once:\n            self.once.add(path)\n\
    \        if path in self.stack:\n            chain = \" -> \".join(item.name for\
    \ item in [*self.stack, path])\n            raise FastBundleUnsupported(f\"include\
    \ cycle without #pragma once: {chain}\")\n\n        self.stack.append(path)\n\
    \        output: list[str] = []\n        conditional_depth = 0\n        try:\n\
    \            for line_index, (line, uncommented_line) in enumerate(\n        \
    \        zip(lines, uncommented_lines)\n            ):\n                directive\
    \ = uncommented_line.rstrip(\"\\r\\n\")\n\n                if (\n            \
    \        PRAGMA_ONCE_DIRECTIVE.match(directive)\n                    or line_index\
    \ in include_guard_lines\n                ):\n                    continue\n\n\
    \                local_include = LOCAL_INCLUDE_DIRECTIVE.match(directive)\n  \
    \              if local_include:\n                    if conditional_depth !=\
    \ 0:\n                        raise FastBundleUnsupported(\n                 \
    \           f\"conditional local include in {path.name}\"\n                  \
    \      )\n                    included = self._resolve(local_include.group(1),\
    \ path)\n                    output.append(self._expand(included))\n         \
    \           continue\n\n                if INCLUDE_DIRECTIVE.match(directive)\
    \ and '\"' in directive:\n                    raise FastBundleUnsupported(\n \
    \                       f\"unsupported local include syntax in {path.name}\"\n\
    \                    )\n\n                system_include = SYSTEM_INCLUDE_DIRECTIVE.match(directive)\n\
    \                if system_include and conditional_depth == 0:\n             \
    \       name = system_include.group(1)\n                    if name in self.system_includes:\n\
    \                        continue\n                    self.system_includes.add(name)\n\
    \n                if re.match(r'^\\s*#\\s*(?:line\\s+)?\\d+\\s+\"', directive):\n\
    \                    continue\n\n                output.append(line)\n       \
    \         if CONDITIONAL_OPEN_DIRECTIVE.match(directive):\n                  \
    \  conditional_depth += 1\n                elif CONDITIONAL_CLOSE_DIRECTIVE.match(directive):\n\
    \                    conditional_depth -= 1\n                    if conditional_depth\
    \ < 0:\n                        raise FastBundleUnsupported(\n               \
    \             f\"unmatched #endif in {path.name}\"\n                        )\n\
    \n            if conditional_depth != 0:\n                raise FastBundleUnsupported(f\"\
    unmatched conditional in {path.name}\")\n            return \"\".join(output)\n\
    \        finally:\n            self.stack.pop()\n\n    def bundle(self, source:\
    \ Path) -> str:\n        return self._expand(source)\n\n\ndef remove_line_directives(text:\
    \ str) -> str:\n    return \"\".join(\n        line\n        for line in text.splitlines(keepends=True)\n\
    \        if LINE_DIRECTIVE.match(line.rstrip(\"\\r\\n\")) is None\n    )\n\n\n\
    def run_oj_bundle(\n    *,\n    executable: str,\n    source: Path,\n    include_paths:\
    \ Sequence[Path],\n    cxx: str,\n) -> str:\n    command = [executable]\n    for\
    \ include_path in include_paths:\n        command.extend([\"-I\", str(include_path)])\n\
    \    command.append(source.name)\n    env = os.environ.copy()\n    env[\"CXX\"\
    ] = cxx\n    result = run_command(command, cwd=source.parent, env=env)\n    if\
    \ result.returncode != 0:\n        raise ToolError(f\"oj-bundle failed:\\n{redact_home(result.stderr).rstrip()}\"\
    )\n    return result.stdout\n\n\ndef privacy_check_line_directives(text: str)\
    \ -> None:\n    home = str(Path.home())\n    user = Path.home().name\n    for\
    \ line in text.splitlines():\n        if not LINE_DIRECTIVE.match(line):\n   \
    \         continue\n        if home in line or f\"/{user}/\" in line:\n      \
    \      raise ToolError(\"a #line directive contains private home-directory information\"\
    )\n\n\ndef common_compiler_from_args(\n    args: argparse.Namespace,\n    *,\n\
    \    include_paths: Sequence[Path],\n    reporter: Reporter,\n) -> CompilerWorkspace:\n\
    \    cxx = resolve_executable(args.cxx, (\"g++-15\", \"g++-14\", \"g++\"))\n \
    \   timeout = None if args.timeout == 0 else args.timeout\n    return CompilerWorkspace(\n\
    \        cxx=cxx,\n        standard=normalize_standard(args.std),\n        include_paths=include_paths,\n\
    \        extra_args=args.cxx_arg,\n        timeout=timeout,\n        reporter=reporter,\n\
    \    )\n\n\ndef expand_command(args: argparse.Namespace, repo_root: Path) -> None:\n\
    \    reporter = Reporter(args.verbose, args.color)\n    source = Path(args.source).expanduser().resolve()\n\
    \    output = Path(args.output).expanduser().resolve()\n    if not source.is_file():\n\
    \        raise ToolError(f\"source file does not exist: {source}\")\n    if source\
    \ == output:\n        raise ToolError(\"bundle output must differ from the source\
    \ file\")\n\n    include_paths = merge_include_paths(\n        default_include_paths(repo_root,\
    \ source), args.include\n    )\n    started = time.monotonic()\n    method = \"\
    fast\"\n    reporter.phase(\"bundle\", \"expanding local includes\")\n    try:\n\
    \        bundled = FastBundler(include_paths).bundle(source)\n    except FastBundleUnsupported\
    \ as exc:\n        if args.no_fallback:\n            raise\n        method = \"\
    oj-bundle fallback\"\n        reporter.warning(f\"fast expansion unsupported:\
    \ {exc}\")\n        reporter.phase(\"bundle\", \"running oj-bundle compatibility\
    \ fallback\")\n        cxx = resolve_executable(args.cxx, (\"g++-15\", \"g++-14\"\
    , \"g++\"))\n        oj_bundle = discover_oj_bundle(repo_root, args.oj_bundle)\n\
    \        bundled = remove_line_directives(\n            run_oj_bundle(\n     \
    \           executable=oj_bundle,\n                source=source,\n          \
    \      include_paths=include_paths,\n                cxx=cxx,\n            )\n\
    \        )\n\n    privacy_check_line_directives(bundled)\n    atomic_write(output,\
    \ bundled)\n    reporter.done(\n        \"bundle\",\n        f\"{method}: {reporter.byte_count(len(bundled.encode()),\
    \ 'green')}\",\n        started,\n    )\n\n\ndef bundle_command(args: argparse.Namespace,\
    \ repo_root: Path) -> None:\n    reporter = Reporter(args.verbose, args.color)\n\
    \    source = Path(args.source).expanduser().resolve()\n    output = Path(args.output).expanduser().resolve()\n\
    \    if not source.is_file():\n        raise ToolError(f\"source file does not\
    \ exist: {source}\")\n    if source == output:\n        raise ToolError(\"bundle\
    \ output must differ from the source file\")\n\n    include_paths = merge_include_paths(\n\
    \        default_include_paths(repo_root, source), args.include\n    )\n    writer\
    \ = CheckpointWriter(output, reporter)\n    command_started = time.monotonic()\n\
    \n    with common_compiler_from_args(\n        args, include_paths=include_paths,\
    \ reporter=reporter\n    ) as compiler:\n        started = time.monotonic()\n\
    \        reporter.phase(\"bundle\", \"running oj-bundle\")\n        oj_bundle\
    \ = discover_oj_bundle(repo_root, args.oj_bundle)\n        raw_bundle = run_oj_bundle(\n\
    \            executable=oj_bundle,\n            source=source,\n            include_paths=include_paths,\n\
    \            cxx=compiler.cxx,\n        )\n        reporter.done(\"bundle\", \"\
    oj-bundle completed\", started)\n        resolver = OriginResolver(\n        \
    \    base_dir=source.parent,\n            source=source,\n            include_paths=include_paths,\n\
    \        )\n        protected = read_direct_local_includes(\n            source,\n\
    \            include_paths=include_paths,\n            resolver=resolver,\n  \
    \      )\n        document = BundleDocument.parse(\n            raw_bundle,\n\
    \            resolver=resolver,\n            protected=protected,\n        )\n\
    \        for line in document.lines:\n            if line.origin_key is None:\n\
    \                continue\n            if re.match(\n                r\"^\\s*#\\\
    s*include\\s*<bits/stdc\\+\\+\\.h>\\s*$\",\n                line.text,\n     \
    \       ):\n                document.protected.add(line.origin_key)\n\n      \
    \  initial = document.render(set(), keep_line=True)\n        privacy_check_line_directives(initial)\n\
    \        started = time.monotonic()\n        reporter.phase(\"bundle\", \"validating\
    \ the initial bundle\")\n        valid, stderr = compiler.validate(initial)\n\
    \        if not valid:\n            raise ToolError(f\"the initial bundle does\
    \ not compile: {first_error(stderr)}\")\n        reporter.done(\"bundle\", \"\
    initial bundle validated\", started)\n        writer.save(\"bundled\", initial,\
    \ force=True)\n\n        if args.no_prune_headers:\n            removed: set[str]\
    \ = set()\n        else:\n            reporter.phase(\"headers\", \"checking removable\
    \ bundled headers\")\n            removed = prune_header_origins(\n          \
    \      document,\n                compiler=compiler,\n                writer=writer,\n\
    \                keep_patterns=args.keep_header,\n                keep_line=True,\n\
    \                max_passes=args.header_passes,\n                reporter=reporter,\n\
    \            )\n\n        bundled = document.render(removed, keep_line=args.keep_line)\n\
    \        privacy_check_line_directives(bundled)\n        if bundled != writer.current:\n\
    \            valid, stderr = compiler.validate(bundled)\n            if not valid:\n\
    \                raise ToolError(f\"line cleanup did not validate: {first_error(stderr)}\"\
    )\n            writer.save(\"line directives\", bundled)\n\n        if not args.no_safe_cleanup:\n\
    \            cppcheck = None\n            if args.aggressive:\n              \
    \  cppcheck = resolve_executable(args.cppcheck, (\"cppcheck\",))\n           \
    \ bundled = cleanup_text(\n                bundled,\n                compiler=compiler,\n\
    \                writer=writer,\n                cppcheck=cppcheck,\n        \
    \        keep_functions=DEFAULT_KEEP_FUNCTIONS.union(args.keep_function),\n  \
    \              max_passes=args.cleanup_passes,\n                safe_only=not\
    \ args.aggressive,\n                require_validation=False,\n              \
    \  reporter=reporter,\n            )\n\n        writer.save(\"final\", bundled,\
    \ force=True)\n    reporter.done(\n        \"bundle\",\n        f\"finished at\
    \ {reporter.byte_count(encoded_size(bundled), 'green')}\",\n        command_started,\n\
    \    )\n\n\ndef cleanup_command(args: argparse.Namespace, repo_root: Path) ->\
    \ None:\n    reporter = Reporter(args.verbose, args.color)\n    command_started\
    \ = time.monotonic()\n    path = Path(args.file).expanduser().resolve()\n    if\
    \ not path.is_file():\n        raise ToolError(f\"file does not exist: {path}\"\
    )\n    text = path.read_text(encoding=\"utf-8\")\n    writer = CheckpointWriter(path,\
    \ reporter)\n    writer.current = text\n    target_bytes = None if args.target_bytes\
    \ == 0 else args.target_bytes\n    if target_bytes is not None and target_bytes\
    \ <= 0:\n        raise ToolError(\"target bytes must be positive, or 0 to disable\
    \ the target\")\n    writer.preserve_below = target_bytes\n    target_description\
    \ = (\n        \"disabled\"\n        if target_bytes is None\n        else f\"\
    <{reporter.byte_count(target_bytes, 'yellow')}\"\n    )\n    reporter.phase(\n\
    \        \"cleanup\",\n        f\"starting at {reporter.byte_count(encoded_size(text),\
    \ 'cyan')}; \"\n        f\"target {target_description}\",\n    )\n    if below_byte_limit(text,\
    \ target_bytes):\n        reporter.checkpoint(\n            f\"submission already\
    \ ready: \"\n            f\"{reporter.byte_count(encoded_size(text), 'green')}\
    \ < \"\n            f\"{reporter.byte_count(target_bytes, 'yellow')}; cleanup\
    \ continues\"\n        )\n\n    include_paths = merge_include_paths(\n       \
    \ default_include_paths(repo_root, path), args.include\n    )\n    with common_compiler_from_args(\n\
    \        args, include_paths=include_paths, reporter=reporter\n    ) as compiler:\n\
    \        resolver = OriginResolver(\n            base_dir=path.parent,\n     \
    \       source=path,\n            include_paths=include_paths,\n        )\n  \
    \      document = BundleDocument.parse(text, resolver=resolver)\n        sanitized\
    \ = document.render(set(), keep_line=True)\n        privacy_check_line_directives(sanitized)\n\
    \        if sanitized != text:\n            text = sanitized\n\n        cppcheck\
    \ = None\n        if not args.safe_only:\n            cppcheck = resolve_executable(args.cppcheck,\
    \ (\"cppcheck\",))\n        cleaned = cleanup_text(\n            text,\n     \
    \       compiler=compiler,\n            writer=writer,\n            cppcheck=cppcheck,\n\
    \            keep_functions=DEFAULT_KEEP_FUNCTIONS.union(args.keep_function),\n\
    \            max_passes=args.cleanup_passes,\n            safe_only=args.safe_only,\n\
    \            require_validation=True,\n            reporter=reporter,\n      \
    \      target_bytes=target_bytes,\n        )\n        writer.save(\"final\", cleaned)\n\
    \        checkpoint_available = below_byte_limit(cleaned, target_bytes) or (\n\
    \            writer.current is not None\n            and below_byte_limit(writer.current,\
    \ target_bytes)\n        )\n        if target_bytes is not None and not checkpoint_available:\n\
    \            reporter.warning(\n                f\"target not reached: \"\n  \
    \              f\"{reporter.byte_count(encoded_size(cleaned), 'red')} >= \"\n\
    \                f\"{reporter.byte_count(target_bytes, 'yellow')}\"\n        \
    \    )\n    reporter.done(\n        \"cleanup\",\n        f\"finished at {reporter.byte_count(encoded_size(cleaned),\
    \ 'green')}\",\n        command_started,\n    )\n\n\ndef add_reporter_options(parser:\
    \ argparse.ArgumentParser) -> None:\n    parser.add_argument(\"-v\", \"--verbose\"\
    , action=\"store_true\")\n    parser.add_argument(\n        \"--color\",\n   \
    \     choices=(\"auto\", \"always\", \"never\"),\n        default=\"auto\",\n\
    \        help=\"colored progress output (default: auto)\",\n    )\n\n\ndef add_compiler_options(parser:\
    \ argparse.ArgumentParser) -> None:\n    parser.add_argument(\"--cxx\", help=\"\
    C++ compiler (default: g++-15, g++-14, g++)\")\n    parser.add_argument(\"--std\"\
    , default=\"c++23\", help=\"target C++ standard\")\n    parser.add_argument(\n\
    \        \"--cxx-arg\",\n        action=\"append\",\n        default=[],\n   \
    \     metavar=\"ARG\",\n        help=\"additional compiler argument; use --cxx-arg=-DNAME\
    \ for leading '-'\",\n    )\n    parser.add_argument(\"-I\", \"--include\", action=\"\
    append\", default=[], metavar=\"DIR\")\n    parser.add_argument(\n        \"--timeout\"\
    ,\n        type=float,\n        default=0,\n        help=\"seconds per external\
    \ command; 0 disables the timeout\",\n    )\n    add_reporter_options(parser)\n\
    \n\ndef parse_args(argv: Sequence[str]) -> argparse.Namespace:\n    parser = argparse.ArgumentParser(\n\
    \        description=\"Bundle and reduce C++ submission code with validated checkpoints.\"\
    \n    )\n    subparsers = parser.add_subparsers(dest=\"command\", required=True)\n\
    \n    expand = subparsers.add_parser(\n        \"expand\", help=\"quickly expand\
    \ ordinary local includes\"\n    )\n    expand.add_argument(\"source\")\n    expand.add_argument(\"\
    -o\", \"--output\", default=\"bundle.cpp\")\n    expand.add_argument(\"--oj-bundle\"\
    , help=\"path used by the compatibility fallback\")\n    expand.add_argument(\"\
    --cxx\", help=\"compiler used by the compatibility fallback\")\n    expand.add_argument(\"\
    -I\", \"--include\", action=\"append\", default=[], metavar=\"DIR\")\n    expand.add_argument(\n\
    \        \"--no-fallback\",\n        action=\"store_true\",\n        help=\"fail\
    \ instead of using oj-bundle for unsupported include syntax\",\n    )\n    add_reporter_options(expand)\n\
    \n    bundle = subparsers.add_parser(\"bundle\", help=\"run oj-bundle and reduce\
    \ the result\")\n    bundle.add_argument(\"source\")\n    bundle.add_argument(\"\
    -o\", \"--output\", default=\"bundle.cpp\")\n    bundle.add_argument(\"--oj-bundle\"\
    , help=\"path to oj-bundle\")\n    bundle.add_argument(\"--no-prune-headers\"\
    , action=\"store_true\")\n    bundle.add_argument(\"--header-passes\", type=int,\
    \ default=1)\n    bundle.add_argument(\n        \"--keep-header\",\n        action=\"\
    append\",\n        default=[],\n        metavar=\"GLOB\",\n        help=\"do not\
    \ remove matching sanitized header paths\",\n    )\n    bundle.add_argument(\n\
    \        \"--keep-line\",\n        action=\"store_true\",\n        help=\"keep\
    \ sanitized #line directives in the final output\",\n    )\n    bundle.add_argument(\"\
    --no-safe-cleanup\", action=\"store_true\")\n    bundle.add_argument(\n      \
    \  \"--aggressive\",\n        action=\"store_true\",\n        help=\"also remove\
    \ declarations reported as unused\",\n    )\n    bundle.add_argument(\"--cleanup-passes\"\
    , type=int, default=3)\n    bundle.add_argument(\"--cppcheck\", help=\"cppcheck\
    \ executable\")\n    bundle.add_argument(\"--keep-function\", action=\"append\"\
    , default=[])\n    add_compiler_options(bundle)\n\n    cleanup = subparsers.add_parser(\"\
    cleanup\", help=\"reduce an existing bundle in place\")\n    cleanup.add_argument(\"\
    file\")\n    cleanup.add_argument(\n        \"--safe-only\",\n        action=\"\
    store_true\",\n        help=\"only remove comments and inactive branches and collapse\
    \ blank lines\",\n    )\n    cleanup.add_argument(\"--cleanup-passes\", type=int,\
    \ default=3)\n    cleanup.add_argument(\n        \"--target-bytes\",\n       \
    \ type=int,\n        default=DEFAULT_BYTE_LIMIT,\n        help=(\n           \
    \ \"write a checkpoint as soon as the file is smaller than this many \"\n    \
    \        \"bytes, then continue cleanup (default: 65536; 0 disables the checkpoint)\"\
    \n        ),\n    )\n    cleanup.add_argument(\"--cppcheck\", help=\"cppcheck\
    \ executable\")\n    cleanup.add_argument(\"--keep-function\", action=\"append\"\
    , default=[])\n    add_compiler_options(cleanup)\n\n    return parser.parse_args(argv)\n\
    \n\ndef main(argv: Sequence[str] | None = None) -> int:\n    args = parse_args(sys.argv[1:]\
    \ if argv is None else argv)\n    repo_root = Path(__file__).resolve().parent.parent\n\
    \    try:\n        if args.command == \"expand\":\n            expand_command(args,\
    \ repo_root)\n        elif args.command == \"bundle\":\n            bundle_command(args,\
    \ repo_root)\n        else:\n            cleanup_command(args, repo_root)\n  \
    \  except ToolError as exc:\n        Reporter(color=getattr(args, \"color\", \"\
    auto\")).error(str(exc))\n        return 1\n    return 0\n\n\nif __name__ == \"\
    __main__\":\n    raise SystemExit(main())\n"
  dependsOn: []
  isVerificationFile: false
  path: tools/submit_code.py
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: tools/submit_code.py
layout: document
redirect_from:
- /library/tools/submit_code.py
- /library/tools/submit_code.py.html
title: tools/submit_code.py
---
