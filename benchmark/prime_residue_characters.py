#!/usr/bin/env python3
"""Sequential, separate-process measurements with answer comparison (macOS/Linux)."""
import argparse
import csv
import os
import random
import subprocess
import sys
import tempfile
import time

parser = argparse.ArgumentParser()
parser.add_argument("binary")
parser.add_argument("--n", type=int, nargs="+", default=[10**11])
parser.add_argument("--q", type=int, nargs="+", default=[3, 5, 6, 7, 8, 9, 12, 16, 24, 30])
parser.add_argument("--k", type=int, nargs="+", default=[0, 1])
parser.add_argument("--repeat", type=int, default=3)
parser.add_argument("--methods", nargs="+", choices=["full", "units", "characters", "characters_sequential"],
                    default=["full", "units", "characters", "characters_sequential"])
parser.add_argument("--output", required=True)
args = parser.parse_args()
methods = args.methods
rows = [(n, q, k, method, rep) for n in args.n for q in args.q for k in args.k
        for rep in range(args.repeat) for method in methods]
random.Random(20260910).shuffle(rows)
answers = {}
with open(args.output, "w", newline="") as target:
    writer = csv.DictWriter(target, fieldnames=["n", "q", "k", "method", "repeat", "seconds", "wall_seconds", "cpu_seconds", "rss_mib", "answer"])
    writer.writeheader()
    for n, q, k, method, rep in rows:
        with tempfile.TemporaryFile() as out, tempfile.TemporaryFile() as err:
            start = time.monotonic()
            p = subprocess.Popen([args.binary, str(n), str(q), str(k), method], stdout=out, stderr=err)
            _, status, usage = os.wait4(p.pid, 0)
            p.returncode = os.waitstatus_to_exitcode(status)
            wall = time.monotonic() - start
            out.seek(0); answer = out.read().decode().strip()
            err.seek(0); timing = err.read().decode().strip()
            if p.returncode:
                raise RuntimeError((n, q, k, method, p.returncode, timing))
        key = (n, q, k)
        if key in answers and answers[key] != answer:
            raise RuntimeError(("answer mismatch", key, method))
        answers[key] = answer
        rss = usage.ru_maxrss / (1024**2 if sys.platform == "darwin" else 1024)
        writer.writerow(dict(n=n, q=q, k=k, method=method, repeat=rep, seconds=timing,
                             wall_seconds=wall, cpu_seconds=usage.ru_utime + usage.ru_stime,
                             rss_mib=rss, answer=answer))
        target.flush()
        print(n, q, k, method, rep, timing, flush=True)
