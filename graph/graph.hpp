#pragma once

#include "../template/template_all_but_modint.hpp"
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
  Edge() : index(-1) {}
  Edge(int s, int t, Cost c, int i) : from(s), to(t), cost(c), index(i) {}
  operator int() const { return to; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<int>, from, to, cost)
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<ll>, from, to, cost)
#endif

// コンストラクタ: n, es, is_directed
template <class Cost, Cost dflt_cost = 1>
struct Graph
{
private:
  int n;
  CSR<Edge<Cost>> g;

public:
  // n, es, is_directed
  Graph() : n(0) {}
  template <class I>
  Graph(int n, const vc<pair<I, I>> &es, bool is_directed) : n(n)
  {
    const int m = es.size();
    if (is_directed)
    {
      vc<pair<int, Edge<Cost>>> edges(m);
      repi(i, m)
      {
        auto [u, v] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[i] = {u, Edge<Cost>(u, v, dflt_cost, i)};
      }
      g = CSR<Edge<Cost>>(n, edges);
    }
    else
    {
      vc<pair<int, Edge<Cost>>> edges(2 * m);
      repi(i, m)
      {
        auto [u, v] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[2 * i] = {u, Edge<Cost>(u, v, dflt_cost, i)};
        edges[2 * i + 1] = {v, Edge<Cost>(v, u, dflt_cost, i)};
      }
      g = CSR<Edge<Cost>>(n, edges);
    }
  }
  template <class I>
  Graph(int n, const vc<tuple<I, I, Cost>> &es, bool is_directed) : n(n)
  {
    const int m = es.size();
    if (is_directed)
    {
      vc<pair<int, Edge<Cost>>> edges(m);
      repi(i, m)
      {
        auto [u, v, w] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[i] = {u, Edge<Cost>(u, v, w, i)};
      }
      g = CSR<Edge<Cost>>(n, edges);
    }
    else
    {
      vc<pair<int, Edge<Cost>>> edges(2 * m);
      repi(i, m)
      {
        auto [u, v, w] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[2 * i] = {u, Edge<Cost>(u, v, w, i)};
        edges[2 * i + 1] = {v, Edge<Cost>(v, u, w, i)};
      }
      g = CSR<Edge<Cost>>(n, edges);
    }
  }

  template <class I = ll>
  I size() const { return n; }

  auto out_edges(int v) const { return g.row(v); }

  vc<Edge<Cost>> edges() const
  {
    int m = 0;
    repi(i, n) fe(e : out_edges(i)) chmax(m, e.index);
    vc<Edge<Cost>> res(m);
    repi(i, n - 1, -1, -1) fe(e : out_edges(i)) res[e.index] = e;
    return res;
  }
  vvc<Edge<Cost>> adj_list() const { return g.to_vv(); }
};
