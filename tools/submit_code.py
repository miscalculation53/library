#!/usr/bin/env python3
from __future__ import annotations

import argparse
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
import fnmatch
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import time
from typing import Iterable, Sequence
from urllib.parse import quote


LINE_DIRECTIVE = re.compile(
    r'^\s*#\s*line\s+(\d+)\s+("(?:\\.|[^"\\])*")(?:\s+.*)?$'
)
GNU_LINE_MARKER = re.compile(
    r'^\s*#\s+(\d+)\s+("(?:\\.|[^"\\])*")(?:\s+.*)?$'
)
INCLUDE_DIRECTIVE = re.compile(r"^\s*#\s*include\b")
LOCAL_INCLUDE_DIRECTIVE = re.compile(r'^\s*#\s*include\s*"([^"]+)"\s*$')
SYSTEM_INCLUDE_DIRECTIVE = re.compile(r"^\s*#\s*include\s*<([^>]+)>\s*$")
PRAGMA_ONCE_DIRECTIVE = re.compile(r"^\s*#\s*pragma\s+once\s*$")
CONDITIONAL_OPEN_DIRECTIVE = re.compile(r"^\s*#\s*(?:if|ifdef|ifndef)\b")
CONDITIONAL_BRANCH_DIRECTIVE = re.compile(r"^\s*#\s*(?:else|elif)\b")
CONDITIONAL_CLOSE_DIRECTIVE = re.compile(r"^\s*#\s*endif\b")
INCLUDE_GUARD_OPEN_DIRECTIVE = re.compile(
    r"^\s*#\s*ifndef\s+([_A-Za-z]\w*)\s*$"
)
IDENTIFIER = re.compile(r"\b[_A-Za-z]\w*\b")
DEFAULT_KEEP_FUNCTIONS = {"read", "write", "print", "init", "main2", "test"}
DEFAULT_BYTE_LIMIT = 65536
LIBRARY_SOURCE_BASE = "https://github.com/miscalculation53/library/tree/wip/"
LEGACY_LIBRARY_SITE_BASE = "https://miscalculation53.github.io/library/"
LIBRARY_SOURCE_LINK = re.compile(
    rf"^\s*//\s*((?:{re.escape(LIBRARY_SOURCE_BASE)}\S+\.hpp|"
    rf"{re.escape(LEGACY_LIBRARY_SITE_BASE)}\S+\.html#unbundled))\s*$"
)
SOURCE_LINK_PRAGMA = re.compile(
    r"^\s*#\s*pragma\s+submit_code_source_link\s+(\d+)\s*$"
)


class ToolError(RuntimeError):
    pass


class FastBundleUnsupported(ToolError):
    pass


def library_source_url(path: Path, library_root: Path | None) -> str | None:
    if library_root is None:
        return None
    try:
        relative = path.resolve().relative_to(library_root.resolve())
    except ValueError:
        return None
    if relative.suffix != ".hpp":
        return None
    encoded = quote(relative.as_posix(), safe="/._-")
    return f"{LIBRARY_SOURCE_BASE}{encoded}"


def source_link_comment(url: str) -> str:
    return f"// {url}\n"


def protect_source_links(text: str) -> tuple[str, list[str]]:
    output: list[str] = []
    links: list[str] = []
    for line in text.splitlines(keepends=True):
        match = LIBRARY_SOURCE_LINK.match(line.rstrip("\r\n"))
        if match is None:
            output.append(line)
            continue
        links.append(source_link_comment(match.group(1)))
        output.append(f"#pragma submit_code_source_link {len(links) - 1}\n")
    return "".join(output), links


def restore_source_links(text: str, links: Sequence[str]) -> str:
    output: list[str] = []
    for line in text.splitlines(keepends=True):
        match = SOURCE_LINK_PRAGMA.match(line.rstrip("\r\n"))
        if match is None:
            output.append(line)
            continue
        index = int(match.group(1))
        if index >= len(links):
            raise ToolError("invalid protected source-link marker")
        output.append(links[index])
    return "".join(output)


def strip_cleanup_comments(text: str) -> str:
    protected, links = protect_source_links(text)
    return restore_source_links(strip_comments(protected), links)


class Reporter:
    COLORS = {
        "bold": "\033[1m",
        "cyan": "\033[36m",
        "green": "\033[32m",
        "yellow": "\033[33m",
        "red": "\033[31m",
        "dim": "\033[2m",
    }
    RESET = "\033[0m"

    def __init__(
        self,
        verbose: bool = False,
        color: str = "auto",
        stream=None,
    ) -> None:
        self.verbose = verbose
        self.stream = sys.stderr if stream is None else stream
        if color not in {"auto", "always", "never"}:
            raise ValueError(f"invalid color mode: {color}")
        self.use_color = color == "always" or (
            color == "auto"
            and "NO_COLOR" not in os.environ
            and bool(getattr(self.stream, "isatty", lambda: False)())
        )

    def _emit(self, tag: str, message: str, color: str | None = None) -> None:
        label = f"[{tag}]"
        if self.use_color and color is not None:
            label = f"{self.COLORS[color]}{label}{self.RESET}"
        print(f"{label} {message}", file=self.stream, flush=True)

    def byte_count(
        self,
        value: int,
        color: str | None = None,
        *,
        signed: bool = False,
    ) -> str:
        text = f"{value:+,} B" if signed and value != 0 else f"{value:,} B"
        if not self.use_color:
            return text
        color_code = self.COLORS[color] if color is not None else ""
        return f"{self.COLORS['bold']}{color_code}{text}{self.RESET}"

    def info(self, message: str) -> None:
        self._emit("info", message)

    def phase(self, stage: str, message: str) -> None:
        self._emit(stage, message, "cyan")

    def done(self, stage: str, message: str, started: float) -> None:
        elapsed = time.monotonic() - started
        self._emit(stage, f"{message} ({elapsed:.2f}s)", "green")

    def warning(self, message: str) -> None:
        self._emit("warning", message, "yellow")

    def checkpoint(self, message: str) -> None:
        self._emit("checkpoint", message, "green")

    def error(self, message: str) -> None:
        self._emit("error", message, "red")

    def detail(self, message: str) -> None:
        if self.verbose:
            self._emit("detail", message, "dim")

    def size(self, stage: str, before: int, after: int) -> None:
        saved = before - after
        color = "green" if saved >= 0 else "yellow"
        self._emit(
            "SIZE",
            f"{stage}: {self.byte_count(before)}  ->  "
            f"{self.byte_count(after, color)}  "
            f"({self.byte_count(after - before, color, signed=True)})",
            color,
        )


def atomic_write(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    mode: int | None
    try:
        mode = path.stat().st_mode
    except FileNotFoundError:
        mode = None

    temporary_path: Path | None = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            encoding="utf-8",
            newline="",
            dir=path.parent,
            prefix=f".{path.name}.",
            suffix=".tmp",
            delete=False,
        ) as temporary:
            temporary_path = Path(temporary.name)
            temporary.write(text)
            temporary.flush()
            os.fsync(temporary.fileno())
        if mode is not None:
            os.chmod(temporary_path, mode)
        os.replace(temporary_path, path)
        temporary_path = None
    finally:
        if temporary_path is not None:
            try:
                temporary_path.unlink()
            except FileNotFoundError:
                pass


class CheckpointWriter:
    def __init__(self, path: Path, reporter: Reporter) -> None:
        self.path = path
        self.reporter = reporter
        self.current: str | None = None
        self.preserve_below: int | None = None

    def save(self, stage: str, text: str, *, force: bool = False) -> bool:
        if not force and text == self.current:
            return False
        if (
            self.preserve_below is not None
            and self.current is not None
            and len(self.current.encode()) < self.preserve_below
            and len(text.encode()) >= self.preserve_below
        ):
            return False
        before = len(self.current.encode()) if self.current is not None else len(text.encode())
        atomic_write(self.path, text)
        self.current = text
        self.reporter.size(stage, before, len(text.encode()))
        return True


def safe_decode(data: bytes) -> str:
    return data.decode("utf-8", errors="replace")


def encoded_size(text: str) -> int:
    return len(text.encode())


def below_byte_limit(text: str, limit: int | None) -> bool:
    return limit is not None and encoded_size(text) < limit


def redact_home(text: str) -> str:
    home = str(Path.home())
    return text.replace(home, "~")


def run_command(
    command: Sequence[str],
    *,
    cwd: Path | None = None,
    env: dict[str, str] | None = None,
    input_text: str | None = None,
    timeout: float | None = None,
) -> subprocess.CompletedProcess[str]:
    try:
        return subprocess.run(
            list(command),
            cwd=cwd,
            env=env,
            input=input_text,
            capture_output=True,
            text=True,
            errors="replace",
            timeout=timeout,
            check=False,
        )
    except FileNotFoundError as exc:
        raise ToolError(f"executable not found: {command[0]}") from exc
    except subprocess.TimeoutExpired as exc:
        raise ToolError(f"command timed out: {command[0]}") from exc
    except OSError as exc:
        raise ToolError(f"failed to run {command[0]}: {exc}") from exc


def resolve_executable(value: str | None, candidates: Sequence[str]) -> str:
    if value:
        resolved = shutil.which(value)
        if resolved is None and Path(value).is_file():
            resolved = str(Path(value).resolve())
        if resolved is None:
            raise ToolError(f"executable not found: {value}")
        return resolved

    for candidate in candidates:
        resolved = shutil.which(candidate)
        if resolved is not None:
            return resolved
    raise ToolError(f"none of these executables were found: {', '.join(candidates)}")


def discover_oj_bundle(repo_root: Path, value: str | None) -> str:
    if value:
        return resolve_executable(value, ())

    on_path = shutil.which("oj-bundle")
    if on_path is not None:
        return on_path

    candidates = [
        repo_root / ".venv/bin/oj-bundle",
        repo_root / "venv/bin/oj-bundle",
        repo_root.parent / "venv/bin/oj-bundle",
    ]
    for candidate in candidates:
        if candidate.is_file() and os.access(candidate, os.X_OK):
            return str(candidate.resolve())
    raise ToolError("oj-bundle was not found; pass --oj-bundle PATH")


def normalize_standard(value: str) -> str:
    value = value.strip()
    if value.startswith("-std="):
        value = value[5:]
    if value.isdigit():
        return f"c++{value}"
    if re.fullmatch(r"(?:c|gnu)\+\+\d+", value):
        return value
    raise ToolError(f"invalid C++ standard: {value}")


@dataclass(frozen=True)
class Origin:
    key: str
    display: str
    resolved: Path | None


class OriginResolver:
    def __init__(
        self,
        *,
        base_dir: Path,
        source: Path,
        include_paths: Sequence[Path],
    ) -> None:
        self.base_dir = base_dir.resolve()
        self.source = source.resolve()
        self.home_name = Path.home().name
        self.external_ids: dict[str, int] = {}

        roots: list[tuple[str, Path]] = [("source", self.source.parent)]
        seen = {self.source.parent}
        for include_path in include_paths:
            root = include_path.resolve()
            if root in seen:
                continue
            seen.add(root)
            roots.append((f"include-{len(roots) - 1}", root))
        self.roots = roots

    @property
    def source_key(self) -> str:
        return str(self.source)

    def _clean_component(self, value: str) -> str:
        if value == self.home_name:
            return "_user_"
        cleaned = re.sub(r"[^-._A-Za-z0-9]+", "_", value)
        return cleaned or "_"

    def _clean_relative(self, path: Path) -> str:
        return "/".join(self._clean_component(part) for part in path.parts)

    def resolve(self, raw_path: str) -> Origin:
        if raw_path.startswith("bundle/"):
            return Origin(f"virtual:{raw_path}", raw_path, None)
        if raw_path.startswith("<") and raw_path.endswith(">"):
            key = raw_path
            label = self._clean_component(raw_path[1:-1])
            return Origin(key, f"bundle/special/{label}", None)

        path = Path(raw_path).expanduser()
        if not path.is_absolute():
            path = self.base_dir / path
        resolved = path.resolve()
        key = str(resolved)

        if resolved == self.source:
            return Origin(key, f"bundle/source/{self._clean_component(resolved.name)}", resolved)

        for label, root in self.roots:
            try:
                relative = resolved.relative_to(root)
            except ValueError:
                continue
            return Origin(key, f"bundle/{label}/{self._clean_relative(relative)}", resolved)

        external_id = self.external_ids.setdefault(key, len(self.external_ids))
        basename = self._clean_component(resolved.name)
        return Origin(key, f"bundle/external-{external_id}/{basename}", resolved)


@dataclass(frozen=True)
class BundledLine:
    text: str
    origin_key: str | None
    origin_display: str | None
    source_line: int


class BundleDocument:
    def __init__(
        self,
        lines: Sequence[BundledLine],
        origins: dict[str, Origin],
        protected: Iterable[str],
        library_root: Path | None,
    ) -> None:
        self.lines = list(lines)
        self.origins = origins
        self.protected = set(protected)
        self.library_root = library_root.resolve() if library_root is not None else None

    @classmethod
    def parse(
        cls,
        text: str,
        *,
        resolver: OriginResolver,
        protected: Iterable[str] = (),
        library_root: Path | None = None,
    ) -> "BundleDocument":
        lines: list[BundledLine] = []
        origins: dict[str, Origin] = {}
        current: Origin | None = None
        source_line = 1

        for line in text.splitlines(keepends=True):
            match = LINE_DIRECTIVE.match(line.rstrip("\r\n"))
            if match:
                try:
                    raw_path = json.loads(match.group(2))
                except json.JSONDecodeError as exc:
                    raise ToolError(f"invalid #line path: {line.rstrip()}") from exc
                current = resolver.resolve(raw_path)
                origins[current.key] = current
                source_line = int(match.group(1))
                continue

            lines.append(
                BundledLine(
                    text=line,
                    origin_key=current.key if current else None,
                    origin_display=current.display if current else None,
                    source_line=source_line,
                )
            )
            source_line += 1

        protected_keys = set(protected)
        protected_keys.add(resolver.source_key)
        return cls(lines, origins, protected_keys, library_root)

    def origin_sizes(self) -> dict[str, int]:
        sizes: dict[str, int] = Counter()
        for line in self.lines:
            if line.origin_key is not None:
                sizes[line.origin_key] += len(line.text.encode())
        return dict(sizes)

    def origin_first_positions(self) -> dict[str, int]:
        result: dict[str, int] = {}
        for index, line in enumerate(self.lines):
            if line.origin_key is not None:
                result.setdefault(line.origin_key, index)
        return result

    def render(self, removed: set[str], *, keep_line: bool) -> str:
        output: list[str] = []
        previous_key: str | None = None
        expected_line: int | None = None
        linked: set[str] = set()

        for line in self.lines:
            if line.origin_key in removed:
                continue

            if line.origin_key is not None and line.origin_key not in linked:
                origin = self.origins[line.origin_key]
                url = (
                    library_source_url(origin.resolved, self.library_root)
                    if origin.resolved is not None
                    else None
                )
                if url is not None:
                    output.append(source_link_comment(url))
                linked.add(line.origin_key)

            if keep_line and line.origin_key is not None:
                if line.origin_key != previous_key or line.source_line != expected_line:
                    assert line.origin_display is not None
                    quoted = json.dumps(line.origin_display, ensure_ascii=True)
                    output.append(f"#line {line.source_line} {quoted}\n")
                previous_key = line.origin_key
                expected_line = line.source_line + 1
            else:
                previous_key = line.origin_key
                expected_line = line.source_line + 1
            output.append(line.text)

        return "".join(output)


def read_direct_local_includes(
    source: Path,
    *,
    include_paths: Sequence[Path],
    resolver: OriginResolver,
) -> set[str]:
    try:
        text = strip_comments(source.read_text(encoding="utf-8"))
    except OSError:
        return set()

    result: set[str] = set()
    pattern = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]', re.MULTILINE)
    for match in pattern.finditer(text):
        opening, name = match.groups()
        search_roots: list[Path] = []
        if opening == '"':
            search_roots.append(source.parent)
        search_roots.extend(include_paths)
        for root in search_roots:
            candidate = (root / name).resolve()
            if candidate.is_file():
                result.add(str(candidate))
                break
    return result


class CompilerWorkspace:
    def __init__(
        self,
        *,
        cxx: str,
        standard: str,
        include_paths: Sequence[Path],
        extra_args: Sequence[str],
        timeout: float | None,
        reporter: Reporter,
    ) -> None:
        self.cxx = cxx
        self.standard = standard
        self.include_paths = list(include_paths)
        self.extra_args = list(extra_args)
        self.timeout = timeout
        self.reporter = reporter
        self.temporary = tempfile.TemporaryDirectory(prefix="submit-code-")
        self.directory = Path(self.temporary.name)
        self.source = self.directory / "candidate.cpp"
        self.analysis_source = self.directory / "analysis.cpp"
        self.object = self.directory / "candidate.o"
        self.executable = self.directory / "candidate.out"
        self.pch_header = self.directory / "standard_prelude.hpp"
        self.pch_output = self.directory / "standard_prelude.hpp.gch"
        self.pch_available: bool | None = None
        self.pch_key: str | None = None
        self.pch_prefix_end: int | None = None
        self.syntax_cache: dict[str, tuple[bool, str]] = {}
        self.link_cache: dict[str, tuple[bool, str]] = {}

    def close(self) -> None:
        self.temporary.cleanup()

    def __enter__(self) -> "CompilerWorkspace":
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.close()

    def write(self, text: str) -> None:
        self.source.write_text(text, encoding="utf-8", newline="")

    def write_analysis_source(self, text: str) -> None:
        self.analysis_source.write_text(text, encoding="utf-8", newline="")

    def _base_command(self) -> list[str]:
        command = [self.cxx, f"-std={self.standard}", "-w"]
        for include_path in self.include_paths:
            command.extend(["-I", str(include_path)])
        command.extend(self.extra_args)
        return command

    def _pch_prefix(self, text: str) -> tuple[str, int] | None:
        include = re.search(r"^\s*#\s*include\s*<bits/stdc\+\+\.h>\s*$", text, re.MULTILINE)
        if include is None:
            return None
        prefix = strip_comments(text[: include.start()])
        continued = False
        allowed_directives = {
            "define",
            "undef",
            "if",
            "ifdef",
            "ifndef",
            "elif",
            "else",
            "endif",
            "pragma",
            "line",
        }
        for line in prefix.splitlines():
            stripped = line.strip()
            if not stripped:
                continue
            if continued:
                continued = line.rstrip().endswith("\\")
                continue
            match = re.match(r"^#\s*([A-Za-z_]\w*)", stripped)
            if match is None or match.group(1) not in allowed_directives:
                return None
            continued = line.rstrip().endswith("\\")
        if continued:
            return None
        return text[: include.end()] + "\n", include.end()

    def _ensure_pch(self, text: str) -> bool:
        prefix = self._pch_prefix(text)
        if prefix is None:
            self.pch_prefix_end = None
            return False
        if any((path / "bits/stdc++.h").is_file() for path in self.include_paths):
            self.pch_prefix_end = None
            return False
        header_text, prefix_end = prefix

        try:
            compiler_stat = Path(self.cxx).stat()
            compiler_stamp = [compiler_stat.st_size, compiler_stat.st_mtime_ns]
        except OSError:
            compiler_stamp = [0, 0]
        cache_key = hashlib.sha256(
            json.dumps(
                {
                    "compiler": str(Path(self.cxx).resolve()),
                    "compiler_stamp": compiler_stamp,
                    "standard": self.standard,
                    "extra_args": self.extra_args,
                    "header": header_text,
                },
                sort_keys=True,
            ).encode()
        ).hexdigest()[:20]
        if self.pch_key == cache_key and self.pch_available is not None:
            self.pch_prefix_end = prefix_end if self.pch_available else None
            return self.pch_available

        self.pch_key = cache_key
        self.pch_available = None
        self.pch_prefix_end = None
        cache_root = Path(
            os.environ.get(
                "SUBMIT_CODE_CACHE_DIR",
                str(Path(tempfile.gettempdir()) / "submit-code-cache"),
            )
        )
        cache_dir = cache_root / cache_key
        cache_dir.mkdir(parents=True, exist_ok=True)
        self.pch_header = cache_dir / "standard_prelude.hpp"
        self.pch_output = cache_dir / "standard_prelude.hpp.gch"
        if self.pch_header.is_file() and self.pch_output.is_file():
            try:
                if self.pch_header.read_text(encoding="utf-8") == header_text:
                    self.pch_available = True
                    self.pch_prefix_end = prefix_end
                    return True
            except OSError:
                pass

        atomic_write(self.pch_header, header_text)
        temporary_output = cache_dir / f".{self.pch_output.name}.{os.getpid()}.tmp"
        result = self._run(
            [
                self.cxx,
                f"-std={self.standard}",
                "-w",
                *self.extra_args,
                "-x",
                "c++-header",
                "-c",
                str(self.pch_header),
                "-o",
                str(temporary_output),
            ]
        )
        self.pch_available = result.returncode == 0
        if self.pch_available:
            os.replace(temporary_output, self.pch_output)
            self.pch_prefix_end = prefix_end
        else:
            try:
                temporary_output.unlink()
            except FileNotFoundError:
                pass
        if not self.pch_available:
            self.reporter.detail(
                f"standard-library PCH is unavailable: {first_error(result.stderr)}"
            )
        return self.pch_available

    def _prepare_candidate_source(self, text: str) -> bool:
        use_pch = self._ensure_pch(text)
        if use_pch and self.pch_prefix_end is not None:
            prefix = text[: self.pch_prefix_end]
            masked_prefix = "".join("\n" if char == "\n" else " " for char in prefix)
            self.write(masked_prefix + text[self.pch_prefix_end :])
        else:
            self.write(text)
        return use_pch

    def _candidate_command(self, use_pch: bool) -> list[str]:
        command = self._base_command()
        if use_pch:
            command.extend(["-include", str(self.pch_header)])
        return command

    def _run(self, command: Sequence[str]) -> subprocess.CompletedProcess[str]:
        return run_command(command, timeout=self.timeout)

    def syntax(self, text: str) -> tuple[bool, str]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        cached = self.syntax_cache.get(digest)
        if cached is not None:
            return cached
        use_pch = self._prepare_candidate_source(text)
        result = self._run([*self._candidate_command(use_pch), "-fsyntax-only", str(self.source)])
        value = result.returncode == 0, result.stderr
        self.syntax_cache[digest] = value
        return value

    def link(self, text: str) -> tuple[bool, str]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        cached = self.link_cache.get(digest)
        if cached is not None:
            return cached
        use_pch = self._prepare_candidate_source(text)
        for artifact in (self.object, self.executable):
            try:
                artifact.unlink()
            except FileNotFoundError:
                pass

        if not self.extra_args:
            result = self._run(
                [
                    *self._candidate_command(use_pch),
                    "-O0",
                    str(self.source),
                    "-o",
                    str(self.executable),
                ]
            )
            value = result.returncode == 0, result.stderr
            self.link_cache[digest] = value
            return value

        compile_result = self._run(
            [
                *self._candidate_command(use_pch),
                "-O0",
                "-c",
                str(self.source),
                "-o",
                str(self.object),
            ]
        )
        if compile_result.returncode != 0:
            value = False, compile_result.stderr
            self.link_cache[digest] = value
            return value

        link_result = self._run(
            [
                self.cxx,
                "-w",
                str(self.object),
                "-o",
                str(self.executable),
                *self.extra_args,
            ]
        )
        value = link_result.returncode == 0, link_result.stderr
        self.link_cache[digest] = value
        return value

    def validate(self, text: str) -> tuple[bool, str]:
        return self.link(text)

    def compiler_warnings(self, text: str, *, allow_pch: bool = True) -> str:
        if allow_pch:
            use_pch = self._prepare_candidate_source(text)
        else:
            self.write(text)
            use_pch = False
        command = [self.cxx, f"-std={self.standard}"]
        for include_path in self.include_paths:
            command.extend(["-I", str(include_path)])
        command.extend(self.extra_args)
        if use_pch:
            command.extend(["-include", str(self.pch_header)])
        result = self._run(
            [
                *command,
                "-Wall",
                "-Wextra",
                "-Wunused",
                "-Wunused-macros",
                "-c",
                str(self.source),
                "-o",
                os.devnull,
            ]
        )
        if result.returncode != 0:
            self.reporter.detail(redact_home(result.stderr))
            return ""
        return result.stderr

    def ipa_unused_functions(self, text: str) -> list[tuple[int, str]]:
        for path in self.directory.glob("ipa.*"):
            if path.is_file():
                path.unlink()

        use_pch = self._prepare_candidate_source(text)
        started = time.monotonic()
        result = self._run(
            [
                *self._candidate_command(use_pch),
                "-Og",
                "-fdump-ipa-cgraph",
                "-fdump-tree-original-raw",
                "-dumpdir",
                str(self.directory) + os.sep,
                "-dumpbase",
                "ipa.",
                "-c",
                str(self.source),
                "-o",
                str(self.directory / "ipa.o"),
            ]
        )
        if result.returncode != 0:
            self.reporter.detail(
                "IPA analysis failed: " + first_error(result.stderr)
            )
            return []

        cgraph = next(self.directory.glob("ipa.*.cgraph"), None)
        original = next(self.directory.glob("ipa.*.original"), None)
        if cgraph is None or original is None:
            self.reporter.detail("IPA analysis did not produce GCC dump files")
            return []

        signatures = ipa_removed_signatures(
            cgraph.read_text(encoding="utf-8", errors="replace")
        )
        functions = ipa_function_locations(
            original,
            signatures=signatures,
            source_name=self.source.name,
        )
        self.reporter.detail(
            f"IPA analysis: {len(functions)} mapped function(s), "
            f"{time.monotonic() - started:.2f}s"
        )
        return functions

    def preprocess_directives(self, text: str) -> tuple[bool, str, str]:
        self.write(text)
        result = self._run(
            [
                self.cxx,
                f"-std={self.standard}",
                "-E",
                "-fdirectives-only",
                "-x",
                "c++",
                *(
                    argument
                    for include_path in self.include_paths
                    for argument in ("-I", str(include_path))
                ),
                *self.extra_args,
                str(self.source),
            ]
        )
        return result.returncode == 0, result.stdout, result.stderr


def first_error(stderr: str) -> str:
    for line in stderr.splitlines():
        if "error:" in line:
            return redact_home(line.strip())
    for line in stderr.splitlines():
        if line.strip():
            return redact_home(line.strip())
    return "validation failed"


def split_weighted(items: Sequence[str], sizes: dict[str, int]) -> tuple[list[str], list[str]]:
    if len(items) < 2:
        return list(items), []
    total = sum(max(1, sizes.get(item, 1)) for item in items)
    left: list[str] = []
    weight = 0
    for item in items:
        if left and weight >= total / 2:
            break
        left.append(item)
        weight += max(1, sizes.get(item, 1))
    if len(left) == len(items):
        left = list(items[: len(items) // 2])
    return left, list(items[len(left) :])


def prune_header_origins(
    document: BundleDocument,
    *,
    compiler: CompilerWorkspace,
    writer: CheckpointWriter,
    keep_patterns: Sequence[str],
    keep_line: bool,
    max_passes: int,
    reporter: Reporter,
) -> set[str]:
    sizes = document.origin_sizes()
    first_positions = document.origin_first_positions()
    removed: set[str] = set()
    validation_cache: dict[frozenset[str], tuple[bool, str]] = {}

    candidates = []
    for key, origin in document.origins.items():
        if key in document.protected:
            continue
        if any(fnmatch.fnmatch(origin.display, pattern) for pattern in keep_patterns):
            continue
        candidates.append(key)

    candidates.sort(key=lambda key: first_positions.get(key, -1), reverse=True)

    def try_remove(batch: Sequence[str]) -> bool:
        nonlocal removed
        proposal = frozenset(removed.union(batch))
        if proposal == frozenset(removed):
            return True
        text = document.render(set(proposal), keep_line=keep_line)
        cached = validation_cache.get(proposal)
        if cached is None:
            started = time.monotonic()
            valid, stderr = compiler.validate(text)
            validation_cache[proposal] = valid, stderr
            reporter.detail(
                f"header trial: remove {len(batch)} file(s), "
                f"{time.monotonic() - started:.2f}s, {'accepted' if valid else 'rejected'}"
            )
        else:
            valid, stderr = cached
        if not valid:
            reporter.detail(first_error(stderr))
            return False

        removed = set(proposal)
        writer.save(f"removed {len(batch)} header group(s)", text)
        if reporter.verbose:
            for key in batch:
                reporter.detail(f"  {document.origins[key].display}")
        return True

    def reduce_batch(items: Sequence[str]) -> None:
        active = [item for item in items if item not in removed]
        if not active:
            return
        if try_remove(active):
            return
        if len(active) == 1:
            return
        left, right = split_weighted(active, sizes)
        reduce_batch(left)
        reduce_batch(right)

    for pass_index in range(1, max_passes + 1):
        before = len(removed)
        reduce_batch([candidate for candidate in candidates if candidate not in removed])
        if len(removed) == before:
            reporter.detail(f"header pruning converged after pass {pass_index}")
            break

    return removed


def strip_comments(text: str) -> str:
    output: list[str] = []
    index = 0
    size = len(text)

    while index < size:
        if text.startswith("//", index):
            index += 2
            while index < size:
                if text[index] == "\n":
                    output.append("\n")
                    index += 1
                    break
                if text[index] == "\\" and index + 1 < size and text[index + 1] == "\n":
                    output.append("\n")
                    index += 2
                    continue
                index += 1
            continue

        if text.startswith("/*", index):
            output.append(" ")
            index += 2
            while index < size and not text.startswith("*/", index):
                if text[index] == "\n":
                    output.append("\n")
                index += 1
            if index < size:
                index += 2
            continue

        raw_match = re.match(r'(?:u8|u|U|L)?R"([^ ()\\\t\r\n]{0,16})\(', text[index:])
        if raw_match:
            opener = raw_match.group(0)
            delimiter = raw_match.group(1)
            closer = f'){delimiter}"'
            end = text.find(closer, index + len(opener))
            if end < 0:
                output.append(text[index:])
                break
            end += len(closer)
            output.append(text[index:end])
            index = end
            continue

        ch = text[index]
        if ch in {'"', "'"}:
            quote = ch
            start = index
            index += 1
            escaped = False
            while index < size:
                current = text[index]
                index += 1
                if escaped:
                    escaped = False
                elif current == "\\":
                    escaped = True
                elif current == quote:
                    break
            output.append(text[start:index])
            continue

        output.append(ch)
        index += 1

    return "".join(output)


def preprocessor_lines(text: str) -> list[str]:
    """Return comment-free lines, masking lines continued from a literal or macro."""

    lines = strip_comments(text).splitlines(keepends=True)
    result: list[str] = []
    raw_closer: str | None = None
    quote: str | None = None
    previous_continues = False

    for line in lines:
        protected_at_start = (
            raw_closer is not None or quote is not None or previous_continues
        )
        if protected_at_start:
            result.append("\n" if line.endswith("\n") else "")
        else:
            result.append(line)

        index = 0
        escaped = False
        while index < len(line):
            if raw_closer is not None:
                end = line.find(raw_closer, index)
                if end < 0:
                    index = len(line)
                    continue
                index = end + len(raw_closer)
                raw_closer = None
                continue

            if quote is not None:
                ch = line[index]
                index += 1
                if escaped:
                    escaped = False
                elif ch == "\\":
                    escaped = True
                elif ch == quote:
                    quote = None
                continue

            raw_match = re.match(
                r'(?:u8|u|U|L)?R"([^ ()\\\t\r\n]{0,16})\(', line[index:]
            )
            if raw_match:
                raw_closer = f'){raw_match.group(1)}"'
                index += len(raw_match.group(0))
                continue

            ch = line[index]
            if ch in {'"', "'"}:
                quote = ch
            index += 1

        previous_continues = line.rstrip("\r\n").endswith("\\")
        if quote is not None and not previous_continues:
            quote = None

    return result


def collapse_blank_lines(text: str) -> str:
    output: list[str] = []
    raw_closer: str | None = None
    quote: str | None = None
    previous_continues = False
    previous_blank = False

    for line in text.splitlines(keepends=True):
        preserve_indentation = (
            raw_closer is not None or quote is not None or previous_continues
        )
        protected = preserve_indentation
        index = 0
        while index < len(line):
            if raw_closer is not None:
                protected = True
                end = line.find(raw_closer, index)
                if end < 0:
                    index = len(line)
                    continue
                index = end + len(raw_closer)
                raw_closer = None
                continue

            if quote is not None:
                protected = True
                escaped = False
                while index < len(line):
                    ch = line[index]
                    index += 1
                    if escaped:
                        escaped = False
                    elif ch == "\\":
                        escaped = True
                    elif ch == quote:
                        quote = None
                        break
                continue

            raw_match = re.match(
                r'(?:u8|u|U|L)?R"([^ ()\\\t\r\n]{0,16})\(',
                line[index:],
            )
            if raw_match:
                protected = True
                delimiter = raw_match.group(1)
                raw_closer = f'){delimiter}"'
                index += len(raw_match.group(0))
                continue

            ch = line[index]
            if ch in {'"', "'"}:
                protected = True
                quote = ch
                index += 1
                continue
            index += 1

        previous_continues = line.rstrip("\r\n").endswith("\\")
        if protected:
            output.append(line)
            previous_blank = False
            continue
        if line.strip():
            output.append(line)
            previous_blank = False
            continue
        if not previous_blank:
            output.append(line)
            previous_blank = True

    return "".join(output)


def wrap_include_directives(text: str) -> tuple[str, dict[int, str]]:
    lines = text.splitlines(keepends=True)
    output: list[str] = []
    includes: dict[int, str] = {}
    index = 0

    while index < len(lines):
        line = lines[index]
        is_line_directive = LINE_DIRECTIVE.match(line.rstrip("\r\n")) is not None
        if not INCLUDE_DIRECTIVE.match(line) and not is_line_directive:
            output.append(line)
            index += 1
            continue

        include_lines = [line]
        index += 1
        while include_lines[-1].rstrip("\r\n").endswith("\\") and index < len(lines):
            include_lines.append(lines[index])
            index += 1
        include_id = len(includes)
        includes[include_id] = "".join(include_lines)
        output.append(f"#pragma submit_code_include_begin {include_id}\n")
        if not is_line_directive:
            output.extend(include_lines)
        output.append(f"#pragma submit_code_include_end {include_id}\n")

    return "".join(output), includes


def filter_preprocessor_output(
    output: str,
    *,
    source_path: Path,
    includes: dict[int, str],
) -> str:
    source_resolved = source_path.resolve()
    current_file: Path | None = None
    result: list[str] = []
    begin_pattern = re.compile(r"^\s*#pragma\s+submit_code_include_begin\s+(\d+)\s*$")
    end_pattern = re.compile(r"^\s*#pragma\s+submit_code_include_end\s+(\d+)\s*$")

    for line in output.splitlines(keepends=True):
        marker = GNU_LINE_MARKER.match(line.rstrip("\r\n"))
        if marker:
            try:
                raw_path = json.loads(marker.group(2))
            except json.JSONDecodeError:
                current_file = None
                continue
            if raw_path.startswith("<"):
                current_file = None
            else:
                current_file = Path(raw_path).resolve()
            continue

        if current_file != source_resolved:
            continue
        begin = begin_pattern.match(line)
        if begin:
            include_id = int(begin.group(1))
            result.append(includes[include_id])
            continue
        if end_pattern.match(line):
            continue
        result.append(line)

    return "".join(result)


def safe_cleanup_candidate(text: str, compiler: CompilerWorkspace) -> str:
    protected, links = protect_source_links(text)
    uncommented = strip_comments(protected)
    wrapped, includes = wrap_include_directives(uncommented)
    valid, output, stderr = compiler.preprocess_directives(wrapped)
    if not valid:
        raise ToolError(f"preprocessor failed: {first_error(stderr)}")
    filtered = filter_preprocessor_output(
        output,
        source_path=compiler.source,
        includes=includes,
    )
    return collapse_blank_lines(restore_source_links(filtered, links))


def insert_anonymous_namespace(text: str) -> str:
    """Put bundled declarations after the initial include block in an anonymous namespace."""

    lines = text.splitlines(keepends=True)
    if not lines:
        return text

    first_include = next(
        (index for index, line in enumerate(lines) if INCLUDE_DIRECTIVE.match(line)),
        None,
    )
    include_end = 0
    if first_include is not None:
        uncommented = strip_comments(text).splitlines()
        include_end = first_include
        continued = False
        while include_end < len(lines):
            stripped = uncommented[include_end].strip()
            if not continued and stripped and not stripped.startswith("#"):
                break
            continued = lines[include_end].rstrip("\r\n").endswith("\\")
            include_end += 1

    tokens = structural_tokens(lines)
    brace_depth = 0
    main_line: int | None = None
    main_pattern = re.compile(r"^\s*(?:[_A-Za-z]\w*(?:::\w+)*[\s*&]+)*main\s*\(")
    for line_index, line in enumerate(lines):
        if brace_depth == 0 and main_pattern.match(line):
            main_line = line_index
            break
        if line.lstrip().startswith("#"):
            continue
        for token in tokens[line_index]:
            if token == "{":
                brace_depth += 1
            elif token == "}":
                brace_depth = max(0, brace_depth - 1)

    if main_line is None or include_end > main_line:
        return text

    previous = main_line - 1
    while previous >= 0 and not lines[previous].strip():
        previous -= 1
    if (
        include_end < len(lines)
        and lines[include_end].strip() == "namespace {"
        and previous >= 0
        and lines[previous].strip() == "}"
    ):
        return text

    return "".join(
        [
            *lines[:include_end],
            "namespace {\n",
            *lines[include_end:main_line],
            "}\n",
            *lines[main_line:],
        ]
    )


def structural_tokens(lines: Sequence[str]) -> list[list[str]]:
    result: list[list[str]] = []
    in_block_comment = False
    quote: str | None = None
    escaped = False
    raw_closer: str | None = None

    for line in lines:
        tokens: list[str] = []
        index = 0
        while index < len(line):
            if raw_closer is not None:
                end = line.find(raw_closer, index)
                if end < 0:
                    index = len(line)
                    continue
                index = end + len(raw_closer)
                raw_closer = None
                continue
            if in_block_comment:
                end = line.find("*/", index)
                if end < 0:
                    index = len(line)
                    continue
                index = end + 2
                in_block_comment = False
                continue

            ch = line[index]
            next_ch = line[index + 1] if index + 1 < len(line) else ""
            if quote is not None:
                if escaped:
                    escaped = False
                elif ch == "\\":
                    escaped = True
                elif ch == quote:
                    quote = None
                index += 1
                continue
            if ch == "/" and next_ch == "/":
                break
            if ch == "/" and next_ch == "*":
                in_block_comment = True
                index += 2
                continue
            if ch in {'"', "'"}:
                quote = ch
                index += 1
                continue
            if ch == "R" and next_ch == '"':
                open_paren = line.find("(", index + 2, index + 19)
                if open_paren >= 0:
                    delimiter = line[index + 2 : open_paren]
                    if all(c not in " ()\\\t\r\n" for c in delimiter):
                        raw_closer = ")" + delimiter + '"'
                        index = open_paren + 1
                        continue
            if ch in "()[]{};":
                tokens.append(ch)
            index += 1
        if quote is not None and not line.rstrip().endswith("\\"):
            quote = None
            escaped = False
        result.append(tokens)
    return result


def code_identifiers(text: str) -> Iterable[str]:
    in_block_comment = False
    quote: str | None = None
    escaped = False
    raw_closer: str | None = None
    index = 0

    while index < len(text):
        if raw_closer is not None:
            end = text.find(raw_closer, index)
            if end < 0:
                return
            index = end + len(raw_closer)
            raw_closer = None
            continue
        if in_block_comment:
            end = text.find("*/", index)
            if end < 0:
                return
            index = end + 2
            in_block_comment = False
            continue

        ch = text[index]
        next_ch = text[index + 1] if index + 1 < len(text) else ""
        if quote is not None:
            if escaped:
                escaped = False
            elif ch == "\\":
                escaped = True
            elif ch == quote:
                quote = None
            index += 1
            continue
        if ch == "/" and next_ch == "/":
            newline = text.find("\n", index + 2)
            if newline < 0:
                return
            index = newline + 1
            continue
        if ch == "/" and next_ch == "*":
            in_block_comment = True
            index += 2
            continue
        if ch == "R" and next_ch == '"':
            open_paren = text.find("(", index + 2, index + 19)
            if open_paren >= 0:
                delimiter = text[index + 2 : open_paren]
                if all(c not in " ()\\\t\r\n" for c in delimiter):
                    raw_closer = ")" + delimiter + '"'
                    index = open_paren + 1
                    continue
        if ch in {'"', "'"}:
            quote = ch
            index += 1
            continue
        if ch == "_" or ch.isalpha():
            end = index + 1
            while end < len(text) and (text[end] == "_" or text[end].isalnum()):
                end += 1
            yield text[index:end]
            index = end
            continue
        index += 1


def find_braced_entity_end(tokens: Sequence[Sequence[str]], start: int) -> int | None:
    paren_depth = 0
    bracket_depth = 0
    body_depth = 0
    body_opened = False
    for line_index in range(start, len(tokens)):
        for token in tokens[line_index]:
            if body_opened:
                if token == "{":
                    body_depth += 1
                elif token == "}":
                    body_depth -= 1
                    if body_depth == 0:
                        return line_index
                continue
            if token == "(":
                paren_depth += 1
            elif token == ")":
                paren_depth = max(0, paren_depth - 1)
            elif token == "[":
                bracket_depth += 1
            elif token == "]":
                bracket_depth = max(0, bracket_depth - 1)
            elif token == "{" and paren_depth == 0 and bracket_depth == 0:
                body_opened = True
                body_depth = 1
            elif token == ";" and paren_depth == 0 and bracket_depth == 0:
                return None
    return None


def find_statement_end(tokens: Sequence[Sequence[str]], start: int) -> int | None:
    paren_depth = bracket_depth = brace_depth = 0
    for line_index in range(start, len(tokens)):
        for token in tokens[line_index]:
            if token == "(":
                paren_depth += 1
            elif token == ")":
                paren_depth = max(0, paren_depth - 1)
            elif token == "[":
                bracket_depth += 1
            elif token == "]":
                bracket_depth = max(0, bracket_depth - 1)
            elif token == "{":
                brace_depth += 1
            elif token == "}":
                brace_depth = max(0, brace_depth - 1)
            elif token == ";" and paren_depth == bracket_depth == brace_depth == 0:
                return line_index
    return None


def template_prefix_start(lines: Sequence[str], start: int) -> int:
    if start <= 0 or not lines[start - 1].strip().endswith(">"):
        return start
    for line_index in range(start - 1, max(-1, start - 65), -1):
        stripped = lines[line_index].strip()
        if stripped.startswith("template"):
            return line_index
        if stripped.endswith(";") or stripped in {"{", "}"}:
            break
    return start


def ipa_removed_signatures(cgraph: str) -> set[str]:
    if "Initial Symbol table:" not in cgraph or "Removing unused symbols:" not in cgraph:
        return set()
    initial = cgraph.split("Initial Symbol table:", 1)[1].split(
        "Removing unused symbols:", 1
    )[0]
    signatures: dict[str, str] = {}
    for line in initial.splitlines():
        match = re.match(r"^.+/(\d+) \((.*)\)$", line)
        if match is not None:
            signatures[match.group(1)] = match.group(2)

    removed = cgraph.split("Removing unused symbols:", 1)[1].split(
        "Reclaimed Symbol table:", 1
    )[0]
    return {
        signatures[symbol_id]
        for symbol_id in re.findall(r"/(\d+)(?=\s|$)", removed)
        if symbol_id in signatures
    }


def ipa_function_locations(
    original: Path,
    *,
    signatures: set[str],
    source_name: str,
) -> list[tuple[int, str]]:
    result: set[tuple[int, str]] = set()
    signature: str | None = None
    in_function_decl = False
    saw_function_decl = False
    found_decl = False
    location_pattern = re.compile(
        rf"\bsrcp:\s+(?:.*[/\\])?{re.escape(source_name)}:(\d+)"
    )

    with original.open(encoding="utf-8", errors="replace") as dump:
        for line in dump:
            if line.startswith(";; Function "):
                header = line.rstrip()[len(";; Function ") :]
                candidate = header.rsplit(" (", 1)[0]
                signature = candidate if candidate in signatures else None
                in_function_decl = False
                saw_function_decl = False
                found_decl = False
                continue
            if signature is None or found_decl:
                continue
            if line.startswith("@"):
                is_function_decl = re.match(
                    r"^@\d+\s+function_decl\b", line
                ) is not None
                in_function_decl = is_function_decl and not saw_function_decl
                saw_function_decl = saw_function_decl or is_function_decl
            if not in_function_decl:
                continue
            match = location_pattern.search(line)
            if match is not None:
                result.add((int(match.group(1)), signature))
                found_decl = True
    return sorted(result)


@dataclass(frozen=True, order=True)
class Removal:
    start: int
    end: int
    label: str

    @property
    def size(self) -> int:
        return self.end - self.start + 1


def removal_from_function_line(
    lines: Sequence[str], tokens: Sequence[Sequence[str]], line_index: int, label: str
) -> Removal | None:
    if not 0 <= line_index < len(lines):
        return None
    end = find_braced_entity_end(tokens, line_index)
    if end is None:
        return None
    start = template_prefix_start(lines, line_index)
    return Removal(start, end, label)


def ipa_unused_candidates(
    text: str,
    *,
    compiler: CompilerWorkspace,
    keep_functions: set[str],
) -> list[Removal]:
    functions = compiler.ipa_unused_functions(text)
    if not functions:
        return []

    lines = text.splitlines(keepends=True)
    tokens = structural_tokens(lines)
    names_by_range: dict[tuple[int, int], set[str]] = {}
    for line_number, signature in functions:
        if "<lambda" in signature or "operator" in signature:
            continue
        names = re.findall(
            r"([_A-Za-z]\w*)\s*\(", signature.split(" [with ", 1)[0]
        )
        if not names:
            continue
        removal = removal_from_function_line(
            lines,
            tokens,
            line_number - 1,
            "unused function (IPA)",
        )
        if removal is None:
            continue
        name = names[-1]
        source = "".join(lines[removal.start : removal.end + 1])
        if name not in set(code_identifiers(source)):
            continue
        names_by_range.setdefault((removal.start, removal.end), set()).add(name)

    ranges_by_name: dict[str, set[tuple[int, int]]] = {}
    for source_range, names in names_by_range.items():
        for name in names:
            ranges_by_name.setdefault(name, set()).add(source_range)

    declarations_by_name: dict[str, set[tuple[int, int]]] = {}
    for line_index, line in enumerate(lines):
        if any(start <= line_index <= end for start, end in names_by_range):
            continue
        for name in ranges_by_name:
            if re.match(
                rf"^\s*(?:template\s*<[^;{{}}]*>\s*)?"
                rf"(?:[_A-Za-z]\w*(?:::[_A-Za-z]\w*|<[^;{{}}]*>)?[\s*&]+)+"
                rf"{re.escape(name)}\s*\([^;{{}}]*\)\s*"
                rf"(?:const\s*)?(?:noexcept(?:\([^;{{}}]*\))?\s*)?;\s*$",
                line,
            ):
                declarations_by_name.setdefault(name, set()).add(
                    (line_index, line_index)
                )

    covered = {
        line_index
        for start, end in names_by_range
        for line_index in range(start, end + 1)
    }
    covered.update(
        line_index
        for declarations in declarations_by_name.values()
        for start, end in declarations
        for line_index in range(start, end + 1)
    )
    outside_names = set(
        code_identifiers(
            "".join(
                line
                for line_index, line in enumerate(lines)
                if line_index not in covered
            )
        )
    )
    outside_names.update(keep_functions)

    graph: dict[tuple[int, int], set[tuple[int, int]]] = {
        source_range: set() for source_range in names_by_range
    }
    for source_range in graph:
        start, end = source_range
        identifiers = set(code_identifiers("".join(lines[start : end + 1])))
        for name in identifiers:
            graph[source_range].update(ranges_by_name.get(name, ()))

    reachable = {
        source_range
        for name in outside_names
        for source_range in ranges_by_name.get(name, ())
    }
    stack = list(reachable)
    while stack:
        source_range = stack.pop()
        for next_range in graph[source_range]:
            if next_range not in reachable:
                reachable.add(next_range)
                stack.append(next_range)

    unreachable = graph.keys() - reachable
    candidates = [
        Removal(start, end, "unused function group (IPA)")
        for start, end in unreachable
    ]
    for name, declarations in declarations_by_name.items():
        if ranges_by_name[name].issubset(unreachable):
            candidates.extend(
                Removal(start, end, "unused function declaration (IPA)")
                for start, end in declarations
            )
    deduplicated: list[Removal] = []
    for candidate in sorted(candidates, key=lambda value: (value.start, -value.end)):
        if any(
            existing.start <= candidate.start and candidate.end <= existing.end
            for existing in deduplicated
        ):
            continue
        deduplicated.append(candidate)
    return deduplicated


def unused_candidates(
    text: str,
    *,
    compiler: CompilerWorkspace,
    cppcheck: str | None,
    keep_functions: set[str],
    allow_pch: bool = True,
    reporter: Reporter,
) -> list[Removal]:
    lines = text.splitlines(keepends=True)
    tokens = structural_tokens(lines)
    removals: set[Removal] = set()

    cppcheck_result: subprocess.CompletedProcess[str] | None = None
    if cppcheck is None:
        compiler_output = compiler.compiler_warnings(text, allow_pch=allow_pch)
    else:
        compiler.write_analysis_source(text)

        def run_cppcheck() -> subprocess.CompletedProcess[str]:
            return run_command(
                [
                    cppcheck,
                    f"--std={compiler.standard}",
                    "--enable=unusedFunction",
                    "--suppress=missingIncludeSystem",
                    "--template=gcc",
                    str(compiler.analysis_source),
                ],
                timeout=compiler.timeout,
            )

        with ThreadPoolExecutor(max_workers=2) as executor:
            compiler_future = executor.submit(
                compiler.compiler_warnings, text, allow_pch=allow_pch
            )
            cppcheck_future = executor.submit(run_cppcheck)
            compiler_output = compiler_future.result()
            cppcheck_result = cppcheck_future.result()

    warning_pattern = re.compile(
        rf"^{re.escape(str(compiler.source))}:(\d+):\d+: warning:.*"
        r"\[-Wunused-(function|macros)\]$"
    )
    for warning in compiler_output.splitlines():
        match = warning_pattern.match(warning)
        if not match:
            continue
        line_index = int(match.group(1)) - 1
        if match.group(2) == "macros":
            end = line_index
            while end + 1 < len(lines) and lines[end].rstrip().endswith("\\"):
                end += 1
            removals.add(Removal(line_index, end, "unused macro"))
        else:
            removal = removal_from_function_line(
                lines, tokens, line_index, "unused function (compiler)"
            )
            if removal is not None:
                removals.add(removal)

    if cppcheck_result is not None:
        cppcheck_pattern = re.compile(
            rf"^{re.escape(str(compiler.analysis_source))}:(\d+):\d+:.*\[unusedFunction\]$"
        )
        function_name = re.compile(r"(?:function|Function) ['\"]([^'\"]+)['\"]")
        for warning in cppcheck_result.stderr.splitlines():
            match = cppcheck_pattern.match(warning)
            if not match:
                continue
            name_match = function_name.search(warning)
            if name_match and name_match.group(1) in keep_functions:
                continue
            line_index = int(match.group(1)) - 1
            removal = removal_from_function_line(
                lines, tokens, line_index, "unused function (cppcheck)"
            )
            if removal is not None:
                removals.add(removal)
        if cppcheck_result.returncode not in {0, 1}:
            reporter.detail(redact_home(cppcheck_result.stderr))

    counts = Counter(code_identifiers(text))
    macro_pattern = re.compile(r"^\s*#\s*define\s+([_A-Za-z]\w*)\b")
    struct_pattern = re.compile(r"^\s*struct\s+([_A-Za-z]\w*)\b")
    using_pattern = re.compile(r"^\s*using\s+([_A-Za-z]\w*)\s*=")
    for line_index, line in enumerate(lines):
        macro_match = macro_pattern.match(line)
        if macro_match and counts[macro_match.group(1)] == 1:
            end = line_index
            while end + 1 < len(lines) and lines[end].rstrip().endswith("\\"):
                end += 1
            removals.add(Removal(line_index, end, "unused macro (text scan)"))
        struct_match = struct_pattern.match(line)
        if struct_match and counts[struct_match.group(1)] == 1:
            end = find_braced_entity_end(tokens, line_index)
            if end is not None:
                removals.add(
                    Removal(
                        template_prefix_start(lines, line_index),
                        end,
                        f"unused struct {struct_match.group(1)}",
                    )
                )
        using_match = using_pattern.match(line)
        if using_match and counts[using_match.group(1)] == 1:
            end = find_statement_end(tokens, line_index)
            if end is not None:
                removals.add(
                    Removal(
                        template_prefix_start(lines, line_index),
                        end,
                        f"unused using {using_match.group(1)}",
                    )
                )

    ordered = sorted(removals)
    deduplicated: list[Removal] = []
    for removal in ordered:
        if any(
            existing.start <= removal.start and removal.end <= existing.end
            for existing in deduplicated
        ):
            continue
        deduplicated.append(removal)
    return deduplicated


def apply_removals(text: str, removals: Iterable[Removal]) -> str:
    lines = text.splitlines(keepends=True)
    use = [True] * len(lines)
    for removal in removals:
        for line_index in range(max(0, removal.start), min(len(lines), removal.end + 1)):
            use[line_index] = False
    return "".join(line for line_index, line in enumerate(lines) if use[line_index])


def reduce_unused_candidates(
    text: str,
    candidates: Sequence[Removal],
    *,
    compiler: CompilerWorkspace,
    writer: CheckpointWriter,
    reporter: Reporter,
    target_bytes: int | None = None,
    stage: str = "cleanup",
) -> tuple[str, int]:
    started = time.monotonic()
    accepted: set[Removal] = set()
    cache: dict[frozenset[Removal], tuple[bool, str]] = {}
    validations = 0
    target_checkpointed = below_byte_limit(text, target_bytes) or (
        writer.current is not None
        and below_byte_limit(writer.current, target_bytes)
    )

    byte_sizes = {
        candidate: len(
            "".join(text.splitlines(keepends=True)[candidate.start : candidate.end + 1]).encode()
        )
        for candidate in candidates
    }

    def split(items: Sequence[Removal]) -> tuple[list[Removal], list[Removal]]:
        keys = {str(index): byte_sizes[item] for index, item in enumerate(items)}
        left_keys, _ = split_weighted(list(keys), keys)
        count = len(left_keys)
        return list(items[:count]), list(items[count:])

    def try_accept(batch: Sequence[Removal]) -> bool:
        nonlocal text, target_checkpointed, validations
        proposal = frozenset(accepted.union(batch))
        if proposal == frozenset(accepted):
            return True
        candidate_text = collapse_blank_lines(
            apply_removals(original_text, proposal)
        )
        cached = cache.get(proposal)
        if cached is None:
            validations += 1
            reporter.phase(
                stage,
                f"validation {validations}: testing {len(batch)} candidate(s)",
            )
            valid, stderr = compiler.validate(candidate_text)
            cache[proposal] = valid, stderr
        else:
            valid, stderr = cached
        if not valid:
            reporter.detail(first_error(stderr))
            return False
        accepted.update(batch)
        text = candidate_text
        writer.save(f"removed {len(batch)} unused declaration(s)", text)
        if not target_checkpointed and below_byte_limit(text, target_bytes):
            target_checkpointed = True
            reporter.checkpoint(
                f"submission ready: {reporter.byte_count(encoded_size(text), 'green')} "
                f"< {reporter.byte_count(target_bytes, 'yellow')}; cleanup continues"
            )
        if reporter.verbose:
            for candidate in batch:
                reporter.detail(
                    f"  lines {candidate.start + 1}-{candidate.end + 1}: {candidate.label}"
                )
        return True

    def reduce_batch(items: Sequence[Removal]) -> None:
        active = [item for item in items if item not in accepted]
        if not active:
            return
        if try_accept(active):
            return
        if len(active) == 1:
            return
        left, right = split(active)
        reduce_batch(left)
        reduce_batch(right)

    original_text = text
    reduce_batch(candidates)
    reporter.done(
        stage,
        f"accepted {len(accepted)}/{len(candidates)} candidate(s)",
        started,
    )
    return text, len(accepted)


def cleanup_text(
    text: str,
    *,
    compiler: CompilerWorkspace,
    writer: CheckpointWriter,
    cppcheck: str | None,
    keep_functions: set[str],
    max_passes: int,
    safe_only: bool,
    require_validation: bool,
    reporter: Reporter,
    target_bytes: int | None = None,
) -> str:
    target_checkpointed = below_byte_limit(text, target_bytes) or (
        writer.current is not None
        and below_byte_limit(writer.current, target_bytes)
    )

    def report_target_checkpoint() -> None:
        nonlocal target_checkpointed
        if target_checkpointed or not below_byte_limit(text, target_bytes):
            return
        target_checkpointed = True
        reporter.checkpoint(
            f"submission ready: {reporter.byte_count(encoded_size(text), 'green')} "
            f"< {reporter.byte_count(target_bytes, 'yellow')}; cleanup continues"
        )

    if target_bytes is not None:
        quick_candidate = collapse_blank_lines(strip_cleanup_comments(text))
        if below_byte_limit(quick_candidate, target_bytes):
            started = time.monotonic()
            reporter.phase("cleanup", "validating a quick comment-only checkpoint")
            valid, stderr = compiler.syntax(quick_candidate)
            if valid:
                if quick_candidate != text:
                    text = quick_candidate
                    writer.save("comments and blank lines", text)
                report_target_checkpoint()
            else:
                reporter.detail(
                    "quick cleanup was rejected: " + first_error(stderr)
                )
            reporter.done("cleanup", "quick checkpoint checked", started)

    started = time.monotonic()
    reporter.phase("cleanup", "processing comments and conditional branches")
    safe_candidate = safe_cleanup_candidate(text, compiler)
    if below_byte_limit(safe_candidate, target_bytes):
        if safe_candidate != text or require_validation:
            valid, stderr = compiler.syntax(safe_candidate)
            if not valid:
                raise ToolError(
                    f"safe cleanup did not validate: {first_error(stderr)}"
                )
        if safe_candidate != text:
            text = safe_candidate
            writer.save("comments, inactive branches, and blank lines", text)
        report_target_checkpoint()
    reporter.done(
        "cleanup",
        f"preprocessing complete: "
        f"{reporter.byte_count(encoded_size(safe_candidate), 'cyan')}",
        started,
    )

    if safe_only:
        if safe_candidate != text:
            valid, stderr = compiler.validate(safe_candidate)
            if not valid:
                raise ToolError(f"safe cleanup did not validate: {first_error(stderr)}")
            text = safe_candidate
            writer.save("comments, inactive branches, and blank lines", text)
        elif require_validation:
            valid, stderr = compiler.validate(text)
            if not valid:
                raise ToolError(f"the input file does not compile: {first_error(stderr)}")
        return text

    safe_changed = safe_candidate != text
    text = safe_candidate
    pending_validation = safe_changed or require_validation

    started = time.monotonic()
    reporter.phase("namespace", "checking anonymous namespace insertion")
    namespace_candidate = insert_anonymous_namespace(text)
    if namespace_candidate != text:
        valid, stderr = compiler.validate(namespace_candidate)
        if valid:
            text = namespace_candidate
            writer.save("anonymous namespace", text)
            pending_validation = False
        else:
            reporter.detail(
                "anonymous namespace was not inserted: " + first_error(stderr)
            )
    reporter.done("namespace", "anonymous namespace check complete", started)

    def validate_pending() -> None:
        nonlocal pending_validation
        if not pending_validation:
            return
        valid, stderr = compiler.validate(text)
        if not valid:
            label = "safe cleanup" if safe_changed else "the input file"
            raise ToolError(f"{label} does not compile: {first_error(stderr)}")
        if safe_changed:
            writer.save("comments, inactive branches, and blank lines", text)
        pending_validation = False

    def run_unused_pass(
        analysis_cppcheck: str | None,
        stage: str,
    ) -> tuple[bool, int]:
        nonlocal text, pending_validation
        started = time.monotonic()
        analyzers = "GCC" + (" + cppcheck" if analysis_cppcheck else "")
        reporter.phase(stage, f"finding unused declarations with {analyzers}")
        candidates = unused_candidates(
            text,
            compiler=compiler,
            cppcheck=analysis_cppcheck,
            keep_functions=keep_functions,
            allow_pch=not pending_validation,
            reporter=reporter,
        )
        reporter.done(
            stage,
            f"found {len(candidates)} candidate(s)",
            started,
        )
        if not candidates:
            validate_pending()
            return False, 0
        text, accepted = reduce_unused_candidates(
            text,
            candidates,
            compiler=compiler,
            writer=writer,
            reporter=reporter,
            target_bytes=target_bytes,
            stage=stage,
        )
        if accepted == 0:
            validate_pending()
        else:
            pending_validation = False
        return True, accepted

    for pass_index in range(1, max_passes + 1):
        stage = f"pass {pass_index}/{max_passes}"
        had_candidates, accepted = run_unused_pass(cppcheck, stage)
        if not had_candidates:
            reporter.detail(f"unused cleanup converged after pass {pass_index}")
            break
        if accepted == 0:
            reporter.detail(f"no unused candidate was accepted in pass {pass_index}")
            break
    validate_pending()

    started = time.monotonic()
    reporter.phase("IPA", "finding unreachable recursive function groups")
    ipa_candidates = ipa_unused_candidates(
        text,
        compiler=compiler,
        keep_functions=keep_functions,
    )
    reporter.done(
        "IPA",
        f"found {len(ipa_candidates)} candidate(s)",
        started,
    )
    if ipa_candidates:
        text, accepted = reduce_unused_candidates(
            text,
            ipa_candidates,
            compiler=compiler,
            writer=writer,
            reporter=reporter,
            target_bytes=target_bytes,
            stage="IPA",
        )
        reporter.detail(
            f"IPA cleanup accepted {accepted}/{len(ipa_candidates)} candidate(s)"
        )
    return text


def default_include_paths(repo_root: Path, source: Path) -> list[Path]:
    candidates = [
        source.parent,
        repo_root,
        repo_root.parent / "cpp-dump-main",
        repo_root.parent.parent / "ac-library",
    ]
    result: list[Path] = []
    seen: set[Path] = set()
    for candidate in candidates:
        resolved = candidate.resolve()
        if resolved in seen or not resolved.is_dir():
            continue
        seen.add(resolved)
        result.append(resolved)
    return result


def merge_include_paths(defaults: Sequence[Path], provided: Sequence[str]) -> list[Path]:
    result: list[Path] = []
    seen: set[Path] = set()
    for path in [*defaults, *(Path(value) for value in provided)]:
        resolved = path.expanduser().resolve()
        if resolved in seen:
            continue
        if not resolved.is_dir():
            raise ToolError(f"include directory does not exist: {path}")
        seen.add(resolved)
        result.append(resolved)
    return result


class FastBundler:
    """Expand ordinary local includes without invoking a compiler for every file."""

    def __init__(
        self,
        include_paths: Sequence[Path],
        *,
        library_root: Path | None = None,
    ) -> None:
        self.include_paths = [path.resolve() for path in include_paths]
        self.library_root = (
            library_root.resolve() if library_root is not None else None
        )
        self.once: set[Path] = set()
        self.system_includes: set[str] = set()
        self.stack: list[Path] = []

    def _resolve(self, name: str, included_from: Path) -> Path:
        for root in [included_from.parent, *self.include_paths]:
            candidate = (root / name).resolve()
            if candidate.is_file():
                return candidate
        raise FastBundleUnsupported(f'local include was not found: "{name}"')

    @staticmethod
    def _include_guard_lines(lines: Sequence[str]) -> set[int]:
        meaningful = [index for index, line in enumerate(lines) if line.strip()]
        if len(meaningful) < 3:
            return set()

        first, define, last = meaningful[0], meaningful[1], meaningful[-1]
        match = INCLUDE_GUARD_OPEN_DIRECTIVE.match(lines[first].rstrip("\r\n"))
        if match is None:
            return set()
        name = match.group(1)
        if re.match(
            rf"^\s*#\s*define\s+{re.escape(name)}(?:\s+.*)?$",
            lines[define].rstrip("\r\n"),
        ) is None:
            return set()
        if CONDITIONAL_CLOSE_DIRECTIVE.match(lines[last].rstrip("\r\n")) is None:
            return set()

        depth = 0
        for index in range(first, last + 1):
            directive = lines[index].rstrip("\r\n")
            if CONDITIONAL_OPEN_DIRECTIVE.match(directive):
                depth += 1
            elif CONDITIONAL_BRANCH_DIRECTIVE.match(directive) and depth == 1:
                return set()
            elif CONDITIONAL_CLOSE_DIRECTIVE.match(directive):
                depth -= 1
                if depth < 0 or (depth == 0 and index != last):
                    return set()
        if depth != 0:
            return set()
        return {first, define, last}

    def _expand(self, path: Path, *, add_source_link: bool = True) -> str:
        path = path.resolve()
        try:
            text = path.read_text(encoding="utf-8")
        except (OSError, UnicodeError) as exc:
            raise FastBundleUnsupported(f"could not read {path.name}: {exc}") from exc
        if not text.endswith("\n"):
            text += "\n"

        lines = text.splitlines(keepends=True)
        uncommented_lines = preprocessor_lines(text)
        if len(lines) != len(uncommented_lines):
            raise FastBundleUnsupported(
                f"comment parsing changed the line count in {path.name}"
            )

        has_pragma_once = any(
            PRAGMA_ONCE_DIRECTIVE.match(line.rstrip("\r\n"))
            for line in uncommented_lines
        )
        include_guard_lines = self._include_guard_lines(uncommented_lines)
        guarded_once = has_pragma_once or bool(include_guard_lines)
        if guarded_once and path in self.once:
            return ""
        if guarded_once:
            self.once.add(path)
        if path in self.stack:
            chain = " -> ".join(item.name for item in [*self.stack, path])
            raise FastBundleUnsupported(f"include cycle without #pragma once: {chain}")

        self.stack.append(path)
        output: list[str] = []
        if add_source_link:
            url = library_source_url(path, self.library_root)
            if url is not None:
                output.append(source_link_comment(url))
        conditional_depth = 0
        try:
            for line_index, (line, uncommented_line) in enumerate(
                zip(lines, uncommented_lines)
            ):
                directive = uncommented_line.rstrip("\r\n")

                if (
                    PRAGMA_ONCE_DIRECTIVE.match(directive)
                    or line_index in include_guard_lines
                ):
                    continue

                local_include = LOCAL_INCLUDE_DIRECTIVE.match(directive)
                if local_include:
                    if conditional_depth != 0:
                        raise FastBundleUnsupported(
                            f"conditional local include in {path.name}"
                        )
                    included = self._resolve(local_include.group(1), path)
                    output.append(self._expand(included))
                    continue

                if INCLUDE_DIRECTIVE.match(directive) and '"' in directive:
                    raise FastBundleUnsupported(
                        f"unsupported local include syntax in {path.name}"
                    )

                system_include = SYSTEM_INCLUDE_DIRECTIVE.match(directive)
                if system_include and conditional_depth == 0:
                    name = system_include.group(1)
                    if name in self.system_includes:
                        continue
                    self.system_includes.add(name)

                if re.match(r'^\s*#\s*(?:line\s+)?\d+\s+"', directive):
                    continue

                output.append(line)
                if CONDITIONAL_OPEN_DIRECTIVE.match(directive):
                    conditional_depth += 1
                elif CONDITIONAL_CLOSE_DIRECTIVE.match(directive):
                    conditional_depth -= 1
                    if conditional_depth < 0:
                        raise FastBundleUnsupported(
                            f"unmatched #endif in {path.name}"
                        )

            if conditional_depth != 0:
                raise FastBundleUnsupported(f"unmatched conditional in {path.name}")
            return "".join(output)
        finally:
            self.stack.pop()

    def bundle(self, source: Path) -> str:
        return self._expand(source, add_source_link=False)


def run_oj_bundle(
    *,
    executable: str,
    source: Path,
    include_paths: Sequence[Path],
    cxx: str,
) -> str:
    command = [executable]
    for include_path in include_paths:
        command.extend(["-I", str(include_path)])
    command.append(source.name)
    env = os.environ.copy()
    env["CXX"] = cxx
    result = run_command(command, cwd=source.parent, env=env)
    if result.returncode != 0:
        raise ToolError(f"oj-bundle failed:\n{redact_home(result.stderr).rstrip()}")
    return result.stdout


def privacy_check_line_directives(text: str) -> None:
    home = str(Path.home())
    user = Path.home().name
    for line in text.splitlines():
        if not LINE_DIRECTIVE.match(line):
            continue
        if home in line or f"/{user}/" in line:
            raise ToolError("a #line directive contains private home-directory information")


def common_compiler_from_args(
    args: argparse.Namespace,
    *,
    include_paths: Sequence[Path],
    reporter: Reporter,
) -> CompilerWorkspace:
    cxx = resolve_executable(args.cxx, ("g++-15", "g++-14", "g++"))
    timeout = None if args.timeout == 0 else args.timeout
    return CompilerWorkspace(
        cxx=cxx,
        standard=normalize_standard(args.std),
        include_paths=include_paths,
        extra_args=args.cxx_arg,
        timeout=timeout,
        reporter=reporter,
    )


def expand_command(args: argparse.Namespace, repo_root: Path) -> None:
    reporter = Reporter(args.verbose, args.color)
    source = Path(args.source).expanduser().resolve()
    output = Path(args.output).expanduser().resolve()
    if not source.is_file():
        raise ToolError(f"source file does not exist: {source}")
    if source == output:
        raise ToolError("bundle output must differ from the source file")

    include_paths = merge_include_paths(
        default_include_paths(repo_root, source), args.include
    )
    started = time.monotonic()
    method = "fast"
    reporter.phase("bundle", "expanding local includes")
    try:
        bundled = FastBundler(include_paths, library_root=repo_root).bundle(source)
    except FastBundleUnsupported as exc:
        if args.no_fallback:
            raise
        method = "oj-bundle fallback"
        reporter.warning(f"fast expansion unsupported: {exc}")
        reporter.phase("bundle", "running oj-bundle compatibility fallback")
        cxx = resolve_executable(args.cxx, ("g++-15", "g++-14", "g++"))
        oj_bundle = discover_oj_bundle(repo_root, args.oj_bundle)
        raw_bundle = run_oj_bundle(
            executable=oj_bundle,
            source=source,
            include_paths=include_paths,
            cxx=cxx,
        )
        resolver = OriginResolver(
            base_dir=source.parent,
            source=source,
            include_paths=include_paths,
        )
        bundled = BundleDocument.parse(
            raw_bundle,
            resolver=resolver,
            library_root=repo_root,
        ).render(
            set(),
            keep_line=False,
        )

    privacy_check_line_directives(bundled)
    atomic_write(output, bundled)
    reporter.done(
        "bundle",
        f"{method}: {reporter.byte_count(len(bundled.encode()), 'green')}",
        started,
    )


def bundle_command(args: argparse.Namespace, repo_root: Path) -> None:
    reporter = Reporter(args.verbose, args.color)
    source = Path(args.source).expanduser().resolve()
    output = Path(args.output).expanduser().resolve()
    if not source.is_file():
        raise ToolError(f"source file does not exist: {source}")
    if source == output:
        raise ToolError("bundle output must differ from the source file")

    include_paths = merge_include_paths(
        default_include_paths(repo_root, source), args.include
    )
    writer = CheckpointWriter(output, reporter)
    command_started = time.monotonic()

    with common_compiler_from_args(
        args, include_paths=include_paths, reporter=reporter
    ) as compiler:
        started = time.monotonic()
        reporter.phase("bundle", "running oj-bundle")
        oj_bundle = discover_oj_bundle(repo_root, args.oj_bundle)
        raw_bundle = run_oj_bundle(
            executable=oj_bundle,
            source=source,
            include_paths=include_paths,
            cxx=compiler.cxx,
        )
        reporter.done("bundle", "oj-bundle completed", started)
        resolver = OriginResolver(
            base_dir=source.parent,
            source=source,
            include_paths=include_paths,
        )
        protected = read_direct_local_includes(
            source,
            include_paths=include_paths,
            resolver=resolver,
        )
        document = BundleDocument.parse(
            raw_bundle,
            resolver=resolver,
            protected=protected,
            library_root=repo_root,
        )
        for line in document.lines:
            if line.origin_key is None:
                continue
            if re.match(
                r"^\s*#\s*include\s*<bits/stdc\+\+\.h>\s*$",
                line.text,
            ):
                document.protected.add(line.origin_key)

        initial = document.render(set(), keep_line=True)
        privacy_check_line_directives(initial)
        started = time.monotonic()
        reporter.phase("bundle", "validating the initial bundle")
        valid, stderr = compiler.validate(initial)
        if not valid:
            raise ToolError(f"the initial bundle does not compile: {first_error(stderr)}")
        reporter.done("bundle", "initial bundle validated", started)
        writer.save("bundled", initial, force=True)

        if args.no_prune_headers:
            removed: set[str] = set()
        else:
            reporter.phase("headers", "checking removable bundled headers")
            removed = prune_header_origins(
                document,
                compiler=compiler,
                writer=writer,
                keep_patterns=args.keep_header,
                keep_line=True,
                max_passes=args.header_passes,
                reporter=reporter,
            )

        bundled = document.render(removed, keep_line=args.keep_line)
        privacy_check_line_directives(bundled)
        if bundled != writer.current:
            valid, stderr = compiler.validate(bundled)
            if not valid:
                raise ToolError(f"line cleanup did not validate: {first_error(stderr)}")
            writer.save("line directives", bundled)

        if not args.no_safe_cleanup:
            cppcheck = None
            if args.aggressive:
                cppcheck = resolve_executable(args.cppcheck, ("cppcheck",))
            bundled = cleanup_text(
                bundled,
                compiler=compiler,
                writer=writer,
                cppcheck=cppcheck,
                keep_functions=DEFAULT_KEEP_FUNCTIONS.union(args.keep_function),
                max_passes=args.cleanup_passes,
                safe_only=not args.aggressive,
                require_validation=False,
                reporter=reporter,
            )

        writer.save("final", bundled, force=True)
    reporter.done(
        "bundle",
        f"finished at {reporter.byte_count(encoded_size(bundled), 'green')}",
        command_started,
    )


def cleanup_command(args: argparse.Namespace, repo_root: Path) -> None:
    reporter = Reporter(args.verbose, args.color)
    command_started = time.monotonic()
    path = Path(args.file).expanduser().resolve()
    if not path.is_file():
        raise ToolError(f"file does not exist: {path}")
    text = path.read_text(encoding="utf-8")
    writer = CheckpointWriter(path, reporter)
    writer.current = text
    target_bytes = None if args.target_bytes == 0 else args.target_bytes
    if target_bytes is not None and target_bytes <= 0:
        raise ToolError("target bytes must be positive, or 0 to disable the target")
    writer.preserve_below = target_bytes
    target_description = (
        "disabled"
        if target_bytes is None
        else f"<{reporter.byte_count(target_bytes, 'yellow')}"
    )
    reporter.phase(
        "cleanup",
        f"starting at {reporter.byte_count(encoded_size(text), 'cyan')}; "
        f"target {target_description}",
    )
    if below_byte_limit(text, target_bytes):
        reporter.checkpoint(
            f"submission already ready: "
            f"{reporter.byte_count(encoded_size(text), 'green')} < "
            f"{reporter.byte_count(target_bytes, 'yellow')}; cleanup continues"
        )

    include_paths = merge_include_paths(
        default_include_paths(repo_root, path), args.include
    )
    with common_compiler_from_args(
        args, include_paths=include_paths, reporter=reporter
    ) as compiler:
        resolver = OriginResolver(
            base_dir=path.parent,
            source=path,
            include_paths=include_paths,
        )
        document = BundleDocument.parse(text, resolver=resolver)
        sanitized = document.render(set(), keep_line=True)
        privacy_check_line_directives(sanitized)
        if sanitized != text:
            text = sanitized

        cppcheck = None
        if not args.safe_only:
            cppcheck = resolve_executable(args.cppcheck, ("cppcheck",))
        cleaned = cleanup_text(
            text,
            compiler=compiler,
            writer=writer,
            cppcheck=cppcheck,
            keep_functions=DEFAULT_KEEP_FUNCTIONS.union(args.keep_function),
            max_passes=args.cleanup_passes,
            safe_only=args.safe_only,
            require_validation=True,
            reporter=reporter,
            target_bytes=target_bytes,
        )
        writer.save("final", cleaned)
        checkpoint_available = below_byte_limit(cleaned, target_bytes) or (
            writer.current is not None
            and below_byte_limit(writer.current, target_bytes)
        )
        if target_bytes is not None and not checkpoint_available:
            reporter.warning(
                f"target not reached: "
                f"{reporter.byte_count(encoded_size(cleaned), 'red')} >= "
                f"{reporter.byte_count(target_bytes, 'yellow')}"
            )
    reporter.done(
        "cleanup",
        f"finished at {reporter.byte_count(encoded_size(cleaned), 'green')}",
        command_started,
    )


def add_reporter_options(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument(
        "--color",
        choices=("auto", "always", "never"),
        default="auto",
        help="colored progress output (default: auto)",
    )


def add_compiler_options(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--cxx", help="C++ compiler (default: g++-15, g++-14, g++)")
    parser.add_argument("--std", default="c++23", help="target C++ standard")
    parser.add_argument(
        "--cxx-arg",
        action="append",
        default=[],
        metavar="ARG",
        help="additional compiler argument; use --cxx-arg=-DNAME for leading '-'",
    )
    parser.add_argument("-I", "--include", action="append", default=[], metavar="DIR")
    parser.add_argument(
        "--timeout",
        type=float,
        default=0,
        help="seconds per external command; 0 disables the timeout",
    )
    add_reporter_options(parser)


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Bundle and reduce C++ submission code with validated checkpoints."
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    expand = subparsers.add_parser(
        "expand", help="quickly expand ordinary local includes"
    )
    expand.add_argument("source")
    expand.add_argument("-o", "--output", default="bundle.cpp")
    expand.add_argument("--oj-bundle", help="path used by the compatibility fallback")
    expand.add_argument("--cxx", help="compiler used by the compatibility fallback")
    expand.add_argument("-I", "--include", action="append", default=[], metavar="DIR")
    expand.add_argument(
        "--no-fallback",
        action="store_true",
        help="fail instead of using oj-bundle for unsupported include syntax",
    )
    add_reporter_options(expand)

    bundle = subparsers.add_parser("bundle", help="run oj-bundle and reduce the result")
    bundle.add_argument("source")
    bundle.add_argument("-o", "--output", default="bundle.cpp")
    bundle.add_argument("--oj-bundle", help="path to oj-bundle")
    bundle.add_argument("--no-prune-headers", action="store_true")
    bundle.add_argument("--header-passes", type=int, default=1)
    bundle.add_argument(
        "--keep-header",
        action="append",
        default=[],
        metavar="GLOB",
        help="do not remove matching sanitized header paths",
    )
    bundle.add_argument(
        "--keep-line",
        action="store_true",
        help="keep sanitized #line directives in the final output",
    )
    bundle.add_argument("--no-safe-cleanup", action="store_true")
    bundle.add_argument(
        "--aggressive",
        action="store_true",
        help="also remove declarations reported as unused",
    )
    bundle.add_argument("--cleanup-passes", type=int, default=3)
    bundle.add_argument("--cppcheck", help="cppcheck executable")
    bundle.add_argument("--keep-function", action="append", default=[])
    add_compiler_options(bundle)

    cleanup = subparsers.add_parser("cleanup", help="reduce an existing bundle in place")
    cleanup.add_argument("file")
    cleanup.add_argument(
        "--safe-only",
        action="store_true",
        help="only remove comments and inactive branches and collapse blank lines",
    )
    cleanup.add_argument("--cleanup-passes", type=int, default=3)
    cleanup.add_argument(
        "--target-bytes",
        type=int,
        default=DEFAULT_BYTE_LIMIT,
        help=(
            "write a checkpoint as soon as the file is smaller than this many "
            "bytes, then continue cleanup (default: 65536; 0 disables the checkpoint)"
        ),
    )
    cleanup.add_argument("--cppcheck", help="cppcheck executable")
    cleanup.add_argument("--keep-function", action="append", default=[])
    add_compiler_options(cleanup)

    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(sys.argv[1:] if argv is None else argv)
    repo_root = Path(__file__).resolve().parent.parent
    try:
        if args.command == "expand":
            expand_command(args, repo_root)
        elif args.command == "bundle":
            bundle_command(args, repo_root)
        else:
            cleanup_command(args, repo_root)
    except ToolError as exc:
        Reporter(color=getattr(args, "color", "auto")).error(str(exc))
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
