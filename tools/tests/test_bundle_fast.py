from pathlib import Path
import hashlib
import io
import json
import shutil
import tempfile
import unittest
from unittest.mock import patch

import tools_test_paths
import submit_code as s
import bundle_fast as f


class FastFeatureTest(unittest.TestCase):
    def setUp(self):
        self.cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if not self.cxx:
            self.skipTest("GNU C++ is unavailable")
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.token = "a" * 32
        self.reporter = s.Reporter(stream=io.StringIO())

    def marked(self, body):
        return f.marker(self.token, True, "math") + "\n" + body + f.marker(self.token, False, "math") + "\n"

    def compiler(self):
        return s.CompilerWorkspace(cxx=self.cxx, standard="c++23", include_paths=[self.root],
                                   extra_args=[], timeout=None, reporter=self.reporter)

    def reduce(self, compiler, source):
        writer = s.CheckpointWriter(self.root / "bundle.cpp", self.reporter)
        writer.current = source
        return f.try_cleanup(s, source, compiler=compiler, writer=writer, reporter=self.reporter,
                             target_bytes=65536, feature_token=self.token)

    def test_unused_constexpr_body_keeps_the_declaration(self):
        source = self.marked("template<class T>\nconstexpr int dormant(T x){return x+1;}\n") + "int main(){return 0;}\n"
        with patch.dict(s.os.environ, {"SUBMIT_CODE_CACHE_DIR": str(self.root / "cache")}), self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertNotIn("return x+1", result)
            self.assertEqual(compiler.object_signature(source), compiler.object_signature(result))

    def test_requires_with_deduced_return_keeps_its_body(self):
        source = self.marked("template<class T>\nauto probe(T x){return x;}\n")
        source += "int main(){return requires { probe(0); };}\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertTrue(result is None or "return x;" in result)

    def test_unreferenced_library_method_is_removed_with_its_declaration(self):
        source = self.marked("struct Box {\n  int value() const {return 7;}\n"
                             "  int dormant_method() const {return 9;}\n};\n")
        source += "int main(){return Box{}.value() != 7;}\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertIn("value()", result)
            self.assertNotIn("dormant_method", result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_method_needed_by_a_requires_expression_keeps_its_declaration(self):
        source = self.marked("struct Box {\n  int probe() const {return 9;}\n};\n"
                             "template<class T> int dormant(T x){return x+1;}\n")
        source += "int main(){return !requires(Box x){x.probe();};}\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertIn("probe()", result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def prune_declarations(self, compiler, source):
        writer = s.CheckpointWriter(self.root / "bundle.cpp", self.reporter)
        writer.current = source
        return f.prune_function_declarations(s, source, compiler=compiler, writer=writer,
            reporter=self.reporter, keep_functions=s.DEFAULT_KEEP_FUNCTIONS)

    def test_unused_overload_declaration_is_removed_even_when_the_name_is_used(self):
        source = "struct Box {\n  int value(int x) const {return x;}\n  int value(double x) const;\n};\n"
        source += "int main(){return Box{}.value(0);}\n"
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertNotIn("value(double", result)
            self.assertIn("value(int", result)
            self.assertEqual(compiler.submission_object_signature(source), compiler.submission_object_signature(result))

    def test_declaration_pruning_preserves_type_detection_and_removes_other_methods(self):
        source = "struct Box {\n  int probe() const;\n  int dormant() const;\n};\n"
        source += "template<class T> constexpr bool has_probe=requires(T x){x.probe();};\n"
        source += "int main(){return !has_probe<Box>;}\n"
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertIn("probe()", result)
            self.assertNotIn("dormant()", result)
            self.assertEqual(compiler.submission_object_signature(source), compiler.submission_object_signature(result))

    def test_declaration_pruning_keeps_special_and_virtual_members(self):
        source = "struct Box {\n  Box();\n  ~Box();\n  virtual int value() const;\n  int dormant() const;\n};\n"
        source += "int main(){return 0;}\n"
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertIn("Box();", result)
            self.assertIn("~Box();", result)
            self.assertIn("virtual int value()", result)
            self.assertNotIn("dormant()", result)

    def test_function_candidates_skip_constructor_local_definitions(self):
        source = "struct Box {\n  Box() {\n    struct Local {\n      int invoke(int x) {return x;}\n    };\n  }\n};\n"
        candidates = f.definition_candidates(s, source, ["math"] * len(source.splitlines()))
        self.assertNotIn("invoke", [item.name for item in candidates])

    def test_compaction_preserves_coordinates_across_inactive_branches(self):
        source = "#if 0\n" + "\n" * 12 + "int inactive;\n#else\nint active=__LINE__;\n#endif\n"
        result = f.compact_lines(s, source)
        with self.compiler() as compiler:
            self.assertEqual(compiler.preprocessed_code_fingerprint(source), compiler.preprocessed_code_fingerprint(result))

    def test_preprocessed_fingerprint_preserves_code_coordinates_and_raw_string_content(self):
        with self.compiler() as compiler:
            original = 'const char* s=R"x(a\n\nb)x";\nint main(){return s[0];}\n'
            changed = original.replace('a\n\nb', 'a\nb')
            self.assertNotEqual(compiler.preprocessed_code_fingerprint(original), compiler.preprocessed_code_fingerprint(changed))
            a = "int value;\nint main(){return value;}\n"
            b = "\n" + a
            self.assertNotEqual(compiler.preprocessed_code_fingerprint(a), compiler.preprocessed_code_fingerprint(b))
            self.assertEqual(compiler.preprocessed_code_fingerprint(a), compiler.preprocessed_code_fingerprint("#line 1\n" + a))

    def test_declaration_pruning_preserves_global_initialization(self):
        source = "int calls;\nint initialize(){return ++calls;}\nint value=initialize();\n"
        source += "struct Box {\n  int dormant() const;\n};\nint main(){return calls!=1;}\n"
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertIn("value=initialize()", result)
            self.assertNotIn("dormant()", result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_priority_checkpoint_reuses_the_equivalent_compilation(self):
        source = self.marked('#define UNUSED_PADDING "' + 'x' * 140000 + '"\n')
        source += 'int main(){return 0;}\n'
        first_validation_counts = []
        with self.compiler() as compiler:
            writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
            writer.current = source
            writer.preserve_below = 65536
            save = writer.save
            with patch.object(compiler, 'validate_submission', wraps=compiler.validate_submission) as validate:
                def record(stage, value, **options):
                    changed = save(stage, value, **options)
                    if changed and not first_validation_counts:
                        first_validation_counts.append(validate.call_count)
                    return changed
                with patch.object(writer, 'save', side_effect=record):
                    result = f.try_cleanup(s, source, compiler=compiler, writer=writer,
                        reporter=self.reporter, target_bytes=65536, priority_bytes=131072,
                        feature_token=self.token)
            self.assertIsNotNone(result)
            self.assertEqual(first_validation_counts, [1])
            self.assertNotIn('UNUSED_PADDING', result)

    def test_submission_pch_tracks_dependencies_and_preserves_source_lines(self):
        header = self.root / 'headers/bits/stdc++.h'
        header.parent.mkdir(parents=True)
        header.write_text('inline int header_value(){return 1;}\n')
        environment = {'CPLUS_INCLUDE_PATH': str(self.root / 'headers'),
                       'SUBMIT_CODE_CACHE_DIR': str(self.root / 'pch-cache')}
        source = '#define BEFORE_BITS 1\n#include <bits/stdc++.h>\nint main(){return __LINE__==3 ? header_value() : 9;}\n'
        with patch.dict(s.os.environ, environment):
            with self.compiler() as compiler:
                self.assertTrue(compiler.validate_submission(source, build_pch=True)[0])
                workspace = compiler.submission_workspace
                self.assertTrue(workspace.pch_available)
                first_key = workspace.pch_key
                self.assertIn(str(header.resolve()), workspace.object_cache[hashlib.sha256(source.encode()).hexdigest()][1])
                self.assertEqual(s.run_command([str(workspace.executable)]).returncode, 1)
            header.write_text('inline int header_value(){return 2;}\n')
            source = '// same preamble\n' + source.replace('__LINE__==3', '__LINE__==4')
            with self.compiler() as compiler:
                self.assertTrue(compiler.validate_submission(source, build_pch=True)[0])
                workspace = compiler.submission_workspace
                self.assertEqual(workspace.pch_key, first_key)
                self.assertEqual(s.run_command([str(workspace.executable)]).returncode, 2)

    def test_submission_and_conservative_pch_use_their_own_optimization_modes(self):
        header = self.root / 'headers/bits/stdc++.h'
        header.parent.mkdir(parents=True)
        header.write_text('inline int mode(){\n#ifdef __OPTIMIZE__\nreturn 1;\n#else\nreturn 0;\n#endif\n}\n')
        source = '#include <bits/stdc++.h>\nint main(){return mode();}\n'
        with patch.dict(s.os.environ, {'CPLUS_INCLUDE_PATH': str(self.root / 'headers'),
                                       'SUBMIT_CODE_CACHE_DIR': str(self.root / 'pch-cache')}), self.compiler() as compiler:
            self.assertTrue(compiler.validate(source)[0])
            self.assertEqual(s.run_command([str(compiler.executable)]).returncode, 0)
            self.assertTrue(compiler.validate_submission(source, build_pch=True)[0])
            self.assertEqual(s.run_command([str(compiler.submission_workspace.executable)]).returncode, 1)
            self.assertNotEqual(compiler.pch_key, compiler.submission_workspace.pch_key)

    def test_pch_reuses_equivalent_active_preamble_macros(self):
        header = self.root / 'headers/bits/stdc++.h'
        header.parent.mkdir(parents=True)
        header.write_text('inline int value(){return MODE;}\n')
        a = '#ifndef LOCAL\n#define MODE 7\n#endif\n#include <bits/stdc++.h>\nint main(){return value();}\n'
        b = '#define MODE 7\n#line 4\n#include <bits/stdc++.h>\nint main(){return value();}\n'
        with patch.dict(s.os.environ, {'CPLUS_INCLUDE_PATH': str(self.root / 'headers'),
                                       'SUBMIT_CODE_CACHE_DIR': str(self.root / 'pch-cache')}), self.compiler() as compiler:
            self.assertTrue(compiler.validate_submission(a, build_pch=True)[0])
            first_key = compiler.submission_workspace.pch_key
            self.assertTrue(compiler.validate_submission(b, build_pch=True)[0])
            self.assertEqual(first_key, compiler.submission_workspace.pch_key)
            self.assertEqual(s.run_command([str(compiler.submission_workspace.executable)]).returncode, 7)

    def test_failed_pch_validation_retries_the_unmodified_source(self):
        header = self.root / 'headers/bits/stdc++.h'
        header.parent.mkdir(parents=True)
        header.write_text('inline int value(){return 7;}\n')
        source = '#include <bits/stdc++.h>\nint main(){return value();}\n'
        execute = s.CompilerWorkspace._run
        for conservative in (False, True):
            failed = False
            def run(workspace, command):
                nonlocal failed
                if '-include' in command and not failed:
                    failed = True
                    return s.subprocess.CompletedProcess(command, 1, '', 'internal compiler error')
                return execute(workspace, command)
            with self.subTest(conservative=conservative), patch.dict(s.os.environ, {
                    'CPLUS_INCLUDE_PATH': str(self.root / 'headers'),
                    'SUBMIT_CODE_CACHE_DIR': str(self.root / f'pch-retry-{conservative}')
                }), self.compiler() as compiler, patch.object(s.CompilerWorkspace, '_run', run):
                valid, diagnostic = compiler.validate(source) if conservative else compiler.validate_submission(source, build_pch=True)
                self.assertTrue(valid, diagnostic)
                workspace = compiler if conservative else compiler.submission_workspace
                self.assertTrue(failed)
                self.assertFalse(workspace.pch_available)
                self.assertEqual(s.run_command([str(workspace.executable)]).returncode, 7)

    def test_initialization_is_checked_in_each_precompilation_mode(self):
        header = self.root / 'headers/bits/stdc++.h'
        header.parent.mkdir(parents=True)
        header.write_text('struct Guard {~Guard(){}};\ninline void dormant(){static Guard g;}\n')
        source = '#include <bits/stdc++.h>\nint initialize(int x){return x;}\nint state=initialize(1);\nint main(){return state!=1;}\n'
        for first_pch in (False, True):
            with self.subTest(first_pch=first_pch), patch.dict(s.os.environ, {
                    'CPLUS_INCLUDE_PATH': str(self.root / 'headers'),
                    'SUBMIT_CODE_CACHE_DIR': str(self.root / f'pch-init-{first_pch}')
                }), self.compiler() as compiler:
                self.assertTrue(compiler.validate_submission(source, build_pch=first_pch)[0])
                if first_pch:
                    compiler.submission_workspace.pch_disabled = True
                self.assertTrue(compiler.validate_submission(source + '\n', build_pch=not first_pch)[0])
                self.assertEqual(set(compiler.submission_workspace.initialization_references), {False, True})
                valid, diagnostic = compiler.validate_submission(source.replace('state=initialize(1)', 'state=initialize(2)'))
                self.assertFalse(valid)
                self.assertIn('static initialization or destruction changed', diagnostic)

    def test_executable_analysis_preserves_registered_initializers(self):
        source = self.marked('int calls;\n__attribute__((constructor)) static void initialize(){++calls;}\n'
                             'template<class T> int dormant(T x){return x+1;}\n')
        source += 'int main(){return calls!=1;}\n'
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertTrue(compiler.submission_workspace.whole_program)
            self.assertTrue(compiler.validate_submission(result)[0])
            self.assertEqual(s.run_command([str(compiler.submission_workspace.executable)]).returncode, 0)

    def test_external_definitions_keep_the_ordinary_analysis_mode(self):
        source = self.marked('extern "C" int exported(){return 7;}\n'
                             'template<class T> int dormant(T x){return x+1;}\n')
        source += 'int main(){return 0;}\n'
        with self.compiler() as compiler:
            self.reduce(compiler, source)
            self.assertFalse(compiler.submission_workspace.whole_program)

    def test_priority_checkpoint_can_be_created_by_function_body_reduction(self):
        source = self.marked('template<class T> int dormant(T){const char* s="'
                             + 'x' * 140000 + '";return 0;}\n')
        source += 'int main(){return 0;}\n'
        stages = []
        with self.compiler() as compiler:
            writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
            writer.current = source
            writer.preserve_below = 65536
            original = writer.save
            def save(stage, value, **options):
                changed = original(stage, value, **options)
                if changed:
                    stages.append(stage)
                return changed
            with patch.object(writer, 'save', side_effect=save):
                result = f.try_cleanup(s, source, compiler=compiler, writer=writer,
                    reporter=self.reporter, target_bytes=65536, priority_bytes=131072,
                    feature_token=self.token)
            self.assertIsNotNone(result)
            self.assertEqual(writer.reached_limits, {131072, 65536})
            self.assertIn('131,072 B checkpoint', stages[0])
            self.assertNotIn('dormant', result)

    def test_constant_evaluation_keeps_its_body(self):
        source = self.marked("template<class T>\nconstexpr int probe(T x){return x+1;}\n")
        source += "int main(){return probe(0);}\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertTrue(result is None or "return x+1;" in result)

    def test_used_overloads_do_not_require_individual_object_comparisons(self):
        body = "template<int I> struct Tag {};\n"
        body += "".join(f"inline int pick(Tag<{i}>) {{return {i + 1};}}\n" for i in range(24))
        source = f.marker(self.token, True, "float_io") + "\n" + body
        source += f.marker(self.token, False, "float_io") + "\n"
        source += self.marked("template<class T> int dormant(T x){return x+1;}\n")
        source += "int main(){return pick(Tag<0>{});}\n"
        with self.compiler() as compiler, patch.object(compiler, "submission_object_signature", wraps=compiler.submission_object_signature) as proof:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertLessEqual(proof.call_count, 10)
            self.assertIn("return 1;", result)
            self.assertNotIn("return 24;", result)
            self.assertEqual(compiler.object_signature(source), compiler.object_signature(result))

    def test_helper_declaration_remains_available_to_uninstantiated_code(self):
        source = self.marked("namespace internal {\ninline int helper(int x){return x+1;}\n}\n")
        source += "template<class T> int wrapper(T x){return internal::helper(x);}\n"
        source += "int main(){return 7;}\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertEqual(compiler.object_signature(source), compiler.object_signature(result))

    def test_verified_target_finishes_before_declaration_reanalysis(self):
        source = self.marked("template<class T> int dormant(T x){return x+1;}\n")
        source += "int main(){return 0;}\n"
        with self.compiler() as compiler, patch.object(s, "unreferenced_declaration_candidates", side_effect=AssertionError):
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertNotIn("return x+1;", result)
            self.assertEqual(compiler.object_signature(source), compiler.object_signature(result))

    def test_raw_marker_text_and_blank_lines_are_literal(self):
        raw = 'R"tag(' + f.marker(self.token, True, "arithmetic") + '\n\n\n'
        raw += '// https://github.com/miscalculation53/library/tree/wip/template/template_arithmetic.hpp\n'
        raw += f.marker(self.token, True, "arithmetic") + '\n' + f.marker(self.token, False, "arithmetic") + ')tag"'
        source = 'const char *value=' + raw + ';\n'
        source += self.marked("template<class T>\nconstexpr int dormant(T x){return x+1;}\n")
        source += "int main(){return value[0]!='/' ;}\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertIn(raw, result)
            self.assertEqual(compiler.object_signature(source), compiler.object_signature(result))

    def test_shortening_preserves_indentation_and_important_comments(self):
        used = "int evaluate(){\n    int result = 7;\n    return result;\n}\n"
        source = self.marked("/*! retained license */\n" + used
                             + "template<class T> int dormant(T x){return x+1;}\n")
        main = "int main(){\n    return evaluate() != 7;\n}\n"
        source += main
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertIn(used, result)
            self.assertIn(main, result)
            self.assertIn("/*! retained license */", result)
            self.assertNotIn("return x+1;", result)

    def test_unused_macros_keep_the_portable_branch_dependencies(self):
        source = "#define UNUSED(x) \\\n    ((x) + 1)\n#define PORTABLE_VALUE 7\n"
        source += "#if defined(__STDCPP_FLOAT128_T__)\n"
        source += "template<class T> int portable(T){return PORTABLE_VALUE;}\n#endif\nint main(){}\n"
        with self.compiler() as compiler:
            candidates = f.unused_macro_candidates(s, source, ["math"] * len(source.splitlines()), compiler)
            self.assertEqual({item.name for item in candidates}, {"UNUSED"})
            self.assertEqual(candidates[0].end, 1)

    def test_unrecognized_marker_cannot_select_user_code(self):
        source = self.marked("constexpr int fn(){return 0;}\n") + "int main(){return fn();}\n"
        text, owners = f.feature_lines(s, source, "b" * 32)
        self.assertEqual(text, source)
        self.assertTrue(all(owner is None for owner in owners))

    def test_layout_keeps_comments_on_condition_directives(self):
        source = "#if 1 //! condition license\nint value;\n#endif // source note\n"
        result = f.finish_layout(s, source, source, [None] * 3)
        self.assertIn("//! condition license", result)
        self.assertIn("// source note", result)

    def test_failed_final_object_checks_cannot_agree(self):
        source = self.marked("template<class T> int dormant(T x){return x+1;}\n\n\n\n") + "int main(){return 0;}\n"
        calls = 0
        with self.compiler() as compiler:
            original = compiler.submission_object_signature
            def signature(text):
                nonlocal calls
                calls += 1
                return original(text) if calls == 1 else None
            with patch.object(compiler, "submission_object_signature", side_effect=signature):
                self.assertIsNone(self.reduce(compiler, source))
        self.assertGreater(calls, 1)

    def test_target_macro_alias_is_frozen_for_the_selected_target(self):
        source = "#define TARGET_VERSION __GNUC__\n"
        source += self.marked("template<class T>\nconstexpr int dormant(T x){return x+1;}\n")
        source += "#if TARGET_VERSION >= 100\nint main(){return dormant(0);}\n#else\nint main(){return 0;}\n#endif\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertNotIn("int main(){return dormant(0);}", result)
            self.assertIn("int main(){return 0;}", result)
            self.assertEqual(compiler.object_signature(source), compiler.object_signature(result))

    def test_explicit_source_path_uses_the_conservative_path(self):
        source = self.marked("constexpr int dormant(){return 0;}\n") + 'const char *path=__FILE__;\nint main(){}\n'
        with self.compiler() as compiler, patch.object(compiler, "submission_object_signature", side_effect=AssertionError):
            self.assertIsNone(self.reduce(compiler, source))

    def test_optimization_condition_keeps_the_conservative_path(self):
        source = self.marked("struct Unused {};\n")
        source += "#if defined(__OPTIMIZE__)\nint main(){return 1;}\n#else\nint main(){return 2;}\n#endif\n"
        with self.compiler() as compiler:
            self.assertIsNone(self.reduce(compiler, source))

    def test_external_macro_alias_to_a_path_uses_the_conservative_path(self):
        (self.root / "config.hpp").write_text("#define CURRENT_FILE __FILE_NAME__\n")
        source = '#include "config.hpp"\n' + self.marked("constexpr int dormant(){return 0;}\n")
        source += "const char *path=CURRENT_FILE;\nint main(){}\n"
        with self.compiler() as compiler:
            self.assertIsNone(self.reduce(compiler, source))

    def test_external_pasted_macro_uses_the_conservative_path(self):
        (self.root / "config.hpp").write_text("#define CAT0(a,b) a##b\n#define CAT(a,b) CAT0(a,b)\n#define CURRENT_FILE CAT(__FILE_,NAME__)\n")
        source = '#include "config.hpp"\n' + self.marked("constexpr int dormant(){return 0;}\n")
        source += "const char *path=CURRENT_FILE;\nint main(){}\n"
        with self.compiler() as compiler:
            self.assertIsNone(self.reduce(compiler, source))

    def test_external_counter_macro_keeps_declarations(self):
        (self.root / "counter.hpp").write_text("#define NEXT __COUNTER__\n")
        source = '#include "counter.hpp"\n' + self.marked("constexpr int Unused=NEXT;\n")
        source += "int main(){return NEXT;}\n"
        with self.compiler() as compiler:
            self.assertIsNone(self.reduce(compiler, source))

    def test_constructor_attribute_macro_keeps_its_type(self):
        for attribute in ['__attribute__((constructor))', '[[gnu::constructor]]']:
            with self.subTest(attribute=attribute):
                source = '#include <cstdio>\n#define STARTUP ' + attribute + '\n'
                source += self.marked('struct Unused { STARTUP static void start(){std::puts("startup");} };\n')
                source += 'int main(){std::puts("main");}\n'
                with self.compiler() as compiler:
                    result = self.reduce(compiler, source)
                    self.assertTrue(result is None or "struct Unused" in result)

    def test_bracket_macros_distinguish_attributes_and_bindings(self):
        source = '#define LOOP for(auto [i,j]=pair<int,int>{0,0};i<1;++i)\n'
        source += '#define LEFT [\n#define STARTUP [[gnu::constructor]]\nint main(){}\n'
        with self.compiler() as compiler:
            dangerous = compiler.dangerous_macro_names(source)
            self.assertNotIn("LOOP", dangerous)
            self.assertIn("LEFT", dangerous)
            self.assertIn("STARTUP", dangerous)

    def test_cache_notices_a_new_optional_external_header(self):
        (self.root / "config.hpp").write_text(
            '#if __has_include("enabled.hpp")\n#include "enabled.hpp"\n'
            'inline int mode(){return 2;}\n#else\ninline int mode(){return 1;}\n#endif\n')
        source = '#include "config.hpp"\n' + self.marked("template<class T>\nint dormant(T x){return x;}\n")
        source += "int main(){return mode();}\n"
        with patch.dict(s.os.environ, {"SUBMIT_CODE_CACHE_DIR": str(self.root / "cache")}):
            with self.compiler() as compiler:
                self.assertIsNotNone(self.reduce(compiler, source))
            (self.root / "enabled.hpp").write_text("\n")
            with self.compiler() as compiler, patch.object(compiler, "submission_object_signature", wraps=compiler.submission_object_signature) as proof:
                self.assertIsNotNone(self.reduce(compiler, source))
                self.assertGreater(proof.call_count, 0)

    def test_cache_checks_its_body_and_external_dependencies(self):
        header = self.root / "config.hpp"
        header.write_text("#define VALUE 1\n")
        source = '#include "config.hpp"\n' + self.marked("template<class T>\nconstexpr int dormant(T x){return x+1;}\n")
        source += "int main(){return VALUE;}\n"
        with patch.dict(s.os.environ, {"SUBMIT_CODE_CACHE_DIR": str(self.root / "cache")}):
            with self.compiler() as compiler:
                first = self.reduce(compiler, source)
            self.assertIsNotNone(first)
            with self.compiler() as compiler, patch.object(compiler, "submission_object_signature", side_effect=AssertionError):
                self.assertEqual(self.reduce(compiler, source), first)
            header.write_text("#define VALUE 2\n")
            with self.compiler() as compiler, patch.object(compiler, "submission_object_signature", wraps=compiler.submission_object_signature) as proof:
                self.assertIsNotNone(self.reduce(compiler, source))
                self.assertGreater(proof.call_count, 0)
            for record_path in (self.root / "cache" / "feature-results").glob("*.json"):
                record = json.loads(record_path.read_text())
                record["text"] = "int main(){return 99;}\n"
                record_path.write_text(json.dumps(record))
            with self.compiler() as compiler, patch.object(compiler, "submission_object_signature", wraps=compiler.submission_object_signature) as proof:
                self.reduce(compiler, source)
                self.assertGreater(proof.call_count, 0)


if __name__ == "__main__":
    unittest.main()
