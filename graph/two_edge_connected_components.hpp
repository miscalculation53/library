#pragma once

#include "lowlink.hpp"

/**
 * @brief 二辺連結成分分解
 * @docs docs/graph/two_edge_connected_components.md
 */

template <class I = ll>
vc<I> two_edge_connected_component_ids(const LowLink &lowlink)
{
  vc<I> result(lowlink.size());
  int components = 0;
  for (int v : lowlink.dfs_order())
  {
    // 無向 DFS では、親辺が橋の頂点と根で low[v] == order[v] になる。
    if (lowlink.low(v) == lowlink.order(v)) result[v] = components++;
    else result[v] = result[lowlink.parent(v)->first];
  }
  return result;
}

template <class I = ll, class Cost, bool is_erasable>
vc<I> two_edge_connected_component_ids(const GraphUndirected<Cost, is_erasable> &g)
{
  const LowLink decomposition(g);
  return two_edge_connected_component_ids<I>(decomposition);
}
