from pathlib import Path
import selectors
import shutil
import subprocess
import tempfile
import unittest

import tools_test_paths


class TemplateInteractiveTest(unittest.TestCase):
    def test_fast_streams_are_ready_in_init_and_each_print_is_flushed(self):
        cxx = shutil.which('g++-15') or shutil.which('g++-14')
        if cxx is None:
            self.skipTest('GNU C++ compiler is unavailable')
        with tempfile.TemporaryDirectory(prefix='template-interactive-') as temporary:
            root = Path(temporary)
            source, executable = root / 'main.cpp', root / 'main'
            source.write_text('''#define SINGLE_TESTCASE
#define INTERACTIVE
#include "template/template_all_but_modint.hpp"
void init()
{
  assert(cin.tie() == nullptr);
  assert(!ios::sync_with_stdio(false));
}
void test() {}
void main2()
{
  repi(i, 2)
  {
    PRINT("?", i);
    INT(answer);
    PRINT("!", answer);
  }
}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
''')
            subprocess.run([cxx, '-std=c++23', '-O2', '-I',
                            str(tools_test_paths.REPOSITORY_ROOT), str(source),
                            '-o', str(executable), '-lquadmath'], check=True,
                           capture_output=True, text=True)
            with subprocess.Popen([str(executable)], stdin=subprocess.PIPE,
                                  stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                  bufsize=0) as process:
                try:
                    with selectors.DefaultSelector() as selector:
                        selector.register(process.stdout, selectors.EVENT_READ)

                        def read_line():
                            self.assertTrue(selector.select(timeout=3),
                                            'PRINT output was buffered while waiting for input')
                            return process.stdout.readline()

                        for i, answer in enumerate((42, 0)):
                            self.assertEqual(read_line(), f'? {i}\n'.encode())
                            process.stdin.write(f'{answer}\n'.encode())
                            self.assertEqual(read_line(), f'! {answer}\n'.encode())
                    process.stdin.close()
                    self.assertEqual(process.wait(timeout=3), 0, process.stderr.read())
                finally:
                    if process.poll() is None:
                        process.kill()
                        process.wait()


if __name__ == '__main__':
    unittest.main()
