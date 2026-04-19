#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "sparse_table.hpp"
#include "disjoint_sparse_table.hpp"

/**
 * @brief ブロック分割 + (disjoint) sparse table
 * @docs docs/ds/static_range/block_sparse_table.md
 */

template <class M, class ST, int B = 16>
struct BlockSparseTableBase
{
  using S = typename M::S;

protected:
  int n;
  vc<S> v;
  // ブロック i は l = iB, r = min((i+1)B, n) として [l, r) を担当
  // j を含むブロックが [l, r) であるとして、
  // cl[j] = prod[l, j)
  // cr[j] = prod[j, r)
  vc<S> cl, cr;
  ST st;

public:
  BlockSparseTableBase() {}
  BlockSparseTableBase(const vc<S> &v) : n(v.size()), v(v)
  {
    const int m = divceil(n, B);
    cl.resize(n + 1, M::e()), cr.resize(n + 1, M::e());
    vc<S> stdat(m, M::e());
    repi(i, m)
    {
      const int l = i * B, r = min(n, (i + 1) * B);
      repi(j, l + 1, r) cl[j] = M::op(cl[j - 1], v[j - 1]);
      repi(j, r - 1, l - 1, -1) cr[j] = M::op(v[j], cr[j + 1]);
      repi(j, r - l) stdat[i] = M::op(stdat[i], v[l + j]);
    }
    st = ST(stdat);
  }

  S get(int p) const
  {
    assert(0 <= p && p < n);
    return v[p];
  }
  S prod(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return M::e();
    const int i = l / B, j = r == n ? (n - 1) / B + 1 : r / B;
    if (i < j)
      return M::op(M::op(cr[l], st.prod(i + 1, j)), cl[r]);
    else
    {
      S res = M::e();
      repi(k, l, r) res = M::op(res, v[k]);
      return res;
    }
  }
};

// M は冪等 (max, min, and, or, gcd, lcm など)
// 前計算の時空間 O(n/B log(n/B))
// クエリ 最悪 O(B)
template <class M, int B = 16>
using BlockSparseTable = BlockSparseTableBase<M, SparseTable<M>, B>;

// 前計算の時空間 O(n/B log(n/B))
// クエリ 最悪 O(B)
template <class M, int B = 16>
using BlockDisjointSparseTable = BlockSparseTableBase<M, DisjointSparseTable<M>, B>;
