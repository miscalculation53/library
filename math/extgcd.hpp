#pragma once

#include "../template/template_all.hpp"

/**
 * @brief 拡張ユークリッド互除法 (extgcd)
 * @docs docs/math/extgcd.md
 */

// g == gcd(x, y) >= 0, ax + by == g を満たす (g, x, y)
// max(|x|, |y|) <= max(|a|, |b|)
template <class T = ll>
constexpr tuple<T, T, T> extgcd(const T &a, const T &b)
{
  if (a == 0 && b == 0)
    return {0, 0, 0};
  
  // a*x1 + b*y1 == z1  ...(1)
  // a*x2 + b*y2 == z2  ...(2)
  T x1 = 1, y1 = 0, z1 = a;
  T x2 = 0, y2 = 1, z2 = b;
  while (z2 != 0)
  {
    // (1)' = (2)
    // (2)' = (1) - q*(2)
    T q = z1 / z2;
    tie(x1, x2) = make_pair(x2, x1 - q * x2);
    tie(y1, y2) = make_pair(y2, y1 - q * y2);
    tie(z1, z2) = make_pair(z2, z1 - q * z2);
  }
  if (z1 < 0)
    x1 = -x1, y1 = -y1, z1 = -z1;
  return {z1, x1, y1};
}