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
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<bool>, from, to, cost)
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
template <bool is_directed, class Cost, bool is_erasable = false>
struct Graph
{
  using E = Edge<Cost>;

protected:
  int n, m, era;
  CSR<E, is_erasable> g;
  vc<int> eid_to_elist_id;

  template <class F>
  void build(F get_edge)
  {
    if constexpr (is_directed)
    {
      vc<pair<int, E>> edges(m);
      repi(i, m)
      {
        auto [u, v, w] = get_edge(i);
        edges[i] = {u, E(u, v, w, i)};
      }
      g = CSR<E, is_erasable>(n, edges);
      if constexpr (is_erasable)
      {
        eid_to_elist_id.resize(m, -1);
        int k = 0;
        repi(v, n) fec(e : g[v]) eid_to_elist_id[e.index] = k++;
      }
    }
    else
    {
      vc<pair<int, E>> edges;
      edges.reserve(2 * m);
      repi(i, m)
      {
        auto [u, v, w] = get_edge(i);
        edges.eb(u, E(u, v, w, i));
        if (u != v)
          edges.eb(v, E(v, u, w, i));
      }
      g = CSR<E, is_erasable>(n, edges);
      if constexpr (is_erasable)
      {
        eid_to_elist_id.resize(2 * m, -1);
        int k = 0;
        repi(v, n) fec(e : g[v]) eid_to_elist_id[2 * e.index + (e.from <= e.to)] = k++;
      }
    }
  }

public:
  Graph() {}
  template <class I>
  Graph(int n, const vc<pair<I, I>> &es, const Cost &dflt_cost = 1) : n(n), m(es.size()), era(0)
  {
    build(LMD(i, (tuple{es[i].first, es[i].second, dflt_cost})));
  }
  template <class I>
  Graph(int n, const vc<tuple<I, I, Cost>> &es) : n(n), m(es.size()), era(0)
  {
    build(LMD(i, es[i]));
  }
  Graph(int n, const vc<E> &es) : n(n), m(es.size()), era(0)
  {
    build(LMD(i, (tuple{es[i].from, es[i].to, es[i].cost})));
  }

  // 頂点数
  template <class I = ll>
  I size() const { return n; }
  // 辺数
  template <class I = ll>
  I num_of_edges() const { return m - era; }

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

  // すべての辺を返す。辺番号順とは限らない
  // 無向グラフの場合、各辺は from <= to を満たす
  vc<E> edges() const
  {
    vc<E> res;
    res.reserve(m);
    if constexpr (is_directed)
    {
      repi(v, n) fec(e : g[v])
      {
        res.eb(e);
      }
    }
    else
    {
      repi(v, n) fec(e : g[v])
      {
        if (e.from <= e.to) res.eb(e);
      }
    }
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
      if constexpr (!is_directed)
        if (e.from != e.to)
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
      if constexpr (!is_directed)
        if (e.from != e.to)
          res[e.from]++;
    }
    return res;
  }
  // 出次数の列
  template <class I = ll>
  vc<I> outdegs() const
  {
    vc<I> res(n);
    repi(v, n) res[v] = g[v].size();
    return res;
  }

  void erase_edge(int eid)
  {
    static_assert(is_erasable);
    assert(0 <= eid && eid < m);
    if constexpr (is_directed)
    {
      internal_erase(eid_to_elist_id[eid]);
      eid_to_elist_id[eid] = -1;
    }
    else
    {
      repi(j, 2)
      {
        internal_erase(eid_to_elist_id[2 * eid + j]);
        eid_to_elist_id[2 * eid + j] = -1;
      }
    }
    era++;
  }

private:
  void internal_erase(int elist_id)
  {
    if (elist_id < 0)
      return;
    auto &elist = g.get_elist();
    auto &target = elist[elist_id];
    const int u = target.from;
    auto &last = g[u].back();
    const int last_elist_id = &last - &elist[0];
    if (elist_id != last_elist_id)
    {
      swap(target, last);
      int eid = target.index;
      if constexpr (is_directed)
        eid_to_elist_id[eid] = elist_id;
      else
      {
        if (eid_to_elist_id[2 * eid] == last_elist_id)
          eid_to_elist_id[2 * eid] = elist_id;
        else
          eid_to_elist_id[2 * eid + 1] = elist_id;
      }
    }
    g.pop_back(u);
  }
};

template <class Cost, bool is_erasable = false>
using GraphDirected = Graph<true, Cost, is_erasable>;
template <class Cost, bool is_erasable = false>
using GraphUndirected = Graph<false, Cost, is_erasable>;

template <class Cost>
GraphDirected<Cost> rev_graph(const GraphDirected<Cost> &g)
{
  const int n = g.size(), m = g.num_of_edges();
  vc<tuple<int, int, Cost>> uvw;
  uvw.reserve(m);
  fec(e : g.edges()) uvw.eb(e.to, e.from, e.cost);
  return GraphDirected<Cost>(n, uvw);
}
