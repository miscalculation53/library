"""Run the compiled benchmark sequentially; compare answers and record time/RSS.

Example:
  python3 benchmark/multiplicative_prefix_sum.py /tmp/mpbench --n 10000000000000 --repeat 3
"""

import argparse
import hashlib
import json
import os
import subprocess
import sys
import tempfile
import time


def measure(command, data, timeout):
    with tempfile.TemporaryFile() as source, tempfile.TemporaryFile() as out, tempfile.TemporaryFile() as err:
        source.write(data)
        source.seek(0)
        start = time.perf_counter()
        process = subprocess.Popen(command, stdin=source, stdout=out, stderr=err)
        timed_out = False
        while True:
            pid, status, usage = os.wait4(process.pid, os.WNOHANG)
            if pid:
                break
            if time.perf_counter() - start >= timeout:
                timed_out = True
                process.kill()
                _, status, usage = os.wait4(process.pid, 0)
                break
            time.sleep(0.005)
        elapsed = time.perf_counter() - start
        process.returncode = os.waitstatus_to_exitcode(status)
        out.seek(0)
        err.seek(0)
        answer = out.read()
        # ru_maxrss is bytes on macOS and KiB on Linux.
        rss_unit = 1 if sys.platform == "darwin" else 1024
        return {
            "command": command,
            "wall_seconds": round(elapsed, 6),
            "cpu_seconds": round(usage.ru_utime + usage.ru_stime, 6),
            "peak_rss_mib": round(usage.ru_maxrss * rss_unit / 1024**2, 3),
            "exit_code": process.returncode,
            "timeout": timed_out,
            "output_sha256": hashlib.sha256(answer).hexdigest(),
            "output_preview": answer[:160].decode(),
            "stages": err.read().decode().strip(),
        }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary")
    parser.add_argument("--n", type=int, required=True)
    parser.add_argument("--cases", type=int, default=1)
    parser.add_argument("--a", type=int, default=123456789)
    parser.add_argument("--b", type=int, default=234567890)
    parser.add_argument("--repeat", type=int, default=1)
    parser.add_argument("--methods", nargs="+", choices=["auto", "sieve", "sieve23"],
                        default=["auto", "sieve", "sieve23"])
    parser.add_argument("--timeout", type=float, default=90)
    args = parser.parse_args()
    assert args.n >= 1 and args.cases >= 1 and args.repeat >= 1
    data = (f"{args.cases}\n" + f"{args.n} {args.a} {args.b}\n" * args.cases).encode()
    expected = None
    for repeat in range(args.repeat):
        for method in args.methods:
            result = measure([args.binary, method], data, args.timeout)
            result.update(n=args.n, cases=args.cases, a=args.a, b=args.b, repeat=repeat, method=method)
            print(json.dumps(result), flush=True)
            if result["exit_code"] != 0:
                raise RuntimeError("benchmark failed or timed out")
            if expected is None:
                expected = result["output_sha256"]
            elif result["output_sha256"] != expected:
                raise RuntimeError("answers differ between methods or runs")


if __name__ == "__main__":
    main()
