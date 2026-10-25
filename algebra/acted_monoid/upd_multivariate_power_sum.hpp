#pragma once

#include "affine_multivariate_power_sum.hpp"
#include "../upd.hpp"

/**
 * @brief 作用つきモノイド：各数列への代入・混合累乗和取得
 * @docs docs/algebra/acted_monoid/upd_multivariate_power_sum.md
 */

template <class T, int... Ds>
struct ActedMonoidUpdMultivariatePowerSum
{
  using M = MonoidMultivariatePowerSum<T, Ds...>;
  using S = typename M::S;
  using F = array<typename MonoidUpdFlag<T>::S, M::dim>;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, const S &x)
  {
    using AM = ActedMonoidAffineMultivariatePowerSum<T, Ds...>;
    typename AM::F affine;
    repi(d, M::dim)
      if (!f[d].is_id) affine.b[d] = T{}, affine.c[d] = f[d].val;
    return AM::mapping(affine, x);
  }
  static constexpr F composition(const F &f, const F &g)
  {
    F res;
    repi(d, M::dim) res[d] = MonoidUpdFlag<T>::op(f[d], g[d]);
    return res;
  }
  static constexpr F id() { return {}; }
};
