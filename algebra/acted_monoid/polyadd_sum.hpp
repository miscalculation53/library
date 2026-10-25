#pragma once

#include "../index_power_sum.hpp"
#include "../../utils/make_unsigned_ext.hpp"

/**
 * @brief 作用つきモノイド：多項式加算・和取得
 * @docs docs/algebra/acted_monoid/polyadd_sum.md
 */

template <class T, int D>
struct ActedMonoidPolyAddSum
{
  using M = MonoidIndexPowerSum<T, D>;
  using S = typename M::S;
  using F = array<T, D + 1>;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x)
  {
    if constexpr (is_integral_ext<T>)
    {
      using U = decltype(0U + make_unsigned_ext_t<T>{});
      U val = U(x.val);
      repi(k, D + 1) val += U(f[k]) * U(x.power_sum[k]);
      x.val = T(val);
    }
    else
      repi(k, D + 1) x.val = x.val + f[k] * x.power_sum[k];
    return x;
  }
  static constexpr F composition(const F &f, const F &g)
  {
    F res;
    repi(k, D + 1) res[k] = f[k] + g[k];
    return res;
  }
  static constexpr F id() { return {}; }
};
