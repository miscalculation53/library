"""Select optional definitions only when the complete object stays identical."""
from __future__ import annotations

import hashlib
from bisect import bisect_left
import json
import os
from pathlib import Path
import re
import secrets
import tempfile


FEATURE_HEADERS = {
    "template/template_types_base.hpp": "core_types",
    "template/template_float128_types.hpp": "float_types",
    "template/template_float128_math.hpp": "float_math",
    "template/template_float128_format.hpp": "float_format",
    "template/template_inout.hpp": "float_io",
    "template/template_dump.hpp": "float_dump",
    "template/template_arithmetic_impl.hpp": "arithmetic",
    "template/template_arithmetic.hpp": "arithmetic_declarations",
    "template/template_math_functions.hpp": "math",
    "template/template_integer_io.hpp": "integer_io",
    "algebra/internal_power.hpp": "scalar_power",
    "algebra/monoid_power.hpp": "monoid_power",
    "math/convolution/convolution.hpp": "general_convolution",
    "math/crt.hpp": "crt",
}
BEGIN = "// submit-code-feature: begin "
END = "// submit-code-feature: end "


def feature_name(path: Path, library_root: Path | None) -> str | None:
    if library_root is None:
        return None
    try:
        relative = path.resolve().relative_to(library_root).as_posix()
        return FEATURE_HEADERS.get(relative, "source:" + relative if path.suffix == ".hpp" else None)
    except ValueError:
        return None


def marker(token: str, begin: bool, name: str = "") -> str:
    return "// submit-code-feature: " + token + (" begin " if begin else " end ") + name


def feature_lines(api, text: str, token: str) -> tuple[str, list[str | None]]:
    output, owners, stack = [], [], []
    begin, end = marker(token, True), marker(token, False)
    markers = {start for start, stop in api.comment_spans(text)
               if text[start:stop].startswith((begin, end))}
    offset = 0
    for line in text.splitlines(keepends=True):
        if offset in markers and line.startswith(begin):
            name = line[len(begin):].strip()
            if name not in FEATURE_HEADERS.values() and not name.startswith("source:"):
                return text, []
            stack.append(name)
            output.append("\n")
        elif offset in markers and line.startswith(end):
            if not stack or stack.pop() != line[len(end):].strip():
                return text, []
            output.append("\n")
        else:
            output.append(line)
        owners.append(stack[-1] if stack else None)
        offset += len(line)
    return ("".join(output), owners) if not stack else (text, [])


def mask_ranges(api, text: str, ranges: list[tuple[int, int, str]]) -> str:
    protected = [(a, b) for a, b in api.comment_spans(text) if api.is_important_comment(text[a:b])]
    chars = list(text)
    for start, end, replacement in ranges:
        for i in range(start, end):
            if chars[i] not in "\r\n":
                chars[i] = " "
        if replacement:
            chars[start:start + len(replacement)] = replacement
    for start, end in protected:
        chars[start:end] = text[start:end]
    return "".join(chars)


def definition_candidates(api, text: str, owners: list[str | None], *, include_declarations=False):
    lines = text.splitlines(keepends=True)
    visible = api.preprocessor_lines(text)
    tokens = api.structural_tokens(lines)
    candidates = []
    skip_until = -1
    for i, line in enumerate(visible):
        owner = owners[i]
        if owner is None or (owner.startswith(("float_", "core_")) and owner != "float_io") or i <= skip_until or "(" not in line or line.lstrip().startswith("#"):
            continue
        if re.match(r"\s*(?:if|else|for|while|switch|catch|return|co_return|static_assert|assert|case|do|using)\b", line):
            continue
        start = api.template_prefix_start(lines, i, tokens)
        tail = "".join(lines[start:])
        body = api.function_body_span(tail)
        if body is None and not include_declarations:
            continue
        end = (start + tail.count("\n", 0, body[1]) if body is not None
               else api.find_statement_end(tokens, i))
        if end is None:
            continue
        source = "".join(lines[start:end + 1])
        name = api.function_header_name(source)
        if name is None:
            continue
        skip_until = end
        parsed = api.function_header_name_and_prefix(source)
        prefix = parsed[1] if parsed else ""
        if not prefix or prefix.endswith("~") or (body is None and ("=" in prefix or prefix in {"explicit", "constexpr", "consteval", "inline"})):
            continue
        if name in api.DEFAULT_KEEP_FUNCTIONS | {"main"} or name == "operator=":
            continue
        declaration = api.template_function_signature(
            source if body is not None else source.rstrip()[:-1] + "{}", name)
        if declaration is None:
            continue
        if set(api.code_identifiers(source[:body[0]] if body is not None else source)).intersection({"extern", "__declspec"}):
            continue
        candidates.append(api.Removal(start, end, owners[i], name))
    unique = {}
    for candidate in candidates:
        unique.setdefault((candidate.start, candidate.end, candidate.name), candidate)
    return list(unique.values())


def definition_groups(api, text: str, owners: list[str | None], compiler, *,
                      instantiated: str = "", emitted_lines: set[int] | None = None) -> dict[str, list[tuple[int, int, str]]]:
    lines = text.splitlines(keepends=True)
    offsets = [0]
    for line in lines:
        offsets.append(offsets[-1] + len(line))
    candidates = definition_candidates(api, text, owners)
    counts = {}
    for item in candidates:
        counts[item.name] = counts.get(item.name, 0) + 1
    portable_text = "".join("\n" if owner and owner.startswith("float_") else line
                            for line, owner in zip(lines, owners))
    protected = api.portable_body_dependencies(portable_text, candidates)
    emitted = sorted(emitted_lines) if emitted_lines is not None else None
    instantiated_names = set(re.findall(r'(?<![_A-Za-z0-9])([_A-Za-z]\w*)\s*(?:<|\()', instantiated))
    groups: dict[str, list[tuple[int, int, str]]] = {}
    for item in candidates:
        if item.name in protected:
            continue
        source = "".join(lines[item.start:item.end + 1])
        body = api.function_body_span(source)
        assert body is not None
        if emitted is None or counts[item.name] == 1 or set(api.code_identifiers(source[:body[0]])).intersection({"constexpr", "consteval"}):
            if item.name in instantiated_names or (item.name.startswith("operator") and re.search(
                    rf"(?<![_A-Za-z0-9]){re.escape(item.name)}\s*(?:<|\()", instantiated)):
                continue
        else:
            position = bisect_left(emitted, item.start)
            if position < len(emitted) and emitted[position] <= item.end:
                continue
        edit = (offsets[item.start] + body[0], offsets[item.start] + body[1] + 1, ";")
        groups.setdefault(item.label + ":" + item.name + ("@" + str(item.start) if item.label == "float_io" else ""), []).append(edit)
    return groups


def unused_macro_candidates(api, text: str, owners, compiler):
    _, portable = api.wrap_portable_conditionals(text)
    protected = set(api.code_identifiers("\n".join(portable.values())))
    lines = text.splitlines(keepends=True)
    candidates = []
    for index in compiler.unused_macro_locations(text):
        if not 0 <= index < len(lines) or owners[index] is None:
            continue
        match = re.match(r"\s*#\s*define\s+([_A-Za-z]\w*)", lines[index])
        if match is None or match.group(1) in protected:
            continue
        end = index
        while end + 1 < len(lines) and lines[end].rstrip("\r\n").endswith("\\"):
            end += 1
        candidates.append(api.Removal(index, end, "unused macro", match.group(1)))
    return candidates


def compact_lines(api, text: str) -> str:
    if re.search(r"(?m)^\s*#\s*(?:line\b|\d)", text):
        return text
    raw = []
    spans = list(api.comment_spans(text, raw_spans=raw)) + raw
    protected = set()
    for start, end in spans:
        first = text.count("\n", 0, start)
        protected.update(range(first, first + text.count("\n", start, end) + 1))
    lines = text.splitlines(keepends=True)
    visible = api.preprocessor_lines(text)
    output = []
    i = depth = 0
    while i < len(lines):
        if re.match(r"\s*#\s*(?:if|ifdef|ifndef)\b", visible[i]):
            depth += 1
        elif re.match(r"\s*#\s*endif\b", visible[i]):
            depth -= 1
        if depth or lines[i].strip() or i in protected or (i and lines[i - 1].rstrip().endswith("\\")):
            output.append(lines[i])
            i += 1
            continue
        end = i + 1
        while end < len(lines) and not lines[end].strip() and end not in protected:
            end += 1
        directive = f"#line {end + 1}\n" if end < len(lines) else ""
        if end - i > len(directive):
            if directive:
                output.append(directive)
        else:
            output.extend("\n" for _ in range(i, end))
        i = end
    return "".join(output)


def finish_layout(api, text: str, original: str, owners, *, feature_token=None) -> str:
    lines = text.splitlines(keepends=True)
    directive_comments = set()
    for start, end in api.comment_spans(text):
        first = text.count("\n", 0, start)
        directive_comments.update(range(first, text.count("\n", 0, max(start, end - 1)) + 1))
    stack, conditions = [], []
    visible = api.preprocessor_lines(text)
    portable = re.compile(r"__STDCPP_FLOAT128_T__|LIBRARY_HAS_FLOAT128|LIBRARY_USE_QUADMATH")
    for i, line in enumerate(visible):
        if re.match(r"\s*#\s*(?:if|ifdef|ifndef)\b", line):
            stack.append([i, [], True])
        if re.match(r"\s*#\s*(?:if|ifdef|ifndef|elif|else|endif)\b", line) and stack:
            stack[-1][1].append(i)
            stack[-1][2] &= not bool(portable.search(line))
            if re.match(r"\s*#\s*endif\b", line):
                item = stack.pop()
                conditions.append((item[0], i, item[1], item[2]))
    protected_conditions = [(a, b) for a, b, _, safe in conditions if not safe]
    for start, end, directives, safe in conditions:
        if safe and not directive_comments.intersection(directives) and re.fullmatch(r"\s*#\s*if\s+[01]\s*", visible[start]) and not any(
                a <= end and start <= b for a, b in protected_conditions):
            for i in directives:
                lines[i] = "\n"
    raw = []
    protected = set()
    comments = list(api.comment_spans(text, raw_spans=raw))
    links = {text.count("\n", 0, start) for start, end in comments
             if api.LIBRARY_SOURCE_LINK.fullmatch(text[start:end])}
    for start, end in comments + raw:
        first = text.count("\n", 0, start)
        protected.update(range(first, first + text.count("\n", start, end) + 1))
    live = {owner for line, owner in zip(api.preprocessor_lines(text), owners)
            if line.strip() and not re.match(r"\s*#\s*(?:if|ifdef|ifndef|elif|else|endif|line)\b", line)}
    originals = original.splitlines(keepends=True)
    for i, (line, before) in enumerate(zip(lines, originals)):
        link = re.fullmatch(r"// https://github.com/[^/]+/library/tree/[^/]+/(\S+)\s*", line)
        generated = feature_token and i + 1 < len(originals) and originals[i + 1].startswith(marker(feature_token, True))
        if link and i in links and generated and FEATURE_HEADERS.get(link[1], "source:" + link[1]) not in live:
            lines[i] = "\n"
        elif i not in protected and line != before and not line.rstrip().endswith("\\"):
            lines[i] = line.rstrip(" \t\r\n") + ("\n" if line.endswith("\n") else "")
    return compact_lines(api, "".join(lines))


def optional_feature_ranges(api, text, owners):
    lines = text.splitlines(keepends=True)
    offsets = [0]
    for line in lines:
        offsets.append(offsets[-1] + len(line))
    ranges = [(offsets[i], offsets[i + 1], "") for i, owner in enumerate(owners)
              if owner in {"float_math", "float_format"}]
    stack = []
    for i, line in enumerate(api.preprocessor_lines(text)):
        if re.match(r"\s*#\s*(?:if|ifdef|ifndef)\b", line):
            stack.append((i, owners[i] in {"float_io", "float_dump"} and
                          bool(re.fullmatch(r"\s*#\s*if\s+LIBRARY_HAS_FLOAT128\s*", line))))
        elif re.match(r"\s*#\s*endif\b", line) and stack:
            start, optional = stack.pop()
            if optional:
                ranges.append((offsets[start], offsets[i + 1], ""))
    return ranges


def cache_key(text: str, compiler) -> str:
    executable = Path(compiler.cxx).resolve()
    stat = executable.stat()
    data = [text, str(executable), stat.st_mtime_ns, stat.st_size, compiler.standard,
            compiler.extra_args, [str(p) for p in compiler.include_paths],
            compiler.submission_whole_program,
            {name: os.environ.get(name) for name in ("CPLUS_INCLUDE_PATH", "CPATH", "SDKROOT", "MACOSX_DEPLOYMENT_TARGET")},
            Path(__file__).read_text(), Path(__file__).with_name("submit_code.py").read_text()]
    return hashlib.sha256(json.dumps(data, sort_keys=True).encode()).hexdigest()


def freeze_conditions(api, text: str, compiler) -> str | None:
    lines = text.splitlines(keepends=True)
    visible = api.preprocessor_lines(text)
    stack, groups = [], []
    feature = re.compile(r"__STDCPP_FLOAT128_T__|LIBRARY_HAS_FLOAT128|LIBRARY_USE_QUADMATH")
    for i, line in enumerate(visible):
        if re.match(r"\s*#\s*(?:if|ifdef|ifndef)\b", line):
            stack.append([i, [], bool(feature.search(line))])
        if re.match(r"\s*#\s*(?:if|ifdef|ifndef|elif)\b", line):
            if not stack:
                return None
            stack[-1][1].append(i)
            stack[-1][2] |= bool(feature.search(line))
        if re.match(r"\s*#\s*endif\b", line):
            if not stack:
                return None
            start, branches, portable = stack.pop()
            groups.append((start, i, branches, portable))
    if stack:
        return None
    portable_ranges = [(a, b) for a, b, _, portable in groups if portable]
    branches = [i for _, _, items, _ in groups for i in items
                if not any(a <= i <= b for a, b in portable_ranges)]
    if not branches:
        return text
    token = "submit_code_active_" + secrets.token_hex(12)
    insertions, ends = {}, {}
    for i in branches:
        end = i
        while end + 1 < len(lines) and lines[end].rstrip().endswith("\\"):
            end += 1
        insertions[end] = f"#pragma {token} branch {i}\n"
        ends[i] = end
    raw = []
    spans = list(api.comment_spans(text, raw_spans=raw)) + raw
    protected_lines = set()
    for start, stop in spans:
        first = text.count("\n", 0, start)
        protected_lines.update(range(first + 1, first + text.count("\n", start, stop) + 1))
    units, depth, continued = [], 0, False
    structural = api.structural_tokens(lines)
    for i, line in enumerate(lines):
        if i not in protected_lines and not depth and not continued:
            units.append(i)
        directive = visible[i].lstrip().startswith("#") or continued
        if not directive:
            depth += structural[i].count("(") - structural[i].count(")")
        continued = line.rstrip().endswith("\\")
    prefixes = {i: f"#pragma {token} unit {i}\n" for i in units}
    compiler.write("".join(prefixes.get(i, "") + line + insertions.get(i, "") for i, line in enumerate(lines)))
    result = compiler._run([*compiler._base_command(), "-E", "-fdirectives-only", str(compiler.source)])
    if result.returncode:
        return None
    active = {int(m.group(1)) for m in re.finditer(rf"(?m)^\s*#pragma {token} branch (\d+)\s*$", result.stdout)}
    active_units = {int(m.group(1)) for m in re.finditer(rf"(?m)^\s*#pragma {token} unit (\d+)\s*$", result.stdout)}
    offsets = [0]
    for line in lines:
        offsets.append(offsets[-1] + len(line))
    edits = []
    for start, end in zip(units, units[1:] + [len(lines)]):
        if start in active_units or any(a < end and start <= b for a, b in portable_ranges):
            continue
        if any(re.match(r"\s*#\s*(?:if|ifdef|ifndef|elif|else|endif)\b", visible[i]) for i in range(start, end)):
            continue
        edits.append((offsets[start], offsets[end], ""))
    for start, end in ends.items():
        keyword = "elif" if re.match(r"\s*#\s*elif\b", visible[start]) else "if"
        edits.append((offsets[start], offsets[end + 1] - 1, f"#{keyword} {int(start in active)}"))
    return mask_ranges(api, text, edits)


def prune_function_declarations(api, text, *, compiler, writer, reporter, keep_functions):
    lines = text.splitlines(keepends=True)
    candidates = definition_candidates(api, text, ["library"] * len(lines), include_declarations=True)
    macros = api.template_signature_macro_names(text, compiler.external_macro_definitions(text))
    protected = api.portable_body_dependencies(text, candidates)
    candidates = [item for item in candidates if item.name not in keep_functions | protected
                  and api.function_body_span("".join(lines[item.start:item.end + 1])) is None
                  and set(api.code_identifiers("".join(lines[item.start:item.end + 1]))).isdisjoint(macros)]
    if not candidates or set(api.code_identifiers(text)).intersection({"__COUNTER__", "asm", "__asm__", "dlsym", "GetProcAddress"}):
        return text
    valid, diagnostic = compiler.validate_submission(text)
    if not valid:
        return text
    reference = compiler.submission_object_signature(text)
    if reference is None:
        return text
    offsets = [0]
    for line in lines:
        offsets.append(offsets[-1] + len(line))
    current, accepted = text, 0

    def remove(items):
        nonlocal current, accepted
        if not items:
            return
        candidate = mask_ranges(api, current, [(offsets[item.start], offsets[item.end + 1], "") for item in items])
        reporter.phase("signatures", f"checking {len(items)} function/method declaration(s)")
        if compiler.submission_object_signature(candidate) == reference:
            current = candidate
            accepted += len(items)
            return
        if len(items) == 1:
            return
        missing = compiler.object_diagnostic
        blocked = [item for item in items if item.name and re.search(
            rf"(?<![_A-Za-z0-9]){re.escape(item.name)}(?![_A-Za-z0-9])", missing)]
        if blocked and len(blocked) < len(items):
            remove([item for item in items if item not in blocked])
            remove(blocked)
            return
        middle = len(items) // 2
        remove(items[:middle])
        remove(items[middle:])

    remove(candidates)
    if not accepted:
        return text
    result = finish_layout(api, current, text, ["library"] * len(lines))
    fingerprint = compiler.preprocessed_code_fingerprint(current)
    if fingerprint is None or compiler.preprocessed_code_fingerprint(result) != fingerprint:
        result = current
    if writer.size(result) >= writer.size(text):
        return text
    writer.save(f"removed {accepted} function/method declaration(s)", result)
    return result


def try_cleanup(api, text: str, *, compiler, writer, reporter, target_bytes: int | None,
                keep_comments=(), feature_token: str | None = None,
                priority_bytes: int | None = None) -> str | None:
    if target_bytes is None or feature_token is None or not any(line.startswith(marker(feature_token, True)) for line in text.splitlines()):
        return None
    names = set(api.code_identifiers(text))
    if names.intersection({"asm", "__asm__", "dlsym", "GetProcAddress", "__COUNTER__", "__TIME__", "__DATE__", "__TIMESTAMP__",
                           "__FILE__", "__BASE_FILE__", "__FILE_NAME__", "__LINE__", "__builtin_FILE", "__builtin_LINE", "source_location"}):
        return None
    if compiler.extra_args or re.search(r"(?m)^\s*#\s*(?:line\b|\d)", text):
        return None
    initial, owners = feature_lines(api, text, feature_token)
    normalized = initial
    if not owners:
        return None
    macros = compiler.all_macro_definitions(text)
    dependencies = {name: set(api.code_identifiers("\n".join(bodies))) for name, bodies in macros.items()}
    roots = names - {"assert"}
    pending, expanded = list(roots), set(roots)
    while pending:
        for name in dependencies.get(pending.pop(), set()) - expanded:
            expanded.add(name)
            pending.append(name)
    trusted_paste = {"_p_F128_UNARY", "_p_F128_BINARY", "_p_F128_TERNARY_MIXED"}
    header = Path(__file__).resolve().parent.parent / "template/template_float128_math.hpp"
    expected = {}
    logical = api.COMMENT_LINE_SPLICE.sub("", api.strip_comments(header.read_text() if header.is_file() else ""))
    for match in re.finditer(r"(?m)^\s*#\s*define\s+([_A-Za-z]\w*)(.*)$", logical):
        expected[match.group(1)] = re.findall(r"\w+|\S", match.group(2))
    trusted_paste = {name for name in trusted_paste if name in expected and
                     all(re.findall(r"\w+|\S", body) == expected[name] for body in macros.get(name, []))}
    for name in expanded & macros.keys():
        if name not in trusted_paste and any("#" in body for body in macros[name]):
            return None
    if expanded.intersection({"__FILE__", "__BASE_FILE__", "__FILE_NAME__", "__LINE__", "__builtin_FILE", "__builtin_LINE"}):
        return None
    if expanded.intersection({"__OPTIMIZE__", "__OPTIMIZE_SIZE__", "__NO_INLINE__", "__FAST_MATH__", "__COUNTER__"}):
        return None
    cacheable = not expanded.intersection({"__TIME__", "__DATE__", "__TIMESTAMP__", "__COUNTER__", "__has_include"})
    keep = set(keep_comments) | set(api.important_comments(initial))
    untrusted = "".join("\n" if owner and (owner.startswith("float_") or owner == "scalar_power") else line
                        for line, owner in zip(initial.splitlines(keepends=True), owners))
    quad_names = ("acos|acosh|asin|asinh|atan|atanh|atan2|cbrt|ceil|cos|cosh|erf|erfc|exp|exp2|expm1|"
                  "fabs|floor|lgamma|log|log10|log1p|log2|logb|nearbyint|rint|round|sin|sinh|sqrt|tan|tanh|"
                  "tgamma|trunc|fma|hypot|frexp|ldexp|scalbn|scalbln|modf|remquo|lrint|lround|llrint|"
                  "llround|ilogb|finite|isinf|isnan|signbit|nextafter|copysign|fdim|fmax|fmin|fmod|remainder")
    if re.search(rf"\b\w*(?:f128|float128|flt128)\w*\b|\b(?:{quad_names})q\s*\(|\d[\w.']*[qQ]\b", api.strip_comments(untrusted), re.I):
        return None
    keep.update(comment for comment in api.source_comments(initial) if api.LIBRARY_SOURCE_LINK.fullmatch(comment))
    initial = api.strip_comments(initial, keep)
    if len(initial.splitlines()) != len(owners):
        return None
    compiler.submission_whole_program = not names.intersection({"extern", "externally_visible", "weak", "alias"}) and re.search(
        r"\boperator\s+(?:new|delete)\b", api.strip_comments(text)) is None
    context = compiler.preprocessed_code_fingerprint(text)
    if context is None:
        return None
    key = cache_key(normalized + context + json.dumps(sorted(keep)), compiler)
    cache_root = Path(os.environ.get("SUBMIT_CODE_CACHE_DIR", str(Path(tempfile.gettempdir()) / "library-submit-code-pch")))
    cache = cache_root / "feature-results" / (key + ".json")
    try:
        record = json.loads(cache.read_text()) if cacheable else {}
        if all(Path(path).is_file() and hashlib.sha256(Path(path).read_bytes()).hexdigest() == digest
               for path, digest in record["dependencies"].items()):
            result = record["text"]
            if hashlib.sha256(result.encode()).hexdigest() == record["text_sha256"] and writer.below_limit(result, target_bytes):
                writer.save("cached feature selection", result)
                reporter.phase("features", "reused the verified result")
                return result
    except (OSError, ValueError, KeyError):
        pass
    reporter.phase("features", "checking optional definition groups")
    current = freeze_conditions(api, initial, compiler)
    if current is None:
        return None
    checkpoint = None
    if priority_bytes is not None:
        macros = unused_macro_candidates(api, current, owners, compiler)
        if macros:
            positions = [0]
            for line in current.splitlines(keepends=True):
                positions.append(positions[-1] + len(line))
            trimmed = mask_ranges(api, current, [(positions[item.start], positions[item.end + 1], "") for item in macros])
        else:
            trimmed = current
        candidate = finish_layout(api, trimmed, text, owners, feature_token=feature_token)
        if writer.below_limit(candidate, priority_bytes) and compiler.preprocessed_code_fingerprint(candidate) == context:
            checkpoint = candidate
    valid, diagnostic = compiler.validate_submission(text)
    if not valid:
        return None
    reference = compiler.submission_object_signature(text)
    if reference is None:
        return None
    dependencies = dict(compiler.object_dependencies)
    reference_undefined = set(compiler.object_undefined)
    if checkpoint is not None:
        writer.save(f"verified {priority_bytes:,} B checkpoint", checkpoint)
    declaration_source = None
    def publish(result):
        writer.save("verified feature selection", result)
        compiler.submission_workspace._ensure_pch(text)
        if declaration_source is not None:
            lines = declaration_source.splitlines(keepends=True)
            macros = api.template_signature_macro_names(text, compiler.external_macro_definitions(text))
            functions = [item for item in definition_candidates(api, declaration_source, owners)
                         if set(api.code_identifiers("".join(lines[item.start:item.end + 1]))).isdisjoint(macros)
                         and not any(line.lstrip().startswith("#") for line in lines[item.start:item.end + 1])]
            removals = api.unreferenced_function_candidates(
                current, functions, compiler.external_identifiers(text) | api.DEFAULT_KEEP_FUNCTIONS)
            if removals:
                offsets = [0]
                for line in current.splitlines(keepends=True):
                    offsets.append(offsets[-1] + len(line))
                proposal = mask_ranges(api, current, [(offsets[item.start], offsets[item.end + 1], "") for item in removals])
                proposal = finish_layout(api, proposal, text, owners, feature_token=feature_token)
                try:
                    proposal = api.safe_cleanup_candidate(proposal, compiler, keep)
                except api.ToolError:
                    proposal = result
                if writer.size(proposal) < writer.size(result):
                    reporter.phase("methods", f"checking {len(removals)} unreferenced function/method declarations")
                    valid, diagnostic = compiler.validate_submission(proposal)
                    if valid:
                        result = proposal
                        writer.save("unreferenced library functions and methods", result)
                    else:
                        reporter.detail("method declarations retained: " + api.first_error(diagnostic))
        cache.parent.mkdir(parents=True, exist_ok=True)
        if cacheable:
            api.atomic_write(cache, json.dumps({"text": result, "text_sha256": hashlib.sha256(result.encode()).hexdigest(),
                                              "dependencies": dependencies}))
        reporter.phase("features", f"verified checkpoint at {writer.size(result):,} B; continuing cleanup")
        return result

    if priority_bytes is not None and priority_bytes not in writer.reached_limits:
        checkpoint = api.try_size_checkpoint(text, compiler=compiler, writer=writer, reporter=reporter,
                                             limit=priority_bytes, keep_comments=keep_comments,
                                             reference_checked=True)
        if checkpoint is not None and writer.below_limit(checkpoint, target_bytes):
            return publish(checkpoint)

    accepted = 0
    attempts = 0

    def save_priority_checkpoint(candidate):
        if priority_bytes is None or priority_bytes in writer.reached_limits:
            return
        formatted = finish_layout(api, candidate, text, owners, feature_token=feature_token)
        if not writer.below_limit(formatted, priority_bytes):
            return
        valid, diagnostic = compiler.validate_submission(formatted)
        if valid:
            writer.save(f"verified {priority_bytes:,} B checkpoint", formatted)
            writer.report_checkpoint(formatted, priority_bytes)
        else:
            reporter.detail("priority checkpoint retained: " + api.first_error(diagnostic))

    def prove(candidate: str) -> bool:
        nonlocal current, accepted, attempts
        if candidate == current:
            return True
        attempts += 1
        reporter.phase("features", f"object comparison {attempts}")
        if compiler.submission_object_signature(candidate) != reference:
            return False
        current = candidate
        accepted += 1
        save_priority_checkpoint(current)
        return True

    optional = optional_feature_ranges(api, current, owners)
    if optional:
        current = mask_ranges(api, current, optional)
    offsets = [0]
    for line in current.splitlines(keepends=True):
        offsets.append(offsets[-1] + len(line))
    forward = [(offsets[i], offsets[i + 1], "") for i, owner in enumerate(owners)
               if owner == "arithmetic_declarations"]
    if forward:
        current = mask_ranges(api, current, forward)
    declaration_source = current
    instantiated = compiler.instantiated_function_headers(text)
    emitted_lines = compiler.emitted_line_cache.get(hashlib.sha256(text.encode()).hexdigest())
    groups = definition_groups(api, current, owners, compiler,
                               instantiated=instantiated, emitted_lines=emitted_lines)
    def function_name(group):
        return group.rsplit(":", 1)[-1].split("@", 1)[0]
    pending = list(groups)
    has_source_groups = any(name.startswith("source:") for name in groups)

    def select(items):
        if not items or (accepted and writer.below_limit(
                finish_layout(api, current, text, owners, feature_token=feature_token), target_bytes)):
            return
        edits = [edit for name in items for edit in groups[name]]
        if prove(mask_ranges(api, current, edits)) or len(items) == 1:
            return
        missing = "\n".join(compiler.object_undefined - reference_undefined) + compiler.object_diagnostic
        blocked = [name for name in items if sum(function_name(other) == function_name(name) for other in items) == 1 and re.search(
            rf"(?<![_A-Za-z0-9]){re.escape(function_name(name))}(?![_A-Za-z0-9])", missing)]
        if blocked and len(blocked) < len(items):
            reporter.detail("keeping required definitions: " + ", ".join(name.rsplit(":", 1)[-1] for name in blocked))
            select([name for name in items if name not in blocked])
            return
        middle = len(items) // 2
        select(items[:middle])
        select(items[middle:])

    by_name = {}
    for name in groups:
        by_name.setdefault(function_name(name), set()).add(name)
    group_dependencies = {
        name: {dependency for identifier in api.code_identifiers("\n".join(current[a:b] for a, b, _ in edits))
               for dependency in by_name.get(identifier, ())}
        for name, edits in groups.items()
    }
    retained = set()
    combined_valid = False
    for _ in range(4):
        combined = mask_ranges(api, current, [edit for name in pending if name not in retained for edit in groups[name]])
        signature = compiler.submission_object_signature(combined)
        if signature == reference:
            combined_valid = True
            break
        reporter.detail("batch comparison: " + (api.first_error(compiler.object_diagnostic)
                                                if compiler.object_diagnostic else "object differs"))
        missing = "\n".join(compiler.object_undefined - reference_undefined) + compiler.object_diagnostic
        blocked = {name for name in pending if name not in retained and len(by_name[function_name(name)]) == 1 and re.search(
            rf"(?<![_A-Za-z0-9]){re.escape(function_name(name))}(?![_A-Za-z0-9])", missing)}
        if not blocked:
            break
        reporter.detail("keeping definitions: " + ", ".join(sorted(blocked)))
        retained.update(blocked)
        while blocked:
            needed = {dependency for name in blocked for dependency in group_dependencies[name]} - retained
            retained.update(needed)
            blocked = needed
    if combined_valid:
        current = combined
        accepted += int(current != text)
        save_priority_checkpoint(current)
    else:
        current = freeze_conditions(api, initial, compiler)
        if current is None or compiler.submission_object_signature(current) != reference:
            return None
        if optional:
            prove(mask_ranges(api, current, optional))
        if forward:
            prove(mask_ranges(api, current, forward))
        select(pending)
    for round_index in range(4):
        if accepted and writer.below_limit(
                finish_layout(api, current, text, owners, feature_token=feature_token), target_bytes):
            break
        if round_index:
            reference = compiler.submission_object_signature(current)
            if reference is None:
                return None
            reference_undefined = set(compiler.object_undefined)
            instantiated = compiler.instantiated_function_headers(current)
            emitted_lines = compiler.emitted_line_cache.get(hashlib.sha256(current.encode()).hexdigest())
            groups = definition_groups(api, current, owners, compiler,
                                       instantiated=instantiated, emitted_lines=emitted_lines)
            source_groups = {}
            for name, edits in groups.items():
                if name.startswith("source:") and name not in retained:
                    source_groups.setdefault(name.rsplit(":", 1)[0], []).extend(edits)
            for edits in sorted(source_groups.values(), key=lambda edits: sum(b - a for a, b, _ in edits), reverse=True):
                prove(mask_ranges(api, current, edits))
                if writer.below_limit(finish_layout(api, current, text, owners, feature_token=feature_token), target_bytes):
                    break
        candidates = unused_macro_candidates(api, current, owners, compiler)
        candidates += api.unreferenced_declaration_candidates(
            current, compiler.external_identifiers(current) | api.DEFAULT_KEEP_FUNCTIONS,
            compiler.dangerous_macro_names(current))
        lines = current.splitlines(keepends=True)
        filtered = []
        for candidate in candidates:
            if any(owner and owner.startswith("float_") and owner != "float_io"
                   for owner in owners[candidate.start:candidate.end + 1]):
                continue
            source = "".join(lines[candidate.start:candidate.end + 1])
            filtered.append(candidate)
        candidates = filtered
        if not candidates:
            if round_index == 0 and has_source_groups:
                continue
            break
        offsets = [0]
        for line in current.splitlines(keepends=True):
            offsets.append(offsets[-1] + len(line))
        count_before = accepted
        ready_result = None
        def remove(items):
            nonlocal current, accepted, attempts, ready_result
            if not items or attempts >= 64 or ready_result is not None:
                return
            candidate = mask_ranges(api, current, [(offsets[c.start], offsets[c.end + 1], "") for c in items])
            attempts += 1
            reporter.phase("features", f"declaration validation {attempts}")
            formatted = finish_layout(api, candidate, text, owners, feature_token=feature_token)
            ready = writer.below_limit(formatted, target_bytes)
            if not ready:
                try:
                    preprocessed = api.safe_cleanup_candidate(formatted, compiler, keep)
                except api.ToolError:
                    preprocessed = formatted
                if writer.below_limit(preprocessed, target_bytes):
                    formatted, ready = preprocessed, True
            priority_ready = (priority_bytes is not None and priority_bytes not in writer.reached_limits
                              and writer.below_limit(formatted, priority_bytes))
            valid, diagnostic = compiler.validate_submission(formatted if ready or priority_ready else candidate)
            if valid:
                current = candidate
                accepted += 1
                if priority_ready:
                    writer.save(f"verified {priority_bytes:,} B checkpoint", formatted)
                    writer.report_checkpoint(formatted, priority_bytes)
                if ready:
                    ready_result = formatted
                return
            if len(items) == 1:
                return
            missing = diagnostic
            blocked = [item for item in items if item.name and re.search(
                rf"(?<![_A-Za-z0-9]){re.escape(item.name)}(?![_A-Za-z0-9])", missing)]
            if blocked and len(blocked) < len(items):
                remove([item for item in items if item not in blocked])
                return
            middle = len(items) // 2
            remove(items[:middle])
            remove(items[middle:])
        remove(candidates)
        if ready_result is not None:
            return publish(ready_result)
        if accepted == count_before:
            if round_index == 0 and has_source_groups:
                continue
            break
    if not accepted:
        return None
    result = finish_layout(api, current, text, owners, feature_token=feature_token)
    current_signature = compiler.submission_object_signature(current)
    fingerprint = compiler.preprocessed_code_fingerprint(current)
    if fingerprint is not None and compiler.preprocessed_code_fingerprint(result) == fingerprint:
        result_signature = current_signature
    else:
        result_signature = compiler.submission_object_signature(result)
    if current_signature is None or result_signature is None or result_signature != current_signature:
        return None
    if not writer.below_limit(result, target_bytes):
        writer.save("verified partial feature selection", result)
        reporter.phase("features", f"candidate is {writer.size(result):,} B; continuing conservative cleanup")
        return None
    return publish(result)
