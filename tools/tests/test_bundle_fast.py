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

    def compiler(self, **options):
        return s.CompilerWorkspace(cxx=self.cxx, standard="c++23", include_paths=[self.root],
                                   extra_args=[], timeout=None, reporter=self.reporter, **options)

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

    def test_unused_modint_group_and_source_aliases_are_removed_together(self):
        source = f.marker(self.token, True, 'source:math/modint/template_modint.hpp') + '\n'
        source += 'struct Mint {int value;};\nusing modint998244353=Mint;\n'
        source += f.marker(self.token, False, 'source:math/modint/template_modint.hpp') + '\n'
        source += 'using mint=modint998244353;\nint main(){return 0;}\n'
        with self.compiler() as compiler:
            external_identifiers = compiler.external_identifiers
            with patch.object(compiler, 'external_identifiers', side_effect=lambda text: external_identifiers(text) | {'mint'}):
                result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertNotIn('modint998244353', result)
            self.assertNotIn('struct Mint', result)

    def test_used_modint_group_is_kept(self):
        source = f.marker(self.token, True, 'source:math/modint/modint.hpp') + '\n'
        source += 'struct Mint {int value;};\nusing modint998244353=Mint;\n'
        source += f.marker(self.token, False, 'source:math/modint/modint.hpp') + '\n'
        source += 'using mint=modint998244353;\nint main(){return mint{7}.value!=7;}\n'
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertIn('struct Mint', result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_unused_dynamic_modint_policy_can_be_removed_with_its_dependents(self):
        source = '''namespace internal
{
template <int id>
struct policy_barrett32
{
  int value;
};
}
struct Static { int value; };
template <int id>
using dynamic_modint32 = internal::policy_barrett32<id>;
template <class T>
struct is_dynamic_modint {};
template <int id>
struct is_dynamic_modint<dynamic_modint32<id>> {};
'''
        for type_name in ('Static', 'dynamic_modint32<0>'):
            with self.subTest(type_name=type_name), self.compiler() as compiler:
                program = source + f'int main(){{ return {type_name}{{7}}.value != 7; }}\n'
                writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
                writer.current = program
                result = f.prune_function_declarations(s, program, compiler=compiler, writer=writer,
                    reporter=self.reporter, keep_functions=s.DEFAULT_KEEP_FUNCTIONS, include_dynamic_types=True)
                self.assertEqual('struct policy_barrett32' in result, type_name != 'Static')

    def test_discarded_constexpr_arm_is_checked_using_emitted_function_lines(self):
        source = '''template <bool select>
int pick(int x)
{
  if constexpr (select)
  {
    return x + 42;
  }
  else
  {
    return x * 7;
  }
}
int main(){return pick<false>(1) != 7;}
'''
        with self.compiler() as compiler:
            reference, _ = compiler.submission_reference(source)
            self.assertIsNotNone(reference)
            digest = hashlib.sha256(source.encode()).hexdigest()
            edits = f.discarded_constexpr_branches(s, source, ['library'] * len(source.splitlines()),
                                                  compiler.emitted_line_cache[digest])
            proposal = f.mask_ranges(s, source, edits)
            self.assertNotIn('return x + 42', proposal)
            self.assertIn('return x * 7', proposal)
            self.assertEqual(compiler.submission_object_signature(proposal), reference)

    def test_priority_checkpoint_precedes_unused_modint_removal(self):
        source = f.marker(self.token, True, 'source:math/modint/modint.hpp') + '\n'
        source += 'struct Mint {int value;};\nusing modint998244353=Mint;\n'
        source += 'template<class T>\nint dormant(){return sizeof("' + 'x' * 140000 + '");}\n'
        source += f.marker(self.token, False, 'source:math/modint/modint.hpp') + '\n'
        source += 'using mint=modint998244353;\nint main(){return 0;}\n'
        checkpoints = []
        with self.compiler() as compiler:
            writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
            writer.current = source
            save = writer.save
            def record(stage, value, **options):
                changed = save(stage, value, **options)
                if changed and writer.below_limit(value, 131072):
                    checkpoints.append(value)
                return changed
            with patch.object(writer, 'save', side_effect=record), patch.object(
                    s, 'try_size_checkpoint', side_effect=AssertionError('oversized preprocessing retry')):
                result = f.try_cleanup(s, source, compiler=compiler, writer=writer,
                    reporter=self.reporter, target_bytes=65536, priority_bytes=131072,
                    feature_token=self.token)
            self.assertIsNotNone(result)
            self.assertIn('struct Mint', checkpoints[0])
            self.assertNotIn('struct Mint', result)

    def test_structural_refinement_starts_after_both_checkpoints_are_saved(self):
        source = self.marked('#define UNUSED_PADDING "' + 'x' * 140000 + '"\n'+'''template<class T>
struct Box
{
  T value;
  Box() : value(7) {}
  Box(int) : value(9) {}
};
''') + 'int main(){return Box<int>{}.value!=7;}\n'
        calls = []
        with self.compiler() as compiler:
            writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
            writer.current = source
            writer.preserve_below = 65536
            original = f.structural_candidates
            def check(*args):
                self.assertTrue({131072,65536} <= writer.reached_limits)
                calls.append(1)
                return original(*args)
            with patch.object(f, 'structural_candidates', side_effect=check):
                result = s.cleanup_text(source,compiler=compiler,writer=writer,
                    reporter=self.reporter,cppcheck=None,keep_functions=s.DEFAULT_KEEP_FUNCTIONS,
                    max_passes=1,safe_only=False,require_validation=True,target_bytes=65536,
                    feature_token=self.token)
            self.assertTrue(calls)
            self.assertNotIn('Box(int)',result)

    def test_early_declaration_checkpoint_publishes_only_after_validation(self):
        source = """template<class T>
struct Box
{
  T value;
  Box() : value(7) {}
  Box(int) : value(9) {}
};
int main(){return Box<int>{}.value!=7;}
"""
        with self.compiler() as compiler:
            writer = s.CheckpointWriter(self.root / "bundle.cpp", self.reporter)
            writer.current = source
            limit = writer.size(source) - 8
            options = dict(compiler=compiler, writer=writer, reporter=self.reporter,
                           limit=limit, keep_functions=s.DEFAULT_KEEP_FUNCTIONS, keep_comments=set())
            with patch.object(compiler, "validate", return_value=(False, "alternate target rejected")) as validate:
                self.assertIsNone(s.try_early_declaration_checkpoint(source, **options))
            validate.assert_called_once()
            self.assertFalse(writer.path.exists())
            self.assertEqual(writer.current, source)

            compiler.preserve_initialization(source)
            result = s.try_early_declaration_checkpoint(source, **options)
            self.assertIsNotNone(result)
            self.assertLess(writer.size(result), limit)
            self.assertEqual(writer.path.read_text(), result)
            self.assertNotIn("Box(int)", result)

    def test_unused_operator_families_are_removed_without_losing_pair_addition(self):
        source = '#include <array>\n#include <tuple>\n#include <utility>\nusing namespace std;\n'
        source += '''template<class A,class B> pair<A,B>& operator+=(pair<A,B>& x,const pair<A,B>& y){x.first+=y.first;x.second+=y.second;return x;}
template<class T,size_t N> array<T,N>& operator+=(array<T,N>& x,const array<T,N>& y){for(size_t i=0;i<N;i++)x[i]+=y[i];return x;}
template<class T> tuple<T>& operator+=(tuple<T>& x,const tuple<T>& y){get<0>(x)+=get<0>(y);return x;}
int main(){pair<int,int> x{1,2};x+=pair{3,4};return x.first!=4;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertNotIn('array<T,N>& operator+=', result)
            self.assertNotIn('tuple<T>& operator+=', result)
            self.assertIn('pair<A,B>& operator+=', result)
            self.assertTrue(compiler.validate_submission(result)[0])

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

    def test_reference_object_cache_retains_undefined_symbols(self):
        source = '#include <cstdio>\nint main(){return puts("hello") < 0;}\n'
        with self.compiler() as compiler:
            self.assertTrue(compiler.validate_submission(source)[0])
            compiler.submission_object_signature(source)
            cached = compiler.object_undefined
            self.assertTrue(any("puts" in symbol for symbol in cached))
            compiler.submission_workspace.object_cache.clear()
            compiler.submission_object_signature(source)
            self.assertEqual(compiler.object_undefined, cached)

    def test_matching_object_reuses_linked_reference_without_replacing_executable(self):
        source = "int calculate(){return 7;}\nint main(){return calculate()!=7;}\n"
        reformatted = source + "// trailing comment\n"
        source_digest = hashlib.sha256(source.encode()).hexdigest()
        reformatted_digest = hashlib.sha256(reformatted.encode()).hexdigest()
        with self.compiler() as compiler:
            self.assertTrue(compiler.validate_submission(source)[0])
            workspace = compiler.submission_workspace
            signature = compiler.submission_object_signature(source)
            self.assertEqual(compiler.preprocessed_code_fingerprint(source),
                             compiler.preprocessed_code_fingerprint(reformatted))
            compiler.remember_submission_layout(source, reformatted)
            with patch.object(workspace, "_run", side_effect=AssertionError("reference already verified")):
                reused, headers = compiler.submission_reference(reformatted)
            self.assertEqual(reused, signature)
            self.assertIn("calculate", headers)
            self.assertEqual(workspace.executable_digest, source_digest)
            self.assertTrue(compiler.validate_submission(reformatted)[0])
            self.assertEqual(workspace.executable_digest, reformatted_digest)
            self.assertEqual(s.run_command([str(workspace.executable)]).returncode, 0)

    def test_preprocessing_fingerprint_uses_submission_optimization_mode(self):
        source = "#if defined(__OPTIMIZE__)\nint optimized;\n#endif\n"
        with self.compiler() as compiler:
            optimized = compiler.preprocessed_code_fingerprint(source)
            compiler.submission_optimization = "-O0"
            self.assertIsNotNone(optimized)
            self.assertNotEqual(compiler.preprocessed_code_fingerprint(source), optimized)

    def test_qualified_missing_member_diagnostic_allows_one_retry(self):
        source = """namespace Needed {
  int probe(double);
  int probe(char);
  int probe(int);
}
namespace Unused {
  int probe(short);
  int probe(long);
  int probe(unsigned);
}
int main(){return !requires { static_cast<int (*)(int)>(&Needed::probe); };}
"""
        with self.compiler() as compiler:
            with patch.object(compiler, "submission_object_signature", wraps=compiler.submission_object_signature) as proofs:
                result = self.prune_declarations(compiler, source)
            self.assertLessEqual(proofs.call_count, 4)
            self.assertEqual(result.count("int probe("), 1)
            self.assertIn("int probe(int);", result)
            self.assertEqual(compiler.submission_object_signature(source),
                             compiler.submission_object_signature(result))

    def test_required_unique_declaration_is_not_retested_individually(self):
        source = "struct Box {\n  int required() const;\n  int dormant() const;\n};\n"
        source += "int main(){return !requires(Box x){x.required();};}\n"
        with self.compiler() as compiler:
            with patch.object(compiler, "submission_object_signature", wraps=compiler.submission_object_signature) as signatures:
                result = self.prune_declarations(compiler, source)
            self.assertLessEqual(signatures.call_count, 3)
            self.assertIn("required()", result)
            self.assertNotIn("dormant()", result)

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

    def test_unused_constructor_releases_its_helper_declaration(self):
        source = '''int unused_helper(int);
template<class T>
struct Box
{
  T value;
  Box(T x) : value{x} {}
  template<class U>
  Box(int, const U&) : value{unused_helper(3)} {}
};
template<class T>
Box(T) -> Box<T>;
int main(){Box b(7);return b.value!=7;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertNotIn('unused_helper', result)
            self.assertNotIn('Box(int,', result)
            self.assertIn('Box(T x)', result)
            self.assertTrue(compiler.validate_submission(result)[0])
            self.assertEqual(s.run_command([str(compiler.submission_workspace.executable)]).returncode, 0)

    def test_constructor_required_by_type_detection_is_kept(self):
        source = '''struct Box
{
  Box(int) = delete;
  Box(double) {}
};
constexpr bool supported=requires {Box(0.5);};
int main(){return !supported;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertIn('Box(double)', result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_required_deduction_guide_is_kept(self):
        source = '''#include <type_traits>
template<class T>
struct Box
{
  T value;
  template<class U>
  Box(U x) : value(x) {}
};
Box(int) -> Box<long>;
static_assert(std::is_same_v<decltype(Box(7)),Box<long>>);
int main(){Box b(7);return b.value!=7;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertIn('Box(int) -> Box<long>;', result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_unused_primary_template_body_becomes_a_forward_declaration(self):
        source = '''template<class T>
struct Box
{
  T value;
  Box(T x) : value(x) {}
};
template<>
struct Box<void>
{
  int value=7;
};
int main(){return Box<void>{}.value!=7;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertNotIn('T value;', result)
            self.assertIn('struct Box<void>', result)
            self.assertTrue(compiler.validate_submission(result)[0])
            used = source.replace('Box<void>{}', 'Box<int>{7}')
            result = self.prune_declarations(compiler, used)
            self.assertIn('T value;', result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_unused_helper_with_body_macros_and_a_member_name_collision_is_removed(self):
        source = '''#define LOOP(n) for(auto i=0;i<n;++i)
template<class T>
int top(T x){LOOP(2)x+=1;return x;}
struct Box {int top=0;};
int main(){return Box{}.top;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertNotIn('int top(T', result)
            self.assertIn('int top=0', result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_unused_variable_template_with_comma_in_type_arguments_is_removed(self):
        source = '''#include <type_traits>
template<class T>
constexpr bool unused=std::is_same_v<T,int>;
template<class T>
constexpr bool required=std::is_same_v<T,int>;
int main(){return !required<int>;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertNotIn('bool unused=', result)
            self.assertIn('bool required=', result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_empty_namespace_pruning_preserves_namespace_lookup_and_notices(self):
        source = '''namespace unused
{
}
namespace required
{
}
using namespace required;
namespace licensed
{
//! required notice
}
int main(){return 0;}
'''
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertNotIn('namespace unused', result)
            self.assertIn('namespace required', result)
            self.assertIn('//! required notice', result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_wide_stream_cleanup_keeps_used_128_bit_io(self):
        header = Path(s.__file__).parent.parent / 'template/template_integer_io.hpp'
        source = '#include <bits/stdc++.h>\nusing namespace std;\nusing i128=__int128_t;\nusing u128=__uint128_t;\n'
        source += '\n'.join(line for line in header.read_text().splitlines() if not line.startswith(('#include', '#pragma')))+'\n'
        for used in (False, True):
            with self.subTest(used=used), self.compiler() as compiler:
                compiler.submission_whole_program = True
                compiler.submission_optimization = '-Og'
                text = source + ('int main(){i128 x;cin>>x;cout<<x;}' if used else 'int main(){return 0;}')+'\n'
                writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
                writer.current = text
                result = f.prune_function_declarations(s, text, compiler=compiler, writer=writer,
                    reporter=self.reporter, keep_functions=s.DEFAULT_KEEP_FUNCTIONS, final=True)
                self.assertEqual('stoi128(' in result, used)
                self.assertEqual('i128tos(' in result, used)
                self.assertTrue(compiler.validate_submission(result)[0])
                if used:
                    p=s.run_command([str(compiler.submission_workspace.executable)],input_text='-170141183460469231731687303715884105728\n')
                    self.assertEqual(p.stdout, '-170141183460469231731687303715884105728')

    def test_uncalled_function_scan_keeps_template_passed_as_an_argument(self):
        source = """template<class T>
T make_value(){return T(7);}
template<class T>
T make_other(){return T(1);}
int dormant(){return 0;}
int invoke(int (*fn)(), long (*other)()){return fn()+other();}
int main(){return invoke(make_value<int>, make_other<long>)!=8;}
"""
        functions = f.definition_candidates(s, source, ["library"] * len(source.splitlines()))
        groups = f.uncalled_function_groups(s, source, functions)
        self.assertEqual({item.name for group in groups for item in group}, {"dormant"})

    def test_optimized_cleanup_removes_uncalled_overloads_with_a_member_name_collision(self):
        source = '''#include <vector>
using namespace std;
#define LOOP(n) for(auto i=0;i<n;++i)
template<class T>
int top(T x){LOOP(2)x+=1;return x;}
int top(const vector<int>& x){return top(x[0]);}
struct Box
{
  int top;
  Box(int x) : top(x) {}
};
int main(){return Box(0).top;}
'''
        with self.compiler() as compiler:
            compiler.submission_whole_program = True
            writer = s.CheckpointWriter(self.root/'bundle.cpp',self.reporter)
            writer.current = source
            result = f.prune_optimized_declarations(s,source,compiler=compiler,writer=writer,
                reporter=self.reporter,keep_functions=s.DEFAULT_KEEP_FUNCTIONS)
            self.assertNotIn('int top(', result)
            self.assertIn('int top;',result)
            self.assertTrue(compiler.validate_submission(result)[0])

    def test_optimized_cleanup_preserves_shared_wide_integer_helpers(self):
        header = Path(s.__file__).parent.parent/'template/template_integer_io.hpp'
        source = '#include <bits/stdc++.h>\nusing namespace std;\nusing i128=__int128_t;\nusing u128=__uint128_t;\n'
        source += '\n'.join(line for line in header.read_text().splitlines() if not line.startswith(('#include','#pragma')))+'\n'
        source += 'int main(){cout<<i128tos(i128(7));}\n'
        with self.compiler() as compiler:
            compiler.submission_whole_program = True
            writer=s.CheckpointWriter(self.root/'bundle.cpp',self.reporter)
            writer.current=source
            result=f.prune_optimized_declarations(s,source,compiler=compiler,writer=writer,
                reporter=self.reporter,keep_functions=s.DEFAULT_KEEP_FUNCTIONS)
            self.assertIn('string i128tos(',result)
            self.assertNotIn('operator<<',result)
            self.assertNotIn('stoi128(',result)
            self.assertTrue(compiler.validate_submission(result)[0])
            self.assertEqual(s.run_command([str(compiler.submission_workspace.executable)]).stdout,'7')

    def test_function_candidates_skip_constructor_local_definitions(self):
        source = "struct Box {\n  Box() {\n    struct Local {\n      int invoke(int x) {return x;}\n    };\n  }\n};\n"
        candidates = f.definition_candidates(s, source, ["math"] * len(source.splitlines()))
        self.assertNotIn("invoke", [item.name for item in candidates])

    def test_empty_policy_callback_is_kept_without_a_failed_batch(self):
        self.reporter.verbose = True
        source = self.marked("struct Policy {\n template<class T> static void merge(T&) {}\n};\n"
                             "struct Dormant {\n template<class T> static void merge(T& x) {++x;}\n};\n")
        source += "int main(){int x=0;Policy::merge(x);return x;}\n"
        with self.compiler() as compiler:
            result = self.reduce(compiler, source)
            self.assertIsNotNone(result)
            self.assertIn("merge(T&) {}", result)
            self.assertNotIn("++x", result)
            self.assertNotIn("batch comparison:", self.reporter.stream.getvalue())

    def test_compaction_preserves_coordinates_across_inactive_branches(self):
        source = "#if 0\n" + "\n" * 12 + "int inactive;\n#else\nint active=__LINE__;\n#endif\n"
        result = f.compact_lines(s, source)
        with self.compiler() as compiler:
            self.assertEqual(compiler.preprocessed_code_fingerprint(source), compiler.preprocessed_code_fingerprint(result))

    def test_compaction_uses_existing_logical_line_numbers(self):
        source = '#line 700 "fragment.hpp"\nint first=__LINE__;\n' + '\n'*50 + 'int second=__LINE__;\n'
        result = f.compact_lines(s, source)
        self.assertLess(len(result), len(source))
        with self.compiler() as compiler:
            self.assertEqual(compiler.preprocessed_code_fingerprint(source), compiler.preprocessed_code_fingerprint(result))

    def test_compaction_keeps_uncertain_conditional_line_numbers(self):
        source = '#if 0\n#line 900\n#endif\n' + '\n'*40 + 'int value=__LINE__;\n'
        result = f.compact_lines(s, source)
        with self.compiler() as compiler:
            self.assertEqual(compiler.preprocessed_code_fingerprint(source), compiler.preprocessed_code_fingerprint(result))

    def test_tidy_checkpoint_preserves_literal_and_comment_layout(self):
        original = ('const char *s = R"raw(first\n#line 40\n\n\nlast)raw";\n'
                    '/*! credit\n\n  original indentation */\n'
                    '#define VALUE \\\n#line 50\n'
                    '#define VALUE_WITH_SPACE \\\x20\n#line 55\n'
                    '  int result;\n')
        candidate = original.replace('  int result;', '\n\n#line 60\n\n  int result;')
        expected = original.replace('  int result;', '\n  int result;')
        self.assertEqual(f.tidy_checkpoint(s, candidate, original=original), expected)

    def test_tidy_checkpoint_preserves_authored_line_controls(self):
        for directive in ('#line 700\n', '#li\\\nne 700\n', '%:line 700\n', '# 700 "source.cpp"\n'):
            with self.subTest(directive=directive):
                original = directive + 'int a;\nint b;\n'
                candidate = directive + 'int a;\n\n\n#line 711\nint b;\n'
                self.assertEqual(f.tidy_checkpoint(s, candidate, original=original), candidate)

    def test_tidy_checkpoint_retargets_line_observers(self):
        original = 'int x;\nvoid f(){\n  assert(x);\n  CHECK(x);\n}\n'
        candidate = 'int x;\n\n\n#line 100\nvoid f(){\n  assert(x);\n  CHECK(x);\n}\n'
        expected = 'int x;\n\nvoid f(){\n#line 101\n  assert(x);\n  CHECK(x);\n}\n'
        self.assertEqual(f.tidy_checkpoint(s, candidate, original=original, observers={'assert', 'CHECK'}), expected)

    def test_tidy_checkpoint_declines_uncertain_conditional_observers(self):
        original = 'int x;\n#if ENABLE\nvoid f(){ assert(x); }\n#endif\n'
        candidate = 'int x;\n\n\n#line 100\n#if ENABLE\nvoid f(){ assert(x); }\n#endif\n'
        self.assertEqual(f.tidy_checkpoint(s, candidate, original=original, observers={'assert'}), candidate)

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

    def test_friend_type_pruning_keeps_injected_functions_needed_by_main(self):
        source = "int injected();\nstruct Required {\n friend int injected(){return 7;}\n};\n"
        source += "struct Dormant {\n friend int unused_friend(Dormant){return 9;}\n};\n"
        source += "int main(){return injected()!=7;}\n"
        with self.compiler() as compiler:
            result = self.prune_declarations(compiler, source)
            self.assertIn("struct Required", result)
            self.assertNotIn("struct Dormant", result)
            self.assertEqual(compiler.submission_object_signature(source), compiler.submission_object_signature(result))

    def test_early_checkpoint_checks_linking(self):
        source = self.marked('#define UNUSED_PADDING "' + 'x' * 140000 + '"\n')
        source += 'int missing();\nint main(){return missing();}\n'
        with self.compiler() as compiler:
            writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
            writer.current = source
            f.try_cleanup(s, source, compiler=compiler, writer=writer, reporter=self.reporter,
                          target_bytes=65536, priority_bytes=131072, feature_token=self.token)
            self.assertEqual(writer.current, source)
            self.assertFalse(writer.path.exists())

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
            optimized_key = compiler.submission_workspace.pch_key
        with patch.dict(s.os.environ, {'CPLUS_INCLUDE_PATH': str(self.root / 'headers'),
                                       'SUBMIT_CODE_CACHE_DIR': str(self.root / 'pch-cache')}), self.compiler() as compiler:
            compiler.submission_optimization = '-Og'
            self.assertTrue(compiler.validate_submission(source, build_pch=True)[0])
            self.assertEqual(s.run_command([str(compiler.submission_workspace.executable)]).returncode, 1)
            self.assertNotEqual(optimized_key, compiler.submission_workspace.pch_key)
            signature = compiler.submission_object_signature(source)
            compiler.submission_workspace.object_cache.clear()
            self.assertEqual(signature, compiler.submission_object_signature(source))

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

    def test_declaration_object_probe_without_pch_marker_retries_raw_source(self):
        prefix = '#include <bits/stdc++.h>\n'
        source = prefix + 'int main(){return 7;}\n'
        invocations = []
        with self.compiler(keep_unused_functions=False) as workspace:
            workspace.pch_prefix_end = len(prefix)
            workspace.pch_available = True

            def compile_object(command):
                use_pch = '-include' in command
                invocations.append(use_pch)
                written = workspace.source.read_text()
                if use_pch:
                    self.assertNotIn(prefix, written)
                    self.assertIn('-Werror=invalid-pch', command)
                else:
                    self.assertEqual(written, source)
                payload = b'probe without PCH-use marker' if use_pch else b'raw object'
                Path(command[command.index('-o') + 1]).write_bytes(payload)
                return s.subprocess.CompletedProcess(command, 0, '', '')

            with patch.object(workspace, '_ensure_pch', return_value=True), \
                 patch.object(workspace, '_run', side_effect=compile_object), \
                 patch.object(workspace, '_undefined_symbols', return_value=set()), \
                 patch.object(workspace, '_object_dependency_hashes', return_value={}):
                signature = workspace.object_signature(source, pch=True)
                self.assertEqual(signature, hashlib.sha256(b'raw object').hexdigest())
                self.assertEqual(workspace.object_signature(source), signature)
            self.assertEqual(invocations, [True, False])
            self.assertFalse(workspace.pch_available)

    def test_declaration_object_pch_is_separate_from_layout_verification(self):
        prefix = '#include <bits/stdc++.h>\n'
        source = prefix + 'int main(){return 7;}\n'
        formatted = prefix + 'int main() { return 7; }\n'
        invocations = []
        dependency = self.root / 'header.hpp'
        dependency.write_text('// precompiled dependency\n')
        dependencies = {str(dependency): hashlib.sha256(dependency.read_bytes()).hexdigest()}
        with self.compiler() as compiler:
            workspace = compiler.submission_workspace = self.compiler(keep_unused_functions=False)

            def compile_object(command):
                if '-x' in command:
                    self.assertEqual(command[command.index('-x') + 1], 'c++-header')
                    payload = b'probe PCH'
                else:
                    self.assertNotIn('-include', command)
                    invocations.append(workspace.source.read_text())
                    payload = b'raw object'
                Path(command[command.index('-o') + 1]).write_bytes(payload)
                return s.subprocess.CompletedProcess(command, 0, '', '')

            with patch.dict(s.os.environ, {'SUBMIT_CODE_CACHE_DIR': str(self.root / 'cache')}), \
                 patch.object(workspace, '_pch_prefix', return_value=(prefix, len(prefix))), \
                 patch.object(workspace, '_run', side_effect=compile_object), \
                 patch.object(workspace, '_undefined_symbols', return_value=set()), \
                 patch.object(workspace, '_object_dependency_hashes', return_value=dependencies):
                self.assertTrue(workspace._ensure_pch(source, probe=True))
                probe_key = workspace.pch_key
                self.assertTrue(compiler.same_submission_layout(source, formatted))
            self.assertNotEqual(workspace.pch_key, probe_key)
            self.assertEqual(invocations, [source, formatted])

    def test_layout_object_verification_uses_the_alternate_float_macro(self):
        source = '#ifdef __STDCPP_FLOAT128_T__\nint value = 1;\n#endif\nint main() {}\n'
        formatted = source.replace('int value = 1;', 'int value = 2;')
        with self.compiler() as compiler:
            portable = compiler.portable_workspace = self.compiler()
            portable.force_float128_macro = True
            observed = []

            def compile_object(command):
                prepared = portable.source.read_text()
                self.assertTrue(prepared.startswith('#undef __STDCPP_FLOAT128_T__\n#define __STDCPP_FLOAT128_T__ 1\n'))
                observed.append(prepared)
                Path(command[command.index('-o') + 1]).write_text(prepared)
                return s.subprocess.CompletedProcess(command, 0, '', '')

            with patch.object(compiler, 'submission_object_signature', return_value='native object'), \
                 patch.object(portable, '_run', side_effect=compile_object), \
                 patch.object(portable, '_undefined_symbols', return_value=set()), \
                 patch.object(portable, '_object_dependency_hashes', return_value={}):
                self.assertFalse(compiler.same_submission_layout(source, formatted))
            self.assertEqual(len(observed), 2)
            self.assertTrue(observed[0].endswith(source))
            self.assertTrue(observed[1].endswith(formatted))

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
        validation_counts = []
        with self.compiler() as compiler:
            writer = s.CheckpointWriter(self.root / 'bundle.cpp', self.reporter)
            writer.current = source
            writer.preserve_below = 65536
            original = writer.save
            def save(stage, value, **options):
                changed = original(stage, value, **options)
                if changed:
                    stages.append(stage)
                    validation_counts.append(validate.call_count)
                return changed
            with patch.object(compiler, 'validate_submission', wraps=compiler.validate_submission) as validate, \
                 patch.object(writer, 'save', side_effect=save):
                result = f.try_cleanup(s, source, compiler=compiler, writer=writer,
                    reporter=self.reporter, target_bytes=65536, priority_bytes=131072,
                    feature_token=self.token)
            self.assertIsNotNone(result)
            self.assertEqual(writer.reached_limits, {131072, 65536})
            self.assertIn('131,072 B checkpoint', stages[0])
            self.assertEqual(validation_counts[0], 1)
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
