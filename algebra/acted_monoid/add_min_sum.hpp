#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../min_max_sum.hpp"

/**
 * @brief 作用つきモノイド：加算・最小値／最大値・和取得
 * @docs docs/algebra/acted_monoid/add_min_sum.md
 */

template <class T, auto infty = INF>
struct ActedMonoidAddMinSum
{
  using M = MonoidMinSum<T, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x)
  {
    if (x.len == 0)
      return x;
    x.mn += f, x.sum += f * x.len;
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = INF>
struct ActedMonoidAddMaxSum
{
  using M = MonoidMaxSum<T, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x)
  {
    if (x.len == 0)
      return x;
    x.mx += f, x.sum += f * x.len;
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = INF>
struct ActedMonoidAddMinMaxSum
{
  using M = MonoidMinMaxSum<T, infty>;
  using MF = MonoidAdd<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x)
  {
    if (x.len == 0)
      return x;
    x.mn += f, x.mx += f, x.sum += f * x.len;
    return x;
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
