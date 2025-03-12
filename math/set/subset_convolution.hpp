#pragma once

#include "../../template/template_all.hpp"
#include "and_or_convolution.hpp"
#include "../../math/algebra/polynomial_ring.hpp"

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
  using P = PolynomialRing<R>;
  assert(a.size() == b.size());
  const int n = a.size();
  vc<typename P::S> fa(n), fb(n);
  repi(i, n)
  {
    const int j = popcount(i);
    fa[i].resize(j + 1, R::e0()), fb[i].resize(j + 1, R::e0());
    fa[i][j] = a[i], fb[i][j] = b[i];
  }
  auto fc = or_convolution<P>(fa, fb);
  vc<typename R::S> c(n);
  repi(i, n) c[i] = fc[i][popcount(i)];
  return c;
}