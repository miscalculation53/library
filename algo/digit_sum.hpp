#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 桁和
 * @docs docs/algo/digit_sum.md
 */

template <class T>
ll digit_sum(T x)
{
  static const int LIM = 100000;
  static int mem[LIM];
  static bool memok = false;
  if (!memok)
  {
    mem[0] = 0;
    repi(x, 1, LIM) mem[x] = x % 10 + mem[x / 10];
    memok = true;
  }
  int ans = 0;
  while (x)
    ans += mem[x % LIM], x /= LIM;
  return ans;
}
