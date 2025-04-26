#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief Disjoint Sparse Table
 * @docs docs/ds/disjoint_sparse_table.md
 */

template <class M>
struct DisjointSparseTable
{
  using S = typename M::S;

private:
  int n;
  // dat[j][i] は [i, mid) か [mid, i]
  vvc<S> dat;

public:
  DisjointSparseTable() {}
  DisjointSparseTable(const vc<S> &v) : n(v.size())
  {
    const int lg = bit_width(n);
    dat.assign(lg, vc<S>(n));
    repi(j, lg)
    {
      const int len = 1 << j;
      repi(m, len, n + 1, 2 * len)
      {
        const int l = m - len, r = min(n, m + len);
        dat[j][m - 1] = v[m - 1];
        repi(i, m - 2, l - 1, -1) dat[j][i] = M::op(v[i], dat[j][i + 1]);
        if (m < n)
        {
          dat[j][m] = v[m];
          repi(i, m + 1, r) dat[j][i] = M::op(dat[j][i - 1], v[i]);
        }
      }
    }
  }

  S get(int p)
  {
    assert(0 <= p && p < n);
    return dat[0][p];
  }
  S prod(int l, int r)
  {
    auto [wl, wr] = prod_left_right(l, r);
    return M::op(wl, wr);
  }
  // prod[l, r) = wl • wr となるような (wl, wr) を返す
  // 使用場面：最終的な答えとしては一般の元の積がいらない場合
  pair<S, S> prod_left_right(int l, int r)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return {M::e(), M::e()};
    if (l + 1 == r)
      return {dat[0][l], M::e()};
    const int j = msb_pos(l ^ (r - 1));
    return {dat[j][l], dat[j][r - 1]};
  }
};
