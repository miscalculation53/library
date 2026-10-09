"""Measure one evaluator's peak RSS in a fresh child process (macOS/Linux)."""
import resource
import subprocess
import sys

executable, modulus, size, variant = sys.argv[1:]
run = subprocess.run(
    [executable, "memory", modulus, size, variant],
    check=True, capture_output=True, text=True,
)
peak = resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss
if sys.platform != "darwin":
    peak *= 1024
print(f"variant={variant}")
print(f"mod={modulus}")
print(f"n={size}")
print(f"checksum={run.stdout.strip()}")
print(f"peak_rss_bytes={peak}")
