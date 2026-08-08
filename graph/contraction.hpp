#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"

/**
 * @brief グラフの縮約
 * @docs docs/graph/contraction.md
 */

// グラフ g と、縮約後の頂点番号 ids を渡す
// ids の番号は [0, k) になっていることを仮定
// コストは特にないグラフを想定、多重辺や自己ループはなし
template <bool is_directed, class Cost, class I>
Graph<is_directed, Cost> contracted_graph(const Graph<is_directed, Cost> &g, const vc<I> &ids)
{
  const int n = g.size();
  assert(SZ(ids) == n);
  const int k = ids.empty() ? 0 : MAX(ids) + 1;
  vc<pair<int, int>> uv;
  fec(e : g.edges())
  {
    if (ids[e.from] != ids[e.to])
      uv.eb(ids[e.from], ids[e.to]);
  }
  sortunique(uv);
  return Graph<is_directed, Cost>(k, uv);
}
