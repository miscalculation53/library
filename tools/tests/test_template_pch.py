import contextlib
import io
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

import tools_test_paths

import run_cpp
import template_pch


class TemplatePchTest(unittest.TestCase):
    def setUp(self):
        self.cxx = shutil.which('g++-15') or shutil.which('g++-14')
        if not self.cxx:
            self.skipTest('GNU C++ compiler is unavailable')
        temporary = tempfile.TemporaryDirectory(prefix='template pch tests ')
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name).resolve()
        self.templates = self.root / 'template'
        self.templates.mkdir()
        self.header = self.templates / 'template_all.hpp'
        self.dependency = self.templates / 'value.hpp'
        self.dependency.write_text('#define VALUE 3\n')
        self.header.write_text('''#pragma once
#include "value.hpp"
#ifdef LOCAL
inline int answer() { return VALUE + EPS + 100; }
#else
inline int answer() { return VALUE + EPS; }
#endif
''')
        (self.root / 'relative.hpp').write_text('#define EXTRA 2\n')
        self.source = self.root / 'main.cpp'
        self.source.write_text('''#define EPS 4
#include "template/template_all.hpp"
#include "relative.hpp"
#include <stdio.h>
static_assert(__LINE__ == 5);
int main() { printf("%d\\t%s\\n", answer() + EXTRA, __FILE__); }
''')
        self.executable = self.root / 'exec'
        self.command = [self.cxx, str(self.source), '-std=c++23', '-o', str(self.executable)]
        self.cache = self.root / 'cache'
        for name, value in [('CACHE_ROOT', self.cache), ('TEMPLATE_DIR', self.templates)]:
            mocked = patch.object(template_pch, name, value)
            mocked.start()
            self.addCleanup(mocked.stop)

    def compile(self, command=None):
        with template_pch.prepare(self.source, command or self.command) as prepared:
            result = subprocess.run([*prepared.command, '-H'], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn('! ', result.stderr)
            self.assertIn('template.hpp.gch', result.stderr)
            self.assertNotIn('not used because', result.stderr)
            self.assertTrue(prepared.unchanged())
        result = subprocess.check_output([str(self.executable)], text=True)
        number, filename = result.strip().split('\t')
        self.assertEqual(filename, str(self.source))
        return int(number)

    def artifacts(self):
        return {str(path): path.stat().st_mtime_ns for path in self.cache.glob('*/template.hpp.gch')}

    def test_uses_pch_and_preserves_source_locations_and_relative_includes(self):
        self.assertEqual(self.compile(), 9)
        before = self.artifacts()
        self.assertEqual(self.compile(), 9)
        self.assertEqual(self.artifacts(), before)
        self.source.write_text(self.source.read_text().replace('answer() + EXTRA', 'answer() + EXTRA + 1'))
        self.assertEqual(self.compile(), 10)
        self.assertEqual(self.artifacts(), before)

    def test_dependency_and_source_macros_trigger_rebuild(self):
        self.assertEqual(self.compile(), 9)
        before = self.artifacts()
        self.dependency.write_text('#define VALUE 8\n')
        self.assertEqual(self.compile(), 14)
        self.assertNotEqual(self.artifacts(), before)
        before = self.artifacts()
        self.source.write_text(self.source.read_text().replace('EPS 4', 'EPS 6'))
        self.assertEqual(self.compile(), 16)
        self.assertNotEqual(self.artifacts(), before)

    def test_compiler_options_get_separate_matching_pch(self):
        self.assertEqual(self.compile(), 9)
        self.assertEqual(self.compile([*self.command, '-DLOCAL']), 109)
        self.assertEqual(len(self.artifacts()), 2)
        self.assertEqual(self.compile(), 9)

    def run_without_opening(self, argv=None, after_compile=None):
        original = subprocess.run
        opened = []

        def execute(command, **options):
            if command[0] == 'open':
                opened.append(command)
                return subprocess.CompletedProcess(command, 0)
            result = original(command, **options)
            if after_compile and any(str(arg).endswith('run_source_hash.cpp') for arg in command):
                after_compile()
            return result

        args = argv or [str(self.source), str(self.executable), '--', *self.command]
        with patch.object(subprocess, 'run', side_effect=execute), contextlib.redirect_stderr(io.StringIO()):
            code = run_cpp.main(args)
        return code, opened

    def test_precompile_only_updates_cache_without_opening_executable(self):
        args = ['--precompile-only', str(self.source), str(self.executable), '--', *self.command]
        code, opened = self.run_without_opening(args)
        self.assertEqual(code, 0)
        self.assertFalse(opened)
        self.assertFalse(self.executable.exists())
        self.assertEqual(len(self.artifacts()), 1)

    def test_failed_rebuild_keeps_old_executable_closed(self):
        self.assertEqual(self.compile(), 9)
        before = self.executable.read_bytes()
        self.dependency.write_text('this is invalid C++;\n')
        code, opened = self.run_without_opening()
        self.assertEqual(code, 1)
        self.assertFalse(opened)
        self.assertEqual(self.executable.read_bytes(), before)

    def test_header_edit_during_compilation_keeps_executable_closed(self):
        code, opened = self.run_without_opening(
            after_compile=lambda: self.dependency.write_text('#define VALUE 8\n'))
        self.assertEqual(code, 1)
        self.assertFalse(opened)


if __name__ == '__main__':
    unittest.main()
