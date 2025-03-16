#pragma once

#include "../template/template_all.hpp"

/**
 * @brief 二次方程式の整数解
 * @docs docs/math/quadratic_equation_integer.md
 */

// 二次方程式 ax^2 + bx + c == 0 の整数解を求める
// first には整数解の個数が入る
// second には解が入る（ないところは未定義）
// T は a, b, c が（余裕を持って）収まる範囲の型
// U は T の 2 乗が収まる範囲の型
template <class T = ll, class U = i128, class A, class B, class C>
pair<int, array<T, 2>> quadratic_equation_integer(A a, B b, C c)
{
  U D = U(b) * U(b) - U(4) * U(a) * U(c);
  if (D < 0)
    return {0, {}};
  if (D == 0)
  {
    if (-T(b) % (T(2) * T(a)) == 0)
      return {1, {-T(b) / (T(2) * T(a)), 0}};
    else
      return {0, {}};
  }
  T sqrtD = iroot<U>(D, 2);
  if (U(sqrtD) * U(sqrtD) != D)
    return {0, {}};
  int cnt = 0;
  array<T, 2> res{};
  if ((-T(b) - sqrtD) % (T(2) * T(a)) == 0)
    res[cnt++] = (-T(b) - sqrtD) / (T(2) * T(a));
  if ((-T(b) + sqrtD) % (T(2) * T(a)) == 0)
    res[cnt++] = (-T(b) + sqrtD) / (T(2) * T(a));
  return {cnt, res};
}