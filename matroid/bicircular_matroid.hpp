#pragma once

#include "../ds/uf/uf.hpp"
#include "../graph/graph.hpp"
#include "linear_matroid.hpp"

/** 行数は vertices、列数は edges.size()。各列の非零数は自己ループで高々 1、その他の辺で高々 2。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &,
          class I = int, class J = int, bool Sparse = false>
auto bicircular_linear_representation(int vertices, const vc<pair<I, J>> &edges,
                                                  Sample &&sample = random_modint<typename F::S>)
{
  assert(vertices >= 0);
  internal::LinearRepresentationBuilder<F, Sparse> columns(vertices, edges.size());
  for (int e = 0; e < int(edges.size()); e++)
  {
    const auto [u, v] = edges[e];
    assert(u128(u) < unsigned(vertices) && u128(v) < unsigned(vertices));
    columns.set(e, int(u), sample());
    if (u128(u) != u128(v)) columns.set(e, int(v), sample());
  }
  return std::move(columns).finish();
}

/**
 * @brief bicircular matroid
 * @docs docs/matroid/bicircular_matroid.md
 */
class BicircularMatroid
{
  int vertices;
  vc<pair<int, int>> edges;
  vc<int> component, independent;
  vc<bool> cyclic, selected;

public:
  BicircularMatroid(int vertices, vc<pair<int, int>> edges)
      : vertices(vertices), edges(move(edges)), selected(this->edges.size())
  {
    assert(vertices >= 0);
    for (auto [u, v] : this->edges)
      assert(0 <= u && u < vertices && 0 <= v && v < vertices);
    build({});
  }

  template <class I = int, class J = int>
  BicircularMatroid(int vertices, const vc<pair<I, J>> &edges)
      : BicircularMatroid(vertices, vc<pair<int, int>>(ALL(edges)))
  {
    for (auto [u, v] : edges)
      assert(u128(u) < unsigned(vertices) && u128(v) < unsigned(vertices));
  }

  CPP_DUMP_DEFINE_DATA(vertices, edges, independent, cyclic);

  int size() const { return edges.size(); }

  /** 行数は頂点数、列数は辺数。各列の非零数は自己ループで高々 1、その他の辺で高々 2。 */
  template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &>
  LinearMatroid<F> linear_representation(Sample &&sample = random_modint<typename F::S>) const
  {
    return bicircular_linear_representation<F>(vertices, edges, std::forward<Sample>(sample));
  }

  /** 行数は頂点数、列数は辺数。各列の非零数は自己ループで高々 1、その他の辺で高々 2。 */
  template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &>
  SparseMatrix<F> sparse_linear_representation(Sample &&sample = random_modint<typename F::S>) const
  {
    return bicircular_linear_representation<F, Sample, int, int, true>(vertices, edges, std::forward<Sample>(sample));
  }

  template <class I = int>
  void build(const vc<I> &s)
  {
    fill(ALL(selected), false);
    independent.assign(ALL(s));
    UnionFind<UFDataEmpty<>> uf(vertices);
    for (auto value : s)
    {
      assert(u128(value) < unsigned(size()));
      const int e = int(value);
      assert(!selected[e]);
      selected[e] = true;
      auto [u, v] = edges[e];
      uf.merge(u, v);
    }
    component.resize(vertices);
    vc<int> count(vertices), vertex_count(vertices);
    for (int v = 0; v < vertices; v++) vertex_count[component[v] = uf.leader(v)]++;
    for (int e : s) count[component[edges[e].first]]++;
    cyclic.assign(vertices, false);
    for (int r = 0; r < vertices; r++)
    {
      assert(count[r] <= vertex_count[r]);
      cyclic[r] = vertex_count[r] > 0 && count[r] == vertex_count[r];
    }
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < size() && !selected[e]);
    auto [a, b] = edges[e];
    const int ca = component[a], cb = component[b];
    if (ca == cb ? !cyclic[ca] : !(cyclic[ca] && cyclic[cb])) return nullopt;
    vc<pair<int, int>> selected_edges;
    selected_edges.reserve(independent.size() + 1);
    vc<int> degree(vertices), candidates;
    auto add = [&](int id)
    {
      auto [u, v] = edges[id];
      selected_edges.emplace_back(u, v);
      degree[u]++, degree[v]++;
    };
    for (int id : independent)
    {
      const int c = component[edges[id].first];
      if (c == ca || c == cb) candidates.push_back(id), add(id);
    }
    add(e);
    const GraphUndirected<> graph(vertices, selected_edges);
    vc<int> queue;
    for (int v = 0; v < vertices; v++) if (degree[v] == 1) queue.push_back(v);
    for (int i = 0; i < int(queue.size()); i++)
    {
      const int v = queue[i];
      degree[v] = 0;
      for (const auto &edge : graph.out_edges(v)) if (degree[edge.to] > 0)
        if (--degree[edge.to] == 1) queue.push_back(edge.to);
    }
    vc<int> result;
    for (int id : candidates)
    {
      auto [u, v] = edges[id];
      if (degree[u] > 0 && degree[v] > 0) result.push_back(id);
    }
    return result;
  }
};
