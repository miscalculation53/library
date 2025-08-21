#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"

/**
 * @brief 三角形列挙
 * @docs docs/graph/triangles.md
 */

// 相互に辺が存在する 3 頂点 u, v, w の組すべてに対し f(u, v, w) を実行
// 入れ替えただけのものは同一視するが、順番は不定
// 組の個数はたかだか m*sqrt(2m) 個
template <class Cost, class F>
void triangles(const GraphUndirected<Cost> &g, const F &f)
{
  const int n = g.size(), m = g.num_of_edges();
  // 大 → 小 で向きづけされていると考えて u → v → w だけを考える
  vc<pair<int, int>> es;
  es.reserve(m);
  fec(e : g.edges())
  {
    int u = e.from, v = e.to;
    if (u == v)
      continue;
    if (g.out_edges(u).size() < g.out_edges(v).size())
      swap(u, v);
    es.eb(u, v);
  }
  GraphDirected<bool> h(n, es);

  vb exists(n, false);
  repi(u, n)
  {
    fec(v : h.out_edges(u)) exists[v] = true;
    fec(v : h.out_edges(u))
    {
      fec(w : h.out_edges(v))
      {
        if (exists[w])
          f(u, v, w);
      }
    }
    fec(v : g.out_edges(u)) exists[v] = false;
  }
}
