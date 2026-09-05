#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../min_max_sum.hpp"
#include "../upd.hpp"

/**
 * @brief 作用つきモノイド：更新・最小値／最大値・和取得
 * @docs docs/algebra/acted_monoid/upd_min_sum.md
 */

template <class T, auto infty = INF>
struct ActedMonoidUpdMinSum
{
  using M = MonoidMinSum<T, infty>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x)
  {
    return f.is_id || x.len == 0 ? x : S{f.val, x.len};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = INF>
struct ActedMonoidUpdMaxSum
{
  using M = MonoidMaxSum<T, infty>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x)
  {
    return f.is_id || x.len == 0 ? x : S{f.val, x.len};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = INF>
struct ActedMonoidUpdMinMaxSum
{
  using M = MonoidMinMaxSum<T, infty>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  static constexpr S mapping(const F &f, S x)
  {
    return f.is_id || x.len == 0 ? x : S{f.val, x.len};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
