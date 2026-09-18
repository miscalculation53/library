#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../ds/csr.hpp"

/**
 * @brief グラフクラス
 * @docs docs/graph/graph.md
 */

template <class Cost = void>
struct Edge
{
  int from, to;
  Cost cost;
  int index;
  CPP_DUMP_DEFINE_DATA(from, to, cost, index);
  Edge() : from(-1), to(-1), index(-1) {}
  Edge(int s, int t, Cost c, int i = -1) : from(s), to(t), cost(c), index(i) {}
  operator int() const { return to; }
  bool operator<(const Edge &rhs) const { return cost < rhs.cost; }
  // 逆辺を返す (もとの辺は変更しない)
  Edge rev() const { return Edge(to, from, cost, index); }
};

template <>
struct Edge<void>
{
  int from, to, index;
  static constexpr ll cost = 1;
  CPP_DUMP_DEFINE_DATA(from, to, cost, index);
  Edge() : from(-1), to(-1), index(-1) {}
  Edge(int s, int t, int i = -1) : from(s), to(t), index(i) {}
  operator int() const { return to; }
  bool operator<(const Edge &) const { return false; }
  Edge rev() const { return Edge(to, from, index); }
};
#ifdef LOCAL
namespace cpp_dump::_detail
{
  // 辺の int 変換より、全メンバの表示を優先する。
  template <class Cost>
  inline constexpr bool is_ostream<Edge<Cost>> = false;
}
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
template <bool is_directed, class Cost = void, bool is_erasable = false>
struct Graph
{
  using E = Edge<Cost>;

protected:
  template <class C, bool unweighted = is_void_v<C>>
  struct InternalEdge
  {
    int to, index;
    C cost;
  };
  template <class C>
  struct InternalEdge<C, true>
  {
    int to, index;
    static constexpr ll cost = 1;
  };
  using GE = conditional_t<is_erasable, E, InternalEdge<Cost>>;
  using Weight = decay_t<decltype(E::cost)>;

  int n = 0, m = 0, era = 0;
  CSR<GE, is_erasable> g;
  vc<int> eid_to_elist_id;

  static E make_edge(int from, int to, const Weight &cost, int index)
  {
    if constexpr (is_void_v<Cost>)
      return E(from, to, index);
    else
      return E(from, to, cost, index);
  }

  struct OutEdgeRow;
  struct OutEdgeIter
  {
    using iterator_category = input_iterator_tag;
    using value_type = E;
    using difference_type = ptrdiff_t;
    using pointer = const E *;
    using reference = const E &;

  private:
    friend struct OutEdgeRow;
    int from = -1;
    typename vc<GE>::const_iterator it;
    mutable E e;
    OutEdgeIter(int from, typename vc<GE>::const_iterator it) : from(from), it(it) {}

  public:
    OutEdgeIter() = default;
    reference operator*() const
    {
      if constexpr (is_erasable)
        return *it;
      else
      {
        e = make_edge(from, it->to, it->cost, it->index);
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
  private:
    const Graph *g;
    int from, l, r;

    OutEdgeIter iter(int pos) const
    {
      return {from, g->g.get_elist().begin() + pos};
    }

  public:
    OutEdgeRow(const Graph *g, int from)
        : g(g), from(from), l(g->g.offset(from)), r(l + g->g[from].size()) {}
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
      return make_edge(from, e.to, e.cost, e.index);
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
        g[u][j] = make_edge(u, v, w, i);
      else if constexpr (is_void_v<Cost>)
        g[u][j] = GE{int(v), i};
      else
        g[u][j] = GE{int(v), i, w};
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
            g[v][j] = make_edge(v, u, w, i);
          else if constexpr (is_void_v<Cost>)
            g[v][j] = GE{int(u), i};
          else
            g[v][j] = GE{int(u), i, w};
          if constexpr (is_erasable)
            eid_to_elist_id[2 * i + (v <= u)] = k;
        }
      }
    }
  }

public:
  Graph() {}
  template <class I>
  Graph(int n, const vc<pair<I, I>> &es) : n(n), m(es.size()), era(0)
  {
    build(LMD(i, (tuple{es[i].first, es[i].second, Weight(1)})));
  }
  template <class I, class C = Cost, enable_if_t<!is_void_v<C>, int> = 0>
  Graph(int n, const vc<pair<I, I>> &es, const Weight &dflt_cost) : n(n), m(es.size()), era(0)
  {
    build(LMD(i, (tuple{es[i].first, es[i].second, dflt_cost})));
  }
  template <class I, class C, enable_if_t<is_same_v<C, Cost> && !is_void_v<C>, int> = 0>
  Graph(int n, const vc<tuple<I, I, C>> &es) : n(n), m(es.size()), era(0)
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
    repi(v, n) fec(e : g[v]) res[v][e.to]++;
    return res;
  }

  // 入次数の列
  template <class I = ll>
  vc<I> indegs() const
  {
    vc<I> res(n);
    if constexpr (is_directed)
    {
      repi(v, n) fec(e : g[v]) res[e.to]++;
    }
    else
      repi(v, n) res[v] = g[v].size();
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

// C++20 の別名テンプレートの型推論で、有向・無向と削除可否を引き継ぐ。
template <bool is_directed, bool is_erasable = false, class I>
Graph(int, const vc<pair<I, I>> &) -> Graph<is_directed, void, is_erasable>;
template <bool is_directed, bool is_erasable = false, class I, class Cost>
Graph(int, const vc<tuple<I, I, Cost>> &) -> Graph<is_directed, Cost, is_erasable>;
template <bool is_directed, bool is_erasable = false, class I, class Cost>
Graph(int, const vc<pair<I, I>> &, const Cost &) -> Graph<is_directed, Cost, is_erasable>;
template <bool is_directed, bool is_erasable = false, class Cost>
Graph(int, const vc<Edge<Cost>> &) -> Graph<is_directed, Cost, is_erasable>;

template <class Cost = void, bool is_erasable = false>
using GraphDirected = Graph<true, Cost, is_erasable>;
template <class Cost = void, bool is_erasable = false>
using GraphUndirected = Graph<false, Cost, is_erasable>;

template <class Cost>
GraphDirected<Cost> rev_graph(const GraphDirected<Cost> &g)
{
  const int n = g.size(), m = g.num_of_edges();
  using InputEdge = conditional_t<is_void_v<Cost>, pair<int, int>, tuple<int, int, Cost>>;
  vc<InputEdge> es;
  es.reserve(m);
  repi(v, n) fec(e : g.out_edges(v))
  {
    if constexpr (is_void_v<Cost>) es.eb(e.to, v);
    else es.eb(e.to, v, e.cost);
  }
  return GraphDirected<Cost>(n, es);
}
