#pragma once

#include "../../graph/graph.hpp"

namespace internal
{
struct BipartiteGraphIndex
{
  vc<int> left, right, index;
  CSR<int> adjacency;

  template <class Cost, bool is_erasable, class Color>
  BipartiteGraphIndex(const GraphUndirected<Cost, is_erasable> &graph, const vc<Color> &color)
  {
    const int n = graph.template size<int>();
    assert(int(color.size()) == n);
    index.resize(n);
    for (int v = 0; v < n; v++)
    {
      assert(color[v] == 0 || color[v] == 1);
      auto &side = color[v] == 1 ? right : left;
      index[v] = side.size();
      side.push_back(v);
    }
    vc<int> degree(left.size());
    for (int u = 0; u < int(left.size()); u++) degree[u] = graph.out_edges(left[u]).size();
    adjacency = CSR<int>(degree);
    for (int v = 0; v < n; v++)
    {
      int next = color[v] == 0 ? adjacency.offset(index[v]) : 0;
      for (auto edge : graph.out_edges(v))
      {
        assert((color[v] == 1) != (color[edge.to] == 1));
        if (color[v] == 0) adjacency.get_elist()[next++] = index[edge.to];
      }
    }
  }
};
}
