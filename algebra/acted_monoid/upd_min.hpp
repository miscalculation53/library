#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../min_max_sum.hpp"
#include "../upd.hpp"

/**
 * @brief 作用つきモノイド：更新・最小値／最大値取得
 * @docs docs/algebra/acted_monoid/upd_min.md
 */

template <class T, T infty = INF, T idnty = INF + 1>
struct ActedMonoidUpdMin
{
  using M = MonoidMin<T, infty>;
  using MF = MonoidUpd<T, idnty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return f == idnty ? x : f; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, T infty = INF, T idnty = INF + 1>
struct ActedMonoidUpdMax
{
  using M = MonoidMax<T, infty>;
  using MF = MonoidUpd<T, idnty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(F f, S x) { return f == idnty ? x : f; }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, T infty = INF, T idnty = INF + 1>
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
    return f == idnty || (x.mn == infty && x.mx == -infty) ? x : S{f};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
