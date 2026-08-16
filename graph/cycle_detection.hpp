#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"

/**
 * @brief サイクル検出
 * @docs docs/graph/cycle_detection.md
 */

// 無向 or 有向 グラフのサイクル検出
// サイクルがなければ (false, 空)
// サイクルがあれば (true, サイクルの辺を並べた配列)
template <bool is_directed, class Cost>
pair<bool, vc<Edge<Cost>>> cycle_detection(const Graph<is_directed, Cost> &g)
{
  const int n = g.size();
  vc<bool> seen(n, false), finished(n, false);
  vc<Edge<Cost>> prv(n, {-1, -1, 1, -1});
  vc<Edge<Cost>> res;
  auto dfs = [&](auto dfs, int v) -> void
  {
    if (!res.empty())
      return;
    seen[v] = true;
    fec(e : g.out_arcs(v))
    {
      if (!res.empty())
        return;
      if (e.index == prv[v].index)
        continue;
      if (finished[e.to])
        continue;
      if (seen[e.to])
      {
        res.eb(v, e.to, e.cost, e.index);
        return;
      }
      prv[e.to] = Edge<Cost>(v, e.to, e.cost, e.index);
      dfs(dfs, e.to);
    }
    finished[v] = true;
  };

  repi(sv, n)
  {
    if (finished[sv])
      continue;
    dfs(dfs, sv);
    if (!res.empty())
      break;
  }
  if (res.empty())
    return {false, {}};
  int v = res.front().from, w = res.front().to;
  while (v != w)
  {
    cauto &e = prv[v];
    res.eb(e);
    v = e.from;
  }
  return {true, reversed(res)};
}
