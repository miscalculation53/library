#pragma once

#include "../graph/graph.hpp"

namespace internal
{
struct MatroidExchangeGraph
{
  GraphDirected<> graph;
  vc<int> sources;
  vc<bool> sink;
};

template <class M1, class M2>
MatroidExchangeGraph matroid_exchange_graph(M1 &first, M2 &second,
                                           const vc<int> &selected)
{
  const int n = first.size();
  assert(n == second.size());
  vc<bool> used(n);
  for (int e : selected)
  {
    assert(0 <= e && e < n && !used[e]);
    used[e] = true;
  }
  first.build(selected), second.build(selected);
  MatroidExchangeGraph graph{GraphDirected<>{}, {}, vc<bool>(n)};
  vc<pair<int, int>> edges;
  for (int e = 0; e < n; e++) if (!used[e])
  {
    auto a = first.circuit(e), b = second.circuit(e);
    if (!a) graph.sources.eb(e), edges.emplace_back(n, e);
    if (!b) graph.sink[e] = true, edges.emplace_back(e, n + 1);
    for (int f : a ? *a : selected) edges.emplace_back(f, e);
    for (int f : b ? *b : selected) edges.emplace_back(e, f);
  }
  graph.graph = GraphDirected<>(n + 2, edges);
  return graph;
}

inline vc<int> matroid_selected(const vc<bool> &used)
{
  vc<int> selected;
  for (int e = 0; e < int(used.size()); e++) if (used[e]) selected.eb(e);
  return selected;
}
}
