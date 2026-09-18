#pragma once

#include "../index_power_sum.hpp"
#include "../upd.hpp"
#include "../../utils/make_unsigned_ext.hpp"

/**
 * @brief 作用つきモノイド：多項式代入・和取得
 * @docs docs/algebra/acted_monoid/polyupd_sum.md
 */

template <class T, int D>
struct ActedMonoidPolyUpdSum
{
  using M = MonoidIndexPowerSum<T, D>;
  using MF = MonoidUpdFlag<array<T, D + 1>>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x)
  {
    if (f.is_id)
      return x;
    if constexpr (is_integral_ext<T>)
    {
      using U = decltype(0U + make_unsigned_ext_t<T>{});
      U val = 0;
      repi(k, D + 1) val += U(f.val[k]) * U(x.power_sum[k]);
      x.val = T(val);
    }
    else
    {
      x.val = T{};
      repi(k, D + 1) x.val = x.val + f.val[k] * x.power_sum[k];
    }
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
