#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"

/**
 * @brief 平方分割による一点加算・区間和クエリ $\langle O(1), O(B + N/B) \rangle$
 * @docs docs/ds/sqrt_decomposition/range_sum.md
 */

// G は可換群 (一部の操作はモノイドでよい)
template <class G, int B = 512>
struct SqrtDecompositionRangeSum
{
  static_assert(B > 0);
  using S = typename G::S;

private:
  int n = 0;
  vc<S> dat, block;

public:
  SqrtDecompositionRangeSum() {}
  SqrtDecompositionRangeSum(int n) : SqrtDecompositionRangeSum(vc<S>(n, G::e())) {}
  SqrtDecompositionRangeSum(const vc<S> &vec) : n(vec.size()), dat(vec)
  {
    block.resize(n / B + (n % B != 0), G::e());
    repi(bid, block.size())
    {
      const int l = bid * B, r = l + min(B, n - l);
      S s = G::e();
      repi(i, l, r) s = G::op(s, dat[i]);
      block[bid] = s;
    }
  }

  // モノイドでよい
  S get(int p) const
  {
    assert(0 <= p && p < n);
    return dat[p];
  }
  // モノイドでよい
  void add(int p, const S &x)
  {
    assert(0 <= p && p < n);
    const int bid = p / B;
    auto &b = block[bid], &d = dat[p];
    b = G::op(b, x);
    d = G::op(d, x);
  }
  // 群であることが必要
  void set(int p, const S &x)
  {
    assert(0 <= p && p < n);
    const int bid = p / B;
    auto &b = block[bid], &d = dat[p];
    b = G::op(b, G::op(G::inv(d), x));
    d = x;
  }
  // モノイドでよい
  S sum(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (r - l <= 32)
    {
      S s = G::e();
      for (int i = l; i < r; i++) s = G::op(s, dat[i]);
      return s;
    }
    const int bl = l / B + (l % B != 0);
    const int br = r == n ? n / B + (n % B != 0) : r / B;
    // 可換性を使って 4 本に分け、演算の依存を短くする。
    S s0 = G::e(), s1 = G::e(), s2 = G::e(), s3 = G::e();
    auto fold = [&](const vc<S> &v, int a, int b)
    {
      for (; b - a >= 4; a += 4)
      {
        s0 = G::op(s0, v[a]);
        s1 = G::op(s1, v[a + 1]);
        s2 = G::op(s2, v[a + 2]);
        s3 = G::op(s3, v[a + 3]);
      }
      for (; a < b; a++) s0 = G::op(s0, v[a]);
    };
    if (bl >= br) fold(dat, l, r);
    else
    {
      fold(dat, l, bl * B);
      fold(block, bl, br);
      if (r != n) fold(dat, br * B, r);
    }
    return G::op(G::op(s0, s1), G::op(s2, s3));
  }

  vc<S> content() const { return dat; }
};
