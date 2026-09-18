#!/usr/bin/env python3
"""Measure cln on frozen bundles from several library combinations."""
from __future__ import annotations

import argparse
from contextlib import redirect_stderr
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
FIXTURES = {
    "affine": "verify/yosupo/range_affine_range.sum.test.cpp",
    "tree": "verify/yosupo/vertex_set_path_composite.test.cpp",
    "multipoint": "verify/yosupo/poly_multipoint_evaluation.test.cpp",
    "scc": "verify/yosupo/scc.test.cpp",
    "matrix": "verify/yosupo/matrix_det.test.cpp",
    "factorize": "verify/yosupo/factorize.test.cpp",
    "rolling": "verify/mytest/ai/rolling_hash_string_interop.test.cpp",
    "cht": "verify/mytest/ai/convex_hull_trick.test.cpp",
    "minimum_cycle": "benchmark/submit_code_minimum_cycle.cpp",
    "beats": "verify/yosupo/range_chmin_chmax_add_range_sum.test.cpp",
    "fps_composition": "verify/yosupo/fps_compositional_inverse.test.cpp",
    "bbla_rank": "verify/yosupo/bbla_rank.test.cpp",
    "dirichlet": "verify/yosupo/dirichlet_inverse_and_prefix_sums.test.cpp",
    "mod2": "verify/yosupo/matrix_product_mod2_dynamic.test.cpp",
    "rectangles": "verify/yosupo/area_of_union_of_rectangles.test.cpp",
    "potential": "benchmark/submit_code_potential.cpp",
    "inversions": "verify/yosupo/static_range_inversions_query.test.cpp",
    "convolution_dynamic": "verify/yosupo/convolution_arbitrary_dynamic.test.cpp",
    "bigint": "verify/yosupo/big_integer_division.test.cpp",
    "aho": "verify/yosupo/aho_corasick.test.cpp",
    "connectivity": "verify/yosupo/offline_dynamic_connectivity.test.cpp",
}


def measure(tool, case: str, directory: Path, label: str, cxx: str, cppcheck: str | None) -> dict:
    fixture = directory / f"{case}.cpp"
    if not fixture.exists():
        original = ROOT / FIXTURES[case]
        bundled = tool.FastBundler(tool.default_include_paths(ROOT, original)).bundle(original)
        fixture.write_text(bundled)
    source = fixture.read_text()
    output = directory / f"{label}-{case}.cpp"
    result = {
        "case": case,
        "fixture": FIXTURES[case],
        "input_bytes": len(source.encode()),
        "input_sha256": hashlib.sha256(source.encode()).hexdigest(),
        "first_seconds": None,
        "first_bytes": None,
        "commands": [],
        "checkpoints": [],
    }
    started = time.monotonic()

    class Reporter(tool.Reporter):
        def _emit(self, tag, message, color=None):
            super()._emit(tag, f"{time.monotonic() - started:.3f}s {message}", color)

    class Writer(tool.CheckpointWriter):
        def save(self, stage, text, *, force=False):
            saved = super().save(stage, text, force=force)
            if saved:
                seconds, size = time.monotonic() - started, len(text.encode())
                result["checkpoints"].append({"seconds": seconds, "bytes": size, "stage": stage})
                if size < 65536 and result["first_seconds"] is None:
                    result.update(first_seconds=seconds, first_bytes=size)
                    output.with_name(output.stem + "-first.cpp").write_text(text)
            return saved

    with output.with_suffix(".log").open("w") as log, redirect_stderr(log):
        reporter = Reporter(color="never", verbose=True)
        writer = Writer(output, reporter)
        writer.current = source
        writer.preserve_below = 65536
        with tool.CompilerWorkspace(
            cxx=cxx, standard="c++23",
            include_paths=tool.default_include_paths(ROOT, fixture), extra_args=[],
            timeout=120, reporter=reporter,
        ) as compiler:
            # Exclude initial PCH construction, bundling, and process startup.
            # Any later PCH construction is included and recorded separately.
            safe = tool.safe_cleanup_candidate(source, compiler)
            compiler._ensure_pch(source)
            compiler._ensure_pch(safe)
            run = compiler._run

            def traced(command):
                before = time.monotonic()
                response = run(command)
                kind = "link"
                for flag, name in [
                    ("-E", "preprocess"), ("-x", "pch"), ("-fdump-lang-raw", "templates"),
                    ("-fdump-ipa-cgraph", "ipa"), ("-fsyntax-only", "syntax"),
                    ("-c", "compile"),
                ]:
                    if flag in command:
                        kind = name
                        break
                result["commands"].append({
                    "kind": kind, "seconds": time.monotonic() - before,
                    "ok": response.returncode == 0,
                })
                return response

            compiler._run = traced
            started = time.monotonic()
            if len(source.encode()) < 65536:
                result.update(first_seconds=0.0, first_bytes=len(source.encode()))
                output.with_name(output.stem + "-first.cpp").write_text(source)
            cleaned = tool.cleanup_text(
                source, compiler=compiler, writer=writer, cppcheck=cppcheck,
                keep_functions=tool.DEFAULT_KEEP_FUNCTIONS, max_passes=3,
                safe_only=False, require_validation=True, reporter=reporter,
                target_bytes=65536,
            )
            writer.save("final", cleaned)
            result.update(total_seconds=time.monotonic() - started, final_bytes=len(cleaned.encode()))
    output.with_suffix(".json").write_text(json.dumps(result, indent=2) + "\n")
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", required=True, type=Path, help="reuse this directory for identical inputs")
    parser.add_argument("--tool", type=Path, default=ROOT / "tools/submit_code.py")
    parser.add_argument("--label", default="current")
    parser.add_argument("--cases", nargs="+", choices=FIXTURES, default=list(FIXTURES))
    parser.add_argument("--cxx", default=shutil.which("g++-15") or shutil.which("g++-14"))
    args = parser.parse_args()
    if args.cxx is None:
        parser.error("GNU C++ compiler was not found; specify --cxx")
    args.output.mkdir(parents=True, exist_ok=True)
    spec = importlib.util.spec_from_file_location("submit_code_benchmark", args.tool)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    for case in args.cases:
        result = measure(module, case, args.output, args.label, args.cxx, shutil.which("cppcheck"))
        print(json.dumps({k: v for k, v in result.items() if k not in {"commands", "checkpoints"}}), flush=True)


if __name__ == "__main__":
    main()
