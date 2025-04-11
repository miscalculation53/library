#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief sparse table
 * @docs docs/ds/sparse_table.md
 */

// M は冪等 (max, min, and, or, gcd, lcm など)
template <class M>
struct SparseTable
{
  using S = typename M::S;

private:
  int n;
  // dat[j][i] = prod[i, i+2^j)
  vvc<S> dat;

public:
  SparseTable() {}
  SparseTable(const vc<S> &v) : n(v.size())
  {
    const int lg = max(1, (int)bit_width(n));
    if (n == 0)
      return;
    dat.resize(lg);
    dat[0] = v;
    repi(j, 1, lg)
    {
      dat[j].resize(n - (1 << j) + 1);
      repi(i, n - (1 << j) + 1) dat[j][i] = M::op(dat[j - 1][i], dat[j - 1][i + (1 << (j - 1))]);
    }
  }

  S get(int p)
  {
    assert(0 <= p && p < n);
    return dat[0][p];
  }
  S prod(int l, int r)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return M::e();
    int j = bit_width(r - l) - 1;
    return M::op(dat[j][l], dat[j][r - (1 << j)]);
  }
};
