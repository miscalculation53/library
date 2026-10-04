#!/usr/bin/env python3
"""Build the current code and frozen references into one benchmark executable."""
import argparse
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
BASELINE = "414acd1acbaecc187ff792f36613efda28090fca"
SOURCES = {
    "math/bigint.hpp": ["BigInteger"],
    "math/quadratic_equation_integer.hpp": ["quadratic_equation_integer"],
    "math/linear_equations_integer.hpp": ["LinearEquationsIntegerResult", "linear_equations_integer"],
    "math/svp2d.hpp": ["svp2d"],
    "math/modint/modint_internal_static.hpp": ["policy_static"],
    "math/crt.hpp": ["crt2", "crt", "crt_mod", "crt_mod_constexpr", "pre_crt"],
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="g++-15")
    parser.add_argument("--repeats", default=7, type=int)
    parser.add_argument("--output", type=Path, default=ROOT / "benchmarks/runtime_width.csv")
    args = parser.parse_args()
    if args.repeats < 1:
        parser.error("--repeats must be positive")
    with tempfile.TemporaryDirectory(prefix="runtime-width-") as directory:
        work = Path(directory)
        references = []
        for name, symbols in SOURCES.items():
            source = subprocess.check_output(["git", "show", f"{BASELINE}:{name}"], cwd=ROOT, text=True)
            if name == "math/crt.hpp":
                # Keep the reference correct for large moduli; the original helper
                # used ll for an intermediate multiplication in safemod.
                source = source.replace("T t = safemod(", "T t = safemod<T>(")
            for symbol in symbols:
                source = re.sub(r"\b" + symbol + r"\b", symbol + "_before", source)
            def include(match):
                path = match[1]
                if path.startswith(".") or (ROOT / name).parent.joinpath(path).is_file():
                    path = str(((ROOT / name).parent / path).resolve().relative_to(ROOT))
                return f'#include "{path}"'
            source = re.sub(r'#include "([^"]+)"', include, source)
            references.append(source.replace("#pragma once", ""))
        (work / "runtime_width_reference.hpp").write_text("\n".join(references))
        binary = work / "bench"
        subprocess.run([args.compiler, "-std=c++17", "-O2", "-DNDEBUG", "-I", str(ROOT),
                        "-I", str(work), str(ROOT / "benchmarks/runtime_width.cpp"),
                        "-o", str(binary)], check=True)
        with args.output.open("w") as output:
            subprocess.run([str(binary), str(args.repeats)], stdout=output, check=True)
        print(args.output)


if __name__ == "__main__":
    main()
