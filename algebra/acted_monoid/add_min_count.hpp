#pragma once

#include "../internal_power.hpp"
#include "../../template/template_all_but_modint.hpp"

#include "../algebra_base.hpp"
#include "../min_count.hpp"

/**
 * @brief 作用つきモノイド：加算・最小値・最小値の個数取得
 * @docs docs/algebra/acted_monoid/add_min_count.md
 */

template <class T, class U = ll, auto infty = nullptr>
struct ActedMonoidAddMinCount
{
  using M = MonoidMinCount<T, U, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }
  static constexpr S mapping(F f, S x) { return {f + x.mn, x.cnt}; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, class U = ll, auto infty = nullptr>
struct ActedMonoidAddMaxCount
{
  using M = MonoidMaxCount<T, U, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }
  static constexpr S mapping(F f, S x) { return {f + x.mx, x.cnt}; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
