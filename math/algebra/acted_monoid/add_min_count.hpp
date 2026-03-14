#pragma once

#include "../../../template/template_all_but_modint.hpp"

/**
 * @brief 作用つきモノイド：加算・最小値・最小値の個数取得
 * @docs docs/math/algebra/acted_monoid/add_min_count.md
 */

// T: 値 (mn) の型
// U: 個数 (cnt) の型
template <class T, class U = ll, T infty = INF>
struct ActedMonoidAddMinCount
{
  struct S
  {
    T mn;
    U cnt;
    S() {}
    S(T mn, U cnt = 1) : mn(mn), cnt(cnt) {}
  };
  static constexpr S op(const S &a, const S &b)
  {
    if (a.mn < b.mn)
      return a;
    else if (a.mn > b.mn)
      return b;
    else
      return {a.mn, a.cnt + b.cnt};
  }
  static constexpr S e() { return {infty, 0}; }
  using F = T;
  static constexpr S mapping(F f, S x) { return {f + x.mn, x.cnt}; }
  static constexpr F composition(F f, F g) { return f + g; }
  static constexpr F id() { return {}; }
};

// T: 値 (mn) の型
// U: 個数 (cnt) の型
template <class T, class U = ll, T infty = INF>
struct ActedMonoidAddMaxCount
{
  struct S
  {
    T mx;
    U cnt;
    S() {}
    S(T mx, U cnt = 1) : mx(mx), cnt(cnt) {}
  };
  static constexpr S op(const S &a, const S &b)
  {
    if (a.mn > b.mn)
      return a;
    else if (a.mn < b.mn)
      return b;
    else
      return {a.mn, a.cnt + b.cnt};
  }
  static constexpr S e() { return {-infty, 0}; }
  using F = T;
  static constexpr S mapping(F f, S x) { return {f + x.mx, x.cnt}; }
  static constexpr F composition(F f, F g) { return f + g; }
  static constexpr F id() { return {}; }
};
