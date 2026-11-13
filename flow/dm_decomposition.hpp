#pragma once

#include "bipartite_matching.hpp"
#include "../graph/scc.hpp"
#include "../graph/contraction.hpp"

/**
 * @brief Dulmage–Mendelsohn 分解
 * @docs docs/flow/dm_decomposition.md
 */
struct DMDecomposition
{
  vc<int> ids;
  CSR<int> left, right;
  GraphDirected<> dag;
  vc<pair<int, int>> matching;

  int size() const { return left.template size<int>(); }
};

// 最大マッチングを元の頂点番号による色0から色1への頂点対で渡す。
template <class Cost, bool is_erasable, class Color>
DMDecomposition dm_decomposition(const GraphUndirected<Cost, is_erasable> &g,
                                const vc<Color> &color, vc<pair<int, int>> matching)
{
  const int n = g.template size<int>();
  assert(int(color.size()) == n);
  for (int v = 0; v < n; v++) assert(color[v] == 0 || color[v] == 1);
  vc<int> mate(n, -1);
  for (auto [u, v] : matching)
  {
    assert(0 <= u && u < n && 0 <= v && v < n);
    assert(color[u] == 0 && color[v] == 1 && mate[u] == -1 && mate[v] == -1);
    mate[u] = v, mate[v] = u;
  }
  vc<pair<int, int>> edges;
  edges.reserve(g.template num_of_edges<int>() + matching.size() + 2 * n + 1);
  for (int v = 0; v < n; v++)
  {
    bool found = mate[v] == -1;
    for (const auto &e : g.out_edges(v))
    {
      assert((color[v] == 1) != (color[e.to] == 1));
      if (color[v] == 0) edges.emplace_back(v, e.to);
      if (mate[v] == e.to) found = true;
    }
    assert(found);
    (void)found;
  }
  for (auto [u, v] : matching) edges.emplace_back(v, u);
  const int original_edges = edges.size();
  for (int v = 0; v < n; v++)
  {
    if (color[v] == 0) edges.emplace_back(v, n);
    else edges.emplace_back(n + 1, v);
    if (mate[v] == -1)
    {
      if (color[v] == 0) edges.emplace_back(n, v);
      else edges.emplace_back(v, n + 1);
    }
  }
  edges.emplace_back(n + 1, n);
  auto ids = scc<void, int>(GraphDirected<>(n + 2, edges));
  assert(ids[n] != ids[n + 1]);
  const int blocks = ids[n] + 1;
  for (int &id : ids) id = blocks - 1 - id;
  edges.resize(original_edges);
  for (auto &[u, v] : edges) swap(u, v);
  auto dag = contracted_graph(GraphDirected<>(n + 2, edges), ids);
  ids.resize(n);
  vc<pair<int, int>> left, right;
  for (int v = 0; v < n; v++)
    (color[v] == 0 ? left : right).emplace_back(ids[v], v);
  return {move(ids), CSR<int>(blocks, left), CSR<int>(blocks, right), move(dag), move(matching)};
}

template <class Cost, bool is_erasable, class Color, class I = int, class J = int>
DMDecomposition dm_decomposition(const GraphUndirected<Cost, is_erasable> &g,
                                const vc<Color> &color, const vc<pair<I, J>> &matching)
{
  for (auto [u, v] : matching)
    assert(u128(u) < u128(g.size()) && u128(v) < u128(g.size()));
  return dm_decomposition(g, color, vc<pair<int, int>>(ALL(matching)));
}

// 最大マッチングも求めてDM分解を返す。
template <class Cost, bool is_erasable, class Color>
DMDecomposition dm_decomposition(const GraphUndirected<Cost, is_erasable> &g, const vc<Color> &color)
{
  return dm_decomposition(g, color, bipartite_matching(g, color));
}
