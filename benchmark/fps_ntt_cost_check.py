"""Check exact transform-weight formulas against independently recorded histograms."""
import csv
from pathlib import Path
root = Path(__file__).resolve().parent
E = lambda n: n * (n.bit_length() - 1)
def inverse(n):
    return 5 * sum(E(1 << k) for k in range(1, n.bit_length()))
def bs(n, split):
    value = sum(5 * E(1 << k) + 6 * E(2 << k) for k in range(1, n.bit_length() - 1))
    return value - (3 * E(n) - 5 * E(n // 2) if split and n >= 4 else 0)
checked = 0
for row in csv.DictReader((root / 'fps_algorithms_exp_ntt.csv').open()):
    n = int(row['n']); name = row['variant']
    if n & (n - 1): continue
    if name == 'current': value = sum(4 * E(1 << k) + 8 * E(2 << k) for k in range(0, n.bit_length() - 1))
    elif name in ('bs_short', 'bs_split'): value = bs(n, name == 'bs_split')
    else: continue
    assert value == int(row['ntt_weight']), (n, name, value, row['ntt_weight'])
    checked += 1
for row in csv.DictReader((root / 'fps_algorithms_multipoint_ntt.csv').open()):
    n = int(row['n']); K = n.bit_length() - 1; name = row['variant']
    if n < 256 or n & (n - 1) or n != int(row['points']) or row['layout'] != 'random': continue
    if name == 'current': value = 14*n*K*K - (614*n - 5*n//64) - 20
    elif name == 'transposed': value = 6*n*K*K + 28*n*K - 292*n + 10
    elif name == 'transposed_cache': value = 3*n*K*K + 19*n*K - 130*n + 10
    else: continue
    assert value == int(row['ntt_weight']), (n, name, value, row['ntt_weight'])
    checked += 1
# Validate all recorded call/weight totals; each prime counts separately under CRT.
for path in root.glob('fps_*.csv'):
    for row in csv.DictReader(path.open()):
        if 'transforms' not in row: continue
        calls = weight = 0
        for item in filter(None, row['transforms'].split(';')):
            size, count = item.split(':'); forward, backward = map(int, count.split('/')); size = int(size)
            calls += forward + backward; weight += E(size) * (forward + backward)
        assert calls == int(row['ntt_calls']) and weight == int(row['ntt_weight']), (path.name, row)
print(f'Exact formulas: {checked} rows; all NTT histogram totals agree.')

for row in csv.DictReader((root/'fps_production_ntt.csv').open()):
    n=int(row['n']);K=n.bit_length()-1
    if n<256 or n&(n-1):continue
    if row['suite']=='multipoint':
        value=(14*n*K*K-(614*n-5*n//64)-20) if row['variant']=='baseline' else 3*n*K*K+19*n*K-130*n+10
    elif row['suite']=='interpolation':
        value=(22*n*K*K+20*n*K-(1022*n-5*n//64)-20) if row['variant']=='baseline' else (9*n*K*K+41*n*K)//2-193*n+10
    else:continue
    assert value==int(row['ntt_weight']),(row,value)
print('Production multipoint/interpolation formulas agree.')
