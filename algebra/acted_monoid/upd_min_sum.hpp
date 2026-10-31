#pragma once

#include "../internal_power.hpp"
#include "../../template/template_all_but_modint.hpp"

#include "../min_max_sum.hpp"
#include "../upd.hpp"

/**
 * @brief 作用つきモノイド：更新・最小値／最大値・和取得
 * @docs docs/algebra/acted_monoid/upd_min_sum.md
 */

template <class T, auto infty = nullptr, class L = ll>
struct ActedMonoidUpdMinSum
{
  using M = MonoidMinSum<T, infty, L>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }
  static constexpr S mapping(const F &f, S x)
  {
    return f.is_id || x.len == 0 ? x : S{f.val, x.len};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = nullptr, class L = ll>
struct ActedMonoidUpdMaxSum
{
  using M = MonoidMaxSum<T, infty, L>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }
  static constexpr S mapping(const F &f, S x)
  {
    return f.is_id || x.len == 0 ? x : S{f.val, x.len};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};

template <class T, auto infty = nullptr, class L = ll>
struct ActedMonoidUpdMinMaxSum
{
  using M = MonoidMinMaxSum<T, infty, L>;
  using MF = MonoidUpdFlag<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }
  static constexpr S mapping(const F &f, S x)
  {
    return f.is_id || x.len == 0 ? x : S{f.val, x.len};
  }
  static constexpr auto composition = MF::op;
  static constexpr auto id = MF::e;
};
