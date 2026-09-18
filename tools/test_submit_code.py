from __future__ import annotations

import io
from pathlib import Path
import shutil
import tempfile
import unittest
from unittest.mock import Mock, patch

import submit_code


class SubmitCodeTest(unittest.TestCase):
    def test_digit_separators_do_not_hide_code_or_comments(self) -> None:
        for number in ["1'000'000'007", "0xAB'CD", "0b1'0", ".1'2e+3'4", "0xAB'CDp-1"]:
            with self.subTest(number=number):
                source = f'''#define VALUE {number} // removed
using Live = int;
using Dead = int;
int main() {{ Live x = VALUE; return x != VALUE; }}
'''
                cleaned = submit_code.strip_comments(source)
                self.assertNotIn("removed", cleaned)
                self.assertIn(number, cleaned)
                identifiers = list(submit_code.code_identifiers(cleaned))
                self.assertEqual(identifiers.count("VALUE"), 3)
                self.assertEqual(identifiers.count("Live"), 2)
                self.assertIn("main", identifiers)
                candidates = submit_code.unreferenced_declaration_candidates(source)
                self.assertEqual({c.name for c in candidates}, {"Dead"})

    def test_digit_separators_preserve_literal_and_function_boundaries(self) -> None:
        source = '''int f() { auto x = 0xAB'CD; return x; }
const char *raw = R"tag(first


last)tag";
char quote = '\\'';
int main() { return f() != 0xABCD; }
'''
        tokens = submit_code.structural_tokens(source.splitlines(keepends=True))
        self.assertEqual(tokens[0], ["(", ")", "{", ";", ";", "}"])
        self.assertEqual(submit_code.function_body_span(source),
                         (source.index("{"), source.index("}")))
        self.assertEqual(submit_code.collapse_blank_lines(source), source)
        visible = submit_code.preprocessor_lines(source)
        self.assertIn("int main()", visible[-1])
        self.assertEqual(list(submit_code.code_identifiers(source)),
                         ["int", "f", "auto", "x", "return", "x", "const", "char", "raw",
                          "char", "quote", "int", "main", "return", "f"])

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

    def test_declaration_graph_removes_self_references_and_alias_chains(self) -> None:
        source = '''template <class T>
struct Unused {
  Unused() {}
  using Self = Unused;
};
using UnusedAlias = Unused<int>;
struct Live { Live() {} };
using LiveAlias = Live;
LiveAlias instance;
int main() {}
'''
        candidates = submit_code.unreferenced_declaration_candidates(source)
        self.assertEqual({c.name for c in candidates}, {"Unused", "UnusedAlias"})
        cleaned = submit_code.apply_removals(source, candidates)
        self.assertIn("struct Live", cleaned)
        self.assertIn("LiveAlias instance;", cleaned)

    def test_declaration_graph_removes_unused_lambdas_with_their_dependencies(self) -> None:
        source = '''using Number = int;
inline constexpr auto unused = [](auto x) { return Number(x); };
inline constexpr auto used = [](int x) { return x; };
int main() { return used(0); }
'''
        candidates = submit_code.unreferenced_declaration_candidates(source)
        self.assertEqual({c.name for c in candidates}, {"Number", "unused"})

    def test_declaration_graph_preserves_initializers_friends_and_literals(self) -> None:
        source = '''int effect();
struct Guard { Guard() { effect(); } } guard;
struct StaticEffect { inline static int value = effect(); };
template <class T>
struct TemplateEffect { inline static int value = effect(); };
struct Friend { friend int effect() { return 0; } };
constexpr auto invoked = [] { return 0; }();
constexpr auto captured = [value = 0] { return value; };
using SharedLine = int; int effect_result = effect();
struct SharedType {}; struct AnotherEffect { inline static int value = effect(); };
constexpr auto shared_lambda = [] {}; int lambda_effect = effect();
#define EFFECT int macro_effect = effect();
using SharedMacro = int; EFFECT
const char *raw = R"tag(
struct InLiteral { InLiteral() {} };
using LiteralAlias = int;
)tag";
#define DEFINE_TYPE \\
  struct InMacro { InMacro() {} };
int main() {}
'''
        self.assertEqual(submit_code.unreferenced_declaration_candidates(source), [])

    def test_declaration_graph_does_not_guess_token_pasted_names(self) -> None:
        source = '''#define CAT(a, b) a ## b
struct Widget { Widget() {} };
CAT(Wid, get) instance;
int main() {}
'''
        self.assertEqual(submit_code.unreferenced_declaration_candidates(source), [])

    def test_template_candidates_distinguish_lambda_and_explicit_call_operators(self) -> None:
        source = '''auto lambda = [](auto x) { return x; };
struct Callable {
  template <class T>
  int operator()(T x) { return x; }
};
'''
        compiler = Mock()
        compiler.uninstantiated_template_functions.return_value = [
            (1, ""), (1, "operator()"), (4, "operator()")
        ]
        candidates = submit_code.uninstantiated_template_candidates(
            source, compiler=compiler, keep_functions=set()
        )
        self.assertEqual([(c.start, c.end) for c in candidates], [(2, 3)])

    def test_template_removal_includes_constraints_and_split_return_types(self) -> None:
        source = '''template <class T>
  requires (
    sizeof(T) > 0)
decltype(auto)
constrained(T x) { return x; }
template <class T>
struct Holder {
  int method() { return 0; }
};
template <class T>
[[deprecated("old helper; use another helper")]]
int deprecated_helper(T) { return 0; }
'''
        compiler = Mock()
        compiler.uninstantiated_template_functions.return_value = [
            (5, "constrained"), (8, "method"), (12, "deprecated_helper")
        ]
        candidates = submit_code.uninstantiated_template_candidates(
            source, compiler=compiler, keep_functions=set()
        )
        self.assertEqual([(c.start, c.end) for c in candidates], [(0, 4), (7, 7), (9, 11)])
        cleaned = submit_code.apply_removals(source, candidates)
        self.assertNotIn("requires", cleaned)
        self.assertNotIn("deprecated", cleaned)
        self.assertIn("template <class T>\nstruct Holder", cleaned)
        adjacent = ['template <class T> int previous(T) { return 0; }\n',
                    'struct Separate {};\n']
        self.assertEqual(submit_code.template_prefix_start(
            adjacent, 1, submit_code.structural_tokens(adjacent)
        ), 1)

    def test_template_candidates_use_constructor_names_from_source(self) -> None:
        source = '''template <class T>
struct Widget {
  template <class U>
  explicit Widget(U) {}
  ~Widget() {}
  explicit(false) Widget() {}
};
'''
        compiler = Mock()
        compiler.uninstantiated_template_functions.return_value = [
            (4, "__ct"), (5, "__dt"), (6, "__ct")
        ]
        candidates = submit_code.uninstantiated_template_candidates(
            source, compiler=compiler, keep_functions=set()
        )
        self.assertEqual([c.name for c in candidates], ["Widget", "~Widget"])

    def test_template_stub_handles_scoped_trailing_return_type(self) -> None:
        source = '''template <class T> struct Pair { using first_type = T; };
template <class T> Pair<T> convert(T) { return {}; }
template <class T>
auto wrapper(T x) -> typename decltype(convert(x))::first_type
{
  return x;
}
int main() { return 0; }
'''
        lines = source.splitlines(keepends=True)
        candidate = submit_code.removal_from_function_line(
            lines, submit_code.structural_tokens(lines), 3, "unused"
        )
        cleaned = submit_code.stub_uninstantiated_template_functions(source, [candidate])
        self.assertIn("decltype(convert(x))::first_type;", cleaned)
        self.assertNotIn("return x", cleaned)

    def test_template_constructor_removal_covers_braced_initializers_and_body(self) -> None:
        source = '''template <class T> struct Box {
  T first, second;
  template <class U>
  Box(U x) : first{x},
             second{T{}}
  /* body begins after both initializers */
  {
    first += second;
  }
  int retained() { return 1; }
};
int main() { return 0; }
'''
        lines = source.splitlines(keepends=True)
        candidate = submit_code.removal_from_function_line(
            lines, submit_code.structural_tokens(lines), 3, "unused"
        )
        self.assertEqual((candidate.start, candidate.end), (2, 8))
        cleaned = submit_code.stub_uninstantiated_template_functions(source, [candidate])
        self.assertIn("Box(U x);", cleaned)
        self.assertNotIn("first +=", cleaned)
        self.assertIn("int retained() { return 1; }", cleaned)

    def test_requires_expression_is_preserved_by_stub_and_removed_with_definition(self) -> None:
        source = '''template <class T> struct Box {
  template <class U>
    requires requires(U x)
    {
      x.value();
    }
  auto value(const U &x) -> decltype(x.value())
  {
    return x.value();
  }
};
int main() { return 0; }
'''
        lines = source.splitlines(keepends=True)
        candidate = submit_code.removal_from_function_line(
            lines, submit_code.structural_tokens(lines), 6, "unused"
        )
        self.assertEqual((candidate.start, candidate.end), (1, 9))
        stubbed = submit_code.stub_uninstantiated_template_functions(source, [candidate])
        self.assertIn("requires requires(U x)", stubbed)
        self.assertIn("auto value(const U &x) -> decltype(x.value());", stubbed)
        self.assertNotIn("return x.value()", stubbed)
        deleted = submit_code.stub_uninstantiated_template_functions(source, [candidate], deleted={candidate})
        self.assertEqual(deleted, 'template <class T> struct Box {\n};\nint main() { return 0; }\n')

        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        with submit_code.CompilerWorkspace(cxx=cxx, standard="c++23", include_paths=[],
                extra_args=[], timeout=None, reporter=submit_code.Reporter()) as compiler:
            for text in (source, stubbed, deleted):
                valid, error = compiler.validate(text)
                self.assertTrue(valid, error)

    def test_parenthesized_requires_expression_stays_with_template(self) -> None:
        source = '''struct Box {
  template <class T>
    requires(!requires(T x) { x.value(); })
  int get(T) { return 3; }
};
int main() { return 0; }
'''
        lines = source.splitlines(keepends=True)
        candidate = submit_code.removal_from_function_line(
            lines, submit_code.structural_tokens(lines), 3, "unused"
        )
        self.assertEqual((candidate.start, candidate.end), (1, 3))
        stubbed = submit_code.stub_uninstantiated_template_functions(source, [candidate])
        self.assertIn("requires(!requires(T x) { x.value(); })", stubbed)
        self.assertIn("int get(T);", stubbed)
        self.assertNotIn("requires", submit_code.apply_removals(source, [candidate]))

    def test_required_constructor_is_retained_without_batch_bisection(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''template <class T>
struct Box {
  template <class U>
  Box(U) {}
  T unused(T x) { return x; }
};
int main() { Box<int> box(0); }
'''
        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory, submit_code.CompilerWorkspace(
            cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
            timeout=None, reporter=reporter,
        ) as compiler:
            with patch.object(compiler, "uninstantiated_template_functions", return_value=[
                (4, "__ct"), (5, "unused")
            ]):
                candidates = submit_code.uninstantiated_template_candidates(
                    source, compiler=compiler, keep_functions=set()
                )
            writer = submit_code.CheckpointWriter(Path(directory) / "bundle.cpp", reporter)
            with patch.object(compiler, "validate", wraps=compiler.validate) as validation:
                cleaned, accepted = submit_code.reduce_template_body_candidates(
                    source, candidates, compiler=compiler, writer=writer, reporter=reporter
                )
                self.assertLessEqual(validation.call_count, 3)
            self.assertEqual(accepted, 1)
            self.assertIn("Box(U) {}", cleaned)
            self.assertNotIn("unused", cleaned)

    def test_required_template_dependencies_are_retained_together(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''template <class T> int leaf(T) { return 7; }
template <class T> int middle(T x) { return leaf<T>(x); }
template <class T> int entry(T unused_function) { return middle<T>(unused_function); }
template <class T> int unused_function(T) { return -1; }
int main() { return entry(0) - 7; }
'''
        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory, submit_code.CompilerWorkspace(
            cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
            timeout=None, reporter=reporter,
        ) as compiler:
            candidates = [submit_code.Removal(i, i, "template", name)
                          for i, name in enumerate(["leaf", "middle", "entry", "unused_function"])]
            writer = submit_code.CheckpointWriter(Path(directory) / "bundle.cpp", reporter)
            with patch.object(compiler, "validate", wraps=compiler.validate) as validation:
                cleaned, accepted = submit_code.reduce_template_body_candidates(
                    source, candidates, compiler=compiler, writer=writer, reporter=reporter
                )
                self.assertLessEqual(validation.call_count, 3)
            self.assertEqual(accepted, 1)
            self.assertNotIn("int unused_function(", cleaned)
            self.assertEqual(submit_code.run_command([str(compiler.executable)]).returncode, 0)

    def test_template_dependency_shortcut_does_not_prevent_size_checkpoint(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = (
            "template <class T> int helper(T) { return 7; }\n"
            "template <class T> int large_unused(T x) { return x" + " + 0" * 100 + "; }\n"
            "template <class T> int entry(T x) { return helper(x); }\n"
            "template <class T> int entry(T x, T) { return large_unused(x); }\n"
            "int main() { return entry(0) - 7; }\n"
        )
        limit = len(source.encode()) - len(source.splitlines(keepends=True)[1].encode()) + 1
        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory, submit_code.CompilerWorkspace(
            cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
            timeout=None, reporter=reporter,
        ) as compiler:
            candidates = [submit_code.Removal(i, i, "template", name)
                          for i, name in enumerate(["helper", "large_unused", "entry", "entry"])]
            writer = submit_code.CheckpointWriter(Path(directory) / "bundle.cpp", reporter)
            writer.current = source
            cleaned, _ = submit_code.reduce_template_body_candidates(
                source, candidates, compiler=compiler, writer=writer,
                reporter=reporter, target_bytes=limit,
            )
            self.assertTrue(submit_code.below_byte_limit(cleaned, limit))
            self.assertNotIn("int large_unused(", cleaned)
            self.assertEqual(submit_code.run_command([str(compiler.executable)]).returncode, 0)

    def test_declaration_checkpoint_precedes_template_analysis_and_checks_linking(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''struct UnusedSelfReferencingType {
  UnusedSelfReferencingType() {}
  using Self = UnusedSelfReferencingType;
  int unused_member() { return 123; }
};
int main() { return 0; }
'''
        class FirstCheckpoint(Exception):
            pass

        class RecordingWriter(submit_code.CheckpointWriter):
            def save(self, stage, text, *, force=False):
                saved = super().save(stage, text, force=force)
                if saved and submit_code.below_byte_limit(text, 80):
                    raise FirstCheckpoint()
                return saved

        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "bundle.cpp"
            output.write_text(source)
            writer = RecordingWriter(output, reporter)
            writer.current = source
            with submit_code.CompilerWorkspace(
                cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
                timeout=None, reporter=reporter,
            ) as compiler:
                with patch.object(compiler, "uninstantiated_template_functions") as analysis:
                    with self.assertRaises(FirstCheckpoint):
                        submit_code.cleanup_text(
                            source, compiler=compiler, writer=writer, cppcheck=None,
                            keep_functions=set(), max_passes=1, safe_only=False,
                            require_validation=True, reporter=reporter, target_bytes=80,
                        )
                    analysis.assert_not_called()
                self.assertTrue(compiler.executable.is_file())
                self.assertEqual(submit_code.run_command([str(compiler.executable)]).returncode, 0)
                self.assertLess(output.stat().st_size, 80)

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
            "// https://github.com/miscalculation53/library/tree/wip/"
            "ds/dep.hpp\n",
            bundled,
        )
        self.assertIn(
            "// https://github.com/miscalculation53/library/tree/wip/"
            "ds/header.hpp\n",
            bundled,
        )
        self.assertNotIn("tree/wip/main.cpp", bundled)

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
        self.assertEqual(rendered.count("/tree/wip/"), 1)
        self.assertIn(
            "// https://github.com/miscalculation53/library/tree/wip/"
            "math/rational.hpp\n",
            rendered,
        )
        self.assertNotIn("tree/wip/dependency.hpp", rendered)

    def test_cleanup_comment_stripping_preserves_source_links(self) -> None:
        source = (
            "// ordinary comment\n"
            "// https://github.com/miscalculation53/library/tree/wip/"
            "ds/segtree.hpp\n"
            "int value; // trailing comment\n"
        )
        cleaned = submit_code.strip_cleanup_comments(source)
        self.assertNotIn("ordinary comment", cleaned)
        self.assertNotIn("trailing comment", cleaned)
        self.assertIn("library/tree/wip/ds/segtree.hpp", cleaned)

    def test_cleanup_preserves_only_comments_from_the_source(self) -> None:
        source = '''// 方針: 左から処理する
int main() { return 0; } // 終了
/* 計算量


O(N): R"tag( や ' も説明の一部
*/
'''
        comments = submit_code.source_comments(source)
        bundled = "// library implementation note\nint helper;\n" + source
        cleaned = submit_code.collapse_blank_lines(
            submit_code.strip_cleanup_comments(bundled, comments)
        )
        self.assertIn(source, cleaned)
        self.assertNotIn("library implementation note", cleaned)
        self.assertNotIn("submit_code_comment_", cleaned)
        literal = 'const char *s = R"tag(// only a string)tag";\n'
        self.assertEqual(submit_code.source_comments(literal), set())

    def test_comment_snapshot_survives_source_edits_and_custom_output_names(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "solution.cpp"
            source.write_text('// 展開時のメモ\nint main() {}\n')
            output = root / "custom.cpp"
            with patch('sys.stderr', io.StringIO()):
                submit_code.expand_command(submit_code.parse_args([
                    'expand', str(source), '-o', str(output), '--no-fallback',
                ]), root)
            source.write_text('// 後からの編集\nint main() {}\n')
            kept = submit_code.load_source_comments(output)
            self.assertEqual(kept, {'// 展開時のメモ'})
            self.assertIn('// 展開時のメモ', submit_code.strip_cleanup_comments(output.read_text(), kept))
            self.assertNotIn(str(root), output.read_text())

    def test_old_bundle_comment_policy_finds_the_original_source(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for original, output in [('main.cpp', 'bundle.cpp'), ('answer.cc', 'answer-bundle.cpp')]:
                (root / original).write_text('// 元コード\nint main() {}\n')
                self.assertEqual(submit_code.load_source_comments(root / output), {'// 元コード'})

    def test_fast_bundle_preserves_notes_on_replaced_includes(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'dep.hpp').write_text('#pragma once\nint value = 0;\n')
            source = root / 'main.cpp'
            original = '''#include "dep.hpp" // 値を使う
#include "dep.hpp" /* 複数行の説明
続き */
int main() { return value; }
'''
            source.write_text(original)
            expanded = submit_code.FastBundler([root]).bundle(source)
            self.assertEqual(submit_code.source_comments(expanded), submit_code.source_comments(original))
            self.assertEqual(expanded.count('int value = 0;'), 1)

    def test_comment_contents_do_not_become_line_or_include_directives(self) -> None:
        source = Path('/tmp/main.cpp')
        text = '''/* example:
#line 100 "not-a-real-file"
#include "missing.hpp"
*/
int main() {}
'''
        resolver = submit_code.OriginResolver(base_dir=source.parent, source=source, include_paths=[])
        self.assertEqual(submit_code.BundleDocument.parse(text, resolver=resolver).render(set(), keep_line=True), text)
        wrapped, includes = submit_code.wrap_include_directives(text)
        self.assertEqual(wrapped, text)
        self.assertEqual(includes, {})
        text = 'int main() {}\n// final note without newline'
        self.assertEqual(submit_code.collapse_blank_lines(text), text)
        self.assertEqual(submit_code.BundleDocument.parse(text, resolver=resolver).render(set(), keep_line=True), text)

    def test_preprocessor_preserves_source_comment_text_and_macro_semantics(self) -> None:
        cxx = shutil.which('g++-15') or shutil.which('g++-14')
        if cxx is None:
            self.skipTest('GNU C++ compiler is unavailable')
        source = '''// 方針
#define VALUE 0 // 値の意味
#define OTHER /* 説明
続き */ 1
static_assert(VALUE == 0 && OTHER == 1);
/* 複数行


#include "missing.hpp"
*/
int main() { int /* 初期値 */ x = VALUE; return x + OTHER - 1; } // 終了
'''
        comments = submit_code.source_comments(source)
        with submit_code.CompilerWorkspace(cxx=cxx, standard='c++23', include_paths=[],
                extra_args=[], timeout=None, reporter=submit_code.Reporter()) as compiler:
            cleaned = submit_code.safe_cleanup_candidate('// library note\n' + source, compiler, comments)
            valid, stderr = compiler.validate(cleaned)
            again = submit_code.safe_cleanup_candidate(cleaned, compiler, comments)
        self.assertTrue(valid, stderr)
        self.assertEqual(submit_code.source_comments(cleaned), comments)
        self.assertEqual(again, cleaned)
        self.assertNotIn('submit_code_comment_', cleaned)

    def test_full_cleanup_keeps_solution_comments_and_removes_unused_library_code(self) -> None:
        cxx = shutil.which('g++-15') or shutil.which('g++-14')
        if cxx is None:
            self.skipTest('GNU C++ compiler is unavailable')
        source = '''// 解法の説明
int main() {
    // 答えは 0
    return 0; // 出力の代わり
}
'''
        bundled = '// library note\nint unused_helper() { return 1; }\n' + source
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / 'bundle.cpp'
            output.write_text(bundled)
            reporter = submit_code.Reporter(stream=io.StringIO())
            writer = submit_code.CheckpointWriter(output, reporter)
            with submit_code.CompilerWorkspace(cxx=cxx, standard='c++23', include_paths=[],
                    extra_args=[], timeout=None, reporter=reporter) as compiler:
                cleaned = submit_code.cleanup_text(bundled, compiler=compiler, writer=writer,
                    cppcheck=None, keep_functions=submit_code.DEFAULT_KEEP_FUNCTIONS,
                    max_passes=2, safe_only=False, require_validation=True, reporter=reporter,
                    target_bytes=65536, keep_comments=submit_code.source_comments(source))
        self.assertTrue(submit_code.source_comments(source) <= submit_code.source_comments(cleaned))
        self.assertNotIn('library note', cleaned)
        self.assertNotIn('unused_helper', cleaned)

    def test_preprocessor_uses_real_system_headers(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        reporter = submit_code.Reporter()
        source = '''// removed comment
// https://github.com/miscalculation53/library/tree/wip/ds/segtree.hpp
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
        self.assertIn("library/tree/wip/ds/segtree.hpp", cleaned)
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

    def test_anonymous_namespace_preserves_standard_specializations(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#include <limits>
template <class T> struct Number {};
namespace std {
template <class T> struct numeric_limits<::Number<T>> {
  static constexpr int digits = 7;
};
}
int unused_helper() { return 10; }
int main() { return std::numeric_limits<Number<int>>::digits - 7; }
'''
        wrapped = submit_code.insert_anonymous_namespace(source)
        self.assertIn("}\nnamespace std {", wrapped)
        self.assertEqual(submit_code.insert_anonymous_namespace(wrapped), wrapped)
        with submit_code.CompilerWorkspace(
            cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
            timeout=None, reporter=submit_code.Reporter(),
        ) as compiler:
            valid, stderr = compiler.validate(wrapped)
            self.assertTrue(valid, stderr)
            warnings = compiler.compiler_warnings(wrapped)
            self.assertIn("unused_helper", warnings)
            self.assertIn("[-Wunused-function]", warnings)

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

    def test_uninstantiated_template_methods_are_removed(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#include <bits/stdc++.h>
template <class T>
struct Box {
  T used(T x) { return x + 1; }
  T unused(T x) { return x + 2; }
  T unused_recursive(T x) { return x ? unused_recursive(x - 1) : 0; }
};
int main() {
  Box<int> box;
  return box.used(0) - 1;
}
'''
        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "bundle.cpp"
            output.write_text(source)
            writer = submit_code.CheckpointWriter(output, reporter)
            writer.current = source
            with submit_code.CompilerWorkspace(
                cxx=cxx,
                standard="c++23",
                include_paths=[],
                extra_args=[],
                timeout=None,
                reporter=reporter,
            ) as compiler:
                candidates = submit_code.uninstantiated_template_candidates(
                    source,
                    compiler=compiler,
                    keep_functions=set(),
                )
                self.assertIn("unused", {candidate.name for candidate in candidates})
                self.assertIn(
                    "unused_recursive", {candidate.name for candidate in candidates}
                )
                cleaned, accepted = submit_code.reduce_unused_candidates(
                    source,
                    candidates,
                    compiler=compiler,
                    writer=writer,
                    reporter=reporter,
                )
        self.assertGreaterEqual(accepted, 2)
        self.assertIn("T used(T x)", cleaned)
        self.assertNotIn("T unused(T x)", cleaned)
        self.assertNotIn("unused_recursive", cleaned)

    def test_uninstantiated_template_bodies_can_be_stubbed_together(self) -> None:
        source = '''template <class T>
struct Box {
  Box() : value{} {}
  T helper(T x) { return x + 1; }
  T unused(T x) { return helper(x); }
  T value;
};
int main() { return 0; }
'''
        lines = source.splitlines(keepends=True)
        tokens = submit_code.structural_tokens(lines)
        candidates = [
            submit_code.removal_from_function_line(lines, tokens, line, "unused")
            for line in (2, 3, 4)
        ]
        cleaned = submit_code.stub_uninstantiated_template_functions(
            source, [candidate for candidate in candidates if candidate is not None]
        )
        self.assertIn("T helper(T x);", cleaned)
        self.assertIn("T unused(T x);", cleaned)
        self.assertIn("Box();", cleaned)
        self.assertNotIn(": value", cleaned)
        self.assertNotIn("return helper", cleaned)

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

    def test_early_syntax_check_reuses_but_does_not_build_a_pch(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''#define VALUE_BEFORE_BITS 7
#include <bits/stdc++.h>
int main() { static_assert(VALUE_BEFORE_BITS == 7); }
'''
        with tempfile.TemporaryDirectory() as directory:
            cache = Path(directory) / "cache"
            with patch.dict(submit_code.os.environ, {"SUBMIT_CODE_CACHE_DIR": str(cache)}):
                with submit_code.CompilerWorkspace(
                    cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
                    timeout=None, reporter=submit_code.Reporter(),
                ) as compiler:
                    valid, stderr = compiler.syntax(source, build_pch=False)
                    self.assertTrue(valid, submit_code.first_error(stderr))
                    self.assertFalse(cache.exists())
                    self.assertIsNone(compiler.pch_available)
                    valid, stderr = compiler.validate(source)
                    self.assertTrue(valid, submit_code.first_error(stderr))
                    self.assertTrue(compiler.pch_output.is_file())
                with submit_code.CompilerWorkspace(
                    cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
                    timeout=None, reporter=submit_code.Reporter(),
                ) as compiler:
                    valid, stderr = compiler.syntax(source, build_pch=False)
                    self.assertTrue(valid, submit_code.first_error(stderr))
                    self.assertTrue(compiler.pch_available)

    def test_template_checkpoint_removes_unreferenced_signatures_immediately(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''template <class T>
T unused_first(T x) { return x + 1; }
template <class T>
T unused_second(T x) { return x + 2; }
int main() { return 0; }
'''
        candidates = [
            submit_code.Removal(0, 1, "unused", "unused_first"),
            submit_code.Removal(2, 3, "unused", "unused_second"),
        ]
        stubbed = submit_code.stub_uninstantiated_template_functions(source, candidates)
        target = submit_code.encoded_size(stubbed)
        saved = []

        class RecordingWriter(submit_code.CheckpointWriter):
            def save(self, stage, text, *, force=False):
                changed = super().save(stage, text, force=force)
                if changed:
                    saved.append(text)
                return changed

        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory:
            writer = RecordingWriter(Path(directory) / "bundle.cpp", reporter)
            writer.current = source
            writer.preserve_below = target
            with submit_code.CompilerWorkspace(
                cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
                timeout=None, reporter=reporter,
            ) as compiler:
                cleaned, accepted = submit_code.reduce_template_body_candidates(
                    source, candidates, compiler=compiler, writer=writer,
                    reporter=reporter, target_bytes=target,
                )
                valid, stderr = compiler.validate(cleaned)
        self.assertTrue(valid, submit_code.first_error(stderr))
        self.assertEqual(accepted, 2)
        self.assertTrue(submit_code.below_byte_limit(saved[0], target))
        self.assertNotIn("unused_first", saved[0])
        self.assertNotIn("unused_second", saved[0])

    def test_template_checkpoint_falls_back_when_a_signature_must_remain(self) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        source = '''template <class T>
requires
    (sizeof(T) > 0)
T constrained_unused(T x) { return x; }
template <class T>
T other_unused(T x) { return x; }
int main() { return 0; }
'''
        # A partial declaration boundary must not leave an orphaned constraint
        # in any checkpoint, even if there are no textual uses of the name.
        candidates = [
            submit_code.Removal(3, 3, "unused", "constrained_unused"),
            submit_code.Removal(4, 5, "unused", "other_unused"),
        ]
        reporter = submit_code.Reporter()
        with tempfile.TemporaryDirectory() as directory:
            writer = submit_code.CheckpointWriter(Path(directory) / "bundle.cpp", reporter)
            writer.current = source
            with submit_code.CompilerWorkspace(
                cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
                timeout=None, reporter=reporter,
            ) as compiler:
                cleaned, accepted = submit_code.reduce_template_body_candidates(
                    source, candidates, compiler=compiler, writer=writer,
                    reporter=reporter, target_bytes=100,
                )
                valid, stderr = compiler.validate(cleaned)
        self.assertTrue(valid, submit_code.first_error(stderr))
        self.assertGreater(accepted, 0)
        self.assertIn("constrained_unused(T x);", cleaned)

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
