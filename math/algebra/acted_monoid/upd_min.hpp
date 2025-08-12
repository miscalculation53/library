#pragma once

#include "../../../template/template_all_but_modint.hpp"

/**
 * @brief 作用つきモノイド：更新・最小値取得
 * @docs docs/math/algebra/acted_monoid/upd_min.md
 */

template <class T, T infty = INF, T idnty = INF + 1>
struct ActedMonoidUpdMin
{
  using S = T;
  static constexpr S op(S a, S b) { return min(a, b); }
  static constexpr S e() { return infty; }
  using F = T;
  static constexpr S mapping(F f, S x) { return f == idnty ? x : f; }
  static constexpr F composition(F f, F g) { return f == idnty ? g : f; }
  static constexpr F id() { return idnty; }
};

template <class T, T infty = INF, T idnty = INF + 1>
struct ActedMonoidUpdMax
{
  using S = T;
  static constexpr S op(S a, S b) { return max(a, b); }
  static constexpr S e() { return -infty; }
  using F = T;
  static constexpr S mapping(F f, S x) { return f == idnty ? x : f; }
  static constexpr F composition(F f, F g) { return f == idnty ? g : f; }
  static constexpr F id() { return idnty; }
};
