#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief 平方分割による一点加算・区間和クエリ $\langle O(1), O(\sqrt{N}) \rangle$
 * @docs docs/ds/sqrt_decomposition_range_sum.md
 */

// G は可換群 (一部の操作はモノイドでよい)
template <class G, int B = 512>
struct SqrtDecompositionRangeSum
{
  using S = typename G::S;

private:
  int n;
  vc<S> dat, block;

public:
  SqrtDecompositionRangeSum() {}
  SqrtDecompositionRangeSum(int n) : SqrtDecompositionRangeSum(vc<S>(n, G::e())) {}
  SqrtDecompositionRangeSum(const vc<S> &vec) : n(vec.size()), dat(vec)
  {
    block.resize(divceil(n, B), G::e());
    repi(bid, block.size())
    {
      const int l = bid * B, r = min(n, (bid + 1) * B);
      repi(i, l, r) block[bid] = G::op(block[bid], dat[i]);
    }
  }

  // モノイドでよい
  S get(int p) const { return dat[p]; }
  // モノイドでよい
  void add(int p, const S &x)
  {
    const int bid = p / B;
    block[bid] = G::op(block[bid], x);
    dat[p] = G::op(dat[p], x);
  }
  // 群であることが必要
  void set(int p, const S &x) { add(p, G::op(G::inv(get(p))), x); }
  // モノイドでよい
  S sum(int l, int r) const
  {
    S sm = G::e();
    int i = l;
    for (; i < r && i % B != 0; i++)
      sm = G::op(sm, dat[i]);
    for (int j = i / B; i + B <= r; j++, i += B)
      sm = G::op(sm, block[j]);
    for (; i < r; i++)
      sm = G::op(sm, dat[i]);
    return sm;
  }
};
