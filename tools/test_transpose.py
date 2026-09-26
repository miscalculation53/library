import contextlib
import io
import os
from pathlib import Path
import random
import shlex
import shutil
import subprocess
import tempfile
import unittest

from transpose import Error, Generator, main


ROOT = Path(__file__).resolve().parents[1]
SOURCE = '''
def stateful(x: Vec, picks: "const std::vector<int>&", start: "int", stop: "int", step: "int") -> "len(x)":
    y = copy(x)
    acc = scalar()
    for p in picks:
        y[p] = 2 * y[p] + x[0]
        y[0] += y[p] - x[0]
        y[p] -= y[p] + x[p]
    for i in range(start, stop, step):
        coefficient = 3 * i - 2
        t = y[i] + acc
        acc += x[i]
        y[i] = coefficient * t
        scratch = zeros(2)
        scratch[0] = 2 * y[i]
        y[(i + 1) % len(x)] += scratch[0]
        if i % 2 == 0:
            u = x[i] - acc
            y[i] += u
        else:
            y[i] *= -2
    y[0] += acc
    return y

def vector_parts(x: Vec, n: "int", left: "int", right: "int") -> "right - left":
    a = resize(x, n)
    b = slice(a, left, right)
    c = reverse(b)
    d = copy(c)
    for i in range(len(d)):
        d[i] += c[i]
    return d

def nested(x: Vec, n: "int") -> "n":
    a = vector_parts(x, n, 0, n)
    b = vector_parts(a, len(x), 0, len(x))
    c = resize(b, n)
    return c

def identity(x: Vec) -> "len(x)":
    return x

def overwrite(x: Vec) -> "len(x)":
    for i in range(len(x)):
        x[i] = 0
    return x
'''


def cpp_vector(values):
    return "{" + ", ".join(map(str, values)) + "}"


class TransposeTest(unittest.TestCase):
    def test_rejections(self):
        bodies = [
            "x[0] = x[0] * x[1]", "x[0] += 1", "x[0] = 1 / x[0]",
            "x[0] /= x[1]", "x[x[0]] += x[1]", "x[0] = abs(x[1])",
            "if x[0] > 0:\n        x[0] += x[1]",
            "for i in range(x[0]):\n        x[i] += x[0]",
            "y = convolution(x, x)", "y = zeros(x[0])",
            "n = len(x)\n    n = 3", "x = copy(x)",
            "for i in range(0, 3, 0):\n        x[i] += x[0]",
            "while True:\n        x[0] += x[1]",
            "a = b = x[0]", "x[0] = unknown", "class_ = len(x[0])",
            "auto = x[0]", "_lt_seed = x[0]",
            "if len(x) > 0:\n        y = copy(x)\n    x[0] = y[0]",
        ]
        for body in bodies:
            with self.subTest(body=body), self.assertRaises(Error):
                Generator('def f(x: Vec) -> "len(x)":\n    ' + body + '\n    return x\n')
        for source in [
            'import os', 'def f(x):\n    return x',
            'def f(x: Vec):\n    return x',
            'def f(x: Vec, a: int) -> "len(x)":\n    return x',
            'def f(x: Vec) -> "x[0]":\n    return x',
            'def f(x: Vec) -> "len(x)":\n    return x\ndef f_transpose(x: Vec) -> "len(x)":\n    return x',
        ]:
            with self.subTest(source=source), self.assertRaises(Error):
                Generator(source)

    def test_cli_and_committed_headers(self):
        with tempfile.TemporaryDirectory() as directory, contextlib.redirect_stderr(io.StringIO()):
            path = Path(directory)
            source, output = path / "f.lin.py", path / "f.hpp"
            source.write_text(SOURCE)
            self.assertEqual(main([str(source), "-o", str(output)]), 0)
            self.assertEqual(main([str(source), "-o", str(output), "--check"]), 0)
            generated = output.read_text()
            output.write_text("old")
            self.assertEqual(main([str(source), "-o", str(output), "--check"]), 1)
            self.assertEqual(output.read_text(), "old")
            source.write_text("import os")
            self.assertEqual(main([str(source), "-o", str(output)]), 2)
            self.assertEqual(output.read_text(), "old")
            self.assertEqual(generated, Generator(SOURCE).generate())
        for name in ["basic", "polynomial"]:
            base = ROOT / "tools/transpose_examples" / name
            self.assertEqual(base.with_suffix(".generated.hpp").read_text(), Generator(base.with_suffix(".lin.py").read_text()).generate())

    def test_compiled_against_dense_python_matrices(self):
        # Each expected matrix is obtained by executing the forward specification
        # on basis vectors in Python, independently of the reverse generator.
        environment = {
            "Vec": list, "zeros": lambda n: [0] * n, "scalar": lambda: 0,
            "copy": lambda a: a[:], "slice": lambda a, l, r: a[l:r],
            "resize": lambda a, n: (a + [0] * n)[:n], "reverse": lambda a: a[::-1],
        }
        exec(SOURCE, environment)
        rng = random.Random(891326)
        cases = []
        for n in range(1, 9):
            for start, stop, step in [(0, n, 1), (n - 1, -1, -1), (0, n, 2), (n - 1, -1, -3), (n, 0, 1), (0, n, -2)]:
                picks = [rng.randrange(n) for _ in range(5)]
                cases.append(("stateful", n, [picks, start, stop, step]))
        for n in range(5):
            for m in range(5):
                cases.append(("vector_parts", n, [m, 0, m]))
                cases.append(("vector_parts", n, [m, m // 2, m]))
                cases.append(("nested", n, [m]))
            cases.extend([(name, n, []) for name in ["identity", "overwrite"]])
        statements = []
        for name, n, args in cases:
            forward = environment[name]
            x = [rng.randrange(-3, 4) for _ in range(n)]
            expected = forward(x[:], *args)
            w = [rng.randrange(-3, 4) for _ in expected]
            transposed = []
            for i in range(n):
                unit = [int(i == j) for j in range(n)]
                transposed.append(sum(a * b for a, b in zip(w, forward(unit, *args))))
            suffix = "".join(", " + (cpp_vector(arg) if isinstance(arg, list) else str(arg)) for arg in args)
            statements += [
                f"assert(({name}<long long>({cpp_vector(x)}{suffix}) == std::vector<long long>{cpp_vector(expected)}));",
                f"assert(({name}_transpose<long long>({cpp_vector(w)}, {n}{suffix}) == std::vector<long long>{cpp_vector(transposed)}));",
            ]
        # Division is field division; T is a modular integer for this case.
        division = '''
def field_division(x: Vec) -> "len(x)":
    for i in range(len(x)):
        x[i] = x[i] / 3
        x[i] /= 2
    return x
'''
        statements += [
            "using M = modint998244353;",
            "assert((field_division<M>({1, 2}) == std::vector<M>{M(1)/6, M(2)/6}));",
            "assert((field_division_transpose<M>({1, 2}, 2) == std::vector<M>{M(1)/6, M(2)/6}));",
        ]
        cxx = shlex.split(os.environ.get("CXX", shutil.which("g++-15") or "g++"))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            (path / "test.hpp").write_text(Generator(SOURCE + division).generate())
            (path / "test.cpp").write_text('#include "test.hpp"\nint main() {\n' + "\n".join(statements) + "\n}\n")
            subprocess.run(cxx + ["-std=c++17", "-O1", "-D_GLIBCXX_DEBUG", "-D_GLIBCXX_ASSERTIONS", "-I", str(ROOT), str(path / "test.cpp"), "-o", str(path / "test")], check=True)
            subprocess.run([str(path / "test")], check=True)


if __name__ == "__main__":
    unittest.main()
