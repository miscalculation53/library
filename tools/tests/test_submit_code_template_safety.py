from __future__ import annotations

import io
from pathlib import Path
import shutil
import tempfile
import unittest

import tools_test_paths

import submit_code


class TemplateSafetyTest(unittest.TestCase):
    def reduce(self, source: str, expected: int, *, forced: list[submit_code.Removal] | None = None) -> str:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        reporter = submit_code.Reporter(stream=io.StringIO())
        with tempfile.TemporaryDirectory() as directory, submit_code.CompilerWorkspace(
            cxx=cxx, standard="c++23", include_paths=[], extra_args=[],
            timeout=None, reporter=reporter,
        ) as compiler:
            valid, diagnostic = compiler.validate(source)
            self.assertTrue(valid, diagnostic)
            self.assertEqual(submit_code.run_command([str(compiler.executable)]).returncode, expected)
            candidates = forced if forced is not None else submit_code.uninstantiated_template_candidates(
                source, compiler=compiler, keep_functions=set(),
            )
            writer = submit_code.CheckpointWriter(Path(directory) / "bundle.cpp", reporter)
            writer.current = source
            cleaned, _ = submit_code.reduce_template_body_candidates(
                source, candidates, compiler=compiler, writer=writer, reporter=reporter,
            )
            valid, diagnostic = compiler.validate(cleaned)
            self.assertTrue(valid, diagnostic)
            self.assertEqual(submit_code.run_command([str(compiler.executable)]).returncode, expected)
            return cleaned

    def test_requires_keeps_the_referenced_declaration(self) -> None:
        source = '''template <class T>
int probe(T) { return 10; }
template <class T>
int pick(T x) {
  if constexpr (requires { probe(x); }) return 1;
  else return 2;
}
int main() { return pick(0); }
'''
        cleaned = self.reduce(source, 1)
        self.assertIn("int probe(T);", cleaned)

    def test_constructor_and_conversion_keep_type_traits(self) -> None:
        source = '''#include <type_traits>
template <class T>
struct Box {
  Box() {}
  operator int() { return 1; }
};
int main() {
  return std::is_trivially_default_constructible_v<Box<int>> +
         !std::is_convertible_v<Box<int>, int>;
}
'''
        cleaned = self.reduce(source, 0)
        self.assertIn("Box() {}", cleaned)
        self.assertIn("operator int() { return 1; }", cleaned)

    def test_auto_and_constexpr_bodies_are_kept_when_referenced(self) -> None:
        source = '''#include <type_traits>
template <class T>
auto deduced(T) { return 1; }
template <class T>
constexpr int constant(T) { return 7; }
template <class T>
int pick(T x) {
  if constexpr (requires { typename std::integral_constant<int, constant(T{})>; })
    return int(sizeof(decltype(deduced(x))));
  else return 99;
}
int main() { return pick(0); }
'''
        candidates = [submit_code.Removal(1, 2, "template", "deduced"),
                      submit_code.Removal(3, 4, "template", "constant")]
        cleaned = self.reduce(source, 4, forced=candidates)
        self.assertIn("auto deduced(T) { return 1; }", cleaned)
        self.assertIn("constexpr int constant(T) { return 7; }", cleaned)

    def test_virtual_functions_keep_polymorphism(self) -> None:
        source = '''#include <type_traits>
template <class T>
struct Box {
  virtual int value() { return 7; }
};
int main() { return !std::is_polymorphic_v<Box<int>>; }
'''
        cleaned = self.reduce(source, 0)
        self.assertIn("virtual int value() { return 7; }", cleaned)

    def test_macros_and_trailing_auto_keep_compile_time_bodies(self) -> None:
        source = '''#include <type_traits>
#define CE constexpr
template <class T>
CE int constant(T) { return 7; }
template <class T>
auto deduced(T) -> auto { return 1; }
template <class T>
int pick(T x) {
  if constexpr (requires {
    typename std::integral_constant<int, constant(T{})>;
    deduced(x);
  }) return 4;
  else return 99;
}
int main() { return pick(0); }
'''
        candidates = [submit_code.Removal(2, 3, "template", "constant"),
                      submit_code.Removal(4, 5, "template", "deduced")]
        cleaned = self.reduce(source, 4, forced=candidates)
        self.assertIn("CE int constant(T) { return 7; }", cleaned)
        self.assertIn("auto deduced(T) -> auto { return 1; }", cleaned)

    def test_external_qualifier_macros_keep_compile_time_bodies(self) -> None:
        source = '''#include <type_traits>
template <class T>
_GLIBCXX_CONSTEXPR int constant(T) { return 7; }
template <class T>
int pick(T) {
  if constexpr (requires { typename std::integral_constant<int, constant(T{})>; }) return 4;
  else return 99;
}
int main() { return pick(0); }
'''
        cleaned = self.reduce(source, 4, forced=[submit_code.Removal(1, 2, "template", "constant")])
        self.assertIn("_GLIBCXX_CONSTEXPR int constant(T) { return 7; }", cleaned)

    def test_type_alias_macros_allow_a_body_stub(self) -> None:
        source = '''#define TYPE int
template <class T>
TYPE probe(T) { return 7; }
template <class T>
int pick(T x) {
  if constexpr (requires { probe(x); }) return 4;
  else return 99;
}
int main() { return pick(0); }
'''
        cleaned = self.reduce(source, 4, forced=[submit_code.Removal(1, 2, "template", "probe")])
        self.assertIn("TYPE probe(T);", cleaned)

    def test_semantic_macro_dependencies_are_protected(self) -> None:
        source = '''#define TYPE vector
#define CE constexpr
#define QUAL CE
#define CAT(a,b) a ## b
#define UNKNOWN __VA_OPT__(auto)
'''
        protected = submit_code.template_signature_macro_names(source, {"EXT": "constexpr"})
        self.assertTrue({"CE", "QUAL", "CAT", "UNKNOWN", "EXT"}.issubset(protected))
        self.assertNotIn("TYPE", protected)

    def test_member_declarations_used_inside_system_concepts_are_kept(self) -> None:
        source = '''#include <ranges>
namespace sample {
template <class T>
struct Range {
  int *begin() { return nullptr; }
  int *end() { return nullptr; }
};
}
int main() { return !std::ranges::range<sample::Range<int>>; }
'''
        candidates = [submit_code.Removal(4, 4, "template", "begin"),
                      submit_code.Removal(5, 5, "template", "end")]
        cleaned = self.reduce(source, 0, forced=candidates)
        self.assertIn("int *begin()", cleaned)
        self.assertIn("int *end()", cleaned)

    def test_unreachable_function_group_can_be_deleted(self) -> None:
        source = '''template <class T>
int first(T x) { return second(x); }
template <class T>
int second(T x) { return first(x); }
template <class T>
int unused(T x) { return x + 1; }
int main() { return 0; }
'''
        cleaned = self.reduce(source, 0)
        self.assertNotIn("int first(", cleaned)
        self.assertNotIn("int second(", cleaned)
        self.assertNotIn("int unused(", cleaned)

    def test_adl_declarations_used_inside_system_concepts_are_kept(self) -> None:
        source = '''#include <ranges>
namespace sample {
template <class T> struct Range {};
template <class T>
int *begin(Range<T> &) { return nullptr; }
template <class T>
int *end(Range<T> &) { return nullptr; }
}
int main() { return !std::ranges::range<sample::Range<int>>; }
'''
        candidates = [submit_code.Removal(3, 4, "template", "begin"),
                      submit_code.Removal(5, 6, "template", "end")]
        cleaned = self.reduce(source, 0, forced=candidates)
        self.assertIn("int *begin(Range<T> &)", cleaned)
        self.assertIn("int *end(Range<T> &)", cleaned)

    def test_shared_line_after_a_function_is_preserved(self) -> None:
        source = '''template <class T>
int unused(T) { return 1; } int side_effect = 7;
int main() { return side_effect; }
'''
        cleaned = self.reduce(source, 7)
        self.assertIn("int unused(T) { return 1; } int side_effect = 7;", cleaned)

    def test_pasted_names_keep_the_declaration(self) -> None:
        source = '''#define JOIN(a, b) a ## b
template <class T>
int probe(T) { return 10; }
template <class T>
int pick(T x) {
  if constexpr (requires { JOIN(pro, be)(x); }) return 1;
  else return 2;
}
int main() { return pick(0); }
'''
        cleaned = self.reduce(source, 1)
        self.assertIn("int probe(T);", cleaned)

    def test_counter_consumption_in_an_unused_function_is_kept(self) -> None:
        source = '''template <class T>
int unused(T) { return __COUNTER__; }
int main() { return __COUNTER__; }
'''
        cleaned = self.reduce(source, 1)
        self.assertEqual(source, cleaned)

    def test_portable_dependencies_include_functions_outside_the_candidate_set(self) -> None:
        source = '''template <class T>
int deep(T) { return 7; }
template <class T>
int helper(T x) { return deep(x); }
#if defined(__STDCPP_FLOAT128_T__)
int target() { return helper(0); }
#endif
int main() { return 0; }
'''
        candidate = submit_code.Removal(0, 1, "template", "deep")
        protected = submit_code.portable_body_dependencies(source, [candidate])
        self.assertIn("deep", protected)
        self.assertEqual(submit_code.removable_template_body_candidates(source, [candidate]), [])
        self.assertEqual(submit_code.stub_uninstantiated_template_functions(source, [candidate]), source)

    def test_portable_dependencies_use_only_the_actual_function_name(self) -> None:
        source = '''template <class T>
auto unrelated(T x) -> decltype(x.size()) { return unrelated_dependency(x); }
#if defined(__STDCPP_FLOAT128_T__)
int target() { return size(0); }
#endif
int main() { return 0; }
'''
        protected = submit_code.portable_body_dependencies(source, [])
        self.assertIn("size", protected)
        self.assertNotIn("unrelated_dependency", protected)


if __name__ == "__main__":
    unittest.main()
