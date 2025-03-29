#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "zeta_mobius_divisor_multiple.hpp"

/**
 * @brief LCM/GCD 畳み込み
 * @docs docs/math/prime/lcm_gcd_convolution.md
 */

// |a| = |b| を仮定
// 1-indexed (a[0], b[0] は使わない)
template <class R>
vc<typename R::S> lcm_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  assert(a.size() == b.size());
  auto za = zeta_divisor<MonoidOfSemiRingAdd<R>>(a);
  auto zb = zeta_divisor<MonoidOfSemiRingAdd<R>>(b);
  repi(i, 1, SZ<int>(a)) za[i] = R::mul(za[i], zb[i]);
  return mobius_divisor<GroupOfRingAdd<R>>(za);
}

// |a| = |b| を仮定
// 1-indexed (a[0], b[0] は使わない)
template <class R>
vc<typename R::S> gcd_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  assert(a.size() == b.size());
  auto za = zeta_multiple<MonoidOfSemiRingAdd<R>>(a);
  auto zb = zeta_multiple<MonoidOfSemiRingAdd<R>>(b);
  repi(i, 1, SZ<int>(a)) za[i] = R::mul(za[i], zb[i]);
  return mobius_multiple<GroupOfRingAdd<R>>(za);
}