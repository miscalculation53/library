#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief マトロイドの重み貪欲法
 * @docs docs/matroid/matroid_greedy.md
 */

// 最大重み基。nonnegative_only=true は最大重み独立集合。
template <class M, class W>
vc<int> matroid_greedy(M matroid, const vc<W> &weight, bool nonnegative_only = false)
{
  const int n = matroid.size();
  assert(int(weight.size()) == n);
  vc<int> order(n), selected;
  iota(ALL(order), 0);
  stable_sort(ALL(order), [&](int e, int f) { return weight[f] < weight[e]; });
  matroid.build(selected);
  for (int e : order)
  {
    if (nonnegative_only && weight[e] < W{}) break;
    if (!matroid.circuit(e))
    {
      selected.eb(e);
      matroid.build(selected);
    }
  }
  sort(ALL(selected));
  return selected;
}
