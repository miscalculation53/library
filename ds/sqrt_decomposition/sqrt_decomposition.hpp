#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 平方分割のブロックと区間の交わりを列挙
 * @docs docs/ds/sqrt_decomposition/sqrt_decomposition.md
 */

struct SqrtDecompositionSegment
{
  // [l, r) は対象部分、[block_l, block_r) はブロック全体。
  int b, l, r, block_l, block_r;
  bool full() const { return l == block_l && r == block_r; }
};

namespace internal
{
template <int B>
struct SqrtDecompositionBlocks
{
  int n, l, r;
  struct Iterator
  {
    int n, l, r, b;
    SqrtDecompositionSegment operator*() const
    {
      const int bl = b * B, br = bl + min(B, n - bl);
      return {b, max(l, bl), min(r, br), bl, br};
    }
    Iterator &operator++() { ++b; return *this; }
    bool operator!=(const Iterator &other) const { return b != other.b; }
  };
  Iterator begin() const { return {n, l, r, l / B}; }
  Iterator end() const { return {n, l, r, l == r ? l / B : (r - 1) / B + 1}; }
};
}

// 各ブロックとの空でない交わりを左から順に返す。配列の確保は行わない。
template <int B = 512>
auto sqrt_decomposition_blocks(int n, int l, int r)
{
  static_assert(B > 0);
  assert(0 <= l && l <= r && r <= n);
  return internal::SqrtDecompositionBlocks<B>{n, l, r};
}

// 各点と完全に含まれるブロックを独立に処理する場合の簡便版。
template <int B = 512, class Point, class Block>
void sqrt_decomposition(int n, int l, int r, Point &&point, Block &&block)
{
  for (auto seg : sqrt_decomposition_blocks<B>(n, l, r))
  {
    if (seg.full()) block(seg.b);
    else
      for (int i = seg.l; i < seg.r; i++) point(i);
  }
}
