#!/usr/bin/env python3
"""Generate temporary iterator variants and benchmark the current segment trees."""
import os
import argparse
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def replace_once(source, before, after):
    assert source.count(before) == 1, before
    return source.replace(before, after)


def candidate(filename, old_name, new_name):
    path = ROOT / 'ds' / 'segtree' / filename
    source = path.read_text().replace(old_name, new_name)
    source = source.replace('ActCompoundAssignment', 'RangeActCompoundAssignment')
    source = re.sub(r'#include "([^"]+)"',
                    lambda m: '#include "' + str((path.parent / m[1]).resolve()) + '"', source)
    source = source.replace('  vc<S> dat;', '  SegmentTreeRange ranges{0};\n  vc<S> dat;')
    if old_name == 'SegmentTree':
        source = source.replace('siz(bit_ceil(vec.size())), dat', 'siz(bit_ceil(vec.size())), ranges(n), dat')
        start = source.index('  void set(int p, const S &x)')
        end = source.index('\n  S get(int p)', start)
        source = source[:start] + '''  void set(int p, const S &x)
  {
    for (ll i : ranges.point_to_nodes_from_bottom(p))
    {
      if (ranges.is_leaf(i)) dat[i] = x;
      else update(i);
    }
  }
''' + source[end:]
    else:
        source = source.replace('lg(countr_zero(siz))', 'lg(countr_zero(siz)), ranges(n)')
        # Full point paths, with the target leaf excluded from push/update.
        source = source.replace('repi(i, lg, 0, -1) push(p >> i);',
                                'for (ll i : ranges.point_to_nodes_from_top(p - siz))\n      if (!ranges.is_leaf(i)) push(i);')
        source = source.replace('repi(i, 1, lg + 1) update(p >> i);',
                                'for (ll i : ranges.point_to_nodes_from_bottom(p - siz))\n      if (!ranges.is_leaf(i)) update(i);')
        # Boundary ancestor handling is kept as in the original implementation.
        if old_name == 'DualSegmentTree':
            before = '''    while (l < r)
    {
      if (l & 1) all_apply(l++, f);
      if (r & 1) all_apply(--r, f);
      l >>= 1, r >>= 1;
    }'''
        else:
            before = '''    while (l < r)
    {
      if (l & 1)
        all_apply(l++, f);
      if (r & 1)
        all_apply(--r, f);
      l >>= 1, r >>= 1;
    }'''
        source = replace_once(source, before, '''    for (ll i : ranges.range_to_nodes_from_bottom(l - siz, r - siz))
      all_apply(i, f);''')
    if old_name != 'DualSegmentTree':
        monoid = 'M' if old_name == 'SegmentTree' else 'AM'
        before = f'''    S sml = {monoid}::e(), smr = {monoid}::e();
    while (l < r)
    {{
      if (l & 1)
        sml = {monoid}::op(sml, dat[l++]);
      if (r & 1)
        smr = {monoid}::op(dat[--r], smr);
      l >>= 1, r >>= 1;
    }}
    return {monoid}::op(sml, smr);'''
        source = replace_once(source, before, f'''    S res = {monoid}::e();
    for (ll i : ranges.range_to_nodes_from_left(l - siz, r - siz))
      res = {monoid}::op(res, dat[i]);
    return res;''')
    return source


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ndebug', action='store_true', help='disable C++ assertions')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='segtree-range-') as directory:
        temp = Path(directory)
        generated = '#include "ds/segtree/segtree_range.hpp"\n'
        for filename, original, transformed in [
            ('segtree.hpp', 'SegmentTree', 'RangeSegmentTree'),
            ('lazy_segtree.hpp', 'LazySegmentTree', 'RangeLazySegmentTree'),
            ('dual_segtree.hpp', 'DualSegmentTree', 'RangeDualSegmentTree'),
        ]:
            generated += candidate(filename, original, transformed) + '\n'
        (temp / 'segtree_range_candidates.hpp').write_text(generated)
        compiler = os.environ.get('CXX', 'g++-15')
        flags = ['-DNDEBUG'] if args.ndebug else []
        subprocess.run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Wno-unused-parameter', *flags,
                        '-I', str(ROOT), '-I', str(temp),
                        str(ROOT / 'benchmark/segtree_range.cpp'), '-o', str(temp / 'bench')], check=True)
        subprocess.run([str(temp / 'bench')], check=True)


if __name__ == '__main__':
    main()
