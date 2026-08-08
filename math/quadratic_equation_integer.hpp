#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../utils/larger_int.hpp"

/**
 * @brief 高々二次の方程式の整数解
 * @docs docs/math/quadratic_equation_integer.md
 */

// 高々二次の方程式 ax^2 + bx + c == 0 の整数解を求める
// first には整数解の個数が入り、恒等式なら -1 が入る
// second には解が入る（ないところは未定義）
// T は a, b, c が（余裕を持って）収まる範囲の型
// U は T の 2 乗が収まる範囲の型
template <class T = ll, class U = larger_int_t<T>, class A, class B, class C>
pair<int, array<T, 2>> quadratic_equation_integer(A a, B b, C c)
{
  const U aa = a, bb = b, cc = c;
  if (aa == 0)
  {
    if (bb == 0)
      return {cc == 0 ? -1 : 0, {}};
    if (-cc % bb != 0)
      return {0, {}};
    return {1, {T(-cc / bb), T(0)}};
  }

  const U D = bb * bb - U(4) * aa * cc;
  if (D < 0)
    return {0, {}};
  const U den = U(2) * aa;
  if (D == 0)
  {
    if (-bb % den == 0)
      return {1, {T(-bb / den), T(0)}};
    return {0, {}};
  }
  const U sqrtD = iroot<U>(D, 2);
  if (sqrtD * sqrtD != D)
    return {0, {}};
  int cnt = 0;
  array<T, 2> res{};
  if ((-bb - sqrtD) % den == 0)
    res[cnt++] = T((-bb - sqrtD) / den);
  if ((-bb + sqrtD) % den == 0)
    res[cnt++] = T((-bb + sqrtD) / den);
  return {cnt, res};
}
