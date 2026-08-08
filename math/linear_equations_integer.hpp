#pragma once

#include "../template/template_all_but_modint.hpp"

#include "extgcd.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief 二元一次連立方程式の整数解
 * @docs docs/math/linear_equations_integer.md
 */

template <class T>
struct LinearEquationsIntegerResult
{
  int dim = -1;
  pair<T, T> sol{};
  array<pair<T, T>, 2> basis{};
};

// ax + by == c, dx + ey == f の整数解全体を返す
// dim == -1 なら解なし。それ以外では sol + Σ k_i basis[i] (k_i は整数) が全解
template <class T = ll, class U = larger_int_t<T>,
          class A, class B, class C, class D, class E, class F>
LinearEquationsIntegerResult<T>
linear_equations_integer(A a, B b, C c, D d, E e, F f)
{
  const U aa = a, bb = b, cc = c, dd = d, ee = e, ff = f;
  const U det = aa * ee - bb * dd;
  if (det != 0)
  {
    const U x = cc * ee - bb * ff;
    const U y = aa * ff - cc * dd;
    if (x % det != 0 || y % det != 0)
      return {};
    return {0, {T(x / det), T(y / det)}, {}};
  }

  const bool zero1 = aa == 0 && bb == 0;
  const bool zero2 = dd == 0 && ee == 0;
  if ((zero1 && cc != 0) || (zero2 && ff != 0))
    return {};
  if (zero1 && zero2)
    return {2, {0, 0}, {{{1, 0}, {0, 1}}}};
  if (!zero1 && !zero2 &&
      (aa * ff != cc * dd || bb * ff != cc * ee))
    return {};

  const U p = zero1 ? dd : aa;
  const U q = zero1 ? ee : bb;
  const U r = zero1 ? ff : cc;
  auto [g, x, y] = extgcd<U>(p, q);
  if (r % g != 0)
    return {};
  const U z = r / g;
  return {1,
          {T(x * z), T(y * z)},
          {{{T(q / g), T(-p / g)}, {0, 0}}}};
}
