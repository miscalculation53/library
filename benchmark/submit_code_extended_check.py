#!/usr/bin/env python3
"""Check frozen original/checkpoint/final bundles against small independent oracles."""
from __future__ import annotations

import argparse
import itertools
import json
from pathlib import Path
import random
import shutil
import subprocess

MOD = 998244353
CASES = (
    'beats', 'fps_composition', 'bbla_rank', 'dirichlet', 'mod2', 'rectangles',
    'potential', 'inversions', 'convolution_dynamic', 'bigint', 'aho', 'connectivity',
)


def line(values):
    return ' '.join(map(str, values)) + '\n'


def product(a, b, mod=MOD):
    out = [0] * (len(a) + len(b) - 1)
    for i, x in enumerate(a):
        for j, y in enumerate(b):
            out[i + j] = (out[i + j] + x * y) % mod
    return out


def matmul(a, b):
    return tuple(sum(a[2*i+k] * b[2*k+j] for k in range(2)) % MOD
                 for i in range(2) for j in range(2))


def matinv(a):
    d = pow((a[0]*a[3] - a[1]*a[2]) % MOD, -1, MOD)
    return tuple(x*d % MOD for x in (a[3], -a[1], -a[2], a[0]))


def make_cases():
    rng = random.Random(20260913)
    cases = {key: [] for key in CASES}

    def add(key, source, expected):
        cases[key].append({'input': source, 'expected': str(expected).split()})

    for n in [1, 2, 7, 16, 33, 64]:
        a = [rng.randrange(-100, 101) for _ in range(n)]
        source = line([n, 120]) + line(a)
        answer = []
        for q in range(120):
            t, l = q % 4, rng.randrange(n)
            r = rng.randrange(l+1, n+1)
            source += line([t, l, r]) if t == 3 else ''
            if t == 3:
                answer.append(sum(a[l:r]))
                continue
            x = rng.randrange(-100, 101)
            source += line([t, l, r, x])
            for i in range(l, r):
                a[i] = min(a[i], x) if t == 0 else max(a[i], x) if t == 1 else a[i]+x
        add('beats', source, line(answer))

    for n in [2, 3, 8, 17, 32, 65]:
        f = [0, rng.randrange(1, MOD)] + [rng.randrange(MOD) for _ in range(n-2)]
        g = [0, pow(f[1], -1, MOD)] + [0]*(n-2)
        for k in range(2, n):
            power = [1]
            coefficient = 0
            for i in range(1, k+1):
                power = product(power, g[:k+1])[:k+1]
                coefficient = (coefficient + f[i]*power[k]) % MOD
            g[k] = -coefficient*g[1] % MOD
        add('fps_composition', line([n])+line(f), line(g))

    for n, m in [(1, 1), (2, 5), (5, 2), (7, 7), (10, 13), (16, 16)]:
        for deficient in [False, True]:
            a = [[rng.randrange(MOD) for _ in range(m)] for _ in range(n)]
            if deficient:
                a[-1] = a[0][:] if n > 1 else [0]*m
            source = line([n, m])+''.join(map(line, a))
            rank = 0
            for j in range(m):
                p = next((i for i in range(rank, n) if a[i][j]), None)
                if p is None:
                    continue
                a[p], a[rank] = a[rank], a[p]
                inverse = pow(a[rank][j], -1, MOD)
                a[rank] = [x*inverse % MOD for x in a[rank]]
                for i in range(rank+1, n):
                    factor = a[i][j]
                    a[i] = [(x-factor*y) % MOD for x, y in zip(a[i], a[rank])]
                rank += 1
            add('bbla_rank', source, rank)

    for n in [1, 2, 7, 16, 47, 100, 511]:
        f = [0, rng.randrange(1, MOD)] + [rng.randrange(MOD) for _ in range(n-1)]
        g = [0]*(n+1)
        g[1] = pow(f[1], -1, MOD)
        for i in range(2, n+1):
            g[i] = -g[1]*sum(f[d]*g[i//d] for d in range(2, i+1) if i % d == 0) % MOD
        coords = sorted({n//i for i in range(1, n+1)})
        fp = list(itertools.accumulate(f))
        gp = list(itertools.accumulate(g))
        add('dirichlet', line([1])+line([n])+line(fp[x] % MOD for x in coords),
            line(gp[x] % MOD for x in coords))

    for n, m, k in [(1, 1, 1), (3, 7, 4), (8, 65, 67), (5, 127, 129), (4, 256, 70)]:
        a = [[rng.randrange(2) for _ in range(m)] for _ in range(n)]
        b = [[rng.randrange(2) for _ in range(k)] for _ in range(m)]
        source = line([n, m, k])+''.join(''.join(map(str, row))+'\n' for row in a+b)
        out = [''.join(str(sum(a[i][t]*b[t][j] for t in range(m)) % 2)
                       for j in range(k)) for i in range(n)]
        add('mod2', source, '\n'.join(out))

    for n in [1, 2, 5, 10, 30, 60]:
        rectangles = []
        cells = set()
        for _ in range(n):
            x1, x2 = sorted(rng.sample(range(21), 2))
            y1, y2 = sorted(rng.sample(range(21), 2))
            rectangles.append((x1, y1, x2, y2))
            cells.update(itertools.product(range(x1, x2), range(y1, y2)))
        add('rectangles', line([n])+''.join(map(line, rectangles)), len(cells))

    for n in [1, 2, 5, 12, 30]:
        graph = [[] for _ in range(n)]
        def path(s, t):
            seen = {s: (1, 0, 0, 1)}
            queue = [s]
            for v in queue:
                for w, a in graph[v]:
                    if w not in seen:
                        seen[w] = matmul(seen[v], a)
                        queue.append(w)
            return seen.get(t)
        source, answer = line([n, 120]), []
        for q in range(120):
            u, v = rng.randrange(n), rng.randrange(n)
            known = path(v, u)
            if q % 3 == 2:
                source += line([1, u, v])
                answer += [-1] if known is None else list(known)
                continue
            while True:
                a = tuple(rng.randrange(MOD) for _ in range(4))
                if (a[0]*a[3]-a[1]*a[2]) % MOD:
                    break
            if q % 5 == 0 and known is not None:
                a = known
            source += line([0, u, v, *a])
            answer.append(int(known is None or a == known))
            if known is None:
                graph[v].append((u, a))
                graph[u].append((v, matinv(a)))
        add('potential', source, line(answer))

    for n in [1, 2, 10, 31, 70, 200]:
        a = [rng.randrange(100) for _ in range(n)]
        queries = []
        answer = []
        for _ in range(50):
            l = rng.randrange(n)
            r = rng.randrange(l+1, n+1)
            queries.append((l, r))
            answer.append(sum(a[i] > a[j] for i in range(l, r) for j in range(i+1, r)))
        add('inversions', line([n, len(queries)])+line(a)+''.join(map(line, queries)), line(answer))

    for n, m in [(1, 1), (1, 200), (8, 17), (61, 70), (129, 140), (256, 300)]:
        a = [rng.randrange(1000000007) for _ in range(n)]
        b = [rng.randrange(1000000007) for _ in range(m)]
        add('convolution_dynamic', line([n, m])+line(a)+line(b), line(product(a, b, 1000000007)))

    for digits in [1, 10, 100, 1000, 3000]:
        pairs = [(0, 1), (1, 1)]
        for _ in range(8):
            a = rng.randrange(10**(digits-1), 10**digits)
            b = rng.randrange(1, 10**max(1, digits//2))
            for x, y in [(a, b), (-a, b), (a, -b), (-a, -b)]:
                pairs.append((x, y))
        add('bigint', line([len(pairs)])+''.join(map(line, pairs)),
            ''.join(line(divmod(a, b)) for a, b in pairs))

    for n in [1, 2, 5, 15, 50, 100]:
        strings = [''.join(rng.choice('abc') for _ in range(rng.randrange(1, 16))) for _ in range(n)]
        ids = {'': 0}
        parents = []
        for s in strings:
            for k in range(1, len(s)+1):
                if s[:k] not in ids:
                    ids[s[:k]] = len(ids)
                    parents.append(ids[s[:k-1]])
        answer = [len(ids)]
        for s, i in list(ids.items())[1:]:
            suffix = next(s[k:] for k in range(1, len(s)+1) if s[k:] in ids)
            answer.extend((parents[i-1], ids[suffix]))
        answer.extend(ids[s] for s in strings)
        add('aho', line([n])+'\n'.join(strings)+'\n', line(answer))

    for n in [1, 2, 6, 15, 40]:
        a = [rng.randrange(100) for _ in range(n)]
        edges = set()
        source, answer = line([n, 150])+line(a), []
        for q in range(150):
            missing = [(u, v) for u in range(n) for v in range(u+1, n) if (u, v) not in edges]
            t = q % 4
            if t == 0 and missing:
                u, v = rng.choice(missing)
                edges.add((u, v))
                source += line([t, u, v])
            elif t == 1 and edges:
                u, v = rng.choice(sorted(edges))
                edges.remove((u, v))
                source += line([t, u, v])
            elif t == 2:
                v, x = rng.randrange(n), rng.randrange(100)
                source += line([t, v, x])
                a[v] += x
            else:
                v = rng.randrange(n)
                source += line([3, v])
                component = {v}
                while True:
                    extended = component | {y for e in edges for x, y in (e, e[::-1]) if x in component}
                    if component == extended:
                        break
                    component = extended
                answer.append(sum(a[v] for v in component))
        add('connectivity', source, line(answer))
    return cases


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--label', default='after')
    parser.add_argument('--cases', nargs='+', choices=CASES, default=CASES)
    parser.add_argument('--cxx', default=shutil.which('g++-15'))
    args = parser.parse_args()
    cases = make_cases()
    args.output.joinpath('oracle-cases.json').write_text(json.dumps(cases, indent=2)+'\n')
    results = []
    for case in args.cases:
        for name in [case, f'{args.label}-{case}-first', f'{args.label}-{case}']:
            source = args.output / f'{name}.cpp'
            binary = args.output / f'{name}.out'
            built = subprocess.run([args.cxx, '-std=c++23', '-O2', str(source), '-o', str(binary)],
                                   capture_output=True, text=True, timeout=120)
            if built.returncode:
                raise RuntimeError(f'{name}: {built.stderr[-6000:]}')
            for index, entry in enumerate(cases[case]):
                result = subprocess.run([str(binary)], input=entry['input'], capture_output=True,
                                        text=True, timeout=20)
                if result.returncode or result.stdout.split() != entry['expected']:
                    raise AssertionError(f'{name} input {index}: return={result.returncode}, '
                                         f'output={result.stdout[:1000]}, stderr={result.stderr[:1000]}, '
                                         f"expected={entry['expected'][:30]}")
            results.append({'artifact': name, 'runs': len(cases[case])})
            print(json.dumps(results[-1]), flush=True)
    args.output.joinpath(f'{args.label}-validation.json').write_text(json.dumps(results, indent=2)+'\n')


if __name__ == '__main__':
    main()
