#pragma once

#include "../../template/template_all.hpp"
#include "zeta_mobius.hpp"

/**
 * @brief and/or 畳み込み
 * @docs docs/math/set/and_or_convolution.md
 */

// R は環
// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
template <class R>
vc<typename R::S> and_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  assert(a.size() == b.size());
  auto za = zeta_supset<MonoidOfSemiRingAdd<R>>(a);
  auto zb = zeta_supset<MonoidOfSemiRingAdd<R>>(b);
  repi(i, za.size()) za[i] = R::mul(za[i], zb[i]);
  return mobius_supset<GroupOfRingAdd<R>>(za);
}
// R は環
// |a| = |b| = 2^n を仮定、O(n 2^n) 時間
template <class R>
vc<typename R::S> or_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  assert(a.size() == b.size());
  auto za = zeta_subset<MonoidOfSemiRingAdd<R>>(a);
  auto zb = zeta_subset<MonoidOfSemiRingAdd<R>>(b);
  repi(i, za.size()) za[i] = R::mul(za[i], zb[i]);
  return mobius_subset<GroupOfRingAdd<R>>(za);
}