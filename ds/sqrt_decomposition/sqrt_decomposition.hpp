#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 平方分割による区間処理
 * @docs docs/ds/sqrt_decomposition/sqrt_decomposition.md
 */

// ブロック b は [b * B, min((b + 1) * B, n)) を担当する。
// [l, r) を左から順に、端の要素には point(i)、全体を含むブロックには block(b) を呼ぶ。
template <int B = 512, class Point, class Block>
void sqrt_decomposition(int n, int l, int r, Point &&point, Block &&block)
{
  static_assert(B > 0);
  assert(0 <= l && l <= r && r <= n);
  const int bl = l / B + (l % B != 0);
  const int br = r == n ? n / B + (n % B != 0) : r / B;
  if (bl >= br)
  {
    for (int i = l; i < r; i++) point(i);
  }
  else
  {
    for (int i = l; i < bl * B; i++) point(i);
    for (int b = bl; b < br; b++) block(b);
    if (r != n)
      for (int i = br * B; i < r; i++) point(i);
  }
}
