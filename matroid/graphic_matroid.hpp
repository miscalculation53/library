#pragma once

#include "../graph/graph.hpp"
#include "../tree/add_virtual_root.hpp"
#include "linear_matroid.hpp"

/** 行数は vertices、列数は edges.size()。各列の非零数は自己ループで 0、その他の辺で 2。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class I = int, class J = int, bool Sparse = false>
auto graphic_linear_representation(int vertices, const vc<pair<I, J>> &edges)
{
  assert(vertices >= 0);
  internal::LinearRepresentationBuilder<F, Sparse> columns(vertices, edges.size());
  for (int e = 0; e < int(edges.size()); e++)
  {
    const auto [u, v] = edges[e];
    assert(u128(u) < unsigned(vertices) && u128(v) < unsigned(vertices));
    if (u128(u) == u128(v)) continue;
    columns.set(e, int(u), F::e1());
    columns.set(e, int(v), F::minus(F::e1()));
  }
  return std::move(columns).finish();
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

  template <class I = int, class J = int>
  GraphicMatroid(int vertices, const vc<pair<I, J>> &edges)
      : GraphicMatroid(vertices, vc<pair<int, int>>(ALL(edges)))
  {
    for (auto [u, v] : edges)
      assert(u128(u) < unsigned(vertices) && u128(v) < unsigned(vertices));
  }

  CPP_DUMP_DEFINE_DATA(vertices, edges, selected);

  int size() const { return edges.size(); }

  /** 行数は頂点数、列数は辺数。各列の非零数は自己ループで 0、その他の辺で 2。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  LinearMatroid<F> linear_representation() const
  {
    return graphic_linear_representation<F>(vertices, edges);
  }

  /** 行数は頂点数、列数は辺数。各列の非零数は自己ループで 0、その他の辺で 2。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  SparseMatrix<F> sparse_linear_representation() const
  {
    return graphic_linear_representation<F, int, int, true>(vertices, edges);
  }

  template <class I = int>
  void build(const vc<I> &s)
  {
    fill(ALL(selected), false);
    vc<pair<int, int>> selected_edges;
    selected_edges.reserve(s.size());
    for (auto value : s)
    {
      assert(u128(value) < unsigned(size()));
      const int e = int(value);
      assert(!selected[e]);
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
        for (const auto &edge : graph.out_edges(v)) if (int(s[edge.index]) != parent_edge[v])
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
