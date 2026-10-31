#pragma once

#include "../internal_power.hpp"
#include "affine_power_sum.hpp"
#include "../upd.hpp"

/**
 * @brief 作用つきモノイド：代入・累乗和取得
 * @docs docs/algebra/acted_monoid/upd_power_sum.md
 */

template <class T, int D>
struct ActedMonoidUpdPowerSum
{
  using M = MonoidPowerSum<T, D>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }
  static constexpr S mapping(const F &f, const S &x)
  {
    if (f.is_id) return x;
    return ActedMonoidAffinePowerSum<T, D>::mapping({T{}, f.val}, x);
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
