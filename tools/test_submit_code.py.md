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
  code: "from __future__ import annotations\n\nimport io\nfrom pathlib import Path\n\
    import shutil\nimport tempfile\nimport unittest\n\nimport submit_code\n\n\nclass\
    \ SubmitCodeTest(unittest.TestCase):\n    def test_strip_comments_preserves_literals(self)\
    \ -> None:\n        source = r'''int a/**/b;\nconst char *s = \"// not a comment\"\
    ;\nconst char *r = R\"tag(/* still text */)tag\";\nint x; // removed\nint y /*\
    \ removed\ntoo */ = 1;\n'''\n        cleaned = submit_code.strip_comments(source)\n\
    \        self.assertIn(\"int a b;\", cleaned)\n        self.assertIn('\"// not\
    \ a comment\"', cleaned)\n        self.assertIn('R\"tag(/* still text */)tag\"\
    ', cleaned)\n        self.assertNotIn(\"// removed\", cleaned)\n        self.assertNotIn(\"\
    too\", cleaned)\n\n    def test_blank_collapse_preserves_raw_strings_and_macro_boundaries(self)\
    \ -> None:\n        source = '''const char *s = R\"tag(first\n\nthird  \n)tag\"\
    ;\n#define VALUE 1 \\\\\n\nint main() { return VALUE; }\n'''\n        cleaned\
    \ = submit_code.collapse_blank_lines(source)\n        self.assertIn('R\"tag(first\\\
    n\\nthird  \\n)tag\"', cleaned)\n        self.assertIn(\"#define VALUE 1 \\\\\\\
    n\\nint main\", cleaned)\n\n    def test_blank_collapse_preserves_nonblank_lines(self)\
    \ -> None:\n        source = '''  int x = 0;  \n\n\n\n  const char *s = R\"tag(first\n\
    \  raw indentation\n)tag\";\n#define JOIN a \\\\\n  b\n'''\n        cleaned =\
    \ submit_code.collapse_blank_lines(source)\n        self.assertTrue(cleaned.startswith(\"\
    \  int x = 0;  \\n\\n  const char\"))\n        self.assertIn(\"\\n  raw indentation\\\
    n\", cleaned)\n        self.assertIn(\"#define JOIN a \\\\\\n  b\\n\", cleaned)\n\
    \n    def test_blank_collapse_preserves_inline_whitespace(self) -> None:\n   \
    \     source = \"  if (a[ i ]) {  f( x, y ); }  \\n \\t\\n\\n\\t\\n  return  0;\\\
    n\"\n        cleaned = submit_code.collapse_blank_lines(source)\n        self.assertEqual(\n\
    \            cleaned,\n            \"  if (a[ i ]) {  f( x, y ); }  \\n \\t\\\
    n  return  0;\\n\",\n        )\n\n    def test_fast_bundler_expands_pragma_once_headers(self)\
    \ -> None:\n        with tempfile.TemporaryDirectory() as directory:\n       \
    \     root = Path(directory)\n            (root / \"dep.hpp\").write_text(\"#pragma\
    \ once\\nint dep = 1;\\n\")\n            (root / \"header.hpp\").write_text(\n\
    \                '#pragma once\\n#include \"dep.hpp\"\\nint header = dep;\\n'\n\
    \            )\n            source = root / \"main.cpp\"\n            source.write_text(\n\
    \                '''#include <vector>\n#include <vector>\n// #include \"missing-comment.hpp\"\
    \nconst char *raw = R\"tag(\n#include \"missing-raw.hpp\"\n)tag\";\n#include \"\
    header.hpp\"\n#include \"header.hpp\"\nint main() { return header - 1; }\n'''\n\
    \            )\n            bundled = submit_code.FastBundler([root]).bundle(source)\n\
    \        self.assertEqual(bundled.count(\"#include <vector>\"), 1)\n        self.assertEqual(bundled.count(\"\
    int dep = 1;\"), 1)\n        self.assertEqual(bundled.count(\"int header = dep;\"\
    ), 1)\n        self.assertIn('#include \"missing-raw.hpp\"', bundled)\n\n    def\
    \ test_fast_bundler_expands_include_guard_headers(self) -> None:\n        with\
    \ tempfile.TemporaryDirectory() as directory:\n            root = Path(directory)\n\
    \            (root / \"dep.hpp\").write_text(\n                \"#ifndef DEP_HPP\\\
    n#define DEP_HPP 1\\nint dep = 1;\\n#endif\\n\"\n            )\n            (root\
    \ / \"header.hpp\").write_text(\n                '#ifndef HEADER_HPP\\n#define\
    \ HEADER_HPP\\n#include \"dep.hpp\"\\n'\n                \"int header = dep;\\\
    n#endif  // HEADER_HPP\\n\"\n            )\n            source = root / \"main.cpp\"\
    \n            source.write_text(\n                '#include \"header.hpp\"\\n#include\
    \ \"header.hpp\"\\n'\n                \"int main() { return header - 1; }\\n\"\
    \n            )\n            bundled = submit_code.FastBundler([root]).bundle(source)\n\
    \        self.assertEqual(bundled.count(\"int dep = 1;\"), 1)\n        self.assertEqual(bundled.count(\"\
    int header = dep;\"), 1)\n        self.assertNotIn(\"HEADER_HPP\", bundled)\n\
    \        self.assertNotIn(\"DEP_HPP\", bundled)\n\n    def test_fast_bundler_rejects_conditional_local_include(self)\
    \ -> None:\n        with tempfile.TemporaryDirectory() as directory:\n       \
    \     root = Path(directory)\n            (root / \"header.hpp\").write_text(\"\
    #pragma once\\n\")\n            source = root / \"main.cpp\"\n            source.write_text(\n\
    \                '#if ENABLED\\n#include \"header.hpp\"\\n#endif\\nint main()\
    \ {}\\n'\n            )\n            with self.assertRaises(submit_code.FastBundleUnsupported):\n\
    \                submit_code.FastBundler([root]).bundle(source)\n\n    def test_reporter_can_force_color(self)\
    \ -> None:\n        stream = io.StringIO()\n        reporter = submit_code.Reporter(color=\"\
    always\", stream=stream)\n        reporter.phase(\"bundle\", \"working\")\n  \
    \      reporter.size(\"cleanup\", 12_345, 10_000)\n        output = stream.getvalue()\n\
    \        self.assertIn(\"\\033[36m[bundle]\\033[0m working\", output)\n      \
    \  self.assertIn(\"\\033[1m\\033[32m10,000 B\\033[0m\", output)\n        self.assertIn(\"\
    [SIZE]\", output)\n\n    def test_line_paths_are_virtualized(self) -> None:\n\
    \        home = Path.home()\n        source = home / \"private-project/main.cpp\"\
    \n        header = home / \"shared-library/header.hpp\"\n        resolver = submit_code.OriginResolver(\n\
    \            base_dir=source.parent,\n            source=source,\n           \
    \ include_paths=[home / \"shared-library\"],\n        )\n        bundled = (\n\
    \            f'#line 1 {submit_code.json.dumps(str(source))}\\n'\n           \
    \ \"int value;\\n\"\n            f'#line 1 {submit_code.json.dumps(str(header))}\\\
    n'\n            \"int helper();\\n\"\n        )\n        document = submit_code.BundleDocument.parse(bundled,\
    \ resolver=resolver)\n        rendered = document.render(set(), keep_line=True)\n\
    \        marker_lines = [line for line in rendered.splitlines() if line.startswith(\"\
    #line\")]\n        self.assertTrue(marker_lines)\n        for line in marker_lines:\n\
    \            self.assertNotIn(str(home), line)\n            self.assertNotIn(f\"\
    /{home.name}/\", line)\n        self.assertIn('\"bundle/source/main.cpp\"', rendered)\n\
    \        self.assertIn('\"bundle/include-0/header.hpp\"', rendered)\n\n    def\
    \ test_virtual_line_paths_are_stable(self) -> None:\n        source = Path(\"\
    /tmp/project/bundle.cpp\")\n        resolver = submit_code.OriginResolver(\n \
    \           base_dir=source.parent,\n            source=source,\n            include_paths=[source.parent],\n\
    \        )\n        text = '#line 4 \"bundle/include-0/a.hpp\"\\nint x;\\n'\n\
    \        document = submit_code.BundleDocument.parse(text, resolver=resolver)\n\
    \        self.assertEqual(document.render(set(), keep_line=True), text)\n\n  \
    \  def test_preprocessor_uses_real_system_headers(self) -> None:\n        cxx\
    \ = shutil.which(\"g++-15\") or shutil.which(\"g++-14\")\n        if cxx is None:\n\
    \            self.skipTest(\"GNU C++ compiler is unavailable\")\n        reporter\
    \ = submit_code.Reporter()\n        source = '''#include <bits/stdc++.h>\n#if\
    \ __cplusplus >= 202302L\nint main() { return 0; }\n#else\nint invalid = ;\n#endif\n\
    '''\n        with submit_code.CompilerWorkspace(\n            cxx=cxx,\n     \
    \       standard=\"c++23\",\n            include_paths=[],\n            extra_args=[],\n\
    \            timeout=None,\n            reporter=reporter,\n        ) as compiler:\n\
    \            cleaned = submit_code.safe_cleanup_candidate(source, compiler)\n\
    \            valid, stderr = compiler.validate(cleaned)\n        self.assertTrue(valid,\
    \ submit_code.first_error(stderr))\n        self.assertIn(\"int main()\", cleaned)\n\
    \        self.assertNotIn(\"invalid\", cleaned)\n        self.assertIn(\"#include\
    \ <bits/stdc++.h>\", cleaned)\n\n    def test_compiler_unused_macro_warning_is_enabled(self)\
    \ -> None:\n        cxx = shutil.which(\"g++-15\") or shutil.which(\"g++-14\"\
    )\n        if cxx is None:\n            self.skipTest(\"GNU C++ compiler is unavailable\"\
    )\n        source = '''#include <bits/stdc++.h>\n#define SUBMIT_CODE_UNUSED_MACRO\
    \ 1\nint main() { return 0; }\n'''\n        with submit_code.CompilerWorkspace(\n\
    \            cxx=cxx,\n            standard=\"c++23\",\n            include_paths=[],\n\
    \            extra_args=[],\n            timeout=None,\n            reporter=submit_code.Reporter(),\n\
    \        ) as compiler:\n            warnings = compiler.compiler_warnings(source)\n\
    \        self.assertIn(\"SUBMIT_CODE_UNUSED_MACRO\", warnings)\n        self.assertIn(\"\
    [-Wunused-macros]\", warnings)\n\n    def test_compiler_unused_function_warning_is_enabled(self)\
    \ -> None:\n        cxx = shutil.which(\"g++-15\") or shutil.which(\"g++-14\"\
    )\n        if cxx is None:\n            self.skipTest(\"GNU C++ compiler is unavailable\"\
    )\n        source = '''#include <bits/stdc++.h>\nnamespace {\nint unused_function()\
    \ { return 1; }\n}\nint main() { return 0; }\n'''\n        with submit_code.CompilerWorkspace(\n\
    \            cxx=cxx,\n            standard=\"c++23\",\n            include_paths=[],\n\
    \            extra_args=[],\n            timeout=None,\n            reporter=submit_code.Reporter(),\n\
    \        ) as compiler:\n            warnings = compiler.compiler_warnings(source)\n\
    \        self.assertIn(\"unused_function\", warnings)\n        self.assertIn(\"\
    [-Wunused-function]\", warnings)\n\n    def test_anonymous_namespace_insertion_is_generic_and_idempotent(self)\
    \ -> None:\n        source = '''#include <bits/stdc++.h>\n#define VALUE 1\nusing\
    \ namespace std;\nint helper() { return VALUE; }\nsigned main() { return 0; }\n\
    '''\n        wrapped = submit_code.insert_anonymous_namespace(source)\n      \
    \  self.assertIn(\n            \"#define VALUE 1\\nnamespace {\\nusing namespace\
    \ std;\",\n            wrapped,\n        )\n        self.assertIn(\"int helper()\
    \ { return VALUE; }\\n}\\nsigned main()\", wrapped)\n        self.assertEqual(submit_code.insert_anonymous_namespace(wrapped),\
    \ wrapped)\n\n    def test_anonymous_namespace_ignores_late_conditional_include(self)\
    \ -> None:\n        source = '''#include <bits/stdc++.h>\nusing namespace std;\n\
    int helper() { return 1; }\n#ifdef LOCAL\n#include <local_debug.hpp>\n#endif\n\
    int main() { return helper(); }\n'''\n        wrapped = submit_code.insert_anonymous_namespace(source)\n\
    \        self.assertIn(\n            \"#include <bits/stdc++.h>\\nnamespace {\\\
    nusing namespace std;\",\n            wrapped,\n        )\n        self.assertIn(\"\
    #endif\\n}\\nint main()\", wrapped)\n\n    def test_validation_checks_linker_errors(self)\
    \ -> None:\n        cxx = shutil.which(\"g++-15\") or shutil.which(\"g++-14\"\
    )\n        if cxx is None:\n            self.skipTest(\"GNU C++ compiler is unavailable\"\
    )\n        source = \"int missing();\\nint main() { return missing(); }\\n\"\n\
    \        with submit_code.CompilerWorkspace(\n            cxx=cxx,\n         \
    \   standard=\"c++23\",\n            include_paths=[],\n            extra_args=[],\n\
    \            timeout=None,\n            reporter=submit_code.Reporter(),\n   \
    \     ) as compiler:\n            valid, _ = compiler.validate(source)\n     \
    \   self.assertFalse(valid)\n\n    def test_checkpoint_writer_keeps_a_submission_under_the_limit(self)\
    \ -> None:\n        with tempfile.TemporaryDirectory() as directory:\n       \
    \     output = Path(directory) / \"bundle.cpp\"\n            output.write_text(\"\
    short\\n\")\n            writer = submit_code.CheckpointWriter(\n            \
    \    output, submit_code.Reporter()\n            )\n            writer.current\
    \ = output.read_text()\n            writer.preserve_below = 10\n            self.assertFalse(writer.save(\"\
    larger\", \"too long here\\n\"))\n            self.assertEqual(output.read_text(),\
    \ \"short\\n\")\n            self.assertTrue(writer.save(\"smaller\", \"ok\\n\"\
    ))\n            self.assertEqual(output.read_text(), \"ok\\n\")\n\n    def test_cleanup_continues_after_target_checkpoint(self)\
    \ -> None:\n        cxx = shutil.which(\"g++-15\") or shutil.which(\"g++-14\"\
    )\n        if cxx is None:\n            self.skipTest(\"GNU C++ compiler is unavailable\"\
    )\n        source = '''#include <bits/stdc++.h>\n\nint unused_after_checkpoint()\
    \ { return 1; }\n\nint unused_cycle_right(int);\nint unused_cycle_left(int x)\
    \ {\n  return x ? unused_cycle_right(x - 1) : 0;\n}\nint unused_cycle_right(int\
    \ x) {\n  return x ? unused_cycle_left(x - 1) : 0;\n}\n\nstruct UsedInitializer\
    \ {\n  constexpr UsedInitializer() {}\n};\nconstexpr UsedInitializer used_initializer;\n\
    \nint selected_overload(int x) { return x; }\ndouble selected_overload(double\
    \ x) { return x; }\n\n\nint main() {\n  return  selected_overload(1) - 1 ;\n}\n\
    '''\n        with tempfile.TemporaryDirectory() as directory:\n            output\
    \ = Path(directory) / \"bundle.cpp\"\n            output.write_text(source)\n\
    \            writer = submit_code.CheckpointWriter(\n                output, submit_code.Reporter()\n\
    \            )\n            writer.current = source\n            writer.preserve_below\
    \ = 10_000\n            with submit_code.CompilerWorkspace(\n                cxx=cxx,\n\
    \                standard=\"c++23\",\n                include_paths=[],\n    \
    \            extra_args=[],\n                timeout=None,\n                reporter=submit_code.Reporter(),\n\
    \            ) as compiler:\n                cleaned = submit_code.cleanup_text(\n\
    \                    source,\n                    compiler=compiler,\n       \
    \             writer=writer,\n                    cppcheck=None,\n           \
    \         keep_functions=set(),\n                    max_passes=1,\n         \
    \           safe_only=False,\n                    require_validation=True,\n \
    \                   reporter=submit_code.Reporter(),\n                    target_bytes=10_000,\n\
    \                )\n            written = output.read_text()\n        self.assertNotIn(\"\
    unused_after_checkpoint\", cleaned)\n        self.assertNotIn(\"unused_after_checkpoint\"\
    , written)\n        self.assertNotIn(\"unused_cycle_left\", cleaned)\n       \
    \ self.assertNotIn(\"unused_cycle_right\", cleaned)\n        self.assertNotIn(\"\
    unused_cycle_left\", written)\n        self.assertNotIn(\"unused_cycle_right\"\
    , written)\n        self.assertIn(\"UsedInitializer()\", cleaned)\n        self.assertIn(\"\
    UsedInitializer()\", written)\n        self.assertIn(\"int selected_overload(int\
    \ x)\", cleaned)\n        self.assertIn(\"int selected_overload(int x)\", written)\n\
    \        self.assertNotIn(\"double selected_overload\", cleaned)\n        self.assertNotIn(\"\
    double selected_overload\", written)\n        self.assertIn(\"\\n  return  selected_overload(1)\
    \ - 1 ;\\n\", cleaned)\n        self.assertIn(\"\\n  return  selected_overload(1)\
    \ - 1 ;\\n\", written)\n        self.assertNotIn(\"\\n\\n\\n\", cleaned)\n   \
    \     self.assertNotIn(\"\\n\\n\\n\", written)\n\n    def test_unused_macro_before_standard_header_is_detected(self)\
    \ -> None:\n        cxx = shutil.which(\"g++-15\") or shutil.which(\"g++-14\"\
    )\n        if cxx is None:\n            self.skipTest(\"GNU C++ compiler is unavailable\"\
    )\n        source = '''#define UNUSED_BEFORE_BITS 1\n#include <bits/stdc++.h>\n\
    int main() { std::cout << \"UNUSED_BEFORE_BITS\"; }\n'''\n        with submit_code.CompilerWorkspace(\n\
    \            cxx=cxx,\n            standard=\"c++23\",\n            include_paths=[],\n\
    \            extra_args=[],\n            timeout=None,\n            reporter=submit_code.Reporter(),\n\
    \        ) as compiler:\n            candidates = submit_code.unused_candidates(\n\
    \                source,\n                compiler=compiler,\n               \
    \ cppcheck=None,\n                keep_functions=set(),\n                reporter=submit_code.Reporter(),\n\
    \            )\n        self.assertTrue(any(candidate.start == 0 for candidate\
    \ in candidates))\n\n    def test_validation_preserves_macros_before_standard_header(self)\
    \ -> None:\n        cxx = shutil.which(\"g++-15\") or shutil.which(\"g++-14\"\
    )\n        if cxx is None:\n            self.skipTest(\"GNU C++ compiler is unavailable\"\
    )\n        source = '''#define VALUE_BEFORE_BITS 7\n#include <bits/stdc++.h>\n\
    int main() { static_assert(VALUE_BEFORE_BITS == 7); }\n'''\n        with submit_code.CompilerWorkspace(\n\
    \            cxx=cxx,\n            standard=\"c++23\",\n            include_paths=[],\n\
    \            extra_args=[],\n            timeout=None,\n            reporter=submit_code.Reporter(),\n\
    \        ) as compiler:\n            valid, stderr = compiler.validate(source)\n\
    \        self.assertTrue(valid, submit_code.first_error(stderr))\n\n    def test_header_pruning_writes_only_valid_checkpoints(self)\
    \ -> None:\n        cxx = shutil.which(\"g++-15\") or shutil.which(\"g++-14\"\
    )\n        if cxx is None:\n            self.skipTest(\"GNU C++ compiler is unavailable\"\
    )\n        home = Path.home()\n        source = home / \"contest/main.cpp\"\n\
    \        unused = home / \"library/unused.hpp\"\n        resolver = submit_code.OriginResolver(\n\
    \            base_dir=source.parent,\n            source=source,\n           \
    \ include_paths=[home / \"library\"],\n        )\n        bundled = (\n      \
    \      f'#line 1 {submit_code.json.dumps(str(source))}\\n'\n            \"#include\
    \ <iostream>\\n\"\n            f'#line 1 {submit_code.json.dumps(str(unused))}\\\
    n'\n            \"int never_called() { return 1; }\\n\"\n            f'#line 2\
    \ {submit_code.json.dumps(str(source))}\\n'\n            'int main() { std::cout\
    \ << \"ok\\\\n\"; }\\n'\n        )\n        document = submit_code.BundleDocument.parse(bundled,\
    \ resolver=resolver)\n        reporter = submit_code.Reporter()\n        with\
    \ tempfile.TemporaryDirectory() as directory:\n            output = Path(directory)\
    \ / \"bundle.cpp\"\n            writer = submit_code.CheckpointWriter(output,\
    \ reporter)\n            initial = document.render(set(), keep_line=True)\n  \
    \          writer.save(\"initial\", initial, force=True)\n            with submit_code.CompilerWorkspace(\n\
    \                cxx=cxx,\n                standard=\"c++23\",\n             \
    \   include_paths=[],\n                extra_args=[],\n                timeout=None,\n\
    \                reporter=reporter,\n            ) as compiler:\n            \
    \    removed = submit_code.prune_header_origins(\n                    document,\n\
    \                    compiler=compiler,\n                    writer=writer,\n\
    \                    keep_patterns=[],\n                    keep_line=True,\n\
    \                    max_passes=2,\n                    reporter=reporter,\n \
    \               )\n                valid, stderr = compiler.validate(output.read_text())\n\
    \            self.assertTrue(valid, submit_code.first_error(stderr))\n       \
    \     self.assertIn(str(unused), removed)\n            self.assertNotIn(\"never_called\"\
    , output.read_text())\n            self.assertNotIn(str(home), output.read_text())\n\
    \n\nif __name__ == \"__main__\":\n    unittest.main()\n"
  dependsOn: []
  isVerificationFile: false
  path: tools/test_submit_code.py
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: tools/test_submit_code.py
layout: document
redirect_from:
- /library/tools/test_submit_code.py
- /library/tools/test_submit_code.py.html
title: tools/test_submit_code.py
---
