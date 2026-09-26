#!/usr/bin/env python3
"""Check every answer at large N using seven distinct evaluation points."""

import argparse
from pathlib import Path
import random
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", type=Path)
    parser.add_argument("--n", type=int, default=250000)
    args = parser.parse_args()
    assert args.n >= 1
    mod = 998244353
    rng = random.Random(712367)
    roots = [rng.randrange(mod) for _ in range(7)]
    group = [rng.randrange(len(roots)) for _ in range(args.n)]
    a = [roots[i] for i in group]
    b = [rng.randrange(mod) for _ in a]
    c = [rng.randrange(mod) for _ in a]
    data = str(args.n) + "\n" + "\n".join(" ".join(map(str, row)) for row in [a, b, c]) + "\n"
    start = time.perf_counter()
    result = subprocess.run([str(args.executable.resolve())], input=data, text=True, capture_output=True, check=True)
    seconds = time.perf_counter() - start
    actual = list(map(int, result.stdout.split()))
    assert len(actual) == args.n, (len(actual), args.n)
    weights = [0] * len(roots)
    for i, weight in zip(group, c):
        weights[i] = (weights[i] + weight) % mod
    for k, bk in enumerate(b):
        for i, root in enumerate(roots):
            weights[i] = weights[i] * (root + bk) % mod
        expected = sum(weights) % mod
        assert actual[k] == expected, (k, actual[k], expected)
    print(f"N={args.n}: all {args.n} answers verified; executable {seconds:.3f} s")


if __name__ == "__main__":
    main()
