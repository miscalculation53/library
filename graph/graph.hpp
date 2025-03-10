#pragma once

#include "../template/template_all.hpp"
#include "../ds/csr.hpp"

/**
 * @brief グラフ（基底クラス）
 * @docs docs/graph/graph.md
 */

template <class Cost, const Cost dflt_cost = 1>
struct Edge
{
  int from, to;
  Cost cost;
  int index;
  Edge() {}
  Edge(int s, int t, Cost c = dflt_cost, int i = -1) : from(s), to(t), cost(c), index(i) {}
  operator int() const { return to; }
};

template <class Cost, const Cost dflt_cost = 1>
struct Graph
{
  int n;
  CSR<Edge<Cost>> g;
  Graph() : n(0) {}
  template <class EdgeId>
  Graph(int n, const vc<pair<EdgeId, EdgeId>> &es, bool is_directed) : n(n)
  {
    const int m = es.size();
    if (is_directed)
    {
      vc<Edge<Cost>> edges(m);
      repi(i, m)
      {
        auto [u, v] = es[i];
        edges[i] = Edge<Cost>(u, v, dflt_cost, i);
      }
      g = CSR<Edge<Cost>>(edges);
    }
    else
    {
      vc<Edge<Cost>> edges(2 * m);
      repi(i, m)
      {
        auto [u, v] = es[i];
        edges[2 * i] = Edge<Cost>(u, v, dflt_cost, i);
        edges[2 * i + 1] = Edge<Cost>(v, u, dflt_cost, i);
      }
      g = CSR<Edge<Cost>>(edges);
    }
  }
  template <class EdgeId>
  Graph(int n, const vc<tuple<EdgeId, EdgeId, Cost>> &es, bool is_directed) : n(n)
  {
    const int m = es.size();
    if (is_directed)
    {
      vc<Edge<Cost>> edges(m);
      repi(i, m)
      {
        auto [u, v, w] = es[i];
        edges[i] = Edge<Cost>(u, v, w, i);
      }
      g = CSR<Edge<Cost>>(edges);
    }
    else
    {
      vc<Edge<Cost>> edges(2 * m);
      repi(i, m)
      {
        auto [u, v, w] = es[i];
        edges[2 * i] = Edge<Cost>(u, v, w, i);
        edges[2 * i + 1] = Edge<Cost>(v, u, w, i);
      }
      g = CSR<Edge<Cost>>(edges);
    }
  }
};