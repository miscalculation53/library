#pragma once

#include "../ds/uf/uf.hpp"
#include "../graph/graph.hpp"
#include "linear_matroid.hpp"

template <class F, class Sample>
LinearMatroid<F> bicircular_linear_representation(int vertices, const vc<pair<int, int>> &edges,
                                                  Sample &&sample)
{
  using S = typename F::S;
  assert(vertices >= 0);
  vvc<S> columns(edges.size(), vc<S>(vertices, F::e0()));
  for (int e = 0; e < int(edges.size()); e++)
  {
    const auto [u, v] = edges[e];
    assert(0 <= u && u < vertices && 0 <= v && v < vertices);
    columns[e][u] = sample();
    if (u != v) columns[e][v] = sample();
  }
  return LinearMatroid<F>(vertices, std::move(columns));
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

  int size() const { return edges.size(); }

  template <class F, class Sample>
  LinearMatroid<F> linear_representation(Sample &&sample) const
  {
    return bicircular_linear_representation<F>(vertices, edges, std::forward<Sample>(sample));
  }

  void build(const vc<int> &s)
  {
    fill(ALL(selected), false);
    independent = s;
    UnionFind<UFDataEmpty<>> uf(vertices);
    for (int e : s)
    {
      assert(0 <= e && e < size() && !selected[e]);
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
