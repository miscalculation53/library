#pragma once

#include "bipartite_matching.hpp"

/**
 * @brief 二部グラフの最小頂点被覆・最大独立集合・最小辺被覆
 * @docs docs/flow/bipartite_cover.md
 */

template <class Cost, bool is_erasable, class Color>
vc<int> bipartite_vertex_cover(const GraphUndirected<Cost, is_erasable> &g,
                               const vc<Color> &color, const vc<pair<int, int>> &matching)
{
  const int n = g.template size<int>();
  assert(int(color.size()) == n);
  vc<int> mate(n, -1), queue;
  vc<bool> reached(n);
  for (auto [u, v] : matching)
  {
    assert(0 <= u && u < n && 0 <= v && v < n);
    assert(mate[u] == -1 && mate[v] == -1 && color[u] != color[v]);
    mate[u] = v, mate[v] = u;
  }
  for (int v = 0; v < n; v++)
  {
    assert(color[v] == 0 || color[v] == 1);
    if (color[v] == 0 && mate[v] == -1) reached[v] = true, queue.eb(v);
  }
  for (int i = 0; i < int(queue.size()); i++)
  {
    const int u = queue[i];
    for (const auto &e : g.out_edges(u))
    {
      const int v = e.to;
      assert(color[v] == 1);
      if (v == mate[u] || reached[v]) continue;
      reached[v] = true;
      const int w = mate[v];
      assert(w != -1); // 最大マッチングなので増加路は存在しない。
      if (!reached[w]) reached[w] = true, queue.eb(w);
    }
  }
  vc<int> result;
  for (int v = 0; v < n; v++)
    if ((color[v] == 1) == reached[v]) result.eb(v);
  return result;
}

template <class Cost, bool is_erasable, class Color>
vc<int> bipartite_vertex_cover(const GraphUndirected<Cost, is_erasable> &g, const vc<Color> &color)
{ return bipartite_vertex_cover(g, color, bipartite_matching(g, color)); }

template <class Cost, bool is_erasable, class Color>
vc<int> bipartite_independent_set(const GraphUndirected<Cost, is_erasable> &g,
                                  const vc<Color> &color, const vc<pair<int, int>> &matching)
{
  vc<bool> covered(g.template size<int>());
  for (int v : bipartite_vertex_cover(g, color, matching)) covered[v] = true;
  vc<int> result;
  for (int v = 0; v < int(covered.size()); v++) if (!covered[v]) result.eb(v);
  return result;
}

template <class Cost, bool is_erasable, class Color>
vc<int> bipartite_independent_set(const GraphUndirected<Cost, is_erasable> &g, const vc<Color> &color)
{ return bipartite_independent_set(g, color, bipartite_matching(g, color)); }

// 一般の無向グラフでも、最大マッチングが与えられれば O(n+m) で構成できる。
// 辺番号を返す。孤立点がある場合は nullopt。
template <class Cost, bool is_erasable>
optional<vc<int>> minimum_edge_cover(const GraphUndirected<Cost, is_erasable> &g,
                                      const vc<pair<int, int>> &matching)
{
  const int n = g.template size<int>();
  vc<int> mate(n, -1);
  for (auto [u, v] : matching)
  {
    assert(0 <= u && u < n && 0 <= v && v < n && u != v);
    assert(mate[u] == -1 && mate[v] == -1);
    mate[u] = v, mate[v] = u;
  }
  vc<int> result;
  result.reserve(n - matching.size());
  for (int v = 0; v < n; v++)
  {
    const auto row = g.out_edges(v);
    if (row.empty()) return nullopt;
    if (mate[v] == -1) result.eb(row.front().index);
    else if (v < mate[v])
    {
      bool found = false;
      for (const auto &e : row) if (e.to == mate[v])
      { result.eb(e.index); found = true; break; }
      assert(found);
    }
  }
  return result;
}

template <class Cost, bool is_erasable, class Color>
optional<vc<int>> bipartite_edge_cover(const GraphUndirected<Cost, is_erasable> &g,
                                       const vc<Color> &color)
{ return minimum_edge_cover(g, bipartite_matching(g, color)); }
