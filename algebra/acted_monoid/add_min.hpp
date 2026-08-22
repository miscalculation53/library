#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../algebra_base.hpp"
#include "../min_max_sum.hpp"

/**
 * @brief 作用つきモノイド：加算・最小値／最大値取得
 * @docs docs/algebra/acted_monoid/add_min.md
 */

template <class T, T infty = INF>
struct ActedMonoidAddMin
{
  using M = MonoidMin<T, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return f + x; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, T infty = INF>
struct ActedMonoidAddMax
{
  using M = MonoidMax<T, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return f + x; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, T infty = INF>
struct ActedMonoidAddMinMax
{
  using M = MonoidMinMax<T, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x)
  {
    if (x.mn == infty && x.mx == -infty)
      return x;
    x.mn += f, x.mx += f;
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
