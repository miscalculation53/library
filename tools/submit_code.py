#!/usr/bin/env python3
from __future__ import annotations

import argparse
from bisect import bisect_left
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
import fnmatch
import hashlib
import io
import json
import os
from pathlib import Path
import re
import secrets
import shlex
import shutil
import subprocess
import sys
import tempfile
import time
from typing import Iterable, Sequence
from urllib.parse import quote

import bundle_fast


LINE_DIRECTIVE = re.compile(
    r'^\s*#\s*line\s+(\d+)\s+("(?:\\.|[^"\\])*")(?:\s+.*)?$'
)
GNU_LINE_MARKER = re.compile(
    r'^\s*#\s+(\d+)\s+("(?:\\.|[^"\\])*")(?:\s+.*)?$'
)
LINE_CONTROL = re.compile(r"^\s*(?:#|%:)\s*(?:line\b|[0-9])")
BARE_LINE_CONTROL = re.compile(r"^\s*#\s*line\s+([0-9]+)\s*$")
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
RAW_STRING_OPEN = re.compile(r'(?:u8|u|U|L)?R"([^ ()\\\t\r\n]{0,16})\(')
# Consume the complete preprocessing number before looking for character
# literals. This includes hexadecimal digits, digit separators, and exponents.
CPP_NUMBER = re.compile(r"(?:[0-9]|\.[0-9])(?:[\w.]|'\w|(?<=[eEpP])[+-])*")
CODE_IDENTIFIER_TOKEN = re.compile(r'R"|[_A-Za-z]\w*|' + CPP_NUMBER.pattern + r'|//|/\*|["\']')
CODE_QUOTE_TOKEN = re.compile(r'\\.|["\']', re.DOTALL)
COMMENT_TOKEN = re.compile(RAW_STRING_OPEN.pattern + r'|//|/\*|' + CPP_NUMBER.pattern + r'|\b[_A-Za-z]\w*\b|["\']')
COMMENT_LINE_END = re.compile(r'[\r\n]')
PREPROCESSOR_TOKEN = re.compile(RAW_STRING_OPEN.pattern + r'|' + CPP_NUMBER.pattern + r'|["\']')
DEFAULT_KEEP_FUNCTIONS = {"read", "write", "print", "init", "main2", "test"}
DEFAULT_BYTE_LIMIT = 65536
PRIORITY_BYTE_LIMIT = 131072
LIBRARY_LAYOUT_DELAY = 10.0
LIBRARY_REPOSITORY_URL = "https://github.com/miscalculation53/library"
LIBRARY_SOURCE_BASE = LIBRARY_REPOSITORY_URL + "/tree/wip/"
LEGACY_LIBRARY_SITE_BASE = "https://miscalculation53.github.io/library/"
LIBRARY_SOURCE_LINK = re.compile(
    rf"^\s*//\s*((?:{re.escape(LIBRARY_REPOSITORY_URL)}/?|{re.escape(LIBRARY_SOURCE_BASE)}\S+\.hpp|"
    rf"{re.escape(LEGACY_LIBRARY_SITE_BASE)}\S+\.html#unbundled))\s*$"
)
SOURCE_LINK_PRAGMA = re.compile(
    r"^\s*#\s*pragma\s+submit_code_source_link\s+(\d+)\s*$"
)
SUBMISSION_PREFIX = "// submit-code: "
SUBMISSION_FIELDS = (
    r"source=(?P<source>[0-9a-f]{64}) input=(?P<input>[0-9a-f]{64}) body=(?P<body>[0-9a-f]{64})"
)
SUBMISSION_HEADER = re.compile(
    re.escape(SUBMISSION_PREFIX) + SUBMISSION_FIELDS + r"\n"
)
SUBMISSION_FOOTER = re.compile(
    r"\n\n" + re.escape(SUBMISSION_PREFIX) + r"hash=(?P<hash>[0-9a-f]{12}) "
    + SUBMISSION_FIELDS + r"\n\Z"
)
SUBMISSION_TRAILER = re.compile(
    r"\n\n" + re.escape(SUBMISSION_PREFIX) + r"[^\n]*\n?\Z"
)


class ToolError(RuntimeError):
    pass


class FastBundleUnsupported(ToolError):
    pass


def diagnostic_messages(diagnostics: str) -> str:
    return "\n".join(line for line in diagnostics.splitlines() if re.search(
        r"\b(?:fatal error|error):|used but never defined|undefined reference|Undefined symbols|^\s+\".*referenced from:",
        line,
    ))


@dataclass(frozen=True)
class SubmissionStamp:
    source_digest: str
    input_digest: str

    def render(self, text: str) -> str:
        body_digest = hashlib.sha256(text.encode()).hexdigest()
        # Blank lines keep the footer separate even after a trailing backslash.
        return (
            text + f"\n\n{SUBMISSION_PREFIX}hash={self.source_digest[:12]} source={self.source_digest} "
            f"input={self.input_digest} body={body_digest}\n"
        )

    @staticmethod
    def detach(text: str) -> tuple[str, bool]:
        trailer = SUBMISSION_TRAILER.search(text)
        if trailer is not None:
            return text[:trailer.start()], True
        if text.startswith(SUBMISSION_PREFIX):
            return text.partition("\n")[2], True
        return text, False

    @staticmethod
    def read(text: str) -> tuple[SubmissionStamp, str]:
        match = SUBMISSION_FOOTER.search(text)
        if match is not None:
            body = text[:match.start()]
            if match.group("hash") != match.group("source")[:12]:
                raise ToolError("表示用ハッシュが変更されています。oj-b で再生成してください。")
        else:
            # Existing files with a first-line record migrate during cleanup.
            match = SUBMISSION_HEADER.match(text)
            body = text[match.end():] if match is not None else ""
        if match is None:
            raise ToolError("照合情報がありません。oj-b で展開し直してください。")
        source_digest, input_digest, body_digest = (match.group(name) for name in ("source", "input", "body"))
        if hashlib.sha256(body.encode()).hexdigest() != body_digest:
            raise ToolError("生成後に提出用ファイルが変更されています。oj-b で再生成してください。")
        return SubmissionStamp(source_digest, input_digest), body


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
    previous = 0
    for start, end in comment_spans(text):
        comment = text[start:end]
        match = LIBRARY_SOURCE_LINK.fullmatch(comment)
        if match is None:
            continue
        line_start = text.rfind("\n", 0, start) + 1
        if text[line_start:start].strip():
            continue
        previous_start = text.rfind("\n", 0, max(0, line_start - 1)) + 1
        previous_line = text[previous_start:line_start]
        if COMMENT_LINE_SPLICE.search(previous_line):
            continue
        line_end = text.find("\n", end)
        line_end = len(text) if line_end < 0 else line_end + 1
        links.append(text[line_start:line_end])
        output.append(text[previous:line_start])
        output.append(f"#pragma submit_code_source_link {len(links) - 1}\n")
        previous = line_end
    output.append(text[previous:])
    return "".join(output), links


def restore_source_links(text: str, links: Sequence[str]) -> str:
    output: list[str] = []
    for line, visible in zip(text.splitlines(keepends=True), preprocessor_lines(text)):
        match = SOURCE_LINK_PRAGMA.match(visible.rstrip("\r\n"))
        if match is None:
            output.append(line)
            continue
        index = int(match.group(1))
        if index >= len(links):
            raise ToolError("invalid protected source-link marker")
        output.append(links[index])
    return "".join(output)


def compact_source_links(text: str) -> str:
    protected, links = protect_source_links(text)
    if not links:
        return text
    repository = source_link_comment(LIBRARY_REPOSITORY_URL)
    if text.startswith(links[0]):
        return restore_source_links(protected, [repository, *["\n"] * (len(links) - 1)])
    body = restore_source_links(protected, ["\n"] * len(links))
    return repository + "#line 1\n" + body


def strip_cleanup_comments(text: str, keep_comments: Iterable[str] = ()) -> str:
    protected, comments = protect_cleanup_comments(text, keep_comments)
    protected, links = protect_source_links(protected)
    stripped = restore_source_links(strip_comments(protected, comments), links)
    return compact_source_links(retain_important_comments(restore_cleanup_comments(stripped, comments), ()))


def source_comments(text: str) -> set[str]:
    return {text[start:end] for start, end in comment_spans(text)}


def important_comments(text: str) -> list[str]:
    return list(dict.fromkeys(
        comment for start, end in comment_spans(text)
        if is_important_comment(comment := text[start:end])
    ))


def retain_important_comments(text: str, comments: Sequence[str]) -> str:
    if not comments and "@keep-if-code" not in text:
        return text
    present = source_comments(text)
    dependencies: dict[str, set[str]] = {}
    for comment in present.union(comments):
        if not is_important_comment(comment) or "@keep-if-code" not in comment:
            continue
        names: set[str] = set()
        for line in comment.splitlines():
            if "@keep-if-code" not in line:
                continue
            match = re.fullmatch(
                r"\s*(?:/\*!|//!|\*)?\s*@keep-if-code\s+"
                r"([_A-Za-z]\w*(?:[ \t]+[_A-Za-z]\w*)*)\s*(?:\*/)?\s*", line,
            )
            if match is None:
                # An incomplete scope keeps the original notice.
                names.clear()
                break
            names.update(match.group(1).split())
        if names:
            dependencies[comment] = names
    if dependencies:
        # C++ joins continued lines before recognizing identifiers and comments.
        live = set(code_identifiers(text))
        patterns = token_paste_patterns(strip_comments(text))
        live.update(name for names in dependencies.values() for name in names
                    if any(pattern.fullmatch(name) for pattern in patterns))
        obsolete = {comment for comment, names in dependencies.items() if names.isdisjoint(live)}
        if obsolete:
            output: list[str] = []
            previous = 0
            for start, end in comment_spans(text):
                if text[start:end] in obsolete:
                    # Preserve token separation and compiler line coordinates.
                    output.extend((text[previous:start], " " + "\n" * text[start:end].count("\n")))
                    previous = end
            output.append(text[previous:])
            text = "".join(output)
            present.difference_update(obsolete)
            comments = [comment for comment in comments if comment not in obsolete]
    # Notices inside removed declarations or branches move to the beginning.
    # An extra newline also terminates a line comment ending in a backslash.
    missing = dict.fromkeys(comment for comment in comments if comment not in present)
    return "".join(comment + "\n\n" for comment in missing) + text


def replace_lines_preserving_comments(
    text: str, replacements: dict[int, str], keep_comments: Iterable[str] = (),
) -> str:
    """Keep notices at changed lines and leave other comments in place."""
    if not replacements:
        return text
    keep = set(keep_comments)
    notices: dict[int, list[str]] = {}
    output: list[str] = []
    previous = previous_line = 0
    for start, end in comment_spans(text):
        first = previous_line + text.count("\n", previous, start)
        last = first + text.count("\n", start, end)
        changed = next(
            (index for index in range(first, last + 1) if index in replacements), None,
        )
        comment = text[start:end]
        output.append(text[previous:start])
        if changed is None:
            output.append(comment)
        else:
            # Strip the whole comment even when only its opening line changes.
            # Preserve line numbers until all replacements have been applied.
            output.append((" " if is_block_comment(comment) else "") + "\n" * comment.count("\n"))
            if is_important_comment(comment) or comment in keep:
                notices.setdefault(changed, []).append(comment)
        previous, previous_line = end, last
    output.append(text[previous:])
    lines = "".join(output).splitlines(keepends=True)
    lines.extend([""] * (len(text.splitlines(keepends=True)) - len(lines)))
    result: list[str] = []
    for index, line in enumerate(lines):
        result.extend(comment + "\n\n" for comment in notices.get(index, ()))
        result.append(replacements.get(index, line))
    return "".join(result)


def comment_record_path(output: Path) -> Path:
    return output.parent / ".submit-code" / output.name / "comments.json"


def save_source_comments(
    output: Path, comments: set[str], *, provenance: dict | None = None,
) -> None:
    # Keep the expansion-time snapshot outside the submitted code, including
    # when the source is edited before cleanup or the bundle has a custom name.
    atomic_write(comment_record_path(output), json.dumps(
        {"comments": sorted(comments), **(provenance or {})}, ensure_ascii=False,
    ) + "\n")


def load_source_comments(output: Path) -> set[str]:
    record = comment_record_path(output)
    if record.is_file():
        return set(json.loads(record.read_text(encoding="utf-8"))["comments"])
    # Also support bundles generated before comment snapshots were introduced.
    stem = "main" if output.name == "bundle.cpp" else output.stem.removesuffix("-bundle")
    for extension in (".cpp", ".cc", ".cxx", ".C"):
        source = output.with_name(stem + extension)
        if source != output and source.is_file():
            return source_comments(source.read_text(encoding="utf-8"))
    return set()


def protect_cleanup_comments(text: str, comments: Iterable[str]) -> tuple[str, dict[str, str]]:
    keep = set(comments).union(important_comments(text))
    if not keep:
        return text, {}
    prefix = "submit_code_comment_" + hashlib.sha256(text.encode()).hexdigest()[:16] + "_"
    while prefix in text:
        prefix += "_"
    output: list[str] = []
    originals: dict[str, str] = {}
    previous = 0
    for start, end in comment_spans(text):
        comment = text[start:end]
        if comment not in keep:
            continue
        marker = f"/*{prefix}{len(originals)}" + "\n" * comment.count("\n") + "*/"
        originals[marker] = comment
        output.extend((text[previous:start], marker))
        previous = end
    output.append(text[previous:])
    return "".join(output), originals


def restore_cleanup_comments(text: str, comments: dict[str, str]) -> str:
    normalized = {marker.replace("\n", ""): comment for marker, comment in comments.items()}
    return re.sub(
        r"/\*(submit_code_comment_[0-9a-f]{16}_+\d+)\s*\*/",
        lambda match: normalized.get(f"/*{match.group(1)}*/", match.group()),
        text,
    ) if comments else text


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


def checkpoint_limits(target_bytes: int | None) -> tuple[int, ...]:
    if target_bytes is None:
        return ()
    if target_bytes == DEFAULT_BYTE_LIMIT:
        return PRIORITY_BYTE_LIMIT, DEFAULT_BYTE_LIMIT
    return (target_bytes,)


class CheckpointWriter:
    def __init__(
        self, path: Path, reporter: Reporter, *, stamp: SubmissionStamp | None = None,
    ) -> None:
        self.path = path
        self.reporter = reporter
        self.current: str | None = None
        self.preserve_below: int | None = None
        self.reached_limits: set[int] = set()
        self.layout_started: float | None = None
        self.display_layout = None
        self.analysis_current: str | None = None
        self.stamp = stamp
        # Count the footer in size decisions, outside compiler analysis and cleanup.
        self.stamp_size = len(stamp.render("").encode()) if stamp else 0

    def size(self, text: str) -> int:
        return encoded_size(text) + self.stamp_size

    def below_limit(self, text: str, limit: int | None) -> bool:
        return limit is not None and self.size(text) < limit

    def allow_library_compaction(self, text: str, limit: int | None) -> bool:
        return (self.layout_started is not None and limit is not None
                and limit not in self.reached_limits and not self.below_limit(text, limit)
                and time.monotonic() - self.layout_started >= LIBRARY_LAYOUT_DELAY)

    def report_checkpoint(self, text: str, limit: int | None) -> None:
        if limit is None or limit in self.reached_limits or not self.below_limit(text, limit):
            return
        self.reached_limits.add(limit)
        self.reporter.checkpoint(
            f"submission ready: {self.reporter.byte_count(self.size(text), 'green')} "
            f"< {self.reporter.byte_count(limit, 'yellow')}; cleanup continues"
        )

    def save(self, stage: str, text: str, *, force: bool = False) -> bool:
        original = text
        limits = checkpoint_limits(self.preserve_below)
        preserved = tuple(limit for limit in limits if self.current is not None
                          and self.below_limit(self.current, limit))
        if not force and any(not self.below_limit(text, limit) for limit in preserved):
            return False
        first_checkpoint = any(limit not in self.reached_limits and self.below_limit(text, limit)
                               for limit in limits)
        if self.display_layout is not None and not first_checkpoint:
            text = self.display_layout(text, force)
        if not force and text == self.current:
            return False
        if any(not self.below_limit(text, limit) for limit in preserved):
            return False
        before = self.size(self.current) if self.current is not None else self.size(text)
        rendered = self.stamp.render(text) if self.stamp else text
        atomic_write(self.path, rendered)
        self.current = text
        self.analysis_current = original
        self.reporter.size(stage, before, self.size(text))
        for limit in limits:
            self.report_checkpoint(text, limit)
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
        self.important_comments = important_comments("".join(line.text for line in self.lines))

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

        for line, visible in zip(text.splitlines(keepends=True), preprocessor_lines(text)):
            match = LINE_DIRECTIVE.match(visible.rstrip("\r\n"))
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
        uses_library = False

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
                    uses_library = True
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

        result = retain_important_comments("".join(output), self.important_comments)
        return source_link_comment(LIBRARY_REPOSITORY_URL) + result if uses_library else result


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
        keep_unused_functions: bool = True,
        whole_program: bool = False,
        optimization: str = "-O2",
    ) -> None:
        self.cxx = cxx
        self.standard = standard
        self.include_paths = list(include_paths)
        self.extra_args = list(extra_args)
        self.timeout = timeout
        self.reporter = reporter
        self.keep_unused_functions = keep_unused_functions
        self.optimization = "-O0" if keep_unused_functions else optimization
        self.whole_program = whole_program
        self.submission_whole_program = False
        self.submission_optimization = "-O2"
        self.temporary = tempfile.TemporaryDirectory(prefix="submit-code-")
        self.directory = Path(self.temporary.name)
        self.source = self.directory / "candidate.cpp"
        self.analysis_source = self.directory / "analysis.cpp"
        self.object = self.directory / "candidate.o"
        self.executable = self.directory / "candidate.out"
        self.pch_header = self.directory / "standard_prelude.hpp"
        self.pch_output = self.directory / "standard_prelude.hpp.gch"
        self.pch_available: bool | None = None
        self.pch_disabled = False
        self.pch_key: str | None = None
        self.pch_prefix_end: int | None = None
        self.pch_preamble_cache: dict[str, str] = {}
        self.predefined_macro_lines: set[str] | None = None
        self.syntax_cache: dict[str, tuple[bool, str]] = {}
        self.link_cache: dict[str, tuple[bool, str]] = {}
        self.link_pch_cache: dict[str, bool] = {}
        self.executable_digest: str | None = None
        self.initialization_cache: dict[str, str] = {}
        self.initialization_reference: str | None = None
        self.initialization_reference_text: str | None = None
        self.initialization_references: dict[bool, str] = {}
        self.object_cache: dict[str, tuple[str, dict[str, str], set[str]]] = {}
        self.object_dependencies: dict[str, str] = {}
        self.object_undefined: set[str] = set()
        self.object_diagnostic = ""
        self.instantiated_header_cache: dict[str, str] = {}
        self.submission_headers_by_object: dict[str, str] = {}
        self.emitted_line_cache: dict[str, set[int]] = {}
        self.preprocessed_code_cache: dict[tuple[str, str, bool], str | None] = {}
        self.all_macro_cache: dict[str, dict[str, list[str]]] = {}
        self.external_name_cache: dict[str, set[str]] = {}
        self.external_content_cache: dict[str, tuple[set[str], set[str], dict[str, str]]] = {}
        self.external_macro_cache: dict[str, set[str]] = {}
        self.external_macro_definition_cache: dict[str, dict[str, str]] = {}
        self.portable_workspace: CompilerWorkspace | None = None
        self.submission_workspace: CompilerWorkspace | None = None
        self.force_float128_macro: bool | None = None

    def close(self) -> None:
        if self.submission_workspace is not None:
            self.submission_workspace.close()
        if self.portable_workspace is not None:
            self.portable_workspace.close()
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
        header_text = text[: include.end()]
        for start, end in reversed(list(comment_spans(header_text))):
            if header_text[start:end].startswith("// submit-code-feature:"):
                header_text = header_text[:start] + "".join(
                    "\n" if char == "\n" else " " for char in header_text[start:end]) + header_text[end:]
        sensitive = {"__COUNTER__", "__LINE__", "__FILE__", "__BASE_FILE__", "__FILE_NAME__",
                     "__TIME__", "__DATE__", "__TIMESTAMP__", "__builtin_FILE", "__builtin_LINE"}
        if set(code_identifiers(prefix)).intersection(sensitive):
            return None
        if "\\" not in prefix and not set(code_identifiers(prefix)).intersection(sensitive) and all(
            not line.strip() or re.match(r"\s*#\s*(?:define|undef|if|ifdef|ifndef|elif|else|endif|line)\b", line)
            for line in prefix.splitlines()
        ):
            normalized = self.pch_preamble_cache.get(prefix)
            if normalized is None:
                prelude = self.directory / "pch_prefix.cpp"
                command = [*self._base_command(), self.optimization, "-E", "-dM", str(prelude)]
                if self.predefined_macro_lines is None:
                    prelude.write_text("")
                    result = self._run(command)
                    if result.returncode:
                        return None
                    self.predefined_macro_lines = set(result.stdout.splitlines())
                prelude.write_text(prefix)
                result = self._run(command)
                if result.returncode:
                    return None
                active = set(result.stdout.splitlines())
                undefined = ["#undef " + line.split()[1].partition("(")[0]
                             for line in self.predefined_macro_lines - active]
                normalized = "\n".join(sorted(undefined) + sorted(active - self.predefined_macro_lines)) + "\n"
                self.pch_preamble_cache[prefix] = normalized
            header_text = normalized + "#include <bits/stdc++.h>"
        return header_text + "\n", include.end()

    def _ensure_pch(self, text: str, *, build: bool = True, probe: bool = False) -> bool:
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
                    "optimization": self.optimization,
                    "whole_program": self.whole_program,
                    "include_paths": [str(path) for path in self.include_paths],
                    "environment": {name: os.environ.get(name) for name in (
                        "CPLUS_INCLUDE_PATH", "CPATH", "SDKROOT", "MACOSX_DEPLOYMENT_TARGET")},
                    "header": header_text,
                },
                sort_keys=True,
            ).encode()
        ).hexdigest()[:20]
        if probe:
            cache_key = "object-" + cache_key
        if self.pch_key == cache_key and self.pch_available is False:
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
        self.pch_header = cache_dir / "standard_prelude.hpp"
        self.pch_output = cache_dir / "standard_prelude.hpp.gch"
        state_path = cache_dir / "dependencies.json"
        if self.pch_header.is_file() and self.pch_output.is_file():
            try:
                dependencies = json.loads(state_path.read_text())
                if self.pch_header.read_text(encoding="utf-8") == header_text and dependencies and all(
                    Path(path).is_file() and hashlib.sha256(Path(path).read_bytes()).hexdigest() == value
                    for path, value in dependencies.items()
                ):
                    self.pch_available = True
                    self.pch_prefix_end = prefix_end
                    return True
            except (OSError, ValueError, AttributeError):
                pass

        # A single early checkpoint is faster to check without building a PCH.
        # Leave availability unknown so later, repeated checks can still build it.
        if not build:
            return False
        cache_dir.mkdir(parents=True, exist_ok=True)
        atomic_write(self.pch_header, header_text)
        temporary_output = cache_dir / f".{self.pch_output.name}.{os.getpid()}.tmp"
        depfile = cache_dir / f".dependencies.{os.getpid()}.d"
        result = self._run(
            [
                *self._base_command(),
                self.optimization,
                *(["-fwhole-program"] if self.whole_program else []),
                *([] if self.keep_unused_functions else ["-g0", "-frandom-seed=0"]),
                "-MD", "-MF", str(depfile),
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
            atomic_write(state_path, json.dumps(self._object_dependency_hashes(depfile)))
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

    def _prepare_candidate_source(self, text: str, *, build_pch: bool = True, probe: bool = False) -> bool:
        if self.force_float128_macro is not None:
            directive = "#undef __STDCPP_FLOAT128_T__\n"
            if self.force_float128_macro:
                directive += "#define __STDCPP_FLOAT128_T__ 1\n"
            output = [directive]
            for line, visible in zip(text.splitlines(keepends=True), preprocessor_lines(text)):
                output.append(line)
                if SYSTEM_INCLUDE_DIRECTIVE.match(visible.rstrip("\r\n")):
                    output.append(directive)
            self.write("".join(output))
            return False
        if self.pch_disabled:
            self.write(text)
            return False
        use_pch = self._ensure_pch(text, build=build_pch, probe=probe)
        if use_pch and self.pch_prefix_end is not None:
            prefix = text[: self.pch_prefix_end]
            masked_prefix = "".join(
                line if re.match(r"\s*#\s*(?:line\b|\d)", line) else
                "".join("\n" if char == "\n" else " " for char in line)
                for line in prefix.splitlines(keepends=True))
            self.write(masked_prefix + text[self.pch_prefix_end :])
        else:
            self.write(text)
        return use_pch

    def _candidate_command(self, use_pch: bool) -> list[str]:
        command = self._base_command()
        if self.whole_program:
            command.append("-fwhole-program")
        if use_pch:
            command.extend(["-include", str(self.pch_header)])
        return command

    def _run(self, command: Sequence[str]) -> subprocess.CompletedProcess[str]:
        return run_command(command, timeout=self.timeout)

    def syntax(self, text: str, *, build_pch: bool = True) -> tuple[bool, str]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        cached = self.syntax_cache.get(digest)
        if cached is not None:
            return cached
        use_pch = self._prepare_candidate_source(text, build_pch=build_pch)
        result = self._run([*self._candidate_command(use_pch), "-fsyntax-only", str(self.source)])
        value = result.returncode == 0, result.stderr
        self.syntax_cache[digest] = value
        return value

    def object_signature(self, text: str, *, pch: bool | None = None) -> str | None:
        digest = hashlib.sha256(text.encode()).hexdigest()
        if pch is None and digest in self.object_cache:
            signature, self.object_dependencies, self.object_undefined = self.object_cache[digest]
            self.object_diagnostic = ""
            return signature
        use_pch = False
        if self.force_float128_macro is not None or (not self.keep_unused_functions and pch is not False):
            use_pch = self._prepare_candidate_source(text, build_pch=False, probe=pch is True)
        else:
            self.write(text)
        if pch is True and not use_pch:
            return self.object_signature(text, pch=False)
        output = self.directory / "semantic.o"
        depfile = self.directory / "semantic.d"
        command = [argument for argument in self._candidate_command(use_pch) if argument != "-w"]
        result = self._run([*command, "-O2" if self.keep_unused_functions else self.optimization,
                            "-g0", "-frandom-seed=0", "-MD", "-MF", str(depfile),
                            *(["-fpch-deps"] if use_pch else []),
                            *(["-H", "-Werror=invalid-pch"] if pch is True else []),
                            "-c", str(self.source), "-o", str(output)])
        self.object_diagnostic = result.stderr
        self.object_undefined = set()
        if pch is True:
            lines = result.stderr.splitlines()
            self.object_diagnostic = "\n".join(line for line in lines if not re.match(r"^[.!x]+ ", line))
            if not any(line.lstrip(".") == f"! {self.pch_output}" for line in lines):
                self.pch_available = False
                return self.object_signature(text, pch=False)
        if result.returncode or "used but never defined" in result.stderr:
            if pch is None and use_pch:
                self.pch_available = False
                return self.object_signature(text)
            return None
        signature = hashlib.sha256(output.read_bytes()).hexdigest()
        self.object_undefined = self._undefined_symbols(output)
        if pch is True:
            return signature
        dependencies = self._object_dependency_hashes(depfile)
        self.object_cache[digest] = signature, dependencies, self.object_undefined
        self.object_dependencies = dependencies
        return signature

    def prepare_object_pch(self, text: str, reference: str) -> bool:
        """Enable declaration probes when both compilation modes agree on the reference."""
        if self.pch_disabled or self.force_float128_macro is not None:
            return False
        if not self._ensure_pch(text, probe=True):
            return False
        if (self.object_signature(text, pch=True) != reference or not self.pch_available
                or self.object_signature(text, pch=False) != reference):
            self.pch_available = False
            return False
        return True

    def _undefined_symbols(self, output: Path) -> set[str]:
        if not shutil.which("nm"):
            return set()
        symbols = self._run(["nm", "-u", str(output)])
        if symbols.returncode:
            return set()
        demangler = shutil.which("c++filt")
        if demangler:
            demangled = run_command([demangler], input_text=symbols.stdout, timeout=self.timeout)
            if demangled.returncode == 0:
                symbols = demangled
        return set(symbols.stdout.splitlines())

    def _object_dependency_hashes(self, depfile: Path) -> dict[str, str]:
        dependencies = {}
        for word in shlex.split(depfile.read_text().replace("\\\n", "").split(":", 1)[1]):
            path = Path(word).resolve()
            if path != self.source.resolve() and path.is_file():
                dependencies[str(path)] = hashlib.sha256(path.read_bytes()).hexdigest()
        return dependencies

    def instantiated_function_headers(self, text: str) -> str:
        digest = hashlib.sha256(text.encode()).hexdigest()
        if digest in self.instantiated_header_cache:
            return self.instantiated_header_cache[digest]
        for dump in self.directory.glob("features.*.original"):
            dump.unlink()
        self.write(text)
        result = self._run([*self._base_command(), "-O0", "-fdump-tree-original",
                            "-dumpdir", str(self.directory) + os.sep, "-dumpbase", "features.",
                            "-c", str(self.source), "-o", str(self.directory / "features.o")])
        dump = next(self.directory.glob("features.*.original"), None)
        if result.returncode or dump is None:
            return ""
        headers = "\n".join(line for line in dump.read_text(errors="replace").splitlines()
                            if line.startswith(";; Function "))
        self.instantiated_header_cache[digest] = headers
        return headers

    def preprocessed_fingerprint(self, text: str) -> str | None:
        self.write(text)
        result = self._run([*self._base_command(), "-O2", "-E", "-dD", str(self.source)])
        if result.returncode:
            return None
        normalized = result.stdout.replace(str(self.source), "<submission>")
        return hashlib.sha256(normalized.encode()).hexdigest()

    def preprocessed_code_fingerprint(self, text: str) -> str | None:
        digest = hashlib.sha256(text.encode()).hexdigest()
        workspace = self.submission_workspace
        if workspace is not None:
            optimization, whole_program = workspace.optimization, workspace.whole_program
        elif self.keep_unused_functions:
            optimization, whole_program = self.submission_optimization, self.submission_whole_program
        else:
            optimization, whole_program = self.optimization, self.whole_program
        key = digest, optimization, whole_program
        if key in self.preprocessed_code_cache:
            return self.preprocessed_code_cache[key]
        self.write(text)
        result = self._run([*self._base_command(), optimization,
                            *(["-fwhole-program"] if whole_program else []), "-E", str(self.source)])
        if result.returncode:
            self.preprocessed_code_cache[key] = None
            return None
        output = result.stdout
        raw = []
        list(comment_spans(output, raw_spans=raw))
        offsets = [0]
        for line in output.splitlines(keepends=True):
            offsets.append(offsets[-1] + len(line))
        protected = set()
        for start, end in raw:
            protected.update(range(bisect_left(offsets, start + 1) - 1,
                                   bisect_left(offsets, end)))
        fingerprint = hashlib.sha256()
        source, number, flags = "", 1, ()
        external = []
        external_position = None

        def flush_external():
            if external:
                content = "".join(external)
                if content.strip():
                    data = content.encode()
                    fingerprint.update(b"E\0" + json.dumps(external_position).encode() + b"\0")
                    fingerprint.update(len(data).to_bytes(8, "big") + data)
                external.clear()

        for index, line in enumerate(output.splitlines(keepends=True)):
            marker = None if index in protected else GNU_LINE_MARKER.match(line.rstrip("\r\n"))
            if marker is not None:
                flush_external()
                source = json.loads(marker.group(2))
                if source == str(self.source):
                    source = "<submission>"
                number = int(marker.group(1))
                flags = tuple(flag for flag in line[marker.end(2):].split() if flag in {"3", "4"})
                continue
            if source != "<submission>":
                if not external:
                    external_position = source, number, flags
                external.append(line)
            elif line.strip() or index in protected:
                fingerprint.update(b"S\0" + json.dumps((source, number, flags, line.rstrip("\r\n"))).encode())
                fingerprint.update(b"\n")
            number += 1
        flush_external()
        value = fingerprint.hexdigest()
        self.preprocessed_code_cache[key] = value
        return value

    def link(self, text: str, *, build_pch: bool = True, force_pch: bool | None = None) -> tuple[bool, str]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        cached = self.link_cache.get(digest)
        if cached is not None and (force_pch is None or self.link_pch_cache.get(digest) == force_pch) and (not cached[0] or (
            self.executable_digest == digest and self.executable.is_file()
        )):
            return cached
        if force_pch is False and self.force_float128_macro is None:
            self.write(text)
            use_pch = False
        else:
            use_pch = self._prepare_candidate_source(text, build_pch=build_pch or force_pch is True)
        command = [argument for argument in self._candidate_command(use_pch) if argument != "-w"]
        command.extend(["-fdump-tree-original", "-fdump-noaddr", "-fdump-unnumbered",
                        "-dumpdir", str(self.directory) + os.sep, "-dumpbase", "validation."])
        if not self.keep_unused_functions:
            command.append("-fdump-tree-gimple-lineno")
        for dump in self.directory.glob("validation.*.original"):
            dump.unlink()
        for dump in self.directory.glob("validation.*.gimple"):
            dump.unlink()
        self.executable_digest = None
        retained = ["-fkeep-inline-functions", "-fkeep-static-functions"] if self.keep_unused_functions else []
        for artifact in (self.object, self.executable):
            try:
                artifact.unlink()
            except FileNotFoundError:
                pass

        if not self.extra_args and self.keep_unused_functions:
            result = self._run(
                [
                    *command,
                    "-O0",
                    *retained,
                    str(self.source),
                    "-o",
                    str(self.executable),
                    "-lquadmath",
                ]
            )
            value = result.returncode == 0 and "used but never defined" not in result.stderr, result.stderr
            if not value[0] and use_pch:
                self.pch_available = False
                return self.link(text, build_pch=False, force_pch=False)
            if value[0]:
                self.executable_digest = digest
                self.initialization_cache[digest] = self._initialization_signature()
            self.link_cache[digest] = value
            self.link_pch_cache[digest] = use_pch
            return value

        compile_result = self._run(
            [
                *command,
                self.optimization,
                *retained,
                *([] if self.keep_unused_functions else ["-g0", "-frandom-seed=0", "-MD", "-MF", str(self.directory / "validation.d")]),
                *(["-fpch-deps"] if use_pch else []),
                "-c",
                str(self.source),
                "-o",
                str(self.object),
            ]
        )
        if compile_result.returncode != 0 or "used but never defined" in compile_result.stderr:
            if use_pch:
                self.pch_available = False
                return self.link(text, build_pch=False, force_pch=False)
            value = False, compile_result.stderr
            self.link_cache[digest] = value
            self.link_pch_cache[digest] = use_pch
            return value

        link_result = self._run(
            [
                self.cxx,
                "-w",
                str(self.object),
                "-o",
                str(self.executable),
                *self.extra_args,
                "-lquadmath",
            ]
        )
        value = link_result.returncode == 0, link_result.stderr
        if value[0]:
            self.executable_digest = digest
            self.initialization_cache[digest] = self._initialization_signature()
        self.link_cache[digest] = value
        self.link_pch_cache[digest] = use_pch
        return value

    def _initialization_signature(self) -> str:
        dump = next(self.directory.glob("validation.*.original"), None)
        if dump is None:
            raise ToolError("GCC did not produce the initialization validation dump")
        functions = re.split(r"(?m)(?=^;; Function )", dump.read_text(encoding="utf-8", errors="replace"))
        if self.executable_digest is not None:
            self.instantiated_header_cache[self.executable_digest] = "\n".join(
                function.split("\n", 1)[0] for function in functions if function.startswith(";; Function ")
            )
            if not self.keep_unused_functions:
                dependencies = self._object_dependency_hashes(self.directory / "validation.d")
                signature = hashlib.sha256(self.object.read_bytes()).hexdigest()
                self.object_cache[self.executable_digest] = signature, dependencies, self._undefined_symbols(self.object)
                gimple = next(self.directory.glob("validation.*.gimple"), None)
                if gimple is not None:
                    self.emitted_line_cache[self.executable_digest] = {
                        int(match.group(1)) - 1 for match in re.finditer(
                            rf"\[{re.escape(str(self.source))}:(\d+):\d+\]", gimple.read_text(errors="replace"))
                    }
        return "".join(function for function in functions if re.search(
            r"__static_initialization_and_destruction_\d+|static (?:initializers|destructors) for|__tls_init|__tcf_",
            function.split("\n", 1)[0],
        ))

    def preserve_initialization(self, text: str) -> None:
        if self.initialization_reference is None:
            valid, stderr = self.validate(text)
            if not valid:
                raise ToolError(f"the reference input does not validate: {first_error(stderr)}")

    def validate_submission(self, text: str, *, build_pch: bool = False) -> tuple[bool, str]:
        if self.submission_workspace is None:
            self.submission_workspace = CompilerWorkspace(
                cxx=self.cxx, standard=self.standard, include_paths=self.include_paths,
                extra_args=self.extra_args, timeout=self.timeout, reporter=self.reporter,
                keep_unused_functions=False,
                whole_program=self.submission_whole_program,
                optimization=self.submission_optimization,
            )
        workspace = self.submission_workspace
        valid, diagnostic = workspace.validate(text, build_pch=build_pch)
        if valid:
            digest = hashlib.sha256(text.encode()).hexdigest()
            headers = workspace.instantiated_header_cache[digest]
            self.instantiated_header_cache[digest] = headers
            self.submission_headers_by_object[workspace.object_cache[digest][0]] = headers
            if digest in workspace.emitted_line_cache:
                self.emitted_line_cache[digest] = workspace.emitted_line_cache[digest]
        return valid, diagnostic

    def remember_submission_layout(self, before: str, after: str) -> None:
        """Reuse the object record after verifying identical preprocessed code and positions."""
        if self.submission_workspace is None or self.submission_workspace.pch_available:
            return
        cache = self.submission_workspace.object_cache
        before_digest = hashlib.sha256(before.encode()).hexdigest()
        if before_digest in cache:
            after_digest = hashlib.sha256(after.encode()).hexdigest()
            cache[after_digest] = cache[before_digest]
            if before_digest in self.instantiated_header_cache:
                self.instantiated_header_cache[after_digest] = self.instantiated_header_cache[before_digest]

    def linked_submission_headers(self, text: str) -> str | None:
        if self.submission_workspace is not None:
            digest = hashlib.sha256(text.encode()).hexdigest()
            cached = self.submission_workspace.object_cache.get(digest)
            if cached is not None and cached[0] in self.submission_headers_by_object:
                return self.instantiated_header_cache.get(digest, self.submission_headers_by_object[cached[0]])
        return None

    def submission_reference(self, text: str) -> tuple[str | None, str]:
        """Return a linked reference signature and conservative function information."""
        headers = self.linked_submission_headers(text)
        if headers is not None:
            return self.submission_object_signature(text), headers
        valid, _ = self.validate_submission(text)
        if not valid:
            return None, ""
        return self.submission_object_signature(text), self.instantiated_function_headers(text)

    def submission_object_signature(self, text: str) -> str | None:
        if self.submission_workspace is None:
            valid, _ = self.validate_submission(text)
            if not valid:
                return None
        workspace = self.submission_workspace
        signature = workspace.object_signature(text)
        self.object_diagnostic = workspace.object_diagnostic
        self.object_dependencies = workspace.object_dependencies
        self.object_undefined = workspace.object_undefined
        return signature

    def same_submission_layout(self, before: str, after: str) -> bool:
        def same_object(signature) -> bool:
            reference = signature(before)
            return reference is not None and signature(after) == reference

        portable = self.portable_workspace
        if portable is None:
            return same_object(self.submission_object_signature)
        with ThreadPoolExecutor(max_workers=1) as executor:
            alternate = executor.submit(same_object, portable.object_signature)
            return same_object(self.submission_object_signature) and alternate.result()

    def validate(self, text: str, *, build_pch: bool = True,
                 parallel_portable: bool = False) -> tuple[bool, str]:
        portable = self.portable_workspace
        if parallel_portable and portable is not None:
            with ThreadPoolExecutor(max_workers=1) as executor:
                alternate = executor.submit(portable.validate, text, build_pch=build_pch)
                result = self._validate_local(text, build_pch=build_pch)
                return alternate.result() if result[0] else result
        result = self._validate_local(text, build_pch=build_pch)
        if result[0] and portable is not None:
            return portable.validate(text, build_pch=build_pch)
        return result

    def _validate_local(self, text: str, *, build_pch: bool) -> tuple[bool, str]:
        valid, stderr = self.link(text, build_pch=build_pch)
        if valid:
            digest = hashlib.sha256(text.encode()).hexdigest()
            mode = self.link_pch_cache[digest]
            if self.initialization_reference is None:
                self.initialization_reference_text = text
                self.initialization_references[mode] = self.initialization_cache[digest]
            while mode not in self.initialization_references:
                reference = self.initialization_reference_text
                assert reference is not None
                reference_valid, reference_stderr = self.link(reference, build_pch=mode, force_pch=mode)
                if not reference_valid:
                    return False, reference_stderr
                reference_digest = hashlib.sha256(reference.encode()).hexdigest()
                reference_mode = self.link_pch_cache[reference_digest]
                self.initialization_references[reference_mode] = self.initialization_cache[reference_digest]
                if reference_mode != mode:
                    self.pch_disabled = True
                valid, stderr = self.link(text, build_pch=mode, force_pch=reference_mode)
                if not valid:
                    return valid, stderr
                mode = self.link_pch_cache[digest]
            self.initialization_reference = self.initialization_references[mode]
            if self.initialization_cache[digest] != self.initialization_reference:
                return False, "static initialization or destruction changed"
        return valid, stderr

    def retain_portable_target(self, text: str) -> None:
        if self.portable_workspace is not None or "__STDCPP_FLOAT128_T__" not in set(code_identifiers(text)):
            return
        self.write(text)
        macros = self._run([*self._base_command(), "-E", "-dM", "-x", "c++", str(self.source)])
        if macros.returncode:
            return
        native = re.search(r"(?m)^#define __STDCPP_FLOAT128_T__\b", macros.stdout) is not None
        target = CompilerWorkspace(cxx=self.cxx, standard=self.standard,
            include_paths=self.include_paths, extra_args=self.extra_args,
            timeout=self.timeout, reporter=self.reporter)
        target.force_float128_macro = not native
        valid, _ = target.validate(text, build_pch=False)
        if valid:
            self.portable_workspace = target
        else:
            target.close()

    def compiler_warnings(self, text: str, *, allow_pch: bool = True) -> str:
        prefix = self._pch_prefix(text)
        if prefix is not None and re.search(r"(?m)^\s*#\s*define\b", strip_comments(prefix[0])):
            allow_pch = False
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

    def unused_macro_locations(self, text: str) -> list[int]:
        rows = warning_line_indices(text)
        if not rows:
            return []
        self.write(text)
        command = [argument for argument in self._base_command() if argument != "-w"]
        result = self._run([*command, "-O2", "-Wunused-macros", "-E", str(self.source), "-o", os.devnull])
        if result.returncode:
            return []
        pattern = re.compile(rf"^{re.escape(str(self.source))}:(\d+):\d+: warning:.*\[-Wunused-macros\]$", re.MULTILINE)
        return [rows[number] for match in pattern.finditer(result.stderr)
                if (number := int(match.group(1))) in rows]

    def ipa_unused_functions(self, text: str) -> list[tuple[int, str]]:
        return self._analyze_ipa(text) or []

    def _analyze_ipa(self, text: str) -> list[tuple[int, str]] | None:
        for path in self.directory.glob("ipa.*"):
            if path.is_file():
                path.unlink()

        use_pch = self._prepare_candidate_source(text)
        started = time.monotonic()
        result = self._run(
            [
                *self._candidate_command(use_pch),
                "-Og",
                "-fwhole-program",
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
            return None

        cgraph = next(self.directory.glob("ipa.*.cgraph"), None)
        original = next(self.directory.glob("ipa.*.original"), None)
        if cgraph is None or original is None:
            self.reporter.detail("IPA analysis did not produce GCC dump files")
            return None

        graph_text = cgraph.read_text(encoding="utf-8", errors="replace")
        signatures = ipa_removed_signatures(graph_text)
        initial = graph_text.split("Initial Symbol table:", 1)[-1].split("Removing unused symbols:", 1)[0]
        all_signatures = {match.group(1) for line in initial.splitlines()
                          if (match := re.match(r"^\S.*/\d+ \((.*)\)$", line))}
        locations = ipa_function_locations(
            original,
            signatures=all_signatures,
            source_name=self.source.name,
        )
        live_lines = {line for line, signature in locations if signature not in signatures}
        functions = [(line, signature) for line, signature in locations
                     if signature in signatures and line not in live_lines]
        self.reporter.detail(
            f"IPA analysis: {len(functions)} mapped function(s), "
            f"{time.monotonic() - started:.2f}s"
        )
        return functions

    def uninstantiated_template_functions(self, text: str) -> list[tuple[int, str]]:
        for path in self.directory.glob("templates.*"):
            if path.is_file():
                path.unlink()

        use_pch = self._prepare_candidate_source(text)
        started = time.monotonic()
        result = self._run(
            [
                *self._candidate_command(use_pch),
                "-O0",
                "-fdump-lang-raw",
                "-dumpdir",
                str(self.directory) + os.sep,
                "-dumpbase",
                "templates.",
                "-fsyntax-only",
                str(self.source),
            ]
        )
        if result.returncode != 0:
            self.reporter.detail(
                "template analysis failed: " + first_error(result.stderr)
            )
            return []

        dump = next(self.directory.glob("templates.*.raw"), None)
        if dump is None:
            self.reporter.detail("template analysis did not produce a GCC dump file")
            return []

        identifiers: dict[str, str] = {}
        pseudo_records: set[tuple[int, str]] = set()
        instantiated_records: set[tuple[int, str]] = set()
        source_pattern = re.compile(
            rf"\bsrcp:\s+(?:.*[/\\])?{re.escape(self.source.name)}:(\d+)\b"
        )
        record_pattern = re.compile(r"^@\d+\s+\w")
        identifier_pattern = re.compile(r"^@(\d+)\s+identifier_node\b")
        spelling_pattern = re.compile(r"\bstrg:\s+(.*?)\s+lngt:\s+\d+")
        body_pattern = re.compile(r"\bbody:\s+@\d+")
        name_pattern = re.compile(r"\bname:\s+@(\d+)")

        def consume(record: str) -> None:
            identifier = identifier_pattern.match(record)
            if identifier is not None:
                spelling = spelling_pattern.search(record)
                if spelling is not None:
                    identifiers[identifier.group(1)] = spelling.group(1)
                return
            if " function_decl " not in record or not body_pattern.search(record):
                return
            match = source_pattern.search(record)
            name = name_pattern.search(record)
            if match is None or name is None:
                return
            line_number = int(match.group(1))
            if "pseudo tmpl" in record:
                pseudo_records.add((line_number, name.group(1)))
            else:
                instantiated_records.add((line_number, name.group(1)))

        record = ""
        with dump.open(encoding="utf-8", errors="replace") as stream:
            for line in stream:
                if line.startswith("@") and record_pattern.match(line):
                    if record:
                        consume(record)
                    # Most of the dump describes standard-library types and
                    # expressions. Only these two record kinds are relevant.
                    record = line if (
                        " identifier_node " in line or " function_decl " in line
                    ) else ""
                elif record:
                    record += line
            if record:
                consume(record)
        try:
            dump.unlink()
        except FileNotFoundError:
            pass
        result = sorted(
            (line, identifiers.get(name, ""))
            for line, name in pseudo_records - instantiated_records
        )
        self.reporter.detail(
            f"template analysis: {len(result)} uninstantiated "
            f"function line(s), {time.monotonic() - started:.2f}s"
        )
        return result

    def preprocess_directives(self, text: str, *, keep_comments: bool = False) -> tuple[bool, str, str]:
        self.write(text)
        result = self._run(
            [
                self.cxx,
                f"-std={self.standard}",
                "-E",
                "-fdirectives-only",
                *(["-CC"] if keep_comments else []),
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

    def external_identifiers(self, text: str) -> set[str]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        if digest in self.external_name_cache:
            return self.external_name_cache[digest]
        self.write(text)
        result = self._run([*self._base_command(), "-E", "-dD", "-x", "c++", str(self.source)])
        if result.returncode:
            raise ToolError("external-header analysis failed: " + first_error(result.stderr))
        output = result.stdout
        external = False
        lines = []
        for line in output.splitlines(keepends=True):
            marker = GNU_LINE_MARKER.match(line.rstrip("\r\n"))
            if marker:
                name = json.loads(marker.group(2))
                external = not name.startswith("<") and Path(name).name != self.source.name
            elif external:
                lines.append(line)
        content = "".join(lines)
        content_digest = hashlib.sha256(content.encode()).hexdigest()
        cached = self.external_content_cache.get(content_digest)
        if cached is None:
            names = set(code_identifiers(content))
            definitions: dict[str, str] = {}
            for line in re.sub(r"\\\r?\n", "", content).splitlines():
                if match := re.match(r"^\s*#\s*define\s+([_A-Za-z]\w*)(.*)", line):
                    name, replacement = match.groups()
                    definitions[name] = definitions.get(name, "") + "\n" + replacement
            cached = names, set(definitions), definitions
            self.external_content_cache[content_digest] = cached
        names, macro_names, definitions = cached
        self.external_name_cache[digest] = names
        self.external_macro_cache[digest] = macro_names
        self.external_macro_definition_cache[digest] = definitions
        all_definitions: dict[str, list[str]] = {}
        for line in re.sub(r"\\\r?\n", "", output).splitlines():
            if match := re.match(r"^\s*#\s*define\s+([_A-Za-z]\w*)(.*)", line):
                all_definitions.setdefault(match.group(1), []).append(match.group(2))
        self.all_macro_cache[digest] = all_definitions
        return names

    def external_macro_names(self, text: str) -> set[str]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        self.external_identifiers(text)
        return self.external_macro_cache[digest]

    def external_macro_definitions(self, text: str) -> dict[str, str]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        self.external_identifiers(text)
        return self.external_macro_definition_cache[digest]

    def all_macro_definitions(self, text: str) -> dict[str, list[str]]:
        digest = hashlib.sha256(text.encode()).hexdigest()
        self.external_identifiers(text)
        return self.all_macro_cache[digest]

    def dangerous_macro_names(self, text: str) -> set[str]:
        macros = self.all_macro_definitions(text)
        dangerous = {"__attribute__", "__declspec", "asm", "__asm__", "__COUNTER__"}
        for name, bodies in macros.items():
            for body in bodies:
                tokens = [token for row in structural_tokens(body.splitlines(keepends=True)) for token in row]
                if tokens.count("[") != tokens.count("]") or any(
                        a == b == "[" for a, b in zip(tokens, tokens[1:])):
                    dangerous.add(name)
        while True:
            added = {name for name, bodies in macros.items()
                     if set(code_identifiers("\n".join(bodies))).intersection(dangerous)} - dangerous
            if not added:
                return dangerous
            dangerous.update(added)

    def uses_counter_macro(self, text: str) -> bool:
        macros = self.all_macro_definitions(text)
        pending = set(code_identifiers(text))
        visited = set()
        while pending:
            name = pending.pop()
            if name == "__COUNTER__":
                return True
            if name not in visited:
                visited.add(name)
                pending.update(set(code_identifiers("\n".join(macros.get(name, [])))) - visited)
        return False


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
    compiler.preserve_initialization(document.render(set(), keep_line=keep_line))
    sizes = document.origin_sizes()
    first_positions = document.origin_first_positions()
    removed: set[str] = set()
    validation_cache: dict[frozenset[str], tuple[bool, str]] = {}

    source_text = "".join(line.text for line in document.lines)
    removable_lines = {
        line_index
        for candidate in unreferenced_declaration_candidates(source_text, compiler.external_identifiers(source_text))
        for line_index in range(candidate.start, candidate.end + 1)
    }
    candidates = []
    for key, origin in document.origins.items():
        if key in document.protected:
            continue
        if any(fnmatch.fnmatch(origin.display, pattern) for pattern in keep_patterns):
            continue
        remainder = strip_comments("".join(
            line.text for index, line in enumerate(document.lines)
            if line.origin_key == key and index not in removable_lines
        ))
        remainder = re.sub(r"\b(?:inline\s+)?namespace(?:\s+[_A-Za-z]\w*(?:::[_A-Za-z]\w*)*)?\s*\{", "", remainder)
        if re.sub(r"[\s{};]", "", remainder):
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


COMMENT_LINE_SPLICE = re.compile(r"\\[ \t\f\v]*(?:\r\n|\n|\r)")


def is_block_comment(comment: str) -> bool:
    return comment.startswith("/*") or COMMENT_LINE_SPLICE.sub("", comment).startswith("/*")


def is_important_comment(comment: str) -> bool:
    return comment.startswith(("/*!", "//!")) or COMMENT_LINE_SPLICE.sub("", comment).startswith(("/*!", "//!"))


def comment_lexical_view(text: str) -> tuple[str, list[int] | None]:
    """Return spliced text and the original offset of each surviving character."""
    if not COMMENT_LINE_SPLICE.search(text):
        return text, None
    parts: list[str] = []
    offsets: list[int] = []
    previous = 0
    for match in COMMENT_LINE_SPLICE.finditer(text):
        parts.append(text[previous:match.start()])
        offsets.extend(range(previous, match.start()))
        previous = match.end()
    parts.append(text[previous:])
    offsets.extend(range(previous, len(text)))
    return "".join(parts), offsets


def comment_spans(
    text: str, *, raw_spans: list[tuple[int, int]] | None = None,
) -> Iterable[tuple[int, int]]:
    logical, offsets = comment_lexical_view(text)
    index = 0
    size = len(logical)

    def original(position: int) -> int:
        if position == size:
            return len(text)
        return position if offsets is None else offsets[position]

    while match := COMMENT_TOKEN.search(logical, index):
        token = match.group()
        index = match.end()
        if token == "//":
            start = original(match.start())
            newline = COMMENT_LINE_END.search(logical, index)
            index = newline.start() if newline else size
            yield start, original(index)
            continue

        if token == "/*":
            start = original(match.start())
            end = logical.find("*/", index)
            if end >= 0:
                index = end + 2
                end = original(index - 1) + 1
            else:
                index = size
                end = len(text)
            yield start, end
            continue

        if match.group(1) is not None:
            delimiter = match.group(1)
            closer = f'){delimiter}"'
            content_start = original(index - 1) + 1
            end = text.find(closer, content_start)
            if end < 0:
                break
            end += len(closer)
            if raw_spans is not None:
                raw_spans.append((original(match.start()), end))
            index = end if offsets is None else bisect_left(offsets, end)
            continue

        if token in {'"', "'"}:
            while quote := CODE_QUOTE_TOKEN.search(logical, index):
                index = quote.end()
                if quote.group() == token:
                    break
            else:
                return


def strip_comments(
    text: str, keep_comments: Iterable[str] = (), *,
    directive_splices: set[int] | None = None,
) -> str:
    keep = set(keep_comments)
    output: list[str] = []
    previous = 0
    logical_line = ""
    raw_spans: list[tuple[int, int]] = []
    raw_cursor = 0
    current_line = 0
    for start, end in comment_spans(text, raw_spans=raw_spans):
        comment = text[start:end]
        gap = text[previous:start]
        output.append(gap)
        current_line += gap.count("\n")
        visible_gap: list[str] = []
        gap_cursor = previous
        for raw_start, raw_end in raw_spans[raw_cursor:]:
            visible_gap.append(text[gap_cursor:raw_start])
            visible_gap.append(text[raw_start:raw_end].replace("\r", " ").replace("\n", " "))
            gap_cursor = raw_end
        visible_gap.append(text[gap_cursor:start])
        raw_cursor = len(raw_spans)
        logical_gap = COMMENT_LINE_SPLICE.sub("", "".join(visible_gap))
        logical_line = (logical_line + logical_gap).rsplit("\n", 1)[-1]
        block = is_block_comment(comment)
        multiline_directive_comment = block and "\n" in comment and logical_line.lstrip().startswith(("#", "%:"))
        newline = "\\\n" if multiline_directive_comment else "\n"
        if multiline_directive_comment and comment not in keep and directive_splices is not None:
            directive_splices.add(current_line)
        output.append(comment if comment in keep else
                      (" " if block else "") + newline * comment.count("\n"))
        logical_line += " "
        current_line += comment.count("\n")
        previous = end
    output.append(text[previous:])
    return "".join(output)


def preprocessor_lines(text: str) -> list[str]:
    """Return comment-free lines, masking lines continued from a literal or macro."""

    directive_splices: set[int] = set()
    lines = strip_comments(text, directive_splices=directive_splices).splitlines(keepends=True)
    lines.extend([""] * (len(text.splitlines(keepends=True)) - len(lines)))
    result: list[str] = []
    raw_closer: str | None = None
    quote: str | None = None
    previous_continues = False

    for line_index, line in enumerate(lines):
        protected_at_start = (
            raw_closer is not None or quote is not None or previous_continues
        )
        if protected_at_start:
            result.append("\n" if line.endswith("\n") else "")
        elif line_index in directive_splices:
            head = line.rstrip("\r\n")
            result.append(head[:-1] + line[len(head):])
        else:
            result.append(line)

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

            if quote is not None:
                while quoted := CODE_QUOTE_TOKEN.search(line, index):
                    index = quoted.end()
                    if quoted.group() == quote:
                        quote = None
                        break
                else:
                    index = len(line)
                continue

            match = PREPROCESSOR_TOKEN.search(line, index)
            if match is None:
                break
            index = match.end()
            if match.group(1) is not None:
                raw_closer = f'){match.group(1)}"'
            elif match.group() in {'"', "'"}:
                quote = match.group()

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

    # Comment contents must not open a raw/string literal in this scanner, and
    # blank lines inside a block comment are part of the author's formatting.
    comment_lines: set[int] = set()
    previous_end = previous_line = 0
    for start, end in comment_spans(text):
        first = previous_line + text.count("\n", previous_end, start)
        last = first + text.count("\n", start, end)
        comment_lines.update(range(first, last + 1))
        previous_end, previous_line = end, last
    lines = text.splitlines(keepends=True)
    visible = strip_comments(text).splitlines(keepends=True)
    visible.extend([""] * (len(lines) - len(visible)))
    for line_index, (original, line) in enumerate(zip(lines, visible)):
        preserve_indentation = (
            raw_closer is not None or quote is not None or previous_continues
        )
        protected = preserve_indentation or line_index in comment_lines
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

            raw_match = RAW_STRING_OPEN.match(line, index)
            if raw_match:
                protected = True
                delimiter = raw_match.group(1)
                raw_closer = f'){delimiter}"'
                index += len(raw_match.group(0))
                continue

            ch = line[index]
            if ch.isdigit() or ch == ".":
                number = CPP_NUMBER.match(line, index)
                if number:
                    index = number.end()
                    continue
            if ch in {'"', "'"}:
                protected = True
                quote = ch
                index += 1
                continue
            index += 1

        previous_continues = line.rstrip(" \t\r\n").endswith("\\")
        if protected:
            output.append(original)
            previous_blank = False
            continue
        if line.strip():
            output.append(original)
            previous_blank = False
            continue
        if not previous_blank:
            output.append(original)
            previous_blank = True

    return "".join(output)


def wrap_include_directives(text: str) -> tuple[str, dict[int, str]]:
    lines = text.splitlines(keepends=True)
    visible = preprocessor_lines(text)
    output: list[str] = []
    includes: dict[int, str] = {}
    index = 0

    while index < len(lines):
        line = lines[index]
        is_line_directive = LINE_DIRECTIVE.match(visible[index].rstrip("\r\n")) is not None
        if not INCLUDE_DIRECTIVE.match(visible[index]) and not is_line_directive:
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


def wrap_portable_conditionals(text: str) -> tuple[str, dict[int, str]]:
    """Retain f128 feature selection for compilation on the judge's target."""
    lines = text.splitlines(keepends=True)
    visible = preprocessor_lines(text)
    feature = re.compile(r"\b(?:LIBRARY_HAS_FLOAT128|LIBRARY_USE_QUADMATH|__STDCPP_FLOAT128_T__)\b")
    stack: list[tuple[int, bool]] = []
    spans: list[tuple[int, int]] = []
    for index, line in enumerate(visible):
        if CONDITIONAL_OPEN_DIRECTIVE.match(line):
            stack.append((index, feature.search(line) is not None))
        elif CONDITIONAL_BRANCH_DIRECTIVE.match(line) and stack:
            start, portable = stack[-1]
            stack[-1] = (start, portable or feature.search(line) is not None)
        elif CONDITIONAL_CLOSE_DIRECTIVE.match(line) and stack:
            start, portable = stack.pop()
            if portable:
                spans.append((start, index + 1))

    output: list[str] = []
    blocks: dict[int, str] = {}
    cursor = 0
    for start, end in sorted(spans):
        if start < cursor:
            continue
        output.extend(lines[cursor:start])
        block_id = len(blocks)
        blocks[block_id] = "".join(lines[start:end])
        output.append(f"#pragma submit_code_portable_begin {block_id}\n")
        output.append(blocks[block_id])
        if not blocks[block_id].endswith("\n"):
            output.append("\n")
        output.append(f"#pragma submit_code_portable_end {block_id}\n")
        cursor = end
    output.extend(lines[cursor:])
    return "".join(output), blocks


def filter_preprocessor_output(
    output: str,
    *,
    source_path: Path,
    includes: dict[int, str],
    portable_blocks: dict[int, str] | None = None,
) -> str:
    source_resolved = source_path.resolve()
    in_source = False
    source_files = {str(source_path): True, str(source_resolved): True}
    result: list[str] = []
    begin_pattern = re.compile(r"^\s*#pragma\s+submit_code_include_begin\s+(\d+)\s*$")
    end_pattern = re.compile(r"^\s*#pragma\s+submit_code_include_end\s+(\d+)\s*$")
    portable_begin = re.compile(r"^\s*#pragma\s+submit_code_portable_begin\s+(\d+)\s*$")
    portable_end = re.compile(r"^\s*#pragma\s+submit_code_portable_end\s+(\d+)\s*$")
    in_portable = False

    for line in output.splitlines(keepends=True):
        marker = GNU_LINE_MARKER.match(line.rstrip("\r\n"))
        if marker:
            try:
                raw_path = json.loads(marker.group(2))
            except json.JSONDecodeError:
                in_source = False
                continue
            if raw_path not in source_files:
                source_files[raw_path] = (
                    not raw_path.startswith("<")
                    and Path(raw_path).resolve() == source_resolved
                )
            in_source = source_files[raw_path]
            continue

        if not in_source:
            continue
        if portable_end.match(line):
            in_portable = False
            continue
        if in_portable:
            continue
        portable = portable_begin.match(line)
        if portable:
            assert portable_blocks is not None
            result.append(portable_blocks[int(portable.group(1))])
            in_portable = True
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


def safe_cleanup_candidate(
    text: str, compiler: CompilerWorkspace, keep_comments: Iterable[str] = (),
) -> str:
    protected, comments = protect_cleanup_comments(text, keep_comments)
    protected, links = protect_source_links(protected)
    uncommented = strip_comments(protected, comments)
    portable, portable_blocks = wrap_portable_conditionals(uncommented)
    wrapped, includes = wrap_include_directives(portable)
    valid, output, stderr = compiler.preprocess_directives(wrapped, keep_comments=bool(comments))
    if not valid:
        raise ToolError(f"preprocessor failed: {first_error(stderr)}")
    filtered = filter_preprocessor_output(
        output,
        source_path=compiler.source,
        includes=includes,
        portable_blocks=portable_blocks,
    )
    filtered = restore_source_links(filtered, links)
    cleaned = restore_cleanup_comments(filtered, comments)
    return compact_source_links(collapse_blank_lines(retain_important_comments(cleaned, important_comments(text))))


def try_size_checkpoint(
    text: str, *, compiler: CompilerWorkspace, writer: CheckpointWriter,
    reporter: Reporter, limit: int, keep_comments: Iterable[str] = (),
    reference_checked: bool = False,
) -> str | None:
    candidate = text if writer.below_limit(text, limit) else collapse_blank_lines(
        strip_cleanup_comments(text, keep_comments))
    if not writer.below_limit(candidate, limit):
        try:
            candidate = safe_cleanup_candidate(text, compiler, keep_comments)
        except ToolError:
            return None
    if not writer.below_limit(candidate, limit):
        return None
    reporter.phase("checkpoint", f"validating the {limit:,} B checkpoint")
    if not reference_checked:
        valid, diagnostic = compiler.validate_submission(text)
        if not valid:
            reporter.detail("checkpoint reference rejected: " + first_error(diagnostic))
            return None
    valid, diagnostic = compiler.validate_submission(candidate)
    if not valid:
        reporter.detail("checkpoint rejected: " + first_error(diagnostic))
        return None
    writer.save(f"verified {limit:,} B checkpoint", candidate, force=True)
    writer.report_checkpoint(candidate, limit)
    return candidate


def insert_anonymous_namespace(text: str, *, source_name: str | None = None) -> str:
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
    standard_namespaces: list[tuple[int, int]] = []
    main_pattern = re.compile(r"^\s*(?:[_A-Za-z]\w*(?:::\w+)*[\s*&]+)*main\s*\(")
    for line_index, line in enumerate(lines):
        if brace_depth == 0 and main_pattern.match(line):
            main_line = line_index
            break
        if line.lstrip().startswith("#"):
            continue
        if brace_depth == 0 and re.match(r"^\s*namespace\s+std\b", line):
            end = find_braced_entity_end(tokens, line_index)
            if end is None:
                return text
            standard_namespaces.append((line_index, end))
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

    # Standard-library specializations must stay in the global std namespace.
    # Separate anonymous-namespace blocks still refer to the same namespace.
    wrapped = [*lines[:include_end], "namespace {\n"]
    def coordinate(line: int) -> list[str]:
        return [f"#line {line + 1} {json.dumps(source_name)}\n"] if source_name else []
    wrapped.extend(coordinate(include_end))
    cursor = include_end
    for start, end in standard_namespaces:
        if not include_end <= start < main_line:
            continue
        wrapped.extend([*lines[cursor:start], "}\n", *coordinate(start), *lines[start : end + 1], "namespace {\n", *coordinate(end + 1)])
        cursor = end + 1
    wrapped.extend([*lines[cursor:main_line], "}\n", *coordinate(main_line), *lines[main_line:]])
    return "".join(wrapped)


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
            if ch.isdigit() or ch == ".":
                number = CPP_NUMBER.match(line, index)
                if number:
                    index = number.end()
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
    if COMMENT_LINE_SPLICE.search(text):
        raw_spans: list[tuple[int, int]] = []
        spans = list(comment_spans(text, raw_spans=raw_spans)) + raw_spans
        pieces = []
        previous = 0
        for start, end in sorted(spans):
            pieces.extend((text[previous:start], " "))
            previous = end
        pieces.append(text[previous:])
        text = COMMENT_LINE_SPLICE.sub("", "".join(pieces))
    if text.isascii():
        index = 0
        while match := CODE_IDENTIFIER_TOKEN.search(text, index):
            token = match.group()
            index = match.end()
            if token == 'R"':
                raw = RAW_STRING_OPEN.match(text, match.start())
                if raw is not None:
                    end = text.find(')' + raw.group(1) + '"', raw.end())
                    if end < 0:
                        return
                    index = end + len(raw.group(1)) + 2
                    continue
                yield 'R'
                index -= 1
            elif token == '//':
                end = text.find('\n', index)
                if end < 0:
                    return
                index = end + 1
            elif token == '/*':
                end = text.find('*/', index)
                if end < 0:
                    return
                index = end + 2
            elif token in {'"', "'"}:
                while quote := CODE_QUOTE_TOKEN.search(text, index):
                    index = quote.end()
                    if quote.group() == token:
                        break
                else:
                    return
            elif token[0] == '_' or token[0].isalpha():
                yield token
        return
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
        if ch.isdigit() or ch == ".":
            number = CPP_NUMBER.match(text, index)
            if number:
                index = number.end()
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


def template_prefix_start(
    lines: Sequence[str], start: int, tokens: Sequence[Sequence[str]]
) -> int:
    limit = max(0, start - 64)
    line_index = start - 1
    while line_index >= limit:
        stripped = lines[line_index].strip()
        if re.match(r"requires\b", stripped):
            stack: list[str] = []
            for token in tokens[line_index]:
                if token in "([{":
                    stack.append(token)
                elif token in ")]}":
                    if not stack or stack.pop() != {")": "(", "]": "[", "}": "{"}[token]:
                        stack = ["unbalanced"]
                        break
            if not stack:
                # Includes parenthesized constraints such as
                # requires(!requires { expression; }).
                line_index -= 1
                continue
        # A requires-expression has its own braces and semicolons. Keep that
        # whole constraint attached to the template instead of leaving it
        # behind when the following definition is deleted.
        if tokens[line_index] and tokens[line_index][-1] == "}":
            depth = 0
            opening_line: int | None = None
            for previous in range(line_index, limit - 1, -1):
                for token in reversed(tokens[previous]):
                    if token == "}":
                        depth += 1
                    elif token == "{":
                        depth -= 1
                        if depth == 0:
                            opening_line = previous
                            break
                if opening_line is not None:
                    break
            if opening_line is not None:
                prefix = "".join(lines[limit:opening_line])
                prefix += lines[opening_line].split("{", 1)[0] + "{"
                constraint = re.search(
                    r"\brequires\s+requires\s*(?:\([^{};]*\))?\s*\{$",
                    strip_comments(prefix),
                )
                if constraint is not None:
                    # strip_comments preserves newlines, including multi-line comments.
                    requires_line = limit + strip_comments(prefix)[:constraint.start()].count("\n")
                    if re.match(r"\s*template\b", lines[requires_line]):
                        return requires_line
                    line_index = requires_line - 1
                    continue
        # A requires-clause or a split return type can lie between the
        # template-head and the function name. Do not leave it behind when
        # deleting the declaration, or repeatedly retry an invalid fragment.
        if (
            any(token in ";{}" for token in tokens[line_index])
            or stripped.startswith("#")
            or re.match(r"(?:public|private|protected)\s*:", stripped)
        ):
            break
        if stripped == "template" or re.match(r"template\s*<", stripped):
            return line_index
        line_index -= 1
    return start


def ipa_removed_signatures(cgraph: str) -> set[str]:
    if "Initial Symbol table:" not in cgraph or "Removing unused symbols:" not in cgraph:
        return set()
    initial = cgraph.split("Initial Symbol table:", 1)[1].split(
        "Removing unused symbols:", 1
    )[0]
    signatures: dict[str, str] = {}
    for line in initial.splitlines():
        match = re.match(r"^\S.*/(\d+) \((.*)\)$", line)
        if match is not None:
            signatures[match.group(1)] = match.group(2)

    removed = cgraph.split("Removing unused symbols:", 1)[1].split(
        "Reclaimed Symbol table:", 1
    )[0]
    result = {
        signatures[symbol_id]
        for symbol_id in re.findall(r"/(\d+)(?=\s|$)", removed)
        if symbol_id in signatures
    }
    if "Optimized Symbol table:" in cgraph:
        optimized = cgraph.split("Optimized Symbol table:", 1)[1].split("Final Symbol table:", 1)[0]
        optimized_symbols = [match.groups() for line in optimized.splitlines()
                             if (match := re.match(r"^\S.*/(\d+) \((.*)\)$", line))]
        remaining = {symbol_id for symbol_id, _ in optimized_symbols}
        optimized_signatures = {signature for _, signature in optimized_symbols}
        remaining.update(symbol_id for symbol_id, signature in signatures.items() if signature in optimized_signatures)
        dependencies: dict[str, set[str]] = {}
        symbol_id: str | None = None
        for line in initial.splitlines():
            if match := re.match(r"^\S.*/(\d+) \(.*\)$", line):
                symbol_id = match.group(1)
            elif symbol_id is not None and re.match(r"\s+(?:Calls|References):", line):
                dependencies.setdefault(symbol_id, set()).update(re.findall(r"/(\d+)(?=\s|$)", line))
        reachable = set(remaining)
        pending = list(remaining)
        while pending:
            added = dependencies.get(pending.pop(), set()) - reachable
            reachable.update(added)
            pending.extend(added)
        result.update(signature for symbol_id, signature in signatures.items() if symbol_id not in reachable)
    return result


def ipa_function_locations(
    original: Path,
    *,
    signatures: set[str],
    source_name: str,
) -> list[tuple[int, str]]:
    result: set[tuple[int, str]] = set()
    signature: str | None = None
    records: dict[str, str] = {}
    record = ""
    record_id = ""
    location_pattern = re.compile(
        rf"\bsrcp:\s+(?:.*[/\\])?{re.escape(source_name)}:(\d+)"
    )

    def finish_record() -> None:
        if record:
            records[record_id] = record

    def finish_function() -> None:
        if signature is None:
            return
        prefix = signature.split(" [with ", 1)[0]
        if re.search(r"\)\s*::", prefix):
            return
        names = re.findall(r"(~?[_A-Za-z]\w*)\s*\(", prefix)
        if not names or "<lambda" in prefix:
            return
        name = names[0]
        results = [value for value in records.values() if re.match(r"@\d+\s+result_decl\b", value)]
        parameters = [value for value in records.values() if re.match(r"@\d+\s+parm_decl\b", value)]
        for value in results + parameters:
            scope = re.search(r"\bscpe:\s+@(\d+)", value)
            declaration = records.get(scope.group(1), "") if scope else ""
            if not re.match(r"@\d+\s+function_decl\b", declaration):
                continue
            node = re.search(r"\bname:\s+@(\d+)", declaration)
            spelling = records.get(node.group(1), "") if node else ""
            if re.search(rf"\bstrg:\s+{re.escape(name)}\s+lngt:", spelling) is None:
                continue
            match = location_pattern.search(declaration)
            if match:
                result.add((int(match.group(1)), signature))
                return

    with original.open(encoding="utf-8", errors="replace") as dump:
        for line in dump:
            if line.startswith(";; Function "):
                finish_record()
                finish_function()
                header = line.rstrip()[len(";; Function ") :]
                candidate = header.rsplit(" (", 1)[0]
                signature = candidate if candidate in signatures else None
                records, record, record_id = {}, "", ""
            elif signature is not None:
                match = re.match(r"@(\d+)\s+(\w+)\b", line)
                if match:
                    finish_record()
                    record_id = match.group(1)
                    record = line if match.group(2) in {"result_decl", "parm_decl", "function_decl", "identifier_node"} else ""
                elif record:
                    record += line
        finish_record()
        finish_function()
    return sorted(result)


@dataclass(frozen=True, order=True)
class Removal:
    start: int
    end: int
    label: str
    name: str | None = field(default=None, compare=False)

    @property
    def size(self) -> int:
        return self.end - self.start + 1


def removal_from_function_line(
    lines: Sequence[str], tokens: Sequence[Sequence[str]], line_index: int, label: str
) -> Removal | None:
    if not 0 <= line_index < len(lines):
        return None
    source = "".join(lines[line_index:])
    body = function_body_span(source)
    if body is None:
        return None
    end = line_index + source.count("\n", 0, body[1])
    start = template_prefix_start(lines, line_index, tokens)
    return Removal(start, end, label)


def ipa_unused_candidates(
    text: str,
    *,
    compiler: CompilerWorkspace,
    keep_functions: set[str],
    functions: Sequence[tuple[int, str]] | None = None,
) -> list[Removal]:
    if functions is None:
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
    return unreferenced_function_candidates(text, deduplicated, keep_functions)


def uninstantiated_template_candidates(
    text: str,
    *,
    compiler: CompilerWorkspace,
    keep_functions: set[str],
) -> list[Removal]:
    lines = text.splitlines(keepends=True)
    tokens = structural_tokens(lines)
    candidates: list[Removal] = []
    for line_number, name in compiler.uninstantiated_template_functions(text):
        line_index = line_number - 1
        if not 0 <= line_index < len(lines) or "(" not in lines[line_index]:
            continue
        if name in {"__ct", "__dt"}:
            # GCC's internal constructor/destructor names do not appear in
            # linker diagnostics. Use the source name to retain required
            # definitions without bisecting every otherwise valid candidate.
            special_member = re.match(
                r"^\s*(?:(?:explicit|constexpr|consteval|inline)\s+)*"
                r"(~?[_A-Za-z]\w*)\s*\(", lines[line_index]
            )
            if special_member is None or special_member.group(1) in {"explicit", "requires"}:
                continue
            name = special_member.group(1)
        if not name:
            source = "".join(lines[line_index : min(len(lines), line_index + 3)])
            operator = re.search(
                r"\boperator\s*(\[\]|\(\)|<=>|<<=?|>>=?|->\*?|"
                r"[+\-*/%&|^~!=<>]=?|[_A-Za-z]\w*)",
                source,
            )
            if operator is None:
                continue
            name = "operator" + operator.group(1)
        if name.startswith("operator") and "operator" not in set(code_identifiers(lines[line_index])):
            # GCC also reports the generated call operators of generic lambdas.
            # Their source is a lambda expression, which cannot be stubbed as a
            # function declaration ("auto f = [](auto x);").
            continue
        if name in keep_functions:
            continue
        removal = removal_from_function_line(
            lines,
            tokens,
            line_index,
            "uninstantiated template function",
        )
        if removal is not None:
            candidates.append(
                Removal(removal.start, removal.end, removal.label, name)
            )

    deduplicated: list[Removal] = []
    for candidate in sorted(set(candidates), key=lambda value: (value.start, -value.end)):
        if any(
            existing.start <= candidate.start and candidate.end <= existing.end
            for existing in deduplicated
        ):
            continue
        deduplicated.append(candidate)
    return deduplicated


def unreachable_function_body_candidates(
    text: str, *, compiler: CompilerWorkspace, keep_functions: set[str],
    functions: Sequence[tuple[int, str]] | None = None,
) -> list[Removal]:
    lines = text.splitlines(keepends=True)
    tokens = structural_tokens(lines)
    candidates = set()
    if functions is None:
        functions = compiler.ipa_unused_functions(text)
    for line_number, signature in functions:
        if "<lambda" in signature:
            continue
        removal = removal_from_function_line(lines, tokens, line_number - 1, "unreachable function body")
        if removal is None:
            continue
        source = "".join(lines[removal.start:removal.end + 1])
        name = function_header_name(source)
        if name is None or name in keep_functions:
            continue
        if re.search(rf"\b{re.escape(name)}\s*\(", signature.split(" [with ", 1)[0]) is None:
            continue
        candidates.add(Removal(removal.start, removal.end, removal.label, name))
    output = []
    for candidate in sorted(candidates, key=lambda value: (value.start, -value.end)):
        if not any(previous.start <= candidate.start and candidate.end <= previous.end for previous in output):
            output.append(candidate)
    return output


def function_body_span(source: str) -> tuple[int, int] | None:
    """Locate a function body, passing over braced constructor initializers."""
    paren_depth = bracket_depth = brace_depth = 0
    opening: int | None = None
    constraint_body = False
    index = 0

    def skip_trivia(position: int) -> int:
        while position < len(source):
            if source[position].isspace():
                position += 1
            elif source.startswith("//", position):
                end = source.find("\n", position + 2)
                position = len(source) if end < 0 else end + 1
            elif source.startswith("/*", position):
                end = source.find("*/", position + 2)
                position = len(source) if end < 0 else end + 2
            else:
                break
        return position

    while index < len(source):
        index = skip_trivia(index)
        if index >= len(source):
            break
        raw = RAW_STRING_OPEN.match(source, index)
        if raw:
            closer = ')' + raw.group(1) + '"'
            end = source.find(closer, raw.end())
            if end < 0:
                return None
            index = end + len(closer)
            continue
        ch = source[index]
        if ch.isdigit() or ch == ".":
            number = CPP_NUMBER.match(source, index)
            if number:
                index = number.end()
                continue
        if ch in {'"', "'"}:
            quote = ch
            index += 1
            while index < len(source):
                if source[index] == "\\":
                    index += 2
                elif source[index] == quote:
                    index += 1
                    break
                else:
                    index += 1
            continue
        if opening is not None:
            if ch == "{":
                brace_depth += 1
            elif ch == "}":
                brace_depth -= 1
                if brace_depth == 0:
                    after = skip_trivia(index + 1)
                    if source.startswith("...", after):
                        after = skip_trivia(after + 3)
                    if constraint_body or (after < len(source) and source[after] in ",{"):
                        # C() : member{...}, other(...) { actual body }
                        opening = None
                    else:
                        return opening, index
        elif ch == "(":
            paren_depth += 1
        elif ch == ")":
            paren_depth = max(0, paren_depth - 1)
        elif ch == "[":
            bracket_depth += 1
        elif ch == "]":
            bracket_depth = max(0, bracket_depth - 1)
        elif ch == "{" and paren_depth == bracket_depth == 0:
            opening = index
            brace_depth = 1
            constraint_body = re.search(
                r"\brequires\s+requires\s*(?:\([^{};]*\))?\s*$",
                strip_comments(source[:index]),
            ) is not None
        elif ch == ";" and paren_depth == bracket_depth == 0:
            return None
        index += 1
    return None


def template_signature_macro_names(
    text: str, names: dict[str, str] | Iterable[str] = (),
) -> set[str]:
    definitions: dict[str, list[str]] = {}
    dangerous = set() if isinstance(names, dict) else set(names)
    if isinstance(names, dict):
        for name, replacement in names.items():
            definitions.setdefault(name, []).append(replacement)
    logical = re.sub(r"\\\r?\n", "", strip_comments(text)).replace("%:", "#")
    for line in logical.splitlines():
        match = re.match(r"\s*#\s*define\s+([_A-Za-z]\w*)(.*)", line)
        if match:
            definitions.setdefault(match.group(1), []).append(match.group(2))
    references: dict[str, set[str]] = {}
    semantic = {
        "auto", "constexpr", "consteval", "virtual", "friend", "operator", "explicit",
        "__attribute__", "__attribute", "__declspec", "asm", "__asm", "__asm__",
        "__VA_OPT__", "__COUNTER__",
    }
    for name, replacements in definitions.items():
        references[name] = set()
        for replacement in replacements:
            identifiers = set(code_identifiers(replacement))
            references[name].update(identifiers)
            if identifiers.intersection(semantic) or any(char in replacement for char in "#[]\\"):
                dangerous.add(name)
    while True:
        added = {name for name, dependencies in references.items()
                 if dependencies.intersection(dangerous)} - dangerous
        if not added:
            return dangerous
        dangerous.update(added)


def template_function_signature(
    source: str, name: str | None, *, macro_names: Iterable[str] = (),
) -> str | None:
    if name is None or (IDENTIFIER.fullmatch(name) is None and re.fullmatch(
        r"operator(?:\[\]|\(\)|<=>|<<|>>|[-+*/%&|^~!=<>]+)", name,
    ) is None) or name == "operator=":
        return None
    body = function_body_span(source)
    if body is None:
        return None
    if strip_comments(source[body[1] + 1:]).strip():
        return None
    declaration = strip_comments(source[:body[0]])
    identifiers = set(code_identifiers(declaration))
    spelling_matches = function_header_name(source) == name if name.startswith("operator") else name in identifiers
    if not spelling_matches or identifiers.intersection(macro_names):
        return None
    inspected = re.sub(r"\b__attribute__\s*\(\(\s*(?:always_inline|noinline)(?:\s*,\s*(?:always_inline|noinline))*\s*\)\)", " ", declaration)
    if "[[" in inspected or re.search(
        r"\b(?:friend|virtual|override|final|__attribute__|__declspec|asm)\b", inspected,
    ):
        return None
    if re.search(
        rf"(?m)^\s*(?:(?:explicit(?:\([^)]*\))?|constexpr|consteval|inline)\s+)*"
        rf"{re.escape(name)}\s*\(", inspected,
    ):
        return None
    return declaration


def template_member_candidates(text: str, candidates: Sequence[Removal]) -> set[Removal]:
    visible = preprocessor_lines(text)
    tokens = structural_tokens(["\n" if line.lstrip().startswith("#") else line for line in visible])
    namespaces: list[tuple[int, int]] = []
    for index, line in enumerate(visible):
        if re.match(r"^\s*(?:inline\s+)?namespace\b", line):
            end = find_braced_entity_end(tokens, index)
            if end is not None:
                namespaces.append((index, end))
    depths = [0]
    for row in tokens:
        depths.append(depths[-1] + row.count("{") - row.count("}"))
    return {candidate for candidate in candidates if depths[candidate.start] != sum(
        first < candidate.start <= last for first, last in namespaces
    )}


def portable_body_dependencies(
    text: str, candidates: Sequence[Removal], external_names: Iterable[str] = (),
) -> set[str]:
    visible = preprocessor_lines(text)
    depth = 0
    definitions = []
    altered = False
    for line in visible:
        if match := re.match(r"\s*#\s*define\s+LIBRARY_HAS_FLOAT128\b(.*)", line):
            definitions.append((depth, match.group(1).strip()))
        if re.match(r"\s*#\s*undef\s+LIBRARY_HAS_FLOAT128\b", line) or re.search(r"\b(?:push_macro|pop_macro)\b", line):
            altered = True
        if CONDITIONAL_OPEN_DIRECTIVE.match(line):
            depth += 1
        elif CONDITIONAL_CLOSE_DIRECTIVE.match(line):
            depth -= 1
    target = text
    if definitions and all(value == (0, "1") for value in definitions) and not altered and "LIBRARY_HAS_FLOAT128" not in set(external_names):
        target = "".join(re.sub(r"\bLIBRARY_HAS_FLOAT128\b", "1", line)
                         if CONDITIONAL_OPEN_DIRECTIVE.match(view) or CONDITIONAL_BRANCH_DIRECTIVE.match(view)
                         else line for line, view in zip(text.splitlines(keepends=True), visible))
    _, blocks = wrap_portable_conditionals(target)
    operator_symbols = {candidate.name: candidate.name[len("operator"):]
                        for candidate in candidates if (candidate.name or "").startswith("operator")}

    def referenced_names(source: str) -> set[str]:
        names = set(code_identifiers(source))
        visible = strip_comments(source)
        names.update(name for name, symbol in operator_symbols.items()
                     if symbol and (symbol in visible or (symbol == "[]" and "[" in visible) or
                                    (symbol == "()" and "(" in visible)))
        return names

    protected = referenced_names("\n".join(blocks.values()))
    if not protected:
        return protected
    lines = text.splitlines(keepends=True)
    references: dict[str, set[str]] = {}
    for candidate in candidates:
        if candidate.name:
            references.setdefault(candidate.name, set()).update(referenced_names(
                "".join(lines[candidate.start:candidate.end + 1])
            ))
    offset = 0
    for line, visible in zip(lines, preprocessor_lines(text)):
        statement = re.match(
            r"\s*(?:if|else|for|while|switch|catch|return|co_return|static_assert|assert|case|do|using)\b",
            visible,
        )
        direct_call = re.match(r"\s*[_A-Za-z]\w*\s*\(", visible)
        assignment = "=" in visible.partition("(")[0]
        if "(" in visible and not (visible.lstrip().startswith("#") or statement or direct_call or assignment):
            source = text[offset:]
            body = function_body_span(source)
            if body is not None:
                function_source = source[:body[1] + 1]
                name = function_header_name(function_source)
                names = [name] if name is not None else re.findall(
                    r"\b([_A-Za-z]\w*)\s*(?=\(|<[^;{}()]*>\s*\()", source[:body[0]],
                )
                if names:
                    identifiers = referenced_names(function_source)
                    for name in names:
                        references.setdefault(name, set()).update(identifiers)
        offset += len(line)
    pending = list(protected)
    while pending:
        dependencies = references.get(pending.pop(), set()) - protected
        protected.update(dependencies)
        pending.extend(dependencies)
    return protected


def unreferenced_template_declarations(
    text: str, candidates: Sequence[Removal], *, keep_names: Iterable[str] = (),
    macro_names: Iterable[str] = (),
) -> set[Removal]:
    if "__COUNTER__" in set(code_identifiers(text)):
        return set()
    lines = text.splitlines(keepends=True)
    macro_names = template_signature_macro_names(text, macro_names)
    members = template_member_candidates(text, candidates)
    eligible = [candidate for candidate in candidates if not (candidate.name or "").startswith("operator") and candidate not in members and template_function_signature(
        "".join(lines[candidate.start:candidate.end + 1]), candidate.name,
        macro_names=macro_names,
    ) is not None]
    by_name: dict[str, set[Removal]] = {}
    references: dict[Removal, set[str]] = {}
    for candidate in eligible:
        by_name.setdefault(candidate.name, set()).add(candidate)
        references[candidate] = set(code_identifiers(
            "".join(lines[candidate.start:candidate.end + 1])
        ))
    outside = set(code_identifiers(apply_removals(text, eligible)))
    outside.update(keep_names)
    pasted = token_paste_patterns(strip_comments(text))
    outside.update(name for name in by_name if any(pattern.fullmatch(name) for pattern in pasted))
    reachable = {candidate for name in outside for candidate in by_name.get(name, ())}
    pending = list(reachable)
    while pending:
        candidate = pending.pop()
        dependencies = {dependency for name in references[candidate]
                        for dependency in by_name.get(name, ())} - reachable
        reachable.update(dependencies)
        pending.extend(dependencies)
    return set(eligible) - reachable


def template_body_can_be_stubbed(
    source: str, name: str | None, *, macro_names: Iterable[str] = (),
) -> bool:
    declaration = template_function_signature(source, name, macro_names=macro_names)
    if declaration is None or re.search(r"\b(?:constexpr|consteval)\b", declaration):
        return False
    parsed = function_header_name_and_prefix(source)
    return_prefix = parsed[1] if parsed else declaration
    if re.search(r"\bdecltype\s*\(\s*auto\s*\)", return_prefix):
        return False
    if re.search(r"->.*\bauto\b", declaration, re.DOTALL):
        return False
    placeholder = re.search(rf"\bauto\s*(?:[&*]\s*)*{re.escape(name)}\s*\(", declaration)
    if "auto" in set(code_identifiers(return_prefix)) and placeholder is None:
        return False
    if placeholder and re.search(
        r"\([^{}]*\)\s*(?:const\s*)?(?:noexcept(?:\([^{}]*\))?\s*)?->", declaration[placeholder.end() - 1:],
    ) is None:
        return False
    return True


def stub_uninstantiated_template_functions(
    text: str,
    candidates: Sequence[Removal],
    *,
    deleted: set[Removal] | None = None,
    keep_names: Iterable[str] = (),
    macro_names: Iterable[str] = (),
    allow_portable_stubs: bool = False,
) -> str:
    notices = important_comments(text)
    lines = text.splitlines(keepends=True)
    macro_names = template_signature_macro_names(text, macro_names)
    protected_bodies = set() if allow_portable_stubs else portable_body_dependencies(text, candidates, keep_names)
    deleted = (deleted or set()).intersection(unreferenced_template_declarations(
        text, candidates, keep_names=keep_names, macro_names=macro_names,
    ))
    preserve_bodies = "__COUNTER__" in set(code_identifiers(text))
    for candidate in sorted(candidates, reverse=True):
        if candidate.name in protected_bodies:
            continue
        if candidate in deleted:
            lines[candidate.start : candidate.end + 1] = []
            continue
        source = "".join(lines[candidate.start : candidate.end + 1])
        if preserve_bodies or not template_body_can_be_stubbed(source, candidate.name, macro_names=macro_names):
            continue
        body = function_body_span(source)
        if body is None:
            continue
        opening, closing = body
        declaration = source[:opening].rstrip()
        initializer = re.search(r"\)\s*:(?!:)\s*", declaration)
        if initializer is not None:
            declaration = declaration[: initializer.start() + 1].rstrip()
        declaration += ";\n"
        lines[candidate.start : candidate.end + 1] = [declaration]
    return collapse_blank_lines(retain_important_comments("".join(lines), notices))


def removable_template_body_candidates(
    text: str, candidates: Sequence[Removal], *, keep_names: Iterable[str] = (),
    macro_names: Iterable[str] = (),
    allow_portable_stubs: bool = False,
) -> list[Removal]:
    lines = text.splitlines(keepends=True)
    macro_names = template_signature_macro_names(text, macro_names)
    if "__COUNTER__" in set(code_identifiers(text)):
        return []
    deletable = unreferenced_template_declarations(
        text, candidates, keep_names=keep_names, macro_names=macro_names,
    )
    protected_bodies = set() if allow_portable_stubs else portable_body_dependencies(text, candidates, keep_names)
    eligible: list[Removal] = []
    for candidate in candidates:
        if candidate.name in protected_bodies:
            continue
        source = "".join(lines[candidate.start : candidate.end + 1])
        if candidate not in deletable and not template_body_can_be_stubbed(
            source, candidate.name, macro_names=macro_names,
        ):
            continue
        eligible.append(candidate)
    return eligible


def reduce_template_body_candidates(
    text: str,
    candidates: Sequence[Removal],
    *,
    compiler: CompilerWorkspace,
    writer: CheckpointWriter,
    reporter: Reporter,
    target_bytes: int | None = None,
) -> tuple[str, int]:
    compiler.preserve_initialization(text)
    keep_names = set(compiler.external_identifiers(text))
    macro_names = template_signature_macro_names(text, compiler.external_macro_definitions(text))
    candidates = removable_template_body_candidates(
        text, candidates, keep_names=keep_names, macro_names=macro_names,
        allow_portable_stubs=compiler.portable_workspace is not None,
    )
    if not candidates:
        return text, 0
    started = time.monotonic()
    original_text = text
    accepted: set[Removal] = set()
    validations = 0
    rejected: dict[tuple[str, str | None, bool | None, bool], str] = {}

    def validate_candidate(candidate: str, action: str) -> tuple[bool, str]:
        nonlocal validations
        key = candidate, compiler.pch_key, compiler.pch_available, compiler.pch_disabled
        if key in rejected:
            return False, rejected[key]
        validations += 1
        reporter.phase("templates", f"validation {validations}: {action}")
        valid, diagnostic = compiler.validate(candidate, parallel_portable=True)
        if not valid:
            key = candidate, compiler.pch_key, compiler.pch_available, compiler.pch_disabled
            rejected[key] = diagnostic
        return valid, diagnostic

    target_checkpointed = writer.below_limit(text, target_bytes) or (
        writer.current is not None
        and writer.below_limit(writer.current, target_bytes)
    )

    # Keep declarations used by lookup, type traits, and pasted identifiers.
    # Only unreachable groups can be deleted together with their signatures.
    quick_deleted = unreferenced_template_declarations(
        text, candidates, keep_names=keep_names, macro_names=macro_names,
    )

    original_lines = original_text.splitlines(keepends=True)
    dependencies: dict[Removal, set[Removal]] | None = None

    def keep_dependencies(required: set[Removal]) -> set[Removal]:
        nonlocal dependencies
        if dependencies is None:
            by_name: dict[str, set[Removal]] = {}
            for candidate in candidates:
                if candidate.name:
                    by_name.setdefault(candidate.name, set()).add(candidate)
            dependencies = {}
            for candidate in candidates:
                source = "".join(original_lines[candidate.start : candidate.end + 1])
                calls = re.findall(
                    r"\b([_A-Za-z]\w*)\s*(?=\(|<[^;{}()]*>\s*\()",
                    strip_comments(source),
                )
                dependencies[candidate] = {
                    dependency
                    for name in calls
                    for dependency in by_name.get(name, ())
                }
        # Retaining a required definition also requires its template callees.
        # Keep same-named overloads conservatively, but do not interpret every
        # variable named "n" or "size" as a reference to a member function.
        # Indirect calls not recognized here are still checked by the linker.
        required = set(required)
        pending = list(required)
        while pending:
            for dependency in dependencies[pending.pop()] - required:
                required.add(dependency)
                pending.append(dependency)
        return required

    byte_sizes = {
        candidate: len(
            "".join(
                original_lines[candidate.start : candidate.end + 1]
            ).encode()
        )
        for candidate in candidates
    }

    def split(items: Sequence[Removal]) -> tuple[list[Removal], list[Removal]]:
        weights = {str(index): byte_sizes[item] for index, item in enumerate(items)}
        left_keys, _ = split_weighted(list(weights), weights)
        count = len(left_keys)
        return list(items[:count]), list(items[count:])

    def mentioned_in(candidate: Removal, diagnostics: str) -> bool:
        diagnostics = diagnostic_messages(diagnostics)
        if not candidate.name:
            return False
        if candidate.name.startswith("operator"):
            return candidate.name in diagnostics
        return re.search(
            rf"(?<![_A-Za-z0-9]){re.escape(candidate.name)}(?![_A-Za-z0-9])",
            diagnostics,
        ) is not None

    active = list(candidates)
    required_definitions: set[Removal] = set()
    for _ in range(min(32, len(candidates))):
        if not active:
            break
        candidate_text = stub_uninstantiated_template_functions(
            original_text, active, deleted=quick_deleted.intersection(active),
            keep_names=keep_names, macro_names=macro_names,
            allow_portable_stubs=compiler.portable_workspace is not None,
        )
        valid, stderr = validate_candidate(candidate_text, f"testing {len(active)} template body(s)")
        if valid:
            accepted.update(active)
            text = candidate_text
            writer.save(
                f"removed {len(active)} uninstantiated template body(s)", text
            )
            if not target_checkpointed and writer.below_limit(text, target_bytes):
                target_checkpointed = True
                writer.report_checkpoint(text, target_bytes)
            break

        blocked = {
            candidate
            for candidate in active
            if mentioned_in(candidate, stderr)
        }
        if not blocked:
            reporter.detail(first_error(stderr))
            if quick_deleted:
                # A declaration can carry a multi-line constraint or be named
                # through a macro. Fall back to retaining all signatures.
                quick_deleted.clear()
                continue
            break
        reporter.detail(
            "keeping required template definition(s): "
            + ", ".join(sorted({candidate.name or "" for candidate in blocked}))
        )
        required_definitions.update(blocked)
        blocked = keep_dependencies(required_definitions)
        if target_bytes is not None and not target_checkpointed:
            smallest = collapse_blank_lines(apply_removals(
                original_text, [candidate for candidate in candidates if candidate not in blocked]
            ))
            if not writer.below_limit(smallest, target_bytes):
                # Broad overload dependencies can keep too much code alive.
                # Prefer the compiler's precise checks when even deleting every
                # remaining candidate cannot meet the submission limit.
                blocked = required_definitions
        # Rebuild from the original set so a later size check can also release
        # dependencies that were kept speculatively on an earlier attempt.
        active = [candidate for candidate in candidates if candidate not in blocked]

    if accepted:
        deleted = unreferenced_template_declarations(
            original_text, list(accepted), keep_names=keep_names, macro_names=macro_names,
        )
        declaration_fallback: list[Removal] | None = None
        for _ in range(min(32, len(deleted))):
            candidate_text = stub_uninstantiated_template_functions(
                original_text, list(accepted), deleted=deleted,
                keep_names=keep_names, macro_names=macro_names,
                allow_portable_stubs=compiler.portable_workspace is not None,
            )
            valid, stderr = validate_candidate(candidate_text, f"removing {len(deleted)} declarations")
            if valid:
                text = candidate_text
                writer.save(
                    f"removed {len(deleted)} uninstantiated template declaration(s)",
                    text,
                )
                if not target_checkpointed and writer.below_limit(text, target_bytes):
                    target_checkpointed = True
                    writer.report_checkpoint(text, target_bytes)
                break

            blocked = {
                candidate
                for candidate in deleted
                if mentioned_in(candidate, stderr)
            }
            if not blocked:
                reporter.detail(
                    "template declaration cleanup was rejected: "
                    + first_error(stderr)
                )
                declaration_fallback = sorted(deleted)
                break
            reporter.detail(
                "keeping required template declaration(s): "
                + ", ".join(sorted({candidate.name or "" for candidate in blocked}))
            )
            deleted.difference_update(blocked)

        if declaration_fallback:
            deleted = quick_deleted.intersection(accepted)

            def try_delete(batch: Sequence[Removal]) -> bool:
                nonlocal text, target_checkpointed
                proposal = deleted.union(batch)
                candidate_text = stub_uninstantiated_template_functions(
                    original_text, list(accepted), deleted=proposal,
                    keep_names=keep_names, macro_names=macro_names,
                    allow_portable_stubs=compiler.portable_workspace is not None,
                )
                valid, stderr = validate_candidate(candidate_text, f"testing {len(batch)} declaration(s)")
                if not valid:
                    reporter.detail(first_error(stderr))
                    return False
                deleted.update(batch)
                text = candidate_text
                writer.save(
                    f"removed {len(batch)} uninstantiated template declaration(s)",
                    text,
                )
                if not target_checkpointed and writer.below_limit(text, target_bytes):
                    target_checkpointed = True
                    writer.report_checkpoint(text, target_bytes)
                return True

            def reduce_declarations(items: Sequence[Removal]) -> None:
                active_items = [item for item in items if item not in deleted]
                if not active_items or try_delete(active_items) or len(active_items) == 1:
                    return
                left, right = split(active_items)
                reduce_declarations(left)
                reduce_declarations(right)

            reduce_declarations(declaration_fallback)

    name_counts = Counter(candidate.name for candidate in candidates)
    refinement_candidates = [candidate for candidate in candidates if candidate not in accepted and (
        not accepted or name_counts[candidate.name] > 1
    )]
    if not refinement_candidates:
        reporter.done(
            "templates",
            f"accepted {len(accepted)}/{len(candidates)} template body(s)",
            started,
        )
        return text, len(accepted)

    def try_accept(batch: Sequence[Removal]) -> bool:
        nonlocal text, target_checkpointed
        proposal = accepted.union(batch)
        candidate_text = stub_uninstantiated_template_functions(
            original_text, proposal, deleted=quick_deleted.intersection(proposal),
            keep_names=keep_names, macro_names=macro_names,
            allow_portable_stubs=compiler.portable_workspace is not None,
        )
        valid, stderr = validate_candidate(candidate_text, f"testing {len(batch)} template body(s)")
        if not valid:
            reporter.detail(first_error(stderr))
            return False
        accepted.update(batch)
        text = candidate_text
        writer.save(f"removed {len(batch)} uninstantiated template body(s)", text)
        if not target_checkpointed and writer.below_limit(text, target_bytes):
            target_checkpointed = True
            writer.report_checkpoint(text, target_bytes)
        return True

    def reduce_batch(items: Sequence[Removal]) -> None:
        active = [item for item in items if item not in accepted]
        if not active or try_accept(active) or len(active) == 1:
            return
        left, right = split(active)
        reduce_batch(left)
        reduce_batch(right)

    reduce_batch(refinement_candidates)
    if accepted:
        candidate_text = stub_uninstantiated_template_functions(
            original_text, list(accepted),
            deleted=unreferenced_template_declarations(
                original_text, list(accepted), keep_names=keep_names, macro_names=macro_names,
            ),
            keep_names=keep_names, macro_names=macro_names,
            allow_portable_stubs=compiler.portable_workspace is not None,
        )
        valid, stderr = validate_candidate(candidate_text, f"removing {len(accepted)} declarations")
        if valid:
            text = candidate_text
            writer.save(
                f"removed {len(accepted)} uninstantiated template declaration(s)",
                text,
            )
            if not target_checkpointed and writer.below_limit(text, target_bytes):
                target_checkpointed = True
                writer.report_checkpoint(text, target_bytes)
        else:
            reporter.detail(
                "template declaration cleanup was rejected: " + first_error(stderr)
            )
    reporter.done(
        "templates",
        f"accepted {len(accepted)}/{len(candidates)} template body(s)",
        started,
    )
    return text, len(accepted)


def token_paste_patterns(text: str) -> list[re.Pattern[str]]:
    """Return conservative identifier patterns produced by token-pasting macros."""
    patterns: list[re.Pattern[str]] = []
    logical = COMMENT_LINE_SPLICE.sub("", text).replace("%:%:", "##")
    token = r"[_A-Za-z0-9]+"
    paste = re.compile(rf"{token}(?:\s*##\s*{token})+")
    for line in logical.splitlines():
        if "##" not in line:
            continue
        if re.search(r"[\\$]|[^\x00-\x7f]", line):
            return [re.compile(r".*")]
        macro = re.match(r"\s*#\s*define\s+[_A-Za-z]\w*(\([^)]*\))?(.*)", line)
        if macro is None:
            return [re.compile(r".*")]
        parameters = set(IDENTIFIER.findall(macro.group(1) or ""))
        if "..." in (macro.group(1) or ""):
            parameters.add("__VA_ARGS__")
        body = macro.group(2)
        if "__VA_OPT__" in body:
            return [re.compile(r".*")]
        expressions = list(paste.finditer(body))
        if sum(match.group().count("##") for match in expressions) != body.count("##"):
            return [re.compile(r".*")]
        for match in expressions:
            parts = re.split(r"\s*##\s*", match.group())
            patterns.append(re.compile("".join(
                r"\w*" if part in parameters else re.escape(part) for part in parts
            )))
    return patterns


def unreferenced_declaration_candidates(text: str, keep_names: Iterable[str] = (),
                                      protected_macros: Iterable[str] = (), *,
                                      include_friend_types: bool = False,
                                      include_structured_constants: bool = False) -> list[Removal]:
    """Find unreachable groups of types, aliases, and constant declarations.

    Names are deliberately unqualified: a same-named declaration in another
    scope keeps both alive. Initializers and declarations that can introduce
    runtime effects are not considered removable here.
    Friend types and structured constants require an object-equivalence check
    by the caller.
    """
    uncommented = strip_comments(text)
    source_names = set(code_identifiers(uncommented))
    if "__COUNTER__" in source_names:
        return []
    pasted_names = token_paste_patterns(uncommented)
    lines = uncommented.splitlines(keepends=True)
    visible_lines = preprocessor_lines(uncommented)
    if len(visible_lines) != len(lines):
        return []
    tokens = structural_tokens(lines)
    type_pattern = re.compile(r"^\s*(?:struct|class)\s+([_A-Za-z]\w*)\b")
    alias_pattern = re.compile(r"^\s*using\s+([_A-Za-z]\w*)\s*=")
    lambda_pattern = re.compile(
        r"^\s*(?:inline\s+)?constexpr\s+auto\s+([_A-Za-z]\w*)\s*=\s*\[\s*\]"
    )
    scalar_pattern = re.compile(
        r"^\s*(?:inline\s+)?constexpr\s+"
        r"(?:bool|char|char8_t|char16_t|char32_t|wchar_t|short|int|long|float|double|signed|unsigned)"
        r"(?:\s+(?:short|int|long|signed|unsigned))*\s+([_A-Za-z]\w*)"
        r"(?:\s*<[^;=(){}]*>)?\s*="
    )
    structured_pattern = re.compile(
        r"^\s*(?:inline\s+)?constexpr\s+(?:(?:std::)?array\s*<[^;={}]+>|uint|ull|ll|i128|u128)\s+"
        r"([_A-Za-z]\w*)\s*(?:\[[^;={}]*\]\s*)*="
    )
    candidates: list[Removal] = []
    references: dict[str, set[str]] = {}
    class_sources: dict[str, str] = {}
    skip_until = -1
    continued = False
    for index, line in enumerate(lines):
        directive = continued or line.lstrip().startswith("#")
        continued = directive and line.rstrip("\r\n").endswith("\\")
        if directive or index <= skip_until:
            continue
        type_match = type_pattern.match(visible_lines[index])
        alias_match = alias_pattern.match(visible_lines[index])
        lambda_match = lambda_pattern.match(visible_lines[index])
        scalar_match = scalar_pattern.match(visible_lines[index]) or (
            structured_pattern.match(visible_lines[index]) if include_structured_constants else None)
        match = type_match or alias_match or lambda_match or scalar_match
        if match is None:
            continue
        start = template_prefix_start(lines, index, tokens)
        if alias_match or scalar_match:
            end = find_statement_end(tokens, index)
        else:
            end = find_braced_entity_end(tokens, index)
        if end is None:
            continue
        skip_until = end
        # Removals cover complete lines. Do not accidentally remove another
        # declaration or an expression sharing the final line.
        statement_tokens = [token for row in tokens[index : end + 1] for token in row]
        depth = 0
        terminators: list[int] = []
        for position, token in enumerate(statement_tokens):
            if token in "([{":
                depth += 1
            elif token in ")]}":
                depth -= 1
            elif token == ";" and depth == 0:
                terminators.append(position)
        if terminators != [len(statement_tokens) - 1] or re.search(r";\s*$", lines[end]) is None:
            continue
        if not (alias_match or scalar_match) and re.search(r"}\s*;\s*$", lines[end]) is None:
            # In particular, retain "struct X { ... } object;" and invoked
            # lambdas: neither may be discarded just because X is unreferenced.
            continue
        source = "".join(lines[start : end + 1])
        if scalar_match and "," in source and not (
            include_structured_constants and (structured_pattern.match(visible_lines[index])
                                               or re.match(r"\s*template\s*<", source))):
            continue
        identifiers = set(code_identifiers(source))
        if identifiers.intersection(protected_macros):
            continue
        if "__COUNTER__" in identifiers:
            continue
        if re.search(r"\[\s*\[", strip_comments(source)) or identifiers.intersection({"__attribute__", "__declspec", "asm", "__asm__"}):
            continue
        if type_match:
            header = "".join(lines[start:index])
            is_template = "template" in header and re.search(r"template\s*<\s*>", header) is None
            if "friend" in identifiers and not include_friend_types:
                continue
            if not is_template and re.search(r"\bstatic\b[^(){};]*\b[_A-Za-z]\w*\s*(?:=|;|\{)", strip_comments(source)):
                continue
        name = match.group(1)
        candidates.append(Removal(start, end, f"unreferenced declaration {name}", name))
        references.setdefault(name, set()).update(identifiers)
        if type_match:
            class_sources[name] = source

    functions: list[Removal] = []
    skip_until = -1
    for index, line in enumerate(visible_lines):
        if source_names.intersection({"asm", "__asm__", "dlsym", "GetProcAddress"}) or index <= skip_until or "(" not in line or line.lstrip().startswith("#"):
            continue
        if re.match(r"\s*(?:if|else|for|while|switch|catch|return|co_return|static_assert|assert|case|do|using)\b", line):
            continue
        start = template_prefix_start(lines, index, tokens)
        if any(candidate.start <= start <= candidate.end for candidate in candidates):
            continue
        tail = "".join(lines[index:])
        body = function_body_span(tail)
        if body:
            end = index + tail.count("\n", 0, body[1])
        else:
            end = find_statement_end(tokens, index)
        if end is None:
            continue
        source = "".join(lines[start:end+1])
        name = function_header_name(source)
        if set(code_identifiers(source)).intersection(protected_macros):
            continue
        if name is None or name.startswith("operator") or name in DEFAULT_KEEP_FUNCTIONS | {"main"}:
            continue
        header = strip_comments(source[:function_body_span(source)[0]] if body else source)
        parsed = function_header_name_and_prefix(source)
        prefix = parsed[1] if parsed else ""
        if not prefix or "=" in prefix or prefix.endswith("~") or prefix in {"explicit", "constexpr", "consteval", "inline"}:
            continue
        if "[[" in header or set(code_identifiers(source)).intersection({"extern", "virtual", "override", "final", "__COUNTER__", "__attribute__", "__declspec"}):
            continue
        if body and (strip_comments("".join(lines[index:end+1]))[body[1]+1:].strip() or re.search(r"(?m)^\s*#", source)):
            continue
        functions.append(Removal(start, end, "unreferenced function declaration", name))
        skip_until = end
    members = template_member_candidates(uncommented, functions)
    for candidate in functions:
        if candidate in members:
            continue
        candidates.append(candidate)
        references.setdefault(candidate.name, set()).update(code_identifiers("".join(lines[candidate.start:candidate.end+1])))

    operators: list[tuple[Removal, set[str]]] = []
    for index, line in enumerate(visible_lines):
        if "operator" not in line or line.lstrip().startswith("#"):
            continue
        start = template_prefix_start(lines, index, tokens)
        if any(candidate.start <= start <= candidate.end for candidate in candidates):
            continue
        tail = "".join(lines[index:])
        body = function_body_span(tail)
        match = re.search(r"\boperator\s*(?:\[\]|\(\)|<=>|<<|>>|[-+*/%&|^~!=<>]+)\s*\(", tail[:body[0]] if body else "")
        if body is None or match is None:
            continue
        depth, end_parameter = 1, match.end()
        while depth and end_parameter < body[0]:
            char = tail[end_parameter]
            depth += (char == "(") - (char == ")")
            end_parameter += 1
        attached = set(code_identifiers(tail[match.end():end_parameter-1])) & class_sources.keys()
        anchors = {name for name in attached if re.search(
            rf"\b{re.escape(name)}\s*\(|\busing\b[^;=]*::[^;=]*;|\bfriend\b|__COUNTER__", class_sources[name],
        ) is None}
        if not anchors:
            continue
        end = index + tail.count("\n", 0, body[1])
        if strip_comments(tail[body[1]+1:tail.find("\n", body[1]+1) if "\n" in tail[body[1]+1:] else len(tail)]).strip():
            continue
        operators.append((Removal(start, end, "unreferenced operator component", f"@operator{start}"), attached))
    member_operators = template_member_candidates(uncommented, [candidate for candidate, _ in operators])
    for candidate, attached in operators:
        if candidate in member_operators:
            continue
        candidates.append(candidate)
        references[candidate.name] = set(code_identifiers("".join(lines[candidate.start:candidate.end+1])))
        for name in attached:
            references[name].add(candidate.name)

    names = set(references)
    live = (set(code_identifiers(apply_removals(uncommented, candidates))) | set(keep_names)) & names
    live.update(name for name in names if any(pattern.fullmatch(name) for pattern in pasted_names))
    pending = list(live)
    while pending:
        for dependency in (references[pending.pop()] & names) - live:
            live.add(dependency)
            pending.append(dependency)
    return [candidate for candidate in candidates if candidate.name not in live]


def function_header_name_and_prefix(source: str) -> tuple[str, str] | None:
    body = function_body_span(source)
    if body is None and not source.rstrip().endswith(";"):
        return None
    header = strip_comments(source[:body[0]] if body else source.rstrip()[:-1])
    header = header.lstrip()
    while match := re.match(r"template\s*<", header):
        angle, paren, position = 1, 0, match.end()
        while position < len(header) and angle:
            char = header[position]
            if char == "(":
                paren += 1
            elif char == ")":
                paren -= 1
            elif not paren and char in "<>":
                angle += 1 if char == "<" else -1
            position += 1
        if angle:
            return None
        header = header[position:].lstrip()
    if re.search(r"\b(?:requires|asm|__asm__)\b", header):
        return None
    if "operator" in set(code_identifiers(header)):
        operator = re.search(r"\boperator\s*(\[\]|\(\)|<=>|<<|>>|[-+*/%&|^~!=<>]+)\s*\(", header)
        if operator is None:
            return None
        return "operator" + operator.group(1), header[:operator.start()].strip()
    depth = bracket = 0
    previous = ""
    previous_position = 0
    ignored = {"decltype", "noexcept", "sizeof", "alignof", "alignas", "__attribute__", "__declspec", "int", "void", "char", "bool", "float", "double", "long", "short"}
    for match in re.finditer(r"[_A-Za-z]\w*|[^\s]", header):
        token = match.group()
        if token == "(":
            if depth == bracket == 0 and IDENTIFIER.fullmatch(previous) and previous not in ignored:
                return previous, header[:previous_position].strip()
            depth += 1
        elif token == ")":
            depth -= 1
        elif token == "[":
            bracket += 1
        elif token == "]":
            bracket -= 1
        previous = token
        previous_position = match.start()
    return None


def function_header_name(source: str) -> str | None:
    parsed = function_header_name_and_prefix(source)
    return parsed[0] if parsed else None


def unreferenced_function_candidates(
    text: str, candidates: Sequence[Removal], keep_functions: set[str],
) -> list[Removal]:
    if "__COUNTER__" in set(code_identifiers(text)):
        return []
    lines = text.splitlines(keepends=True)
    graph: dict[str, set[str]] = {}
    eligible: list[Removal] = []
    for candidate in candidates:
        source = "".join(lines[candidate.start:candidate.end+1])
        body = function_body_span(source)
        if body is None and not source.rstrip().endswith(";"):
            continue
        if body is not None and strip_comments(source[body[1]+1:]).strip():
            continue
        identifiers = set(code_identifiers(source))
        if identifiers.intersection({"__COUNTER__", "virtual", "override", "final", "extern", "constructor", "destructor", "used", "retain", "cleanup", "__attribute__", "__declspec"}) or "[[" in source:
            continue
        name = candidate.name or function_header_name(source)
        if name is None or name in keep_functions or name.startswith("operator"):
            continue
        header = strip_comments(source[:body[0]] if body else source.rstrip()[:-1])
        occurrence = re.search(rf"\b{re.escape(name)}\s*\(", header)
        if occurrence is None:
            continue
        prefix = header[:occurrence.start()].strip()
        if not prefix or prefix.endswith("~") or prefix in {"explicit", "constexpr", "consteval", "inline"}:
            continue
        eligible.append(Removal(candidate.start, candidate.end, candidate.label, name))
        graph.setdefault(name, set()).update(identifiers)
    outside = set(code_identifiers(apply_removals(text, eligible)))
    live = outside.intersection(graph) | keep_functions
    patterns = token_paste_patterns(strip_comments(text))
    live.update(name for name in graph if any(pattern.fullmatch(name) for pattern in patterns))
    if outside.intersection({"asm", "__asm__", "dlsym", "GetProcAddress"}):
        return []
    pending = list(live.intersection(graph))
    while pending:
        for name in (graph[pending.pop()] & graph.keys()) - live:
            live.add(name)
            pending.append(name)
    return [candidate for candidate in eligible if candidate.name not in live]


def warning_line_indices(text: str) -> dict[int, int]:
    """Map unambiguous GCC line numbers to indices in the source file."""
    lines = text.splitlines(keepends=True)
    rows: dict[int, int] = {}
    repeated: set[int] = set()
    number, depth = 1, 0
    for index, line in enumerate(preprocessor_lines(text)):
        if line.lstrip().startswith("%:"):
            line = "#" + line.lstrip()[2:]
        if LINE_CONTROL.match(line):
            marker = BARE_LINE_CONTROL.fullmatch(lines[index])
            if marker is None or depth:
                return {}
            number = int(marker.group(1))
            continue
        if number in rows:
            repeated.add(number)
        rows[number] = index
        number += 1
        if CONDITIONAL_OPEN_DIRECTIVE.match(line):
            depth += 1
        elif CONDITIONAL_CLOSE_DIRECTIVE.match(line):
            depth -= 1
    for number in repeated:
        del rows[number]
    return rows


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
    rows = warning_line_indices(text)

    cppcheck_result: subprocess.CompletedProcess[str] | None = None
    if not rows:
        compiler_output = ""
    elif cppcheck is None or len(rows) != len(lines):
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
        rf"^(?:{re.escape(str(compiler.source))}|{re.escape(compiler.source.name)}):(\d+):\d+: warning:.*"
        r"\[-Wunused-(function|macros)\]$"
    )
    for warning in compiler_output.splitlines():
        match = warning_pattern.match(warning)
        if not match:
            continue
        line_index = rows.get(int(match.group(1)))
        if line_index is None:
            continue
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
    patterns = token_paste_patterns(strip_comments(text))
    macros = set()
    functions = []
    for removal in removals:
        if "macro" in removal.label:
            match = re.match(r"^\s*#\s*define\s+([_A-Za-z]\w*)\b", lines[removal.start])
            if match and counts[match.group(1)] == 1 and not any(pattern.fullmatch(match.group(1)) for pattern in patterns):
                macros.add(removal)
        else:
            functions.append(removal)
    external_names = compiler.external_identifiers(text)
    removals = macros.union(unreferenced_function_candidates(text, functions, keep_functions | external_names))
    removals.update(unreferenced_declaration_candidates(text, external_names | keep_functions,
                                                       compiler.dangerous_macro_names(text)))

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
    line_count = len(text.splitlines(keepends=True))
    replacements: dict[int, str] = {}
    for removal in removals:
        for line_index in range(max(0, removal.start), min(line_count, removal.end + 1)):
            replacements[line_index] = ""
    return retain_important_comments(replace_lines_preserving_comments(text, replacements), ())


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
    compiler.preserve_initialization(text)
    started = time.monotonic()
    accepted: set[Removal] = set()
    cache: dict[frozenset[Removal], tuple[bool, str]] = {}
    validations = 0
    target_checkpointed = writer.below_limit(text, target_bytes) or (
        writer.current is not None
        and writer.below_limit(writer.current, target_bytes)
    )

    original_lines = text.splitlines(keepends=True)
    byte_sizes = {
        candidate: len(
            "".join(original_lines[candidate.start : candidate.end + 1]).encode()
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
            valid, stderr = compiler.validate(candidate_text, parallel_portable=True)
            cache[proposal] = valid, stderr
        else:
            valid, stderr = cached
        if not valid:
            reporter.detail(first_error(stderr))
            return False
        accepted.update(batch)
        text = candidate_text
        writer.save(f"removed {len(batch)} unused declaration(s)", text)
        if not target_checkpointed and writer.below_limit(text, target_bytes):
            target_checkpointed = True
            writer.report_checkpoint(text, target_bytes)
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


def try_early_declaration_checkpoint(
    text: str, *, compiler: CompilerWorkspace, writer: CheckpointWriter,
    reporter: Reporter, limit: int, keep_functions: set[str], keep_comments: set[str],
    verified_layout: str | None = None,
) -> str | None:
    api = sys.modules[__name__]
    structures, _ = bundle_fast.structural_candidates(api, text, "")
    lines = text.splitlines(keepends=True)
    savings = sum(encoded_size("".join(lines[item.start:item.end + 1])) for item in structures)
    if writer.size(text) - savings >= limit:
        return None

    reporter.phase("declarations", f"trying an early {limit:,} B checkpoint")
    scratch = CheckpointWriter(compiler.directory / "declaration-checkpoint.cpp",
                               Reporter(stream=io.StringIO()), stamp=writer.stamp)
    sources = [text]
    if verified_layout is not None and verified_layout != text and compiler.linked_submission_headers(verified_layout) is not None:
        sources.insert(0, verified_layout)
    for source in sources:
        scratch.current = source
        candidate = bundle_fast.prune_function_declarations(
            api, source, compiler=compiler, writer=scratch, reporter=reporter, keep_functions=keep_functions)
        if candidate == source:
            continue
        if source != text or not writer.below_limit(candidate, limit):
            try:
                candidate = safe_cleanup_candidate(candidate, compiler, keep_comments)
            except ToolError as exc:
                reporter.detail("early declaration checkpoint rejected: " + str(exc))
                continue
            if not writer.below_limit(candidate, limit):
                candidate = bundle_fast.prune_optimized_declarations(
                    api, candidate, compiler=compiler, writer=scratch, reporter=reporter,
                    keep_functions=keep_functions, checkpoint_limit=limit)
        if writer.below_limit(candidate, limit):
            valid, diagnostic = compiler.validate(candidate, parallel_portable=True)
            if valid:
                writer.save("verified early declaration checkpoint", candidate)
                return candidate
            reporter.detail("early declaration checkpoint rejected: " + first_error(diagnostic))
    return None


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
    keep_comments: Iterable[str] = (),
    feature_token: str | None = None,
) -> str:
    reference_text = text
    limits = checkpoint_limits(target_bytes)
    priority_limit = limits[0] if len(limits) > 1 else None
    if writer.layout_started is not None and feature_token is not None:
        import submission_layout

        writer.display_layout = submission_layout.CheckpointLayout(
            sys.modules[__name__], text, feature_token, writer, compiler, target_bytes)
    if not safe_only and getattr(compiler, "uses_counter_macro", lambda text: False)(text):
        safe_only = True
        reporter.detail("keeping declarations and bodies that affect __COUNTER__")
    if not safe_only and keep_functions == DEFAULT_KEEP_FUNCTIONS and not writer.below_limit(text, target_bytes):
        fast = bundle_fast.try_cleanup(sys.modules[__name__], text, compiler=compiler, writer=writer,
                                      reporter=reporter, target_bytes=target_bytes, keep_comments=keep_comments,
                                      feature_token=feature_token, priority_bytes=priority_limit)
        if fast is not None:
            text = fast
        published = writer.analysis_current if writer.display_layout is not None else writer.current
        if published is not None and writer.size(published) < writer.size(text):
            text = published
        if text != reference_text:
            text = bundle_fast.prune_function_declarations(
                sys.modules[__name__], text, compiler=compiler, writer=writer,
                reporter=reporter, keep_functions=keep_functions, include_structures=False)
    keep_comments = set(keep_comments)
    if priority_limit is not None and priority_limit not in writer.reached_limits:
        checkpoint = try_size_checkpoint(text, compiler=compiler, writer=writer, reporter=reporter,
                                         limit=priority_limit, keep_comments=keep_comments)
        if checkpoint is not None:
            text = checkpoint
    target_checkpointed = writer.below_limit(text, target_bytes) or (
        writer.current is not None
        and writer.below_limit(writer.current, target_bytes)
    )

    def report_target_checkpoint() -> None:
        nonlocal target_checkpointed
        if target_checkpointed or not writer.below_limit(text, target_bytes):
            return
        target_checkpointed = True
        writer.report_checkpoint(text, target_bytes)

    if target_bytes is not None:
        quick_candidate = collapse_blank_lines(strip_cleanup_comments(text, keep_comments))
        if writer.below_limit(quick_candidate, target_bytes):
            started = time.monotonic()
            reporter.phase("cleanup", "validating a quick comment-only checkpoint")
            valid, stderr = compiler.validate_submission(quick_candidate)
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
    safe_candidate = safe_cleanup_candidate(text, compiler, keep_comments)
    if writer.below_limit(safe_candidate, target_bytes):
        if safe_candidate != text or require_validation:
            valid, stderr = compiler.validate_submission(safe_candidate)
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
        f"{reporter.byte_count(writer.size(safe_candidate), 'cyan')}",
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

    verified_layout = text
    safe_changed = safe_candidate != text
    compiler.preserve_initialization(reference_text)
    compiler.retain_portable_target(reference_text)
    text = safe_candidate
    pending_validation = safe_changed or require_validation
    if feature_token is not None and target_checkpointed:
        previous = text
        text = bundle_fast.prune_function_declarations(
            sys.modules[__name__], text, compiler=compiler, writer=writer,
            reporter=reporter, keep_functions=keep_functions, final=True)
        pending_validation |= text != previous

    def try_declaration_checkpoint() -> None:
        nonlocal text, pending_validation
        if target_bytes is None or target_checkpointed:
            return
        candidates = unreferenced_declaration_candidates(text, compiler.external_identifiers(text) | keep_functions,
                                                         compiler.dangerous_macro_names(text))
        if not candidates:
            return
        candidate_text = collapse_blank_lines(apply_removals(text, candidates))
        if not writer.below_limit(candidate_text, target_bytes):
            return
        started = time.monotonic()
        reporter.phase("declarations", f"checking {len(candidates)} unreferenced declaration(s)")
        valid, stderr = compiler.validate(candidate_text, build_pch=False)
        if valid:
            text = candidate_text
            pending_validation = False
            writer.save("unreferenced types, aliases, and lambdas", text)
            report_target_checkpoint()
        else:
            reporter.detail("quick declaration cleanup was rejected: " + first_error(stderr))
        reporter.done("declarations", "quick checkpoint checked", started)

    try_declaration_checkpoint()
    if (feature_token is not None and not target_checkpointed and target_bytes is not None
            and priority_limit in writer.reached_limits and not compiler.extra_args
            and keep_functions == DEFAULT_KEEP_FUNCTIONS):
        early = try_early_declaration_checkpoint(
            text, compiler=compiler, writer=writer, reporter=reporter, limit=target_bytes,
            keep_functions=keep_functions, keep_comments=keep_comments, verified_layout=verified_layout)
        if early is not None:
            text = early
            pending_validation = False
            report_target_checkpoint()

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

    started = time.monotonic()
    reporter.phase("templates", "finding uninstantiated template functions")
    template_candidates = uninstantiated_template_candidates(
        text,
        compiler=compiler,
        keep_functions=keep_functions,
    )
    reporter.done(
        "templates",
        f"found {len(template_candidates)} candidate(s)",
        started,
    )
    if template_candidates:
        template_candidates = removable_template_body_candidates(
            text, template_candidates,
            allow_portable_stubs=compiler.portable_workspace is not None,
        )
        if template_candidates:
            text, accepted = reduce_template_body_candidates(
                text,
                template_candidates,
                compiler=compiler,
                writer=writer,
                reporter=reporter,
                target_bytes=target_bytes,
            )
            if accepted:
                pending_validation = False
        else:
            reporter.detail("no declaration-only template body is safe to try")
    try_declaration_checkpoint()
    validate_pending()

    for pass_index in range(1, max_passes + 1):
        stage = f"pass {pass_index}/{max_passes}"
        had_candidates, accepted = run_unused_pass(cppcheck, stage)
        if not had_candidates:
            reporter.detail(f"unused cleanup converged after pass {pass_index}")
            break
        if accepted == 0:
            reporter.detail(f"no unused candidate was accepted in pass {pass_index}")
            break
        try_declaration_checkpoint()
    validate_pending()

    started = time.monotonic()
    reporter.phase("IPA", "finding unreachable recursive function groups")
    ipa_text = text
    ipa_functions = compiler._analyze_ipa(text)
    ipa_candidates = ipa_unused_candidates(
        text,
        compiler=compiler,
        keep_functions=keep_functions,
        functions=ipa_functions or (),
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
        if accepted and max_passes > 0:
            run_unused_pass(cppcheck, "after IPA")
    started = time.monotonic()
    reporter.phase("bodies", "finding unreachable function bodies while retaining declarations")
    body_candidates = unreachable_function_body_candidates(
        text, compiler=compiler, keep_functions=keep_functions,
        functions=ipa_functions if text == ipa_text else None,
    )
    reporter.done("bodies", f"found {len(body_candidates)} candidate(s)", started)
    if body_candidates:
        text, accepted = reduce_template_body_candidates(
            text, body_candidates, compiler=compiler, writer=writer, reporter=reporter, target_bytes=target_bytes,
        )
        if accepted and max_passes > 0:
            run_unused_pass(cppcheck, "after bodies")
    try_declaration_checkpoint()
    dynamic_policy_header = "source:math/modint/modint_internal_barrett32.hpp"
    include_dynamic_types = (feature_token is not None and target_bytes is not None
                             and not writer.below_limit(text, target_bytes)
                             and bundle_fast.marker(feature_token, True, dynamic_policy_header) in reference_text)
    return bundle_fast.prune_function_declarations(
        sys.modules[__name__], text, compiler=compiler, writer=writer,
        reporter=reporter, keep_functions=keep_functions, final=True, include_dynamic_types=include_dynamic_types)


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
        self.feature_token = secrets.token_hex(16)
        self.library_root = (
            library_root.resolve() if library_root is not None else None
        )
        self.once: set[Path] = set()
        self.stack: list[Path] = []
        self.inputs: dict[str, str] = {}
        self.uses_library = False

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
            data = path.read_bytes()
            text = data.decode("utf-8").replace("\r\n", "\n").replace("\r", "\n")
        except (OSError, UnicodeError) as exc:
            raise FastBundleUnsupported(f"could not read {path.name}: {exc}") from exc
        self.inputs[str(path)] = hashlib.sha256(data).hexdigest()
        if not text.endswith("\n"):
            text += "\n"

        lines = text.splitlines(keepends=True)
        uncommented_lines = preprocessor_lines(text)
        if len(lines) != len(uncommented_lines):
            raise FastBundleUnsupported(
                f"comment parsing changed the line count in {path.name}"
            )

        has_pragma_once = False
        depth = 0
        for line in uncommented_lines:
            if PRAGMA_ONCE_DIRECTIVE.match(line.rstrip("\r\n")):
                if depth:
                    raise FastBundleUnsupported(f"conditional #pragma once in {path.name}")
                has_pragma_once = True
            elif CONDITIONAL_OPEN_DIRECTIVE.match(line):
                depth += 1
            elif CONDITIONAL_CLOSE_DIRECTIVE.match(line):
                depth -= 1
        include_guard_lines = self._include_guard_lines(uncommented_lines)
        guarded_once = has_pragma_once
        if guarded_once and path in self.once:
            return ""
        if guarded_once:
            self.once.add(path)
        if path in self.stack:
            chain = " -> ".join(item.name for item in [*self.stack, path])
            raise FastBundleUnsupported(f"include cycle without #pragma once: {chain}")

        self.stack.append(path)
        prefix = ""
        if add_source_link:
            url = library_source_url(path, self.library_root)
            if url is not None:
                self.uses_library = True
        replacements: dict[int, str] = {}
        conditional_depth = 0
        try:
            for line_index, uncommented_line in enumerate(uncommented_lines):
                directive = uncommented_line.rstrip("\r\n")

                if PRAGMA_ONCE_DIRECTIVE.match(directive):
                    replacements[line_index] = ""
                    continue
                if line_index in include_guard_lines:
                    continue

                local_include = LOCAL_INCLUDE_DIRECTIVE.match(directive)
                if local_include:
                    if conditional_depth != 0:
                        raise FastBundleUnsupported(
                            f"conditional local include in {path.name}"
                        )
                    included = self._resolve(local_include.group(1), path)
                    replacements[line_index] = self._expand(included)
                    continue

                if INCLUDE_DIRECTIVE.match(directive) and '"' in directive:
                    raise FastBundleUnsupported(
                        f"unsupported local include syntax in {path.name}"
                    )

                marker = LINE_DIRECTIVE.match(directive) or GNU_LINE_MARKER.match(directive)
                if marker:
                    raw = json.loads(marker.group(2))
                    resolver = OriginResolver(base_dir=path.parent, source=path, include_paths=self.include_paths)
                    origin = resolver.resolve(raw)
                    replacements[line_index] = f"#line {marker.group(1)} {json.dumps(origin.display)}\n"
                    continue

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
            feature = bundle_fast.feature_name(path, self.library_root)
            begin = bundle_fast.marker(self.feature_token, True, feature) + "\n" if feature else ""
            end = bundle_fast.marker(self.feature_token, False, feature) + "\n" if feature else ""
            return prefix + begin + replace_lines_preserving_comments(
                text, replacements, source_comments(text) if not add_source_link else (),
            ) + end
        finally:
            self.stack.pop()

    def bundle(self, source: Path) -> str:
        result = self._expand(source, add_source_link=False)
        return source_link_comment(LIBRARY_REPOSITORY_URL) + result if self.uses_library else result


def run_oj_bundle(
    *,
    executable: str,
    source: Path,
    include_paths: Sequence[Path],
    cxx: str,
    inputs: dict[str, str] | None = None,
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
    return restore_oj_bundle_notices(result.stdout, source, include_paths, inputs=inputs)


def restore_oj_bundle_notices(
    text: str, source: Path, include_paths: Sequence[Path],
    *, inputs: dict[str, str] | None = None,
) -> str:
    # oj-bundle removes include/guard lines together with their comments.
    # Read local dependencies as well, including headers reduced to no lines.
    files: dict[Path, str] = {}
    pending = [source.resolve()]
    # Line markers also cover local headers included with angle brackets.
    for line in preprocessor_lines(text):
        match = LINE_DIRECTIVE.match(line.rstrip("\r\n"))
        if match is not None:
            path = Path(json.loads(match.group(2)))
            if not path.is_absolute():
                path = source.parent / path
            if path.is_file():
                pending.append(path.resolve())
    resolver = FastBundler(include_paths)
    while pending:
        path = pending.pop()
        if path in files:
            continue
        data = path.read_bytes()
        if inputs is not None:
            inputs[str(path)] = hashlib.sha256(data).hexdigest()
        original = data.decode("utf-8").replace("\r\n", "\n").replace("\r", "\n")
        if not original.endswith("\n"):
            original += "\n"
        files[path] = original
        for line in preprocessor_lines(original):
            match = LOCAL_INCLUDE_DIRECTIVE.match(line.rstrip("\r\n"))
            if match is not None:
                pending.append(resolver._resolve(match.group(1), path))
    if not any(important_comments(original) for original in files.values()):
        return text

    # Locate emitted source lines before recovering comments on lost directives.
    # Complete comments stay in place; partial block comments are restored whole.
    original_lines = {path: original.splitlines(keepends=True) for path, original in files.items()}
    located: list[tuple[Path | None, int, str, bool]] = []
    emitted: dict[Path, set[int]] = {}
    current: Path | None = None
    source_line = 0
    for line in text.splitlines(keepends=True):
        if original_lines.get(current, [])[source_line : source_line + 1] == [line]:
            located.append((current, source_line, line, False))
            assert current is not None
            emitted.setdefault(current, set()).add(source_line)
            source_line += 1
            continue
        match = LINE_DIRECTIVE.match(line.rstrip("\r\n"))
        if match is not None:
            path = Path(json.loads(match.group(2)))
            if not path.is_absolute():
                path = source.parent / path
            current = path.resolve()
            source_line = int(match.group(1)) - 1
            located.append((current, source_line, line, True))
        else:
            located.append((None, source_line, line, False))
            source_line += 1

    recovered: dict[Path, list[tuple[int, str]]] = {}
    stripped: dict[Path, list[str]] = {}
    for path, original in files.items():
        recovered[path] = []
        parts: list[str] = []
        previous = previous_line = 0
        for start, end in comment_spans(original):
            first = previous_line + original.count("\n", previous, start)
            last = first + original.count("\n", start, end)
            comment = original[start:end]
            parts.append(original[previous:start])
            if all(index in emitted.get(path, set()) for index in range(first, last + 1)):
                parts.append(comment)
            else:
                parts.append((" " if is_block_comment(comment) else "") + "\n" * comment.count("\n"))
                if is_important_comment(comment):
                    recovered[path].append((first, comment))
            previous, previous_line = end, last
        parts.append(original[previous:])
        stripped[path] = "".join(parts).splitlines(keepends=True)
        stripped[path].extend([""] * (len(original_lines[path]) - len(stripped[path])))

    positions: dict[Path, list[tuple[int, int]]] = {}
    for position, (path, index, _, marker) in enumerate(located):
        if path in files and not marker:
            positions.setdefault(path, []).append((index, position))
    insertions: dict[int, list[tuple[Path, int, str]]] = {}
    absent: list[str] = []
    for path, notices in recovered.items():
        rows = positions.get(path, [])
        for first, comment in notices:
            if not rows:
                # A header made only of removed directives has no emitted rows.
                absent.append(comment)
                continue
            position = next((position for index, position in rows if index >= first), rows[-1][1] + 1)
            insertions.setdefault(position, []).append((path, first, comment))

    output: list[str] = []
    for position in range(len(located) + 1):
        for path, first, comment in insertions.get(position, ()):
            # Restore the notice's origin so later rendering keeps its link.
            output.append(f"#line {first + 1} {json.dumps(str(path))}\n{comment}\n\n")
        if position == len(located):
            break
        path, index, line, marker = located[position]
        if position in insertions and path in files and not marker:
            output.append(f"#line {index + 1} {json.dumps(str(path))}\n")
        if path in files and not marker:
            output.extend(stripped[path][index : index + 1])
        else:
            output.append(line)
    return retain_important_comments("".join(output), absent)


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


def canonical_oj_bundle(
    text: str, source: Path, include_paths: Sequence[Path], repo_root: Path,
) -> str:
    resolver = OriginResolver(
        base_dir=source.parent, source=source, include_paths=include_paths,
    )
    return BundleDocument.parse(
        text, resolver=resolver, library_root=repo_root,
    ).render(set(), keep_line=False)


def expand_source(
    source: Path, include_paths: Sequence[Path], repo_root: Path,
    *, cxx: str | None, oj_bundle: str | None,
    no_fallback: bool = False, reporter: Reporter, metadata: dict | None = None,
) -> tuple[str, str, dict[str, str]]:
    try:
        bundler = FastBundler(include_paths, library_root=repo_root)
        bundled = bundler.bundle(source)
        if metadata is not None:
            metadata["feature_token"] = bundler.feature_token
        return bundled, "fast", bundler.inputs
    except FastBundleUnsupported as exc:
        if no_fallback:
            raise
        reporter.warning(f"fast expansion unsupported: {exc}")
    reporter.phase("bundle", "running oj-bundle compatibility fallback")
    inputs: dict[str, str] = {}
    raw_bundle = run_oj_bundle(
        executable=discover_oj_bundle(repo_root, oj_bundle), source=source,
        include_paths=include_paths,
        cxx=resolve_executable(cxx, ("g++-15", "g++-14", "g++")),
        inputs=inputs,
    )
    return canonical_oj_bundle(raw_bundle, source, include_paths, repo_root), "oj-bundle", inputs


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
    reporter.phase("bundle", "expanding local includes")
    metadata: dict = {}
    bundled, method, inputs = expand_source(
        source, include_paths, repo_root, cxx=args.cxx, oj_bundle=args.oj_bundle,
        no_fallback=args.no_fallback, reporter=reporter, metadata=metadata,
    )
    privacy_check_line_directives(bundled)
    stamp = SubmissionStamp(inputs[str(source)], hashlib.sha256(bundled.encode()).hexdigest())
    rendered = stamp.render(bundled)
    save_source_comments(output, source_comments(source.read_text(encoding="utf-8")), provenance={
        "source": str(source), "inputs": inputs, "input_digest": stamp.input_digest, **metadata,
    })
    atomic_write(output, rendered)
    reporter.done(
        "bundle",
        f"{method}: {reporter.byte_count(encoded_size(rendered), 'green')}",
        started,
    )


def expansion_reuse_fingerprint(path: Path) -> bytes:
    raw = path.read_text()
    stamp, text = SubmissionStamp.read(raw)
    try:
        record = json.loads(comment_record_path(path).read_text())
        token = record["feature_token"]
        if stamp is None or record["input_digest"] != stamp.input_digest:
            return raw.encode()
        text, owners = bundle_fast.feature_lines(sys.modules[__name__], text, token)
        if not owners:
            return raw.encode()
        source = Path(record["source"])
        root = Path(__file__).resolve().parent.parent
        reporter = Reporter(stream=io.StringIO())
        with CompilerWorkspace(cxx=resolve_executable(None, ("g++-15", "g++-14", "g++")),
                               standard="c++23", include_paths=default_include_paths(root, source),
                               extra_args=[], timeout=None, reporter=reporter) as compiler:
            context = compiler.preprocessed_fingerprint(text)
        if context is None:
            return raw.encode()
        return (text + context + Path(bundle_fast.__file__).read_text()).encode()
    except (OSError, KeyError, TypeError, ValueError):
        return raw.encode()


def bundle_command(args: argparse.Namespace, repo_root: Path) -> None:
    reporter = Reporter(args.verbose, args.color)
    source = Path(args.source).expanduser().resolve()
    output = Path(args.output).expanduser().resolve()
    if not source.is_file():
        raise ToolError(f"source file does not exist: {source}")
    if source == output:
        raise ToolError("bundle output must differ from the source file")
    comments = source_comments(source.read_text(encoding="utf-8"))

    include_paths = merge_include_paths(
        default_include_paths(repo_root, source), args.include
    )
    command_started = time.monotonic()

    with common_compiler_from_args(
        args, include_paths=include_paths, reporter=reporter
    ) as compiler:
        if (args.aggressive and not args.no_safe_cleanup and not args.keep_line and
                not args.keep_header and not args.keep_function and not args.no_prune_headers):
            metadata = {}
            try:
                expanded, _, inputs = expand_source(source, include_paths, repo_root, cxx=compiler.cxx,
                                                    oj_bundle=args.oj_bundle, no_fallback=True,
                                                    reporter=reporter, metadata=metadata)
            except FastBundleUnsupported:
                expanded = None
            if expanded is not None:
                stamp = SubmissionStamp(inputs[str(source)], hashlib.sha256(expanded.encode()).hexdigest())
                writer = CheckpointWriter(output, reporter, stamp=stamp)
                selected = bundle_fast.try_cleanup(sys.modules[__name__], expanded, compiler=compiler,
                                                   writer=writer, reporter=reporter,
                                                   target_bytes=DEFAULT_BYTE_LIMIT, keep_comments=comments,
                                                   feature_token=metadata.get("feature_token"))
                if selected is not None:
                    save_source_comments(output, comments, provenance={
                        "source": str(source), "inputs": inputs, "input_digest": stamp.input_digest, **metadata,
                    })
                    writer.save("final", selected, force=True)
                    reporter.done("bundle", f"finished at {reporter.byte_count(writer.size(selected), 'green')}",
                                  command_started)
                    return
        started = time.monotonic()
        reporter.phase("bundle", "running oj-bundle")
        oj_bundle = discover_oj_bundle(repo_root, args.oj_bundle)
        inputs: dict[str, str] = {}
        raw_bundle = run_oj_bundle(
            executable=oj_bundle,
            source=source,
            include_paths=include_paths,
            cxx=compiler.cxx,
            inputs=inputs,
        )
        reporter.done("bundle", "oj-bundle completed", started)
        canonical = canonical_oj_bundle(raw_bundle, source, include_paths, repo_root)
        stamp = SubmissionStamp(inputs[str(source)], hashlib.sha256(canonical.encode()).hexdigest())
        save_source_comments(output, comments, provenance={
            "source": str(source), "inputs": inputs, "input_digest": stamp.input_digest,
        })
        writer = CheckpointWriter(output, reporter, stamp=stamp)
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
                keep_comments=comments,
            )

        writer.save("final", bundled, force=True)
    reporter.done(
        "bundle",
        f"finished at {reporter.byte_count(writer.size(bundled), 'green')}",
        command_started,
    )


def cleanup_command(args: argparse.Namespace, repo_root: Path) -> None:
    reporter = Reporter(args.verbose, args.color)
    command_started = time.monotonic()
    path = Path(args.file).expanduser().resolve()
    if not path.is_file():
        raise ToolError(f"file does not exist: {path}")
    text = path.read_bytes().decode("utf-8")
    stamp = None
    detached, had_stamp = SubmissionStamp.detach(text)
    if had_stamp:
        try:
            stamp, text = SubmissionStamp.read(text)
        except ToolError:
            # A hand-edited bundle can still be cleaned, but its original input
            # association must be discarded rather than certified again.
            text = detached
            reporter.warning("生成後の編集を検出したため照合情報を外します。照合には oj-b で再生成してください。")
    writer = CheckpointWriter(path, reporter, stamp=stamp)
    writer.current = text
    target_bytes = None if args.target_bytes == 0 else args.target_bytes
    if target_bytes is not None and target_bytes <= 0:
        raise ToolError("target bytes must be positive, or 0 to disable the target")
    writer.preserve_below = target_bytes
    target_description = (
        "disabled"
        if target_bytes is None
        else " then ".join(f"<{reporter.byte_count(limit, 'yellow')}" for limit in checkpoint_limits(target_bytes))
    )
    reporter.phase(
        "cleanup",
        f"starting at {reporter.byte_count(writer.size(text), 'cyan')}; "
        f"target {target_description}",
    )
    if writer.below_limit(text, target_bytes):
        reporter.checkpoint(
            f"submission already ready: "
            f"{reporter.byte_count(writer.size(text), 'green')} < "
            f"{reporter.byte_count(target_bytes, 'yellow')}; validating and continuing cleanup"
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
        feature_token = None
        if stamp is not None:
            try:
                provenance = json.loads(comment_record_path(path).read_text())
                token = provenance.get("feature_token")
                if provenance.get("input_digest") == stamp.input_digest and isinstance(token, str) and re.fullmatch(r"[0-9a-f]{32}", token):
                    feature_token = token
            except (OSError, ValueError):
                pass
        if feature_token is not None and not args.safe_only:
            writer.layout_started = command_started
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
            keep_comments=load_source_comments(path),
            feature_token=feature_token,
        )
        writer.save("final", cleaned, force=had_stamp)
        required_limit = checkpoint_limits(target_bytes)[0] if target_bytes is not None else None
        checkpoint_available = writer.below_limit(cleaned, required_limit) or (
            writer.current is not None
            and writer.below_limit(writer.current, required_limit)
        )
        if target_bytes is not None and not checkpoint_available:
            raise ToolError(
                f"target not reached: "
                f"{reporter.byte_count(writer.size(cleaned), 'red')} >= "
                f"{reporter.byte_count(required_limit, 'yellow')}"
            )
        if required_limit != target_bytes and target_bytes is not None and not (
                writer.below_limit(cleaned, target_bytes) or
                (writer.current is not None and writer.below_limit(writer.current, target_bytes))):
            reporter.warning(
                f"secondary target not reached: {reporter.byte_count(target_bytes, 'yellow')}; "
                f"the {reporter.byte_count(required_limit, 'green')} checkpoint is available"
            )
    reporter.done(
        "cleanup",
        f"finished at {reporter.byte_count(writer.size(writer.current if writer.current is not None else cleaned), 'green')}",
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
            "write a verified checkpoint below this many bytes, then continue cleanup "
            "(default: 131072 first, then 65536; 0 disables checkpoints)"
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
