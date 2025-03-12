#pragma once

#include "../../template/template_all.hpp"
#include "and_or_convolution.hpp"
#include "../../math/algebra/polynomial_ring.hpp"

/**
 * @brief subset convolution
 * @docs docs/math/set/subset_convolution.md
 */

// R は環
// |a| = |b| = 2^k <= 2^n を仮定、O(k^2 2^k) 時間
template <class R, int n>
vc<typename R::S> subset_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  using P = PolynomialRingArray<R, n + 1>;
  assert(a.size() == b.size());
  const int m = a.size();
  assert(m <= (1 << n));
  vc<typename P::S> fa(m), fb(m);
  repi(i, m)
  {
    const int j = popcount(i);
    fill(ALL(fa[i]), R::e0()), fill(ALL(fb[i]), R::e0());
    fa[i][j] = a[i], fb[i][j] = b[i];
  }
  or_convolution_destructive<P>(fa, fb);
  vc<typename R::S> c(m);
  repi(i, m) c[i] = fa[i][popcount(i)];
  return c;
}