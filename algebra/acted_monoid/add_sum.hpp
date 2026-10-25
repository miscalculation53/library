#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../min_max_sum.hpp"

/**
 * @brief 作用つきモノイド：加算・和取得
 * @docs docs/algebra/acted_monoid/add_sum.md
 */

template <class T, class L = ll>
struct ActedMonoidAddSum
{
  using M = MonoidLenSum<T, L>;
  using UL = typename M::UL;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr auto pow = M::pow;
  static constexpr S mapping(F f, S x) { return S(x.len, x.val + f * UL(x.len)); }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
