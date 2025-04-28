#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 下から $k$ 番目のビットを取得
 * @docs docs/bit/kth_bit.md
 */

// x の立っているビットのうち下から k 番目のものの位置を取得
ull kth_bit_pos(ull x, int k)
{
  assert(0 <= k && k < (int)popcount(x));
  auto judge = [&](int i) -> bool
  { return (int)popcount(x & ((1ULL << i) - 1)) <= k; };
  return binsearch(judge, 0, 64, true, false).first;
}
