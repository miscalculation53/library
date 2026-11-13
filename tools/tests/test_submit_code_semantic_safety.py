from pathlib import Path
import io
import shutil
import tempfile
import unittest
from unittest.mock import patch

import tools_test_paths
import submit_code as s


class SemanticSafetyTest(unittest.TestCase):
    def setUp(self):
        self.cxx = shutil.which('g++-15') or shutil.which('g++-14')
        if not self.cxx:
            self.skipTest('GNU C++ is unavailable')
        self.reporter = s.Reporter(stream=io.StringIO())

    def compiler(self):
        return s.CompilerWorkspace(cxx=self.cxx, standard='c++23', include_paths=[],
                                   extra_args=[], timeout=None, reporter=self.reporter)

    def run_text(self, compiler, text):
        valid, error = compiler.validate(text)
        self.assertTrue(valid, error)
        result = s.run_command([str(compiler.executable)])
        return result.returncode, result.stdout

    def test_macros_used_by_headers_or_token_pasting_are_preserved(self):
        sources = [
            '#define NDEBUG\n#include <cassert>\nint main(){ assert(false); }\n',
            '#include <cstdio>\n#define puts(s) std::printf("changed\\n")\n'
            '#define CAT(a,b) a##b\nint main(){ CAT(pu,ts)("original"); }\n',
        ]
        for source in sources:
            with self.subTest(source=source), self.compiler() as compiler:
                before = self.run_text(compiler, source)
                candidates = s.unused_candidates(source, compiler=compiler, cppcheck=None,
                    keep_functions=set(), reporter=self.reporter)
                after = self.run_text(compiler, s.apply_removals(source, candidates))
                self.assertEqual(before, after)

    def test_fast_bundle_preserves_assert_header_reinclusion(self):
        source = '#define NDEBUG\n#include <cassert>\n#undef NDEBUG\n#include <cassert>\nint main(){ return NDEBUG; }\n'
        source = source.replace('return NDEBUG;', '#if defined(NDEBUG)\nreturn 1;\n#else\nreturn 0;\n#endif\n')
        source = source.replace('int main(){ #if', 'int main(){\n#if')
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'main.cpp'
            path.write_text(source)
            bundled = s.FastBundler([path.parent]).bundle(path)
        self.assertEqual(bundled.count('#include <cassert>'), 2)
        self.assertIn('#undef NDEBUG', bundled)

    def test_include_guard_macro_state_and_reinclusion_are_preserved(self):
        with tempfile.TemporaryDirectory() as directory, self.compiler() as compiler:
            root = Path(directory)
            (root / 'header.hpp').write_text('#ifndef GUARD\n#define GUARD\n#define VALUE 7\n#endif\n')
            source = root / 'main.cpp'
            source.write_text('#include "header.hpp"\n#ifndef GUARD\n#error missing guard\n#endif\n'
                              '#undef GUARD\n#undef VALUE\n#include "header.hpp"\nint main(){return VALUE-7;}\n')
            bundled = s.FastBundler([root]).bundle(source)
            self.assertEqual(self.run_text(compiler, bundled), (0, ''))
            self.assertEqual(bundled.count('#define GUARD'), 2)

    def test_conditional_pragma_once_uses_compiler_fallback(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'header.hpp').write_text('#if ENABLED\n#pragma once\n#endif\n')
            source = root / 'main.cpp'
            source.write_text('#include "header.hpp"\nint main(){}\n')
            with self.assertRaises(s.FastBundleUnsupported):
                s.FastBundler([root]).bundle(source)

    def test_shared_function_line_and_counter_effect_are_preserved(self):
        source = 'int unused(){return 0;} int used(){return 1;}\nint main(){return used();}\n'
        candidate = s.Removal(0, 0, 'unused function')
        self.assertEqual(s.unreferenced_function_candidates(source, [candidate], set()), [])
        source = 'struct Counter { enum { value = __COUNTER__ }; };\nint main(){return __COUNTER__; }\n'
        self.assertEqual(s.unreferenced_declaration_candidates(source), [])

    def test_spliced_identifier_still_references_its_declaration(self):
        source = 'struct Widget {};\nWi\\\r\ndget value;\nint main(){}\n'
        self.assertEqual(s.unreferenced_declaration_candidates(source), [])
        self.assertEqual(list(s.code_identifiers('// ignored \\\r\nWidget\nint value;')), ['int', 'value'])

    def test_pure_unused_template_type_can_be_removed(self):
        source = 'int effect();\ntemplate<class T>\nstruct Dormant { inline static int value = effect(); };\nint main(){}\n'
        self.assertEqual({x.name for x in s.unreferenced_declaration_candidates(source)}, {'Dormant', 'effect'})
        source += 'Dormant<int> instance;\n'
        self.assertEqual(s.unreferenced_declaration_candidates(source), [])

    def test_unused_operator_is_removed_with_its_unreachable_fixed_type(self):
        source = 'struct Tag {};\ntemplate<class T>\nint operator|(T, Tag){return 3;}\nint main(){}\n'
        candidates = s.unreferenced_declaration_candidates(source)
        self.assertEqual(len(candidates), 2)
        self.assertNotIn('Tag', s.apply_removals(source, candidates))
        self.assertEqual(s.unreferenced_declaration_candidates(source.replace('int main(){}', 'int main(){return 1 | Tag{};}')), [])
        converting = source.replace('struct Tag {};', 'struct Tag { Tag(int) {} };')
        self.assertEqual(s.unreferenced_declaration_candidates(converting), [])

    def test_multiline_static_data_initializer_is_preserved(self):
        source = 'int effect();\nstruct Init {\n static inline\n int value = effect();\n};\nint main(){}\n'
        self.assertEqual(s.unreferenced_declaration_candidates(source), [])

    def test_header_pruning_preserves_global_initialization(self):
        with tempfile.TemporaryDirectory() as directory, self.compiler() as compiler:
            root = Path(directory)
            source = root / 'main.cpp'
            header = root / 'effect.hpp'
            resolver = s.OriginResolver(base_dir=root, source=source, include_paths=[root])
            bundled = f'#line 1 {s.json.dumps(str(source))}\n#include <cstdio>\n'
            bundled += f'#line 1 {s.json.dumps(str(header))}\ninline int startup=std::puts("startup");\n'
            bundled += f'#line 2 {s.json.dumps(str(source))}\nint main(){{}}\n'
            document = s.BundleDocument.parse(bundled, resolver=resolver)
            writer = s.CheckpointWriter(root / 'bundle.cpp', self.reporter)
            initial = document.render(set(), keep_line=False)
            writer.save('initial', initial)
            before = self.run_text(compiler, initial)
            removed = s.prune_header_origins(document, compiler=compiler, writer=writer,
                keep_patterns=[], keep_line=False, max_passes=1, reporter=self.reporter)
            self.assertNotIn(str(header), removed)
            self.assertEqual(before, self.run_text(compiler, writer.path.read_text()))

    def test_explicit_line_number_survives_fast_expansion(self):
        with tempfile.TemporaryDirectory() as directory, self.compiler() as compiler:
            root = Path(directory)
            source = root / 'main.cpp'
            source.write_text('#line 500 "logical.cpp"\nint main(){return __LINE__ != 500;}\n')
            bundled = s.FastBundler([root]).bundle(source)
            self.assertEqual(self.run_text(compiler, bundled), (0, ''))

    def test_unused_macro_warnings_follow_compacted_line_numbers(self):
        source = '#line 4\n#define UNUSED_FIRST 1\nint main(){}\n#define UNUSED_LAST 2\n'
        with self.compiler() as compiler:
            self.assertEqual(sorted(compiler.unused_macro_locations(source)), [1, 3])
            removals = s.unused_candidates(source, compiler=compiler, cppcheck=None,
                                          keep_functions=set(), reporter=self.reporter)
        self.assertEqual([(item.start, item.end) for item in removals], [(1, 1), (3, 3)])

    def test_ambiguous_warning_line_numbers_are_skipped(self):
        source = '#line 42\n#define FIRST 1\n#line 42\n#define SECOND 2\nint main(){}\n'
        self.assertEqual(s.warning_line_indices(source), {43: 4})
        conditional = '#if 1\n#line 42\n#define FIRST 1\n#endif\n'
        self.assertEqual(s.warning_line_indices(conditional), {})
        continued = '#line 42 /* first\nsecond */\n#define FIRST 1\n'
        self.assertEqual(s.warning_line_indices(continued), {})

    def test_validation_checks_the_other_retained_float128_branch(self):
        probe = '#if defined(__STDCPP_FLOAT128_T__)\nint flag=1;\n#endif\nint main(){}\n'
        with self.compiler() as compiler:
            compiler.retain_portable_target(probe)
            self.assertIsNotNone(compiler.portable_workspace)
            native = compiler.portable_workspace.force_float128_macro
            condition = 'defined(__STDCPP_FLOAT128_T__)' if native else '!defined(__STDCPP_FLOAT128_T__)'
            helper = 'int alternate_helper(){return 7;}\n'
            source = helper + '#if ' + condition + '\nint selected(){return alternate_helper();}\n#else\nint selected(){return 0;}\n#endif\nint main(){return selected();}\n'
            self.assertTrue(compiler.validate(source)[0])
            candidate = source.replace(helper, '')
            self.assertTrue(compiler.link(candidate)[0])
            self.assertFalse(compiler.validate(candidate)[0])
            self.assertFalse(compiler.validate(candidate, parallel_portable=True)[0])
            self.assertTrue(compiler.validate(source, parallel_portable=True)[0])

    def test_validation_checks_dependencies_of_unused_inline_and_static_functions(self):
        for declaration in ['inline int retained(){return missing();}',
                            'static int retained(){return missing();}']:
            with self.subTest(declaration=declaration), self.compiler() as compiler:
                source = 'int missing();\n' + declaration + '\nint main(){}\n'
                self.assertFalse(compiler.validate(source)[0])
                self.assertTrue(compiler.validate('int missing(){return 3;}\n' + declaration + '\nint main(){}\n')[0])

    def test_cached_link_restores_the_requested_executable(self):
        with self.compiler() as compiler:
            original = 'int main(){return 3;}\n'
            self.assertEqual(self.run_text(compiler, original)[0], 3)
            self.assertEqual(self.run_text(compiler, 'int main(){return 7;}\n')[0], 7)
            self.assertEqual(self.run_text(compiler, original)[0], 3)
            self.assertFalse(compiler.validate('int absent();\nint main(){return absent();}\n')[0])
            self.assertEqual(self.run_text(compiler, original)[0], 3)

    def test_cleanup_reuses_ipa_only_after_success_on_unchanged_text(self):
        main = 'int main(){return 0;}\n'
        dormant = 'int dormant(){return 8;}\n'
        cases = [(main, [], 1), (main, None, 2), (dormant + main, [(1, 'int dormant()')], 2)]
        for source, first_analysis, expected_calls in cases:
            with self.subTest(first_analysis=first_analysis), tempfile.TemporaryDirectory() as directory, self.compiler() as compiler:
                writer = s.CheckpointWriter(Path(directory) / 'bundle.cpp', self.reporter)
                writer.current = source
                with patch.object(compiler, '_analyze_ipa', side_effect=[first_analysis, []]) as analyze:
                    result = s.cleanup_text(source, compiler=compiler, writer=writer, cppcheck=None,
                        keep_functions=set(), max_passes=0, safe_only=False, require_validation=True,
                        reporter=self.reporter)
                self.assertNotIn('dormant', result)
                self.assertEqual(analyze.call_count, expected_calls)
                if first_analysis:
                    self.assertNotIn('dormant', analyze.call_args.args[0])

    def test_validation_rejects_missing_inline_definitions_after_an_early_return(self):
        source = 'inline int missing();\ntemplate<class T> int pick(){if constexpr(sizeof(T)==sizeof(int)) return 0; return missing();}\nint main(){return pick<int>();}\n'
        with self.compiler() as compiler:
            self.assertFalse(compiler.validate(source)[0])
            self.assertTrue(compiler.validate('inline int missing(){return 7;}\n' + source)[0])

    def test_validation_preserves_instantiated_variable_initialization(self):
        definitions = [
            ('template<class T> struct X {inline static int value=std::puts("startup");};\n', 'X<int>::value'),
            ('template<class T> inline int value=std::puts("startup");\n', 'value<int>'),
        ]
        for definition, expression in definitions:
            with self.subTest(expression=expression), self.compiler() as compiler:
                source = '#include <cstdio>\n' + definition
                body = 'int dormant(){return ' + expression + ';}\n'
                original = source + body + 'int main(){}\n'
                self.assertEqual(self.run_text(compiler, original)[1], 'startup\n')
                for replacement in ['', 'int dormant();\n']:
                    self.assertFalse(compiler.validate(source + replacement + 'int main(){}\n')[0])
                self.assertTrue(compiler.validate(original + 'int unrelated(){return 5;}\n')[0])

    def test_type_with_startup_attributes_is_preserved(self):
        for attribute in ['__attribute__((constructor))', '[[gnu::constructor]]']:
            with self.subTest(attribute=attribute), self.compiler() as compiler:
                source = '#include <cstdio>\nstruct Registration {\n'
                source += attribute + ' static void start(){std::puts("startup");}\n};\nint main(){}\n'
                self.assertEqual(self.run_text(compiler, source)[1], 'startup\n')
                self.assertEqual(s.unreferenced_declaration_candidates(source), [])

    def test_overloaded_unused_bodies_are_refined_individually(self):
        source = 'template<class T>\nint pick(T x){return 3;}\n'
        source += 'template<class T>\nint pick(T *x){return 7;}\n'
        source += 'template<class T>\nint unused(T x){return 11;}\n'
        source += 'int main(){return pick(0);}\n'
        with tempfile.TemporaryDirectory() as directory, self.compiler() as compiler:
            writer = s.CheckpointWriter(Path(directory) / 'bundle.cpp', self.reporter)
            writer.current = source
            candidates = [s.Removal(0, 1, 'body', 'pick'), s.Removal(2, 3, 'body', 'pick'),
                          s.Removal(4, 5, 'body', 'unused')]
            cleaned, _ = s.reduce_template_body_candidates(source, candidates, compiler=compiler,
                writer=writer, reporter=self.reporter)
            self.assertEqual(self.run_text(compiler, cleaned)[0], 3)
            self.assertIn('int pick(T *x);', cleaned)
            self.assertNotIn('return 7;', cleaned)

    def test_unused_operator_body_preserves_lookup_and_its_declaration(self):
        source = 'struct Tag {};\ntemplate<class T>\nint operator<<(Tag, T){return 3;}\n'
        source += 'template<class T>\nint pick(T x){if constexpr(requires { Tag{} << x; }) return 1; else return 2;}\n'
        source += 'int main(){return pick(0);}\n'
        candidate = s.Removal(1, 2, 'body', 'operator<<')
        with self.compiler() as compiler:
            self.assertEqual(self.run_text(compiler, source)[0], 1)
            self.assertEqual(s.unreferenced_template_declarations(source, [candidate]), set())
            cleaned = s.stub_uninstantiated_template_functions(source, [candidate], deleted={candidate})
            self.assertIn('int operator<<(Tag, T);', cleaned)
            self.assertNotIn('return 3;', cleaned)
            self.assertEqual(self.run_text(compiler, cleaned)[0], 1)
            used = cleaned.replace('return pick(0);', 'return Tag{} << 0;')
            self.assertFalse(compiler.validate(used)[0])
        self.assertFalse(s.template_body_can_be_stubbed('operator int(){return 0;}', 'operatorint'))
        self.assertFalse(s.template_body_can_be_stubbed('Tag& operator=(const Tag&){return *this;}', 'operator='))

    def test_portable_operator_calls_keep_their_bodies_through_helpers(self):
        source = 'struct Tag {};\nint operator<<(Tag, int){return 3;}\n'
        source += 'int helper(){return Tag{} << 0;}\n'
        source += '#if defined(__STDCPP_FLOAT128_T__)\nint selected(){return helper();}\n#endif\nint main(){}\n'
        candidate = s.Removal(1, 1, 'body', 'operator<<')
        self.assertIn('operator<<', s.portable_body_dependencies(source, [candidate]))
        self.assertEqual(s.stub_uninstantiated_template_functions(source, [candidate]), source)

    def test_optimization_attributes_are_preserved_on_safe_body_stubs(self):
        source = 'template<class T>\n__attribute__((always_inline)) inline int dormant(T x){return x;}\nint main(){}\n'
        candidate = s.Removal(0, 1, 'template', 'dormant')
        stubbed = s.stub_uninstantiated_template_functions(source, [candidate])
        self.assertIn('__attribute__((always_inline)) inline int dormant(T x);', stubbed)
        self.assertNotIn('return x', stubbed)
        with self.compiler() as compiler:
            self.assertTrue(compiler.validate(stubbed)[0])

    def test_constant_feature_guard_does_not_protect_unreachable_bodies(self):
        source = '#define LIBRARY_HAS_FLOAT128 1\n#if LIBRARY_HAS_FLOAT128\nint dormant(){return 0;}\n#endif\nint main(){}\n'
        self.assertNotIn('dormant', s.portable_body_dependencies(source, []))
        self.assertIn('dormant', s.portable_body_dependencies(source, [], {'LIBRARY_HAS_FLOAT128'}))
        conditional = '#if ENABLED\n#define LIBRARY_HAS_FLOAT128 1\n#endif\n' + source.split('\n', 1)[1]
        self.assertIn('dormant', s.portable_body_dependencies(conditional, []))
