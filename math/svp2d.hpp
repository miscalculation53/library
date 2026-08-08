#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../utils/larger_int.hpp"

/**
 * @brief 最短ベクトル問題（$2$ 次元）
 * @docs docs/math/svp2d.md
 */

// 入力: a = (a1, a2), b = (b1, b2)
// 出力: 整数 x, y を用いて v = (v1, v2) = xa + yb と表されるベクトルのうち、
//      ノルムの 2 乗 v1^2 + v2^2 が最小のもののひとつ (符号は不定)
// T は整数、U は T の 2 倍のビット数
// 初期値の √2 倍が T から溢れないなら正しく動く
template <class T = ll, class U = larger_int_t<T>>
pair<T, T> svp2d(const pair<T, T> &a, const pair<T, T> &b)
{
  assert((a != pair<T, T>{0, 0} && b != pair<T, T>{0, 0}));
  auto [a1, a2] = a;
  auto [b1, b2] = b;
  if ((U)a1 * a1 + (U)a2 * a2 < (U)b1 * b1 + (U)b2 * b2)
    swap(a1, b1), swap(a2, b2);
  while ((U)a1 * a1 + (U)a2 * a2 > (U)b1 * b1 + (U)b2 * b2)
  {
    swap(a1, b1), swap(a2, b2);
    T k = divround<U>((U)a1 * b1 + (U)a2 * b2, (U)a1 * a1 + (U)a2 * a2);
    b1 -= k * a1, b2 -= k * a2;
    if (b1 == 0 && b2 == 0)
      return {a1, a2};
  }
  return {a1, a2};
}
