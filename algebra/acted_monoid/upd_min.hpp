#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../min_max_sum.hpp"
#include "../upd.hpp"

/**
 * @brief 作用つきモノイド：更新・最小値／最大値取得
 * @docs docs/algebra/acted_monoid/upd_min.md
 */

template <class T, auto infty = INF, auto idnty = INF + 1>
struct ActedMonoidUpdMin
{
  using M = MonoidMin<T, infty>;
  using MF = MonoidUpd<T, idnty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return f == resolved_value<T, idnty>() ? x : f; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = INF, auto idnty = INF + 1>
struct ActedMonoidUpdMax
{
  using M = MonoidMax<T, infty>;
  using MF = MonoidUpd<T, idnty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return f == resolved_value<T, idnty>() ? x : f; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = INF, auto idnty = INF + 1>
struct ActedMonoidUpdMinMax
{
  using M = MonoidMinMax<T, infty>;
  using MF = MonoidUpd<T, idnty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x)
  {
    if (f == resolved_value<T, idnty>() ||
        (x.mn == resolved_value<T, infty>() && x.mx == -resolved_value<T, infty>()))
      return x;
    return S{f};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
