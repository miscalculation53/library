#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/algebra_basic_ops.hpp"
// #include "and_or_convolution.hpp"
// #include "../../algebra/polynomial_ring.hpp"

/**
 * @brief subset convolution
 * @docs docs/math/set/subset_convolution.md
 */

// R は環
// |a| = |b| = 2^n を仮定、O(n^2 2^n) 時間
template <class R>
vc<typename R::S> subset_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  // using P = PolynomialRingArray<R, 30>;
  using F = array<typename R::S, 30>;
  assert(a.size() == b.size());
  const int len = a.size();
  if (len == 0)
    return {};
  assert(has_single_bit(len));
  const int lg = countr_zero(len);
  static vc<int> pc{0};
  if (int i = pc.size(); i <= len)
  {
    pc.resize(len);
    for (; i < len; i++)
      pc[i] = pc[i - (i & -i)] + 1;
  }
  vc<F> fa(len), fb(len);
  repi(i, len)
  {
    fill(ALL(fa[i]), R::e0()), fill(ALL(fb[i]), R::e0());
    fa[i][pc[i]] = a[i], fb[i][pc[i]] = b[i];
  }
  // or_convolution_destructive<P>(fa, fb);
  auto zeta = [&](vc<F> &f) -> void
  {
    for (int w = 1; w < len; w <<= 1)
    {
      repi(k, 0, len, w * 2) repi(i, w)
      {
        int s = k + i, t = s + w;
        repi(j, pc[t]) f[t][j] = R::add(f[t][j], f[s][j]);
      }
    }
  };
  zeta(fa), zeta(fb);
  repi(s, len)
  {
    F fc;
    fill(ALL(fc), R::e0());
    repi(i, lg + 1) repi(j, lg + 1 - i) fc[i + j] = R::add(fc[i + j], R::mul(fa[s][i], fb[s][j]));
    swap(fa[s], fc);
  }
  for (int w = len >> 1; w; w >>= 1)
  {
    repi(k, 0, len, w * 2) repi(i, w)
    {
      int s = k + i, t = s + w;
      repi(j, pc[t], lg + 1) fa[t][j] = R::add(fa[t][j], R::minus(fa[s][j]));
    }
  }
  vc<typename R::S> c(len);
  repi(i, len) c[i] = fa[i][pc[i]];
  return c;
}