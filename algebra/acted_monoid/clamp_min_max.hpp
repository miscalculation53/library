#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../clamp_add.hpp"
#include "../min_max_sum.hpp"

/**
 * @brief 作用つきモノイド：chmin・chmax・加算、最小値・最大値取得
 * @docs docs/algebra/acted_monoid/clamp_min_max.md
 */

template <class T, T infty = INF>
struct ActedMonoidClampMin
{
  using M = MonoidMin<T, infty>;
  using MF = MonoidClampAdd<T, infty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x) { return x == infty ? x : f(x); }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, T infty = INF>
struct ActedMonoidClampMax
{
  using M = MonoidMax<T, infty>;
  using MF = MonoidClampAdd<T, infty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x) { return x == -infty ? x : f(x); }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, T infty = INF>
struct ActedMonoidClampMinMax
{
  using M = MonoidMinMax<T, infty>;
  using MF = MonoidClampAdd<T, infty>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x)
  {
    return x.mn == infty ? x : S{f(x.mn), f(x.mx)};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
