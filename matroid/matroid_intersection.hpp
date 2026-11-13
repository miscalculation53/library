#pragma once

#include "internal_exchange_graph.hpp"
#include "../graph/sssp.hpp"

/**
 * @brief マトロイド交叉
 * @docs docs/matroid/matroid_intersection.md
 */

template <class M1, class M2, class I = int>
vc<int> matroid_intersection(M1 first, M2 second, const vc<I> &initial = {})
{
  const int n = first.size();
  assert(n == second.size());
  vc<bool> used(n);
  for (auto value : initial)
  {
    assert(u128(value) < unsigned(n));
    const int e = int(value);
    assert(!used[e]);
    used[e] = true;
  }
  while (true)
  {
    auto graph = internal::matroid_exchange_graph(first, second, internal::matroid_selected(used));
    ShortestPath<true, void> shortest_path(graph.graph);
    shortest_path.bfs(n, n + 1);
    auto path = shortest_path.path_vertex(n + 1);
    if (!path) return internal::matroid_selected(used);
    for (int e : *path) if (e < n) used[e] = !used[e];
  }
}
