import contextlib
import hashlib
import io
from pathlib import Path
import selectors
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

import tools_test_paths

import run_cpp
import submit_code


class RunCppTest(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix='run cpp tests ')
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.source = self.root / 'answer with spaces.cpp'
        self.source.write_text('#include <stdio.h>\nint main() { int x; if (scanf("%d", &x) != 1) return 2; printf("%d\\n", x + 1); return 7; }\n')
        self.executable = self.root / 'exec with spaces'

    def test_executable_banner_matches_bundle_and_survives_later_source_edits(self):
        cxx = shutil.which('g++-15') or shutil.which('g++-14')
        if cxx is None:
            self.skipTest('GNU C++ compiler is unavailable')
        output = self.root / 'bundle.cpp'
        with contextlib.redirect_stderr(io.StringIO()):
            submit_code.expand_command(submit_code.parse_args([
                'expand', str(self.source), '-o', str(output), '--no-fallback',
            ]), self.root)
        stamp, _ = submit_code.SubmissionStamp.read(output.read_text())
        run = subprocess.run
        opened = []

        def execute(command, **options):
            if command[0] == 'open':
                opened.append(command)
                return subprocess.CompletedProcess(command, 0)
            return run(command, **options)

        with patch.object(run_cpp.subprocess, 'run', side_effect=execute):
            code = run_cpp.main([str(self.source), str(self.executable), '--', cxx,
                                 str(self.source), '-std=c++23', '-O2', '-o', str(self.executable)])
        self.assertEqual(code, 0)
        self.assertEqual(opened, [['open', str(self.executable.resolve())]])
        self.source.write_text('int main() {}\n')
        result = run([str(self.executable)], input='41\n', capture_output=True, text=True, check=False)
        self.assertEqual(result.stdout, '42\n')
        self.assertEqual(result.returncode, 7)
        self.assertEqual(result.stderr, f'[source hash] #{stamp.source_digest[:12]}\n')
        self.assertIn('hash=' + stamp.source_digest[:12], output.read_text().splitlines()[-1])

    def test_compiler_failure_does_not_open_old_executable(self):
        with patch.object(run_cpp.subprocess, 'run', return_value=subprocess.CompletedProcess([], 3)) as run:
            self.assertEqual(run_cpp.main([str(self.source), str(self.executable), '--', 'compiler']), 3)
        self.assertEqual(run.call_count, 1)

    def test_quadmath_is_linked_once_at_the_end(self):
        for flags in ([], ['-lquadmath']):
            with self.subTest(flags=flags), patch.object(
                run_cpp.subprocess, 'run', return_value=subprocess.CompletedProcess([], 3),
            ) as run:
                self.assertEqual(run_cpp.main([
                    str(self.source), str(self.executable), '--', 'compiler',
                    str(self.source), *flags,
                ]), 3)
                command = run.call_args.args[0]
                self.assertEqual(command.count('-lquadmath'), 1)
                self.assertEqual(command[-1], '-lquadmath')

    def test_hash_is_visible_before_template_global_constructor_waits_for_input(self):
        cxx = shutil.which('g++-15') or shutil.which('g++-14')
        if cxx is None:
            self.skipTest('GNU C++ compiler is unavailable')
        self.source.write_text('''#define SINGLE_TESTCASE
#include "template/template_main.hpp"
void init() {}
void test() {}
void main2() { int x; if (scanf("%d", &x) == 1) printf("%d\\n", x + 1); }
Main<init, main2, test> main_dummy;
int main() {}
''')
        digest = hashlib.sha256(self.source.read_bytes()).hexdigest()
        repository = tools_test_paths.REPOSITORY_ROOT
        run = subprocess.run

        def compile_only(command, **options):
            if command[0] == 'open':
                return subprocess.CompletedProcess(command, 0)
            return run(command, **options)

        with patch.object(run_cpp.subprocess, 'run', side_effect=compile_only):
            result = run_cpp.main([str(self.source), str(self.executable), '--', cxx,
                                   str(self.source), '-std=c++23', '-I', str(repository),
                                   '-o', str(self.executable)])
        self.assertEqual(result, 0)
        with subprocess.Popen([str(self.executable)], stdin=subprocess.PIPE,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
            try:
                with selectors.DefaultSelector() as selector:
                    selector.register(process.stderr, selectors.EVENT_READ)
                    self.assertTrue(selector.select(timeout=3), 'hash missing while the template waits for input')
                self.assertEqual(process.stderr.readline(), f'[source hash] #{digest[:12]}\n'.encode())
                self.assertIsNone(process.poll())
                stdout, _ = process.communicate(input=b'41\n', timeout=3)
                self.assertEqual(stdout, b'42\n')
                self.assertEqual(process.returncode, 0)
            finally:
                if process.poll() is None:
                    process.kill()
                    process.communicate()

    def test_source_change_during_compilation_requires_rerun(self):
        def compile_only(command, **options):
            self.source.write_text('int main() {}\n')
            return subprocess.CompletedProcess(command, 0)

        with patch.object(run_cpp.subprocess, 'run', side_effect=compile_only) as run:
            with contextlib.redirect_stderr(io.StringIO()) as stderr:
                result = run_cpp.main([str(self.source), str(self.executable), '--', 'compiler'])
        self.assertEqual(result, 1)
        self.assertEqual(run.call_count, 1)
        self.assertIn('コンパイル中に元コードが変更', stderr.getvalue())

    def test_compile_interrupt_exits_without_a_traceback(self):
        with patch.object(run_cpp.subprocess, 'run', side_effect=KeyboardInterrupt):
            self.assertEqual(run_cpp.main([str(self.source), str(self.executable), '--', 'compiler']), 130)


if __name__ == '__main__':
    unittest.main()
