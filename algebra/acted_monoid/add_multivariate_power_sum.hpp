#pragma once

#include "affine_multivariate_power_sum.hpp"

/**
 * @brief 作用つきモノイド：各数列への加算・混合累乗和取得
 * @docs docs/algebra/acted_monoid/add_multivariate_power_sum.md
 */

template <class T, int... Ds>
struct ActedMonoidAddMultivariatePowerSum
{
  using M = MonoidMultivariatePowerSum<T, Ds...>;
  using S = typename M::S;
  using F = array<T, M::dim>;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, const S &x)
  {
    using AM = ActedMonoidAffineMultivariatePowerSum<T, Ds...>;
    typename AM::F affine;
    affine.c = f;
    return AM::mapping(affine, x);
  }
  static constexpr F composition(const F &f, const F &g)
  {
    F res;
    repi(d, M::dim) res[d] = f[d] + g[d];
    return res;
  }
  static constexpr F id() { return {}; }
};
