from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import tomllib
import unittest

import tools_test_paths


class VerifyLinkTest(unittest.TestCase):
    def test_configured_environments_link_quadmath_after_source(self):
        compiler = shutil.which("g++-15") or shutil.which("g++-14") or shutil.which("g++")
        if compiler is None:
            self.skipTest("GNU C++ compiler is unavailable")
        version = subprocess.check_output([compiler, "--version"], text=True)
        if "clang" in version.lower():
            self.skipTest("GNU C++ compiler is unavailable")
        root = tools_test_paths.REPOSITORY_ROOT
        config = tomllib.loads((root / ".verify-helper/config.toml").read_text())
        environments = config["languages"]["cpp"]["environments"]
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "quadmath.cpp"
            executable = Path(directory) / "quadmath"
            source.write_text("""#include <quadmath.h>
#include <cstdio>
int main(int argc, char **argv) {
  if (argc != 2) return 1;
  char *end;
  __float128 x = strtoflt128(argv[1], &end);
  if (*end) return 2;
  char output[64];
  quadmath_snprintf(output, sizeof(output), "%.4Qf", sqrtq(x));
  std::puts(output);
}
""")
            for environment in environments:
                with self.subTest(flags=environment["CXXFLAGS"]):
                    command = [compiler, *environment["CXXFLAGS"]]
                    if sys.platform.startswith("linux"):
                        command.append("-Wl,--as-needed")
                    command.extend([str(source), "-o", str(executable)])
                    build = subprocess.run(command, cwd=root, capture_output=True, text=True)
                    self.assertEqual(build.returncode, 0, build.stderr)
                    result = subprocess.run(
                        [str(executable), "2.25"], capture_output=True, text=True
                    )
                    self.assertEqual(result.returncode, 0, result.stderr)
                    self.assertEqual(result.stdout, "1.5000\n")


if __name__ == "__main__":
    unittest.main()
