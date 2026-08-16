#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../affine_function.hpp"
#include "../min_max_sum.hpp"

/**
 * @brief 作用つきモノイド：一次関数作用・和取得
 * @docs docs/algebra/acted_monoid/affine_sum.md
 */

template <class T>
struct ActedMonoidAffineSum
{
  using M = MonoidSumWithLength<T>;
  using MF = MonoidAffineFunction<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return {x.len, f.b * x.val + f.c * x.len}; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
