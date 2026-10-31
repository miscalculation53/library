#pragma once

#include "../internal_power.hpp"
#include "affine_power_sum.hpp"

/**
 * @brief 作用つきモノイド：加算・累乗和取得
 * @docs docs/algebra/acted_monoid/add_power_sum.md
 */

template <class T, int D>
struct ActedMonoidAddPowerSum
{
  using M = MonoidPowerSum<T, D>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }
  static constexpr S mapping(const F &f, const S &x)
  {
    return ActedMonoidAffinePowerSum<T, D>::mapping({T(1), f}, x);
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
