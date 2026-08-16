#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../algebra/algebra_basic_ops.hpp"

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
      repi(i, l, r)
      {
        auto &b = block[bid];
        b = G::op(b, dat[i]);
      }
    }
  }

  // モノイドでよい
  S get(int p) const { return dat[p]; }
  // モノイドでよい
  void add(int p, const S &x)
  {
    const int bid = p / B;
    auto &b = block[bid], &d = dat[p];
    b = G::op(b, x);
    d = G::op(d, x);
  }
  // 群であることが必要
  void set(int p, const S &x)
  {
    const int bid = p / B;
    auto &b = block[bid], &d = dat[p];
    b = G::op(b, G::op(G::inv(d), x));
    d = x;
  }
  // モノイドでよい
  S sum(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    S sm = G::e();
    int bl = divceil(l, B), br = divfloor(r, B);
    if (bl >= br)
      repi(i, l, r) sm = G::op(sm, dat[i]);
    else
    {
      repi(i, l, bl * B) sm = G::op(sm, dat[i]);
      repi(j, bl, br) sm = G::op(sm, block[j]);
      repi(i, br * B, r) sm = G::op(sm, dat[i]);
    }
    return sm;
  }
};
