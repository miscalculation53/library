from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

import tools_test_paths

import compare_performance as performance


class PerformanceComparisonTest(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name).resolve()
        self.input = self.root / 'input'
        self.input.write_text('fixture\n')

    def program(self, name, body):
        path = self.root / name
        path.write_text('#!/bin/sh\n' + body + '\n')
        path.chmod(0o700)
        return path

    def test_matching_output_records_every_sample(self):
        programs = {name: self.program(name, 'cat') for name in ['before', 'after']}
        result = performance.compare(programs, [], self.input, 3, 10, 1)
        self.assertEqual(len(result['samples_wall_ms']['before']), 3)
        self.assertEqual(len(result['samples_wall_ms']['after']), 3)
        self.assertEqual(result['stdout_sha256'], performance.digest(self.input))
        self.assertGreater(result['before_over_after'], 0)

    def test_different_results_reject_speed_comparison(self):
        programs = {'before': self.program('before', 'cat'),
                    'after': self.program('after', 'echo wrong')}
        with self.assertRaisesRegex(RuntimeError, 'output differs'):
            performance.compare(programs, [], self.input, 3, 10, 1)

    def test_failed_workload_is_not_timed_as_success(self):
        program = self.program('failure', 'exit 2')
        with self.assertRaisesRegex(RuntimeError, 'status 2'):
            performance.run_program(program, [], self.input, 10)

    def run_main_with_compiler(self, mutate_input=False, wrong_dependency=False):
        before, after = (self.root / name for name in ('old', 'new'))
        before.mkdir()
        after.mkdir()
        source = after / 'workload.cpp'
        source.write_text('#include "header.hpp"\nint main() {}\n')
        header = after / 'header.hpp'
        header.write_text('// fixture\n')
        output = self.root / 'result.json'
        compiled_roots = []

        def compiler(command, **_kwargs):
            if '-MM' in command:
                workload = Path(command[-1])
                self.assertNotEqual(workload.parent, source.parent)
                self.assertEqual(workload.read_bytes(), source.read_bytes())
                compiled_roots.append(command[command.index('-I') + 1])
                dependencies = str(workload)
                if wrong_dependency:
                    dependencies += ' ' + str(header)
                Path(command[command.index('-MF') + 1]).write_text('x: ' + dependencies + '\n')
            elif '-o' in command:
                Path(command[command.index('-o') + 1]).write_bytes(b'executable fixture')
            return SimpleNamespace(stdout='compiler fixture\n', returncode=0)

        def comparison(*_args):
            if mutate_input:
                self.input.write_text('changed input\n')
            return {'before_over_after': 1.0}

        with mock.patch.object(performance.subprocess, 'run', side_effect=compiler), \
                mock.patch.object(performance, 'compare', side_effect=comparison), \
                mock.patch('builtins.print'):
            result = performance.main([str(source), '--before-root', str(before),
                                       '--after-root', str(after), '--input', str(self.input),
                                       '--repetitions', '3', '--output', str(output)])
        self.assertEqual(compiled_roots, [str(before), str(after)])
        return result, output

    def test_workload_isolated_from_its_original_include_directory(self):
        result, output = self.run_main_with_compiler()
        self.assertEqual(result, 0)
        self.assertTrue(output.is_file())

    def test_input_change_rejects_comparison(self):
        with self.assertRaises(SystemExit) as raised:
            self.run_main_with_compiler(mutate_input=True)
        self.assertEqual(raised.exception.code, 1)
        self.assertFalse((self.root / 'result.json').exists())

    def test_header_from_other_source_tree_rejects_comparison(self):
        with self.assertRaises(SystemExit) as raised:
            self.run_main_with_compiler(wrong_dependency=True)
        self.assertEqual(raised.exception.code, 1)
        self.assertFalse((self.root / 'result.json').exists())


if __name__ == '__main__':
    unittest.main()
