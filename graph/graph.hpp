#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../ds/csr.hpp"

/**
 * @brief グラフクラス
 * @docs docs/graph/graph.md
 */

template <class Cost>
struct Edge
{
  int from, to;
  Cost cost;
  int index;
  Edge() : from(-1), to(-1), index(-1) {}
  Edge(int s, int t, Cost c, int i = -1) : from(s), to(t), cost(c), index(i) {}
  operator int() const { return to; }
  bool operator<(const Edge &rhs) const { return cost < rhs.cost; }
  // 逆辺を返す (もとの辺は変更しない)
  Edge rev() const { return Edge(to, from, cost, index); }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<int>, from, to, cost)
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<ll>, from, to, cost)
#endif

template <class Cost>
vc<Edge<Cost>> rev_path(const vc<Edge<Cost>> &path)
{
  const int len = path.size();
  vc<Edge<Cost>> res(len);
  repi(i, len) res[i] = path[len - 1 - i].rev();
  return res;
}

// コンストラクタ: n, es
template <bool is_directed, class Cost>
struct Graph
{
  using E = Edge<Cost>;

protected:
  int n, m;
  CSR<E> g;

public:
  Graph() {}
  template <class I>
  Graph(int n, const vc<pair<I, I>> &es, const Cost &dflt_cost = 1) : n(n), m(es.size())
  {
    if constexpr (is_directed)
    {
      vc<pair<int, E>> edges(m);
      repi(i, m)
      {
        auto [u, v] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[i] = {u, E(u, v, dflt_cost, i)};
      }
      g = CSR<E>(n, edges);
    }
    else
    {
      vc<pair<int, E>> edges(2 * m);
      repi(i, m)
      {
        auto [u, v] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[2 * i] = {u, E(u, v, dflt_cost, i)};
        edges[2 * i + 1] = {v, E(v, u, dflt_cost, i)};
      }
      g = CSR<E>(n, edges);
    }
  }
  template <class I>
  Graph(int n, const vc<tuple<I, I, Cost>> &es) : n(n), m(es.size())
  {
    if constexpr (is_directed)
    {
      vc<pair<int, E>> edges(m);
      repi(i, m)
      {
        auto [u, v, w] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[i] = {u, E(u, v, w, i)};
      }
      g = CSR<E>(n, edges);
    }
    else
    {
      vc<pair<int, E>> edges(2 * m);
      repi(i, m)
      {
        auto [u, v, w] = es[i];
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        edges[2 * i] = {u, E(u, v, w, i)};
        edges[2 * i + 1] = {v, E(v, u, w, i)};
      }
      g = CSR<E>(n, edges);
    }
  }

  // 頂点数
  template <class I = ll>
  I size() const { return n; }
  // 辺数
  template <class I = ll>
  I num_of_edges() const { return m; }

  // v から出る辺の集合
  auto out_edges(int v) const { return g[v]; }
  // v から出る頂点の集合
  template <class I = ll>
  vc<I> out_vertices(int v) const
  {
    vc<I> res;
    res.reserve(g[v].size());
    fec(e : g[v]) res.eb(e.to);
    return res;
  }

  // 辺番号から辺を取得する
  // 無向グラフの場合 from <= to を満たすように返す
  E get_edge(int eid) const
  {
    if constexpr (is_directed)
      return g.find_by_eid(eid);
    else
    {
      E e = g.find_by_eid(eid * 2);
      return e.from > e.to ? e.rev() : e;
    }
  }

  // すべての辺を返す (辺番号順)
  // 無向グラフの場合、各辺は from <= to を満たす
  vc<E> edges() const
  {
    vc<E> res(m);
    repi(i, m) res[i] = get_edge(i);
    return res;
  }
  // 隣接リスト
  vvc<E> adj_list() const { return g.to_vv(); }
  // 隣接行列 (辺の本数を格納)
  template <class I = ll>
  vvc<I> adj_matrix_ecnt() const
  {
    vvc<I> res(n, vc<I>(n, 0));
    fec(e : edges())
    {
      res[e.from][e.to]++;
      if (!is_directed && e.from != e.to)
        res[e.to][e.from]++;
    }
    return res;
  }

  // 入次数の列
  template <class I = ll>
  vc<I> indegs() const
  {
    vc<I> res(n);
    fec(e : edges())
    {
      res[e.to]++;
      if (!is_directed && e.from != e.to)
        res[e.from]++;
    }
    return res;
  }
  // 出次数の列
  template <class I = ll>
  vc<I> outdegs() const
  {
    vc<I> res(n);
    fec(e : edges()) 
    {
      res[e.from]++;
      if (!is_directed && e.from != e.to)
        res[e.to]++;
    }
    return res;
  }
};

template <class Cost>
using GraphDirected = Graph<true, Cost>;
template <class Cost>
using GraphUndirected = Graph<false, Cost>;

template <class Cost>
GraphDirected<Cost> rev_graph(const GraphDirected<Cost> &g)
{
  const int n = g.size(), m = g.num_of_edges();
  vc<tuple<int, int, Cost>> uvw;
  uvw.reserve(m);
  fec(e : g.edges()) uvw.eb(e.to, e.from, e.cost);
  return GraphDirected<Cost>(n, uvw);
}
