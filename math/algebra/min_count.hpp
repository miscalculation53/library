#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 最小値の個数も持つモノイド
 * @docs docs/math/algebra/min_count.md
 */

template <class T, class U = ll, T infty = INF>
struct MonoidMinCount
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
};
template <class T, class U = ll, T infty = INF>
struct MonoidMaxCount
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
    if (a.mx > b.mx)
      return a;
    else if (a.mx < b.mx)
      return b;
    else
      return {a.mx, a.cnt + b.cnt};
  }
  static constexpr S e() { return {-infty, 0}; }
};
