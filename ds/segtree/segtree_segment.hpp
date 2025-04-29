#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief セグ木の区間に関わる算数
 * @docs docs/ds/segtree/segtree_segment.md
 */

// 深さ d+1 のセグ木: 2^{d+1} - 1 頂点で、[0, 2^d) の区間が扱える
// 頂点 i が表すノード [a, b) を返す
// 計算量: O(1)
template <class T = ll>
T segtree_node_to_segment(int d, T i)
{
  const T n = T(1) << d;
  assert(1 <= i && i < 2 * n);
  const int k = bit_width(i) - 1;
  const T len = T(1) << (d - k), a = len * i - n, b = a + len;
  return {a, b};
}

// 深さ d+1 のセグ木: 2^{d+1} - 1 頂点で、[0, 2^d) の区間が扱える
// 区間 [a, b) は単一のノードで表されるとして、表すノードの番号 i を返す
// 計算量: O(1)
template <class T = ll>
T segtree_segment_to_node(int d, T a, T b)
{
  const T n = T(1) << d;
  assert(0 <= a && a <= b && b <= n);
  const T len = b - a;
  const int k = d + 1 - bit_width(len);
  return (a >> (d - k)) + (T(1) << k);
}

// 深さ d+1 のセグ木: 2^{d+1} - 1 頂点で、[0, 2^d) の区間が扱える
// [l, r) を表すノードの集合を返す
// 順番: 左の区間からになる
// 計算量: O(d)
template <class T = ll>
vc<T> segtree_nodes_from_left(int d, T l, T r)
{
  const T n = T(1) << d;
  assert(0 <= l && l <= r && r <= n);
  vc<T> segs;
  auto dfs = [&](auto dfs, int i, T a, T b) -> void
  {
    if (b < l || r <= a)
      return;
    if (l <= a && b <= r)
    {
      segs.eb(i);
      return;
    }
    if (b - a <= 1)
      return;
    const T c = (a + b) / 2;
    dfs(dfs, 2 * i, a, c), dfs(dfs, 2 * i + 1, c, b);
  };
  dfs(dfs, 1, 0, n);
  return segs;
}

// 深さ d+1 のセグ木: 2^{d+1} - 1 頂点で、[0, 2^d) の区間が扱える
// [l, r) を表すノードの集合を返す
// 順番: セグ木で下にある部分から (同じ段なら左から)
// 計算量: O(d)
template <class T = ll>
vc<T> segtree_nodes_from_bottom(int d, T l, T r)
{
  const T n = T(1) << d;
  assert(0 <= l && l <= r && r <= n);
  vc<T> segs;
  l += n, r += n;
  while (l < r)
  {
    if (l & 1)
      segs.eb(l++);
    if (r & 1)
      segs.eb(--r);
    l >>= 1, r >>= 1;
  }
  return segs;
}
