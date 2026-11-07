#pragma once

#include "../graph/graph.hpp"
#include "../tree/add_virtual_root.hpp"
#include "linear_matroid.hpp"

template <class F>
LinearMatroid<F> graphic_linear_representation(int vertices, const vc<pair<int, int>> &edges)
{
  using S = typename F::S;
  assert(vertices >= 0);
  vvc<S> columns(edges.size(), vc<S>(vertices, F::e0()));
  for (int e = 0; e < int(edges.size()); e++)
  {
    const auto [u, v] = edges[e];
    assert(0 <= u && u < vertices && 0 <= v && v < vertices);
    columns[e][u] = F::e1();
    columns[e][v] = F::add(columns[e][v], F::minus(F::e1()));
  }
  return LinearMatroid<F>(vertices, columns);
}

/**
 * @brief graphic matroid
 * @docs docs/matroid/graphic_matroid.md
 */
class GraphicMatroid
{
  int vertices;
  vc<pair<int, int>> edges;
  RootedTree forest;
  vc<int> component, parent_edge;
  vc<bool> selected;

public:
  GraphicMatroid(int vertices, vc<pair<int, int>> edges)
      : vertices(vertices), edges(move(edges)), selected(this->edges.size())
  {
    assert(vertices >= 0);
    for (auto [u, v] : this->edges)
      assert(0 <= u && u < vertices && 0 <= v && v < vertices);
    build({});
  }

  int size() const { return edges.size(); }

  template <class F>
  LinearMatroid<F> linear_representation() const
  {
    return graphic_linear_representation<F>(vertices, edges);
  }

  void build(const vc<int> &s)
  {
    fill(ALL(selected), false);
    vc<pair<int, int>> selected_edges;
    selected_edges.reserve(s.size());
    for (int e : s)
    {
      assert(0 <= e && e < size() && !selected[e]);
      selected[e] = true;
      selected_edges.push_back(edges[e]);
    }
    const GraphUndirected<> graph(vertices, selected_edges);
    component.assign(vertices, -1);
    vc<int> parent(vertices, -1);
    parent_edge.assign(vertices, -1);
    vc<int> queue;
    for (int root = 0; root < vertices; root++) if (component[root] == -1)
    {
      component[root] = root;
      queue.assign(1, root);
      for (int i = 0; i < int(queue.size()); i++)
      {
        int v = queue[i];
        for (const auto &edge : graph.out_edges(v)) if (s[edge.index] != parent_edge[v])
        {
          const int u = edge.to;
          assert(component[u] == -1);
          component[u] = root;
          parent[u] = v, parent_edge[u] = s[edge.index];
          queue.push_back(u);
        }
      }
    }
    forest = add_virtual_root(vertices, parent);
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < size() && !selected[e]);
    auto [u, v] = edges[e];
    if (component[u] != component[v]) return nullopt;
    vc<int> result;
    while (u != v)
    {
      if (forest.depth<int>(u) < forest.depth<int>(v)) swap(u, v);
      result.push_back(parent_edge[u]);
      u = forest.parent(u);
    }
    return result;
  }
};
