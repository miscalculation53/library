#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 区間の併合
 * @docs docs/algo/merge_intervals.md
 */

// 半開区間 [l, r) の和集合
// 接する区間も併合し、l >= r の空区間は除く
template <class T>
vc<pair<T, T>> merge_intervals(vc<pair<T, T>> intervals)
{
  sort(ALL(intervals));
  vc<pair<T, T>> res;
  for (const auto &[l, r] : intervals)
  {
    if (!(l < r))
      continue;
    if (res.empty() || res.back().second < l)
      res.eb(l, r);
    else if (res.back().second < r)
      res.back().second = r;
  }
  return res;
}
