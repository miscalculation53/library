import contextlib
import io
import os
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

    def compile(self, command=None, source=None):
        source = source or self.source
        with template_pch.prepare(source, command or self.command) as prepared:
            result = subprocess.run([*prepared.command, '-H'], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn('! ', result.stderr)
            self.assertIn('template.hpp.gch', result.stderr)
            self.assertNotIn('not used because', result.stderr)
            self.assertTrue(prepared.unchanged())
        result = subprocess.check_output([str(self.executable)], text=True)
        number, filename = result.strip().split('\t')
        self.assertEqual(filename, str(source))
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

    def test_problem_directories_and_comments_share_pch(self):
        command = [*self.command, '-I', str(self.root)]
        self.assertEqual(self.compile(command), 9)
        before = self.artifacts()
        other = self.root / 'other problem'
        other.mkdir()
        source = other / 'main.cpp'
        source.write_text(self.source.read_text().replace(
            '#define EPS 4\n', '// problem comment\n\n#define EPS 4 // tolerance\n').replace(
            '__LINE__ == 5', '__LINE__ == 7'))
        (other / 'relative.hpp').write_text('#define EXTRA 7\n')
        other_command = [str(source) if arg == str(self.source) else arg for arg in command]
        previous = Path.cwd()
        try:
            os.chdir(other)
            self.assertEqual(self.compile(other_command, source), 14)
        finally:
            os.chdir(previous)
        self.assertEqual(self.artifacts(), before)
        self.assertEqual(self.compile(command), 9)
        self.assertEqual(self.artifacts(), before)

    def test_macro_variants_remain_cached_when_switching_back(self):
        self.assertEqual(self.compile(), 9)
        original = self.source.read_text()
        self.source.write_text(original.replace('EPS 4', 'EPS 6'))
        self.assertEqual(self.compile(), 11)
        before = self.artifacts()
        self.assertEqual(len(before), 2)
        self.source.write_text(original)
        self.assertEqual(self.compile(), 9)
        self.assertEqual(self.artifacts(), before)

    def test_touch_preserves_pch_and_run_checks_use_current_timestamps(self):
        self.assertEqual(self.compile(), 9)
        before = self.artifacts()
        stat = self.dependency.stat()
        os.utime(self.dependency, ns=(stat.st_atime_ns, stat.st_mtime_ns + 1_000_000_000))
        self.assertEqual(self.compile(), 9)
        self.assertEqual(self.artifacts(), before)
        code, opened = self.run_without_opening()
        self.assertEqual(code, 0)
        self.assertTrue(opened)

    def test_source_relative_dependencies_get_separate_pch(self):
        self.header.write_text(self.header.read_text().replace('"value.hpp"', '"selected.hpp"'))
        (self.root / 'selected.hpp').write_text('#define VALUE 3\n')
        command = [*self.command, '-I', str(self.root)]
        self.assertEqual(self.compile(command), 9)
        other = self.root / 'other problem'
        other.mkdir()
        source = other / 'main.cpp'
        source.write_text(self.source.read_text())
        (other / 'relative.hpp').write_text('#define EXTRA 2\n')
        (other / 'selected.hpp').write_text('#define VALUE 20\n')
        other_command = [str(source) if arg == str(self.source) else arg for arg in command]
        self.assertEqual(self.compile(other_command, source), 26)
        before = self.artifacts()
        self.assertEqual(len(before), 2)
        self.assertEqual(self.compile(command), 9)
        self.assertEqual(self.artifacts(), before)

    def test_new_source_relative_header_refreshes_resolution(self):
        common = self.root / 'common'
        common.mkdir()
        (common / 'selected.hpp').write_text('#define VALUE 3\n')
        self.header.write_text(self.header.read_text().replace('"value.hpp"', '"selected.hpp"'))
        command = [*self.command, '-I', str(common)]
        self.assertEqual(self.compile(command), 9)
        self.assertEqual(self.compile(command), 9)
        (self.root / 'selected.hpp').write_text('#define VALUE 20\n')
        self.assertEqual(self.compile(command), 26)
        self.assertEqual(len(self.artifacts()), 2)

    def test_new_header_in_earlier_subdirectory_refreshes_resolution(self):
        first, second = self.root / 'first', self.root / 'second'
        for directory in (first, second):
            (directory / 'sub').mkdir(parents=True)
        (second / 'sub/selected.hpp').write_text('#define VALUE 3\n')
        self.header.write_text(self.header.read_text().replace('"value.hpp"', '<sub/selected.hpp>'))
        command = [*self.command, '-I', str(first), '-I', str(second)]
        self.assertEqual(self.compile(command), 9)
        self.assertEqual(self.compile(command), 9)
        (first / 'sub/selected.hpp').write_text('#define VALUE 20\n')
        self.assertEqual(self.compile(command), 26)
        self.assertEqual(len(self.artifacts()), 2)

    def test_new_quoted_header_and_has_include_refresh_resolution(self):
        common = self.root / 'common'
        (common / 'sub').mkdir(parents=True)
        (self.templates / 'sub').mkdir()
        (common / 'sub/selected.hpp').write_text('#define VALUE 3\n')
        self.header.write_text(self.header.read_text().replace(
            '#include "value.hpp"', '#include "sub/selected.hpp"\n'
            '#if __has_include("sub/optional.hpp")\n#include "sub/optional.hpp"\n#endif'))
        command = [*self.command, '-I', str(common)]
        self.assertEqual(self.compile(command), 9)
        (self.templates / 'sub/selected.hpp').write_text('#define VALUE 20\n')
        self.assertEqual(self.compile(command), 26)
        (self.templates / 'sub/optional.hpp').write_text('#undef VALUE\n#define VALUE 100\n')
        self.assertEqual(self.compile(command), 106)
        self.assertEqual(len(self.artifacts()), 3)

    def test_location_dependent_and_continued_macros_preserve_their_text(self):
        for definition in ['#define EPS __LINE__', '#define EPS (2 + \\\n2)']:
            with self.subTest(definition=definition):
                prefix = definition + '\n'
                self.assertEqual(template_pch.preamble(self.source, prefix),
                                 f'#line 1 "{self.source}"\n' + prefix)
        self.source.write_text(self.source.read_text().replace('EPS 4', 'EPS __LINE__'))
        self.assertEqual(self.compile(), 11)  # answer() expands EPS on line 6 of its header.

    def test_code_before_template_keeps_source_locations(self):
        self.source.write_text('constexpr auto source_file = __FILE__;\n'
                               + self.source.read_text().replace('__LINE__ == 5', '__LINE__ == 6')
                               .replace('answer() + EXTRA, __FILE__', 'answer() + EXTRA, source_file'))
        self.assertEqual(self.compile(), 9)

    def test_update_messages_explain_new_cache_and_changed_header(self):
        with contextlib.redirect_stderr(io.StringIO()) as stderr:
            self.assertEqual(self.compile(), 9)
        self.assertIn('設定に対応するキャッシュ未作成', stderr.getvalue())
        self.dependency.write_text('#define VALUE 8\n')
        with contextlib.redirect_stderr(io.StringIO()) as stderr:
            self.assertEqual(self.compile(), 14)
        self.assertIn('参照ヘッダの内容が変更: value.hpp', stderr.getvalue())

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
