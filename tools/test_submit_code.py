from __future__ import annotations

import io
from pathlib import Path
import shutil
import tempfile
import unittest

import submit_code


class SubmitCodeTest(unittest.TestCase):
    def test_strip_comments_preserves_literals(self) -> None:
        source = r'''int a/**/b;
const char *s = "// not a comment";
const char *r = R"tag(/* still text */)tag";
int x; // removed
int y /* removed
too */ = 1;
'''
        cleaned = submit_code.strip_comments(source)
        self.assertIn("int a b;", cleaned)
        self.assertIn('"// not a comment"', cleaned)
        self.assertIn('R"tag(/* still text */)tag"', cleaned)
        self.assertNotIn("// removed", cleaned)
        self.assertNotIn("too", cleaned)

    def test_blank_collapse_preserves_raw_strings_and_macro_boundaries(self) -> None:
        source = '''const char *s = R"tag(first

third  
)tag";
#define VALUE 1 \\

int main() { return VALUE; }
'''
        cleaned = submit_code.collapse_blank_lines(source)
        self.assertIn('R"tag(first\n\nthird  \n)tag"', cleaned)
        self.assertIn("#define VALUE 1 \\\n\nint main", cleaned)

    def test_blank_collapse_preserves_nonblank_lines(self) -> None:
        source = '''  int x = 0;  



  const char *s = R"tag(first
  raw indentation
)tag";
#define JOIN a \\
  b
'''
        cleaned = submit_code.collapse_blank_lines(source)
        self.assertTrue(cleaned.startswith("  int x = 0;  \n\n  const char"))
        self.assertIn("\n  raw indentation\n", cleaned)
        self.assertIn("#define JOIN a \\\n  b\n", cleaned)

    def test_blank_collapse_preserves_inline_whitespace(self) -> None:
        source = "  if (a[ i ]) {  f( x, y ); }  \n \t\n\n\t\n  return  0;\n"
        cleaned = submit_code.collapse_blank_lines(source)
        self.assertEqual(
            cleaned,
            "  if (a[ i ]) {  f( x, y ); }  \n \t\n  return  0;\n",
        )

    def test_fast_bundler_expands_pragma_once_headers(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "dep.hpp").write_text("#pragma once\nint dep = 1;\n")
            (root / "header.hpp").write_text(
                '#pragma once\n#include "dep.hpp"\nint header = dep;\n'
            )
            source = root / "main.cpp"
            source.write_text(
                '''#include <vector>
#include <vector>
// #include "missing-comment.hpp"
const char *raw = R"tag(
#include "missing-raw.hpp"
)tag";
#include "header.hpp"
#include "header.hpp"
int main() { return header - 1; }
'''
            )
            bundled = submit_code.FastBundler([root]).bundle(source)
        self.assertEqual(bundled.count("#include <vector>"), 1)
        self.assertEqual(bundled.count("int dep = 1;"), 1)
        self.assertEqual(bundled.count("int header = dep;"), 1)
        self.assertIn('#include "missing-raw.hpp"', bundled)

    def test_fast_bundler_links_each_library_header(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "ds").mkdir()
            (root / "ds/dep.hpp").write_text("#pragma once\nint dep;\n")
            (root / "ds/header.hpp").write_text(
                '#pragma once\n#include "dep.hpp"\nint header;\n'
            )
            source = root / "main.cpp"
            source.write_text('#include "ds/header.hpp"\nint main() {}\n')
            bundled = submit_code.FastBundler(
                [root], library_root=root
            ).bundle(source)
        self.assertIn(
            "// https://miscalculation53.github.io/library/"
            "ds/dep.hpp.html#unbundled\n",
            bundled,
        )
        self.assertIn(
            "// https://miscalculation53.github.io/library/"
            "ds/header.hpp.html#unbundled\n",
            bundled,
        )
        self.assertNotIn("main.cpp.html", bundled)

    def test_fast_bundler_expands_include_guard_headers(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "dep.hpp").write_text(
                "#ifndef DEP_HPP\n#define DEP_HPP 1\nint dep = 1;\n#endif\n"
            )
            (root / "header.hpp").write_text(
                '#ifndef HEADER_HPP\n#define HEADER_HPP\n#include "dep.hpp"\n'
                "int header = dep;\n#endif  // HEADER_HPP\n"
            )
            source = root / "main.cpp"
            source.write_text(
                '#include "header.hpp"\n#include "header.hpp"\n'
                "int main() { return header - 1; }\n"
            )
            bundled = submit_code.FastBundler([root]).bundle(source)
        self.assertEqual(bundled.count("int dep = 1;"), 1)
        self.assertEqual(bundled.count("int header = dep;"), 1)
        self.assertNotIn("HEADER_HPP", bundled)
        self.assertNotIn("DEP_HPP", bundled)

    def test_fast_bundler_rejects_conditional_local_include(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "header.hpp").write_text("#pragma once\n")
            source = root / "main.cpp"
            source.write_text(
                '#if ENABLED\n#include "header.hpp"\n#endif\nint main() {}\n'
            )
            with self.assertRaises(submit_code.FastBundleUnsupported):
                submit_code.FastBundler([root]).bundle(source)

    def test_reporter_can_force_color(self) -> None:
        stream = io.StringIO()
        reporter = submit_code.Reporter(color="always", stream=stream)
        reporter.phase("bundle", "working")
        reporter.size("cleanup", 12_345, 10_000)
        output = stream.getvalue()
        self.assertIn("\033[36m[bundle]\033[0m working", output)
        self.assertIn("\033[1m\033[32m10,000 B\033[0m", output)
        self.assertIn("[SIZE]", output)

    def test_line_paths_are_virtualized(self) -> None:
        home = Path.home()
        source = home / "private-project/main.cpp"
        header = home / "shared-library/header.hpp"
        resolver = submit_code.OriginResolver(
            base_dir=source.parent,
            source=source,
            include_paths=[home / "shared-library"],
        )
        bundled = (
            f'#line 1 {submit_code.json.dumps(str(source))}\n'
            "int value;\n"
            f'#line 1 {submit_code.json.dumps(str(header))}\n'
            "int helper();\n"
        )
        document = submit_code.BundleDocument.parse(bundled, resolver=resolver)
        rendered = document.render(set(), keep_line=True)
        marker_lines = [line for line in rendered.splitlines() if line.startswith("#line")]
        self.assertTrue(marker_lines)
        for line in marker_lines:
            self.assertNotIn(str(home), line)
            self.assertNotIn(f"/{home.name}/", line)
        self.assertIn('"bundle/source/main.cpp"', rendered)
        self.assertIn('"bundle/include-0/header.hpp"', rendered)

    def test_virtual_line_paths_are_stable(self) -> None:
        source = Path("/tmp/project/bundle.cpp")
        resolver = submit_code.OriginResolver(
            base_dir=source.parent,
            source=source,
            include_paths=[source.parent],
        )
        text = '#line 4 "bundle/include-0/a.hpp"\nint x;\n'
        document = submit_code.BundleDocument.parse(text, resolver=resolver)
        self.assertEqual(document.render(set(), keep_line=True), text)

    def test_bundle_document_links_only_repository_headers(self) -> None:
        root = Path("/tmp/library")
        source = Path("/tmp/contest/main.cpp")
        header = root / "math/rational.hpp"
        external = Path("/tmp/external/dependency.hpp")
        resolver = submit_code.OriginResolver(
            base_dir=source.parent,
            source=source,
            include_paths=[root, external.parent],
        )
        bundled = (
            f'#line 1 {submit_code.json.dumps(str(header))}\nint rational;\n'
            f'#line 1 {submit_code.json.dumps(str(external))}\nint dependency;\n'
            f'#line 1 {submit_code.json.dumps(str(source))}\nint main() {{}}\n'
        )
        document = submit_code.BundleDocument.parse(
            bundled,
            resolver=resolver,
            library_root=root,
        )
        rendered = document.render(set(), keep_line=False)
        self.assertEqual(rendered.count("#unbundled"), 1)
        self.assertIn(
            "// https://miscalculation53.github.io/library/"
            "math/rational.hpp.html#unbundled\n",
            rendered,
        )
        self.assertNotIn("dependency.hpp.html", rendered)

    def test_cleanup_comment_stripping_preserves_source_links(self) -> None:
        source = (
            "// ordinary comment\n"
            "// https://miscalculation53.github.io/library/"
            "ds/segtree.hpp.html#unbundled\n"
            "int value; // trailing comment\n"
        )
        cleaned = submit_code.strip_cleanup_comments(source)
        self.assertNotIn("ordinary comment", cleaned)
        self.assertNotIn("trailing comment", cleaned)
        self.assertIn("ds/segtree.hpp.html#unbundled", cleaned)

    def test_preprocessor_uses_real_system_headers(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        reporter = submit_code.Reporter()
        source = '''// removed comment
// https://miscalculation53.github.io/library/ds/segtree.hpp.html#unbundled
#include <bits/stdc++.h>
#if __cplusplus >= 202302L
int main() { return 0; }
#else
int invalid = ;
#endif
'''
        with submit_code.CompilerWorkspace(
            cxx=cxx,
            standard="c++23",
            include_paths=[],
            extra_args=[],
            timeout=None,
            reporter=reporter,
        ) as compiler:
            cleaned = submit_code.safe_cleanup_candidate(source, compiler)
            valid, stderr = compiler.validate(cleaned)
        self.assertTrue(valid, submit_code.first_error(stderr))
        self.assertIn("int main()", cleaned)
        self.assertNotIn("invalid", cleaned)
        self.assertNotIn("removed comment", cleaned)
        self.assertIn("ds/segtree.hpp.html#unbundled", cleaned)
        self.assertIn("#include <bits/stdc++.h>", cleaned)

    def test_compiler_unused_macro_warning_is_enabled(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#include <bits/stdc++.h>
#define SUBMIT_CODE_UNUSED_MACRO 1
int main() { return 0; }
'''
        with submit_code.CompilerWorkspace(
            cxx=cxx,
            standard="c++23",
            include_paths=[],
            extra_args=[],
            timeout=None,
            reporter=submit_code.Reporter(),
        ) as compiler:
            warnings = compiler.compiler_warnings(source)
        self.assertIn("SUBMIT_CODE_UNUSED_MACRO", warnings)
        self.assertIn("[-Wunused-macros]", warnings)

    def test_compiler_unused_function_warning_is_enabled(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#include <bits/stdc++.h>
namespace {
int unused_function() { return 1; }
}
int main() { return 0; }
'''
        with submit_code.CompilerWorkspace(
            cxx=cxx,
            standard="c++23",
            include_paths=[],
            extra_args=[],
            timeout=None,
            reporter=submit_code.Reporter(),
        ) as compiler:
            warnings = compiler.compiler_warnings(source)
        self.assertIn("unused_function", warnings)
        self.assertIn("[-Wunused-function]", warnings)

    def test_anonymous_namespace_insertion_is_generic_and_idempotent(self) -> None:
        source = '''#include <bits/stdc++.h>
#define VALUE 1
using namespace std;
int helper() { return VALUE; }
signed main() { return 0; }
'''
        wrapped = submit_code.insert_anonymous_namespace(source)
        self.assertIn(
            "#define VALUE 1\nnamespace {\nusing namespace std;",
            wrapped,
        )
        self.assertIn("int helper() { return VALUE; }\n}\nsigned main()", wrapped)
        self.assertEqual(submit_code.insert_anonymous_namespace(wrapped), wrapped)

    def test_anonymous_namespace_ignores_late_conditional_include(self) -> None:
        source = '''#include <bits/stdc++.h>
using namespace std;
int helper() { return 1; }
#ifdef LOCAL
#include <local_debug.hpp>
#endif
int main() { return helper(); }
'''
        wrapped = submit_code.insert_anonymous_namespace(source)
        self.assertIn(
            "#include <bits/stdc++.h>\nnamespace {\nusing namespace std;",
            wrapped,
        )
        self.assertIn("#endif\n}\nint main()", wrapped)

    def test_validation_checks_linker_errors(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = "int missing();\nint main() { return missing(); }\n"
        with submit_code.CompilerWorkspace(
            cxx=cxx,
            standard="c++23",
            include_paths=[],
            extra_args=[],
            timeout=None,
            reporter=submit_code.Reporter(),
        ) as compiler:
            valid, _ = compiler.validate(source)
        self.assertFalse(valid)

    def test_checkpoint_writer_keeps_a_submission_under_the_limit(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "bundle.cpp"
            output.write_text("short\n")
            writer = submit_code.CheckpointWriter(
                output, submit_code.Reporter()
            )
            writer.current = output.read_text()
            writer.preserve_below = 10
            self.assertFalse(writer.save("larger", "too long here\n"))
            self.assertEqual(output.read_text(), "short\n")
            self.assertTrue(writer.save("smaller", "ok\n"))
            self.assertEqual(output.read_text(), "ok\n")

    def test_cleanup_continues_after_target_checkpoint(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#include <bits/stdc++.h>

int unused_after_checkpoint() { return 1; }

int unused_cycle_right(int);
int unused_cycle_left(int x) {
  return x ? unused_cycle_right(x - 1) : 0;
}
int unused_cycle_right(int x) {
  return x ? unused_cycle_left(x - 1) : 0;
}

struct UsedInitializer {
  constexpr UsedInitializer() {}
};
constexpr UsedInitializer used_initializer;

int selected_overload(int x) { return x; }
double selected_overload(double x) { return x; }


int main() {
  return  selected_overload(1) - 1 ;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "bundle.cpp"
            output.write_text(source)
            writer = submit_code.CheckpointWriter(
                output, submit_code.Reporter()
            )
            writer.current = source
            writer.preserve_below = 10_000
            with submit_code.CompilerWorkspace(
                cxx=cxx,
                standard="c++23",
                include_paths=[],
                extra_args=[],
                timeout=None,
                reporter=submit_code.Reporter(),
            ) as compiler:
                cleaned = submit_code.cleanup_text(
                    source,
                    compiler=compiler,
                    writer=writer,
                    cppcheck=None,
                    keep_functions=set(),
                    max_passes=1,
                    safe_only=False,
                    require_validation=True,
                    reporter=submit_code.Reporter(),
                    target_bytes=10_000,
                )
            written = output.read_text()
        self.assertNotIn("unused_after_checkpoint", cleaned)
        self.assertNotIn("unused_after_checkpoint", written)
        self.assertNotIn("unused_cycle_left", cleaned)
        self.assertNotIn("unused_cycle_right", cleaned)
        self.assertNotIn("unused_cycle_left", written)
        self.assertNotIn("unused_cycle_right", written)
        self.assertIn("UsedInitializer()", cleaned)
        self.assertIn("UsedInitializer()", written)
        self.assertIn("int selected_overload(int x)", cleaned)
        self.assertIn("int selected_overload(int x)", written)
        self.assertNotIn("double selected_overload", cleaned)
        self.assertNotIn("double selected_overload", written)
        self.assertIn("\n  return  selected_overload(1) - 1 ;\n", cleaned)
        self.assertIn("\n  return  selected_overload(1) - 1 ;\n", written)
        self.assertNotIn("\n\n\n", cleaned)
        self.assertNotIn("\n\n\n", written)

    def test_unused_macro_before_standard_header_is_detected(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#define UNUSED_BEFORE_BITS 1
#include <bits/stdc++.h>
int main() { std::cout << "UNUSED_BEFORE_BITS"; }
'''
        with submit_code.CompilerWorkspace(
            cxx=cxx,
            standard="c++23",
            include_paths=[],
            extra_args=[],
            timeout=None,
            reporter=submit_code.Reporter(),
        ) as compiler:
            candidates = submit_code.unused_candidates(
                source,
                compiler=compiler,
                cppcheck=None,
                keep_functions=set(),
                reporter=submit_code.Reporter(),
            )
        self.assertTrue(any(candidate.start == 0 for candidate in candidates))

    def test_validation_preserves_macros_before_standard_header(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#define VALUE_BEFORE_BITS 7
#include <bits/stdc++.h>
int main() { static_assert(VALUE_BEFORE_BITS == 7); }
'''
        with submit_code.CompilerWorkspace(
            cxx=cxx,
            standard="c++23",
            include_paths=[],
            extra_args=[],
            timeout=None,
            reporter=submit_code.Reporter(),
        ) as compiler:
            valid, stderr = compiler.validate(source)
        self.assertTrue(valid, submit_code.first_error(stderr))

    def test_header_pruning_writes_only_valid_checkpoints(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        home = Path.home()
        source = home / "contest/main.cpp"
        unused = home / "library/unused.hpp"
        resolver = submit_code.OriginResolver(
            base_dir=source.parent,
            source=source,
            include_paths=[home / "library"],
        )
        bundled = (
            f'#line 1 {submit_code.json.dumps(str(source))}\n'
            "#include <iostream>\n"
            f'#line 1 {submit_code.json.dumps(str(unused))}\n'
            "int never_called() { return 1; }\n"
            f'#line 2 {submit_code.json.dumps(str(source))}\n'
            'int main() { std::cout << "ok\\n"; }\n'
        )
        document = submit_code.BundleDocument.parse(bundled, resolver=resolver)
        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "bundle.cpp"
            writer = submit_code.CheckpointWriter(output, reporter)
            initial = document.render(set(), keep_line=True)
            writer.save("initial", initial, force=True)
            with submit_code.CompilerWorkspace(
                cxx=cxx,
                standard="c++23",
                include_paths=[],
                extra_args=[],
                timeout=None,
                reporter=reporter,
            ) as compiler:
                removed = submit_code.prune_header_origins(
                    document,
                    compiler=compiler,
                    writer=writer,
                    keep_patterns=[],
                    keep_line=True,
                    max_passes=2,
                    reporter=reporter,
                )
                valid, stderr = compiler.validate(output.read_text())
            self.assertTrue(valid, submit_code.first_error(stderr))
            self.assertIn(str(unused), removed)
            self.assertNotIn("never_called", output.read_text())
            self.assertNotIn(str(home), output.read_text())


if __name__ == "__main__":
    unittest.main()
