"""Generate quotient-prefix input and compare convolution or division methods."""

import argparse
import json
import math
import random

from multiplicative_prefix_sum import measure


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary")
    parser.add_argument("--n", type=int, required=True)
    parser.add_argument("--seed", type=int, default=18631)
    parser.add_argument("--repeat", type=int, default=1)
    parser.add_argument("--timeout", type=float, default=60)
    parser.add_argument("--operation", choices=("product", "division"), default="product")
    args = parser.parse_args()
    assert 1 <= args.n <= 10**12 and args.repeat >= 1
    root = math.isqrt(args.n)
    count = root + args.n // (root + 1)
    rng = random.Random(args.seed)
    data = f"1\n{args.n}\n".encode()
    for _ in range(2):
        data += (" ".join(str(rng.randrange(1, 998244353)) for _ in range(count)) + "\n").encode()
    expected = None
    for repeat in range(args.repeat):
        for method in (("direct", "auto") if args.operation == "product" else ("divide-direct", "divide")):
            result = measure([args.binary, method], data, args.timeout)
            result.update(n=args.n, seed=args.seed, repeat=repeat, method=method, operation=args.operation)
            print(json.dumps(result), flush=True)
            if result["exit_code"] != 0:
                raise RuntimeError("benchmark failed or timed out")
            if expected is None:
                expected = result["output_sha256"]
            elif expected != result["output_sha256"]:
                raise RuntimeError("answers differ between methods or runs")


if __name__ == "__main__":
    main()
