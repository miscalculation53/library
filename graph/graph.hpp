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

template <class Cost>
struct GraphArc
{
  int to, index;
  Cost cost;
  operator int() const { return to; }
  bool operator<(const GraphArc &rhs) const { return cost < rhs.cost; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<bool>, from, to, cost)
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<int>, from, to, cost)
CPP_DUMP_DEFINE_EXPORT_OBJECT(Edge<ll>, from, to, cost)
CPP_DUMP_DEFINE_EXPORT_OBJECT(GraphArc<bool>, to, cost)
CPP_DUMP_DEFINE_EXPORT_OBJECT(GraphArc<int>, to, cost)
CPP_DUMP_DEFINE_EXPORT_OBJECT(GraphArc<ll>, to, cost)
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
  using A = GraphArc<Cost>;

protected:
  using GE = conditional_t<is_erasable, E, A>;

  int n = 0, m = 0, era = 0;
  CSR<GE, is_erasable> g;
  vc<int> eid_to_elist_id;

  struct OutEdgeIter
  {
    using iterator_category = input_iterator_tag;
    using value_type = E;
    using difference_type = ptrdiff_t;
    using pointer = const E *;
    using reference = const E &;

    int from;
    typename vc<GE>::const_iterator it;
    mutable E e;

    reference operator*() const
    {
      if constexpr (is_erasable)
        return *it;
      else
      {
        e = E(from, it->to, it->cost, it->index);
        return e;
      }
    }
    pointer operator->() const { return &**this; }
    OutEdgeIter &operator++()
    {
      ++it;
      return *this;
    }
    OutEdgeIter operator++(int)
    {
      auto res = *this;
      ++*this;
      return res;
    }
    bool operator==(const OutEdgeIter &rhs) const { return it == rhs.it; }
    bool operator!=(const OutEdgeIter &rhs) const { return !(*this == rhs); }
  };

  struct OutEdgeRow
  {
    const Graph *g;
    int from, l, r;

    OutEdgeRow(const Graph *g, int from)
        : g(g), from(from), l(g->g.offset(from)), r(l + g->g[from].size()) {}
    OutEdgeIter iter(int pos) const
    {
      return {from, g->g.get_elist().begin() + pos, {}};
    }
    OutEdgeIter begin() const { return iter(l); }
    OutEdgeIter end() const { return iter(r); }
    template <class I = ll>
    I size() const { return r - l; }
    bool empty() const { return l == r; }
    E operator[](int i) const
    {
      assert(0 <= i && i < size());
      return g->get_edge(from, l + i);
    }
    E at(int i) const
    {
      assert(0 <= i && i < size());
      return (*this)[i];
    }
    E front() const
    {
      assert(!empty());
      return (*this)[0];
    }
    E back() const
    {
      assert(!empty());
      return (*this)[size() - 1];
    }
    vc<E> to_v() const { return vc<E>(ALL(*this)); }
  };

  E get_edge(int from, int pos) const
  {
    if constexpr (is_erasable)
      return g.get_elist()[pos];
    else
    {
      cauto &e = g.get_elist()[pos];
      return E(from, e.to, e.cost, e.index);
    }
  }

  template <class F>
  void build(F input_edge)
  {
    vc<int> row_sizes(n);
    repi(i, m)
    {
      auto [u, v, w] = input_edge(i);
      assert(0 <= u && u < n && 0 <= v && v < n);
      row_sizes[u]++;
      if constexpr (!is_directed)
        if (u != v) row_sizes[v]++;
    }
    g = CSR<GE, is_erasable>(row_sizes);
    if constexpr (is_erasable)
      eid_to_elist_id.assign((is_directed ? 1 : 2) * m, -1);

    vc<int> cnt(n);
    repi(i, m)
    {
      auto [u, v, w] = input_edge(i);
      int j = cnt[u]++;
      int k = g.offset(u) + j;
      if constexpr (is_erasable)
        g[u][j] = E(u, v, w, i);
      else
        g[u][j] = A{int(v), i, w};
      if constexpr (is_erasable)
      {
        int id = is_directed ? i : 2 * i + (u <= v);
        eid_to_elist_id[id] = k;
      }
      if constexpr (!is_directed)
      {
        if (u != v)
        {
          j = cnt[v]++;
          k = g.offset(v) + j;
          if constexpr (is_erasable)
            g[v][j] = E(v, u, w, i);
          else
            g[v][j] = A{int(u), i, w};
          if constexpr (is_erasable)
            eid_to_elist_id[2 * i + (v <= u)] = k;
        }
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
  auto out_edges(int v) const
  {
    return OutEdgeRow(this, v);
  }
  // v から出る辺を from を持たない軽量な形で返す
  auto out_arcs(int v) const { return g[v]; }
  // v から出る頂点の集合
  template <class I = ll>
  vc<I> out_vertices(int v) const
  {
    vc<I> res;
    res.reserve(g[v].size());
    fec(e : out_arcs(v)) res.eb(e.to);
    return res;
  }

  // すべての辺を返す。辺番号順とは限らない
  // 無向グラフの場合、各辺は from <= to を満たす
  vc<E> edges() const
  {
    vc<E> res;
    res.reserve(num_of_edges<int>());
    if constexpr (is_directed)
    {
      repi(v, n) fec(e : out_edges(v))
      {
        res.eb(e);
      }
    }
    else
    {
      repi(v, n) fec(e : out_edges(v))
      {
        if (e.from <= e.to) res.eb(e);
      }
    }
    return res;
  }
  // 隣接リスト
  vvc<E> adj_list() const
  {
    vvc<E> res(n);
    repi(v, n) res[v] = out_edges(v).to_v();
    return res;
  }
  // 隣接行列 (辺の本数を格納)
  template <class I = ll>
  vvc<I> adj_matrix_ecnt() const
  {
    vvc<I> res(n, vc<I>(n, 0));
    repi(v, n) fec(e : out_arcs(v)) res[v][e.to]++;
    return res;
  }

  // 入次数の列
  template <class I = ll>
  vc<I> indegs() const
  {
    vc<I> res(n);
    if constexpr (is_directed)
    {
      repi(v, n) fec(e : out_arcs(v)) res[e.to]++;
    }
    else
      repi(v, n) res[v] = out_arcs(v).size();
    return res;
  }
  // 出次数の列
  template <class I = ll>
  vc<I> outdegs() const
  {
    vc<I> res(n);
    repi(v, n) res[v] = out_arcs(v).size();
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
  repi(v, n) fec(e : g.out_arcs(v)) uvw.eb(e.to, v, e.cost);
  return GraphDirected<Cost>(n, uvw);
}
