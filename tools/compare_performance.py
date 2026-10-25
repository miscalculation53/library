#!/usr/bin/env python3
"""Compile the same workload against two source trees and compare local runtimes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import platform
import random
import statistics
import subprocess
import tempfile
import time

import template_pch


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def run_program(executable, arguments, input_path, timeout):
    with input_path.open('rb') as source, tempfile.TemporaryFile() as output:
        started = time.perf_counter_ns()
        result = subprocess.run([str(executable), *arguments], stdin=source,
                                stdout=output, stderr=subprocess.PIPE, timeout=timeout)
        elapsed = (time.perf_counter_ns() - started) / 1e6
        if result.returncode:
            raise RuntimeError(f'workload failed with status {result.returncode}: '
                               + result.stderr.decode(errors='replace')[-2000:])
        output.seek(0)
        checksum = hashlib.file_digest(output, 'sha256').hexdigest()
        return elapsed, checksum


def compare(executables, arguments, input_path, repetitions, timeout, seed):
    samples = {'before': [], 'after': []}
    expected = None
    orders = [['before', 'after'] if i % 2 == 0 else ['after', 'before']
              for i in range(repetitions)]
    random.Random(seed).shuffle(orders)
    for order in [['before', 'after'], *orders]:
        for name in order:
            elapsed, checksum = run_program(executables[name], arguments, input_path, timeout)
            if expected is None:
                expected = checksum
            if checksum != expected:
                raise RuntimeError(f'{name}: workload output differs; no timing result accepted')
            samples[name].append(elapsed)
    for values in samples.values():
        del values[0]  # one warmup per executable
    before, after = (statistics.median(samples[name]) for name in ('before', 'after'))
    return {'samples_wall_ms': samples, 'median_wall_ms': {'before': before, 'after': after},
            'before_over_after': before / after, 'stdout_sha256': expected}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('--before-root', type=Path, required=True)
    parser.add_argument('--after-root', type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument('--compiler', default='g++-15')
    parser.add_argument('--flag', action='append', help='repeat as --flag=-O2; replaces default flags')
    parser.add_argument('--arg', action='append', default=[], help='workload argument, repeatable')
    parser.add_argument('--input', type=Path)
    parser.add_argument('--repetitions', type=int, default=7)
    parser.add_argument('--seed', type=int, default=20261022)
    parser.add_argument('--timeout', type=float, default=120)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args(argv)
    if args.repetitions < 3 or args.timeout <= 0:
        parser.error('use at least three repetitions and a positive timeout')
    flags = args.flag or ['-std=c++20', '-O2', '-DNDEBUG']
    try:
        with tempfile.TemporaryDirectory(prefix='library-performance-') as temporary:
            directory = Path(temporary).resolve()
            empty = directory / 'empty.in'
            empty.write_bytes(b'')
            input_path = args.input or empty
            input_checksum = digest(input_path)
            source_checksum = digest(args.source)
            # Isolate quoted includes from the original workload's directory.
            # Workloads resolve repository-root-relative includes through -I.
            workload = directory / ('workload' + args.source.suffix)
            workload.write_bytes(args.source.read_bytes())
            executables = {}
            snapshots = {}
            inputs = {}
            roots = {'before': args.before_root.resolve(), 'after': args.after_root.resolve()}
            for name, root in roots.items():
                executable = directory / name
                depfile = directory / (name + '.d')
                subprocess.run([args.compiler, *flags, '-I', str(root.resolve()),
                                '-MM', '-MF', str(depfile), '-MT', name,
                                str(workload)], check=True)
                snapshots[name] = {path: digest(path) for path in template_pch.dependencies(depfile)}
                other = roots['after' if name == 'before' else 'before']
                if root != other:
                    for path in snapshots[name]:
                        if path.is_relative_to(other) and (not root.is_relative_to(other)
                                                          or not path.is_relative_to(root)):
                            raise RuntimeError(f'{name}: dependency resolves to the other source tree: '
                                               + str(path.relative_to(other)))
                subprocess.run([args.compiler, *flags, '-I', str(root.resolve()),
                                str(workload), '-o', str(executable), '-lquadmath'], check=True)
                executables[name] = executable
                inputs[name] = {
                                '$WORKLOAD' if path == workload else
                                str(path.relative_to(root.resolve())) if path.is_relative_to(root.resolve())
                                else str(path).replace(str(Path.home()), '$HOME'): value
                                for path, value in snapshots[name].items()}
            result = compare(executables, args.arg, input_path, args.repetitions, args.timeout, args.seed)
            if any(digest(path) != value for files in snapshots.values() for path, value in files.items()):
                raise RuntimeError('source dependencies changed during the comparison; repeat with fixed source trees')
            if digest(input_path) != input_checksum or digest(args.source) != source_checksum:
                raise RuntimeError('workload source or input changed during the comparison; repeat with fixed files')
            version = subprocess.run([args.compiler, '--version'], check=True,
                                     stdout=subprocess.PIPE, text=True).stdout.splitlines()[0]
            target = subprocess.run([args.compiler, '-dumpmachine'], check=True,
                                    stdout=subprocess.PIPE, text=True).stdout.strip()
            result.update({'compiler': version, 'target': target,
                           'host_system': platform.system(), 'host_machine': platform.machine(),
                           'flags': [flag.replace(str(Path.home()), '$HOME') for flag in flags],
                           'source': args.source.name, 'source_sha256': source_checksum,
                           'arguments': [value.replace(str(Path.home()), '$HOME') for value in args.arg],
                           'input_sha256': input_checksum,
                           'executable_sha256': {name: digest(path) for name, path in executables.items()},
                           'dependencies_sha256': inputs,
                           'repetitions': args.repetitions, 'seed': args.seed,
                           'measurement': 'local process wall time, including startup; one warmup per executable'})
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_text(json.dumps(result, indent=2, ensure_ascii=False) + '\n')
            print(f"before/after: {result['before_over_after']:.3f}; output checksum matched")
        return 0
    except (OSError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(1, f'performance comparison: {error}\n')


if __name__ == '__main__':
    raise SystemExit(main())
