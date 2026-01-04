#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 黄金分割探索
 * @docs docs/algo/golden_section_search.md
 */

// [l, r] は閉区間
template <
  class F,
  class T = decltype(declval<F>()(declval<ll>())),
  class Compare = less<T>
>
pair<ll, T> golden_section_search(F f, ll l, ll r, Compare comp = Compare())
{
  ll s = 1, t = 2;
  while (t < r - l + 2)
    swap(s += t, t);
  ll a = l - 1, x = a + t - s, b = a + t;
  T fx = f(x), fy;
  while (a + b != 2 * x)
  {
    ll y = a + b - x;
    if (r < y or (fy = f(y), comp(fx, fy)))
      b = a, a = y;
    else
      a = x, x = y, fx = fy;
  }
  return {x, fx};
}
