#pragma once

#include "../../../template/template_all_but_modint.hpp"

/**
 * @brief 作用つきモノイド：加算・最小値取得
 * @docs docs/math/algebra/acted_monoid/add_min.md
 */

template <class T, T infty = INF>
struct ActedMonoidAddMin
{
  using S = T;
  static constexpr S op(S a, S b) { return min(a, b); }
  static constexpr S e() { return infty; }
  using F = T;
  static constexpr S mapping(F f, S x) { return f + x; }
  static constexpr F composition(F f, F g) { return f + g; }
  static constexpr F id() { return {}; }
};

template <class T, T infty = INF>
struct ActedMonoidAddMax
{
  using S = T;
  static constexpr S op(S a, S b) { return max(a, b); }
  static constexpr S e() { return -infty; }
  using F = T;
  static constexpr S mapping(F f, S x) { return f + x; }
  static constexpr F composition(F f, F g) { return f + g; }
  static constexpr F id() { return {}; }
};
