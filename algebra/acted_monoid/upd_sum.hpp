#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../min_max_sum.hpp"
#include "../upd.hpp"

/**
 * @brief 作用つきモノイド：更新・和取得
 * @docs docs/algebra/acted_monoid/upd_sum.md
 */

template <class T>
struct ActedMonoidUpdSum
{
  using M = MonoidLenSum<T>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return f.is_id ? x : S{x.len, f.val * x.len}; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
