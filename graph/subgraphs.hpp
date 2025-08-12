#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"
#include "../ds/my_queue.hpp"

/**
 * @brief 部分グラフへの分解
 * @docs docs/graph/subgraphs.md
 */

// グラフ g と、部分グラフの番号 ids を渡す
// ids の番号は [0, k) になっていることを想定
// (graphs, nvids) を返す
// もとのグラフの頂点 u <-> 部分グラフ l の頂点 i の対応は
// u = grp.idxs[l][i]  (GroupIndex を使う)
// l = ids[u], i = nvids[u]
template <bool is_directed, class Cost, class I>
pair<vc<Graph<is_directed, Cost>>, vc<I>> subgraphs(const Graph<is_directed, Cost> &g, const vc<I> &ids)
{
  const int n = g.size();
  assert(SZ(ids) == n);
  const int k = MAX(ids) + 1;
  vc<I> nvids(n), cnt(k);
  repi(u, n) nvids[u] = cnt[ids[u]]++;
  vc<Graph<is_directed, Cost>> graphs(k);
  {
    vc<pair<int, int>> uvs(k);
    fec(e : g.edges())
    {
      if (ids[e.from] == ids[e.to])
        uvs[ids[e.from]].eb(nvids[e.from], nvids[e.to]);
    }
    repi(l, k) graphs[l] = Graph<is_directed, cost>(cnt[l], uvs[l]);
  }
  return {graphs, nvids};
}
