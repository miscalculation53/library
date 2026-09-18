#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"

/**
 * @brief 強連結成分分解
 * @docs docs/graph/scc.md
 */

// 各頂点の成分番号を返す
// 縮約した後の DAG はトポロジカルソートされている
template <class Cost, class I = ll>
vc<I> scc(const GraphDirected<Cost> &g)
{
  const int n = g.size();
  auto rg = rev_graph(g);

  vc<bool> visited(n, false);
  vc<int> ord;
  ord.reserve(n);
  auto dfs1 = [&](auto dfs1, int v) -> void
  {
    if (visited[v])
      return;
    visited[v] = true;
    fec(nv : g.out_edges(v)) dfs1(dfs1, nv);
    ord.eb(v);
  };

  int cnt = 0;
  vc<I> ids(n, -1);
  auto dfs2 = [&](auto dfs2, int v) -> void
  {
    if (ids[v] != -1)
      return;
    ids[v] = cnt;
    fec(nv : rg.out_edges(v)) dfs2(dfs2, nv);
  };

  repi(v, n) dfs1(dfs1, v);
  reverse(ALL(ord));
  fe(v : ord) if (ids[v] == -1) { dfs2(dfs2, v), cnt++; }
  return ids;
}
