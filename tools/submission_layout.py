import hashlib
from bisect import bisect_left, bisect_right
import re
from typing import Sequence


HORIZONTAL_SPACE = re.compile(r"[ \t]+")
PHYSICAL_LINE = re.compile(r"[^\r\n]*(?:\r\n|\r|\n)|[^\r\n]+$")
LOCATION_OBSERVERS = frozenset({
    "assert", "__LINE__", "__builtin_LINE", "__builtin_COLUMN", "__COLUMN__",
    "__builtin_source_location", "source_location",
})


class CheckpointLayout:
    def __init__(self, api, original, feature_token, writer, compiler, limit):
        self.api = api
        self.original = original
        self.writer = writer
        self.compiler = compiler
        self.limit = limit
        self.cache = {}
        self.compaction_used = False
        # Only exact lines exclusive to library headers establish ownership after cleanup.
        # Original rows with ambiguous ownership keep their spacing.
        initial, owners = api.bundle_fast.feature_lines(api, original, feature_token)
        library = set()
        other = set()
        for line, owner in zip(initial.splitlines(keepends=True), owners):
            if owner is None:
                other.add(HORIZONTAL_SPACE.sub("", line))
            else:
                library.add(line)
        self.library = {line for line in library if HORIZONTAL_SPACE.sub("", line) not in other}

    def __call__(self, text, final):
        key = hashlib.sha256(text.encode()).digest()
        compact = (self.writer.allow_library_compaction(text, self.limit)
                   or (self.compaction_used and not self.writer.below_limit(text, self.limit)))
        cache_key = key, compact, final
        if cache_key in self.cache:
            return self.cache[cache_key]
        tidy = text
        if not final and self.library:
            tidy = self.api.bundle_fast.tidy_checkpoint(
                self.api, text, original=self.original, observers=LOCATION_OBSERVERS)
        formatted = tidy
        if compact and self.library:
            lines = tidy.splitlines(keepends=True)
            formatted = compact_library_layout(self.api, tidy, [line in self.library for line in lines])
        result = text
        if (formatted != tidy and self.writer.size(formatted) < self.writer.size(tidy)
                and (final or self.writer.below_limit(formatted, self.limit))
                and self.compiler.same_submission_layout(text, formatted)):
            result = formatted
            self.compaction_used = True
        elif (tidy != text and self.writer.size(tidy) < self.writer.size(text)
                and self.compiler.same_submission_layout(text, tidy)):
            result = tidy
        self.cache[cache_key] = result
        return result


def compact_library_layout(api, text: str, library_lines: Sequence[bool]) -> str:
    """Compact horizontal whitespace on known library lines, retaining line numbers."""
    lines = PHYSICAL_LINE.findall(text)
    if len(lines) != len(library_lines):
        return text
    offsets = [0]
    for line in lines:
        offsets.append(offsets[-1] + len(line))
    raw = []
    comments = list(api.comment_spans(text, raw_spans=raw))
    protected = set()
    for start, end in comments + raw:
        protected.update(range(bisect_right(offsets, start) - 1, bisect_left(offsets, end)))
    for index, line in enumerate(lines):
        if api.COMMENT_LINE_SPLICE.search(line):
            protected.update((index, index + 1))
    for index, line in enumerate(lines):
        if library_lines[index] and index not in protected and not line.lstrip().startswith(("#", "%:")):
            lines[index] = compact_code_line(api, line)
    return "".join(lines)


def compact_code_line(api, line: str) -> str:
    body = line.rstrip("\r\n")
    ending = line[len(body):]
    code = body.lstrip(" \t")
    if not code:
        return ending
    indent = body[:len(body) - len(code)]
    if "\t" not in indent:
        indent = " " * ((len(indent) + 1) // 2)
    output = [indent]
    start = index = 0
    while match := api.PREPROCESSOR_TOKEN.search(code, index):
        index = match.end()
        if match.group(1) is not None:
            return line
        if match.group() not in {'"', "'"}:
            continue
        delimiter = match.group()
        while quote := api.CODE_QUOTE_TOKEN.search(code, index):
            index = quote.end()
            if quote.group() == delimiter:
                break
        else:
            return line
        output.extend((HORIZONTAL_SPACE.sub(" ", code[start:match.start()]), code[match.start():index]))
        start = index
    output.append(HORIZONTAL_SPACE.sub(" ", code[start:]).rstrip(" "))
    return "".join(output) + ending
