#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 尺取り法
 * @docs docs/algo/two_pointers.md
 */

// 長さ n+1 (l = 0, 1, ..., n)
// rok[l] := [l, r) が ok となる最大の r
// add_r(l, r): 現在の区間が [l, r) であるとして r を追加する (0 <= l <= r < n)
// del_l(l, r): 現在の区間が [l, r) であるとして l を削除する (0 <= l < r <= n)
// judge(l, r): 現在の区間が [l, r) であるとして ok か判定する
// 制約: 空の列は ok
template <class T = ll>
vc<T> right_ok(int n, cauto &add_r, cauto &del_l, cauto &judge)
{
  int l = 0, r = 0;  // [l, r)
  assert(judge(0, 0));
  vc<T> rok(n + 1, n);
  while (r < n)
  {
    add_r(l, r++);
    while (!judge(l, r))
    {
      assert(l != r);
      // [l, r) is ng but [l, r-1) was ok
      rok[l] = r - 1;
      del_l(l++, r);
    }
  }
  return rok;
}
// 長さ n+1 (r = 0, 1, ..., n)
// lok[r] := [l, r) が ok となる最小の l
// add_r(l, r): 現在の区間が [l, r) であるとして r を追加する (0 <= l <= r < n)
// del_l(l, r): 現在の区間が [l, r) であるとして l を削除する (0 <= l < r <= n)
// judge(l, r): 現在の区間が [l, r) であるとして ok か判定する
// 制約: 空の列は ok
template <class T = ll>
vc<T> left_ok(int n, cauto &add_r, cauto &del_l, cauto &judge)
{
  int l = 0, r = 0;  // [l, r)
  assert(judge(0, 0));
  vc<T> lok(n + 1, 0);
  while (r < n)
  {
    add_r(l, r++);
    while (!judge(l, r))
    {
      assert(l != r);
      del_l(l++, r);
    }
    // [l, r) is ok but [l-1, r) was ng
    lok[r] = l;
  }
  return lok;
}
