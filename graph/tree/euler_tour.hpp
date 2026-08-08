#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 木のオイラーツアー
 * @docs docs/graph/tree/euler_tour.md
 */

#include "ds/static_range/linear_rmq.hpp"

template <bool need_lca = true, class RMQ = BlockSparseTable<MonoidMin<ull>, 64>>
struct EulerTour
{
protected:
  int n, rt;
  vc<int> in, out, tour, dep;
  RMQ rmq;

  inline int first(ull x) const { return x >> 32; }
  inline int second(ull x) const { return uint(x); }

  template <class I>
  void build(const vc<int> &siz, const vc<I> &par)
  {
    vc<int> ord(n);
    {
      vc<int> cnt(n + 1);
      repi(i, n) cnt[siz[i]]++;
      cnt = cumlsum(cnt);
      repi(i, n) ord[n - 1 - (cnt[siz[i]]++)] = i;
    }
    {
      vc<int> add(n, 0);
      dep.assign(n, 0);
      in[rt] = 0, out[rt] = 2 * n - 2, tour[0] = rt, dep[rt] = 0;
      repi(i, 1, n)
      {
        int v = ord[i], p = par[v];
        dep[v] = dep[p] + 1;
        in[v] = in[p] + 1 + 2 * add[p];
        add[p] += siz[v];
        out[v] = in[v] + 2 * siz[v] - 2;
        tour[in[v]] = v;
        tour[out[v] + 1] = p;
      }
      if constexpr (need_lca)
      {
        vc<ull> rmq_init(2 * n - 1);
        repi(i, 2 * n - 1) rmq_init[i] = (ull(dep[tour[i]]) << 32) | tour[i];
        rmq = RMQ(rmq_init);
        dep.clear();
      }
    }
  }

public:
  EulerTour() {}
  template <class I>
  EulerTour(int n, const vc<I> &par)
  : n(n), rt(-1), in(n), out(n), tour(2 * n - 1)
  {
    assert(n >= 1);
    assert(SZ(par) == n);
    vc<int> deg(n), siz(n, 1);
    repi(i, n)
    {
      if (par[i] < 0 || par[i] == i)
      {
        assert(rt == -1 && "There are more than two roots");
        rt = i;
      }
      else
      {
        deg[par[i]]++;
      }
    }
    assert(rt != -1 && "There is no root");
    repi(i, n)
    {
      int v = i;
      while (v != rt && deg[v] == 0)
      {
        int p = par[v];
        deg[p]--, deg[v]--;
        siz[p] += siz[v];
        v = p;
      }
    }
    build(siz, par);
  }
  template <class P>
  EulerTour(int n, const vc<P> &es, int rt)
  : n(n), rt(rt), in(n), out(n), tour(2 * n - 1)
  {
    assert(n >= 1);
    assert(SZ(es) == n - 1);
    assert(0 <= rt && rt < n);

    vc<int> deg(n), par(n), siz(n, 1);
    fec([ u, v ] : es)
    {
      deg[u]++, deg[v]++;
      par[u] ^= v, par[v] ^= u;
    }
    deg[rt] = 0;
    repi(i, n)
    {
      int v = i;
      while (deg[v] == 1)
      {
        int p = par[v];
        deg[p]--, deg[v]--, par[p] ^= v;
        siz[p] += siz[v];
        v = p;
      }
    }
    build(siz, par);
  }

  // 頂点数を返す
  template <class I = ll>
  I size() const { return n; }
  // 根を返す
  int root() const { return rt; }

  // v の深さ
  template <class I = ll>
  I depth(int v) const
  {
    assert(0 <= v && v < n);
    if constexpr (need_lca)
      return first(rmq.get(in[v]));
    else
      return dep[v];
  }

  // v を根とする部分木のサイズ (v も含む)
  template <class I = ll>
  I subtree_size(int v) const
  {
    assert(0 <= v && v < n);
    return (out[v] - in[v] + 1) / 2;
  }
  // 辺 (u, v) が存在するとして、これを切ったときの
  // u 側、v 側それぞれの連結成分サイズ
  template <class I = ll>
  pair<I, I> cut_and_subtree_size(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    if (depth(u) < depth(v))
      return {n - subtree_size(v), subtree_size(v)};
    else
      return {subtree_size(u), n - subtree_size(u)};
  }

  // v の親を返す。v は根であってはいけない
  int parent(int v) const
  {
    assert(0 <= v && v < n);
    assert(v != root());
    return tour[in[v] - 1];
  }

  // v から根までのパスで通る頂点の列
  vc<int> path_to_root(int v) const
  {
    assert(0 <= v && v < n);
    vc<int> res = {v};
    res.reserve(depth(v) + 1);
    while (v != rt)
    {
      int p = parent(v);
      res.eb(p);
      v = p;
    }
    return res;
  }
  // u から v までのパスで通る頂点の列
  vc<int> path(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    vc<int> pu = {u}, pv = {v};
    while (u != v)
    {
      if (depth(u) > depth(v))
      {
        int p = parent(u);
        pu.eb(p);
        u = p;
      }
      else
      {
        int p = parent(v);
        pv.eb(p);
        v = p;
      }
    }
    pv.pop_back();
    return concat(pu, reversed(pv));
  }

  class ChildIterator;
  class ChildRange;

  class ChildIterator
  {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type        = int;
    using difference_type   = std::ptrdiff_t;
    using pointer           = int*;
    using reference         = int;

  private:
    const EulerTour* t;
    int cur;

    friend class EulerTour;
    friend class ChildRange;
    ChildIterator(const EulerTour* t, int cur) : t(t), cur(cur) {}

  public:
    ChildIterator() : t(nullptr), cur(0) {}
    ChildIterator(const ChildIterator&) = default;
    ChildIterator& operator=(const ChildIterator&) = default;

    int operator*() const { return t->tour[cur]; }

    ChildIterator& operator++()
    {
      int c = t->tour[cur];
      cur = t->out[c] + 2;
      return *this;
    }

    ChildIterator operator++(int)
    {
      ChildIterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator!=(const ChildIterator& r) const { return cur != r.cur; }
    bool operator==(const ChildIterator& r) const { return cur == r.cur; }
  };

  class ChildRange
  {
  private:
    const EulerTour* t;
    int start_pos, end_pos;

    friend class EulerTour;
    ChildRange(const EulerTour* t, int start_pos, int end_pos)
      : t(t), start_pos(start_pos), end_pos(end_pos) {}

  public:
    ChildIterator begin() const { return ChildIterator(t, start_pos); }
    ChildIterator end() const { return ChildIterator(t, end_pos); }
    bool empty() const { return start_pos == end_pos; }
    vc<int> to_v() const { return vc<int>(begin(), end()); }
  };

  ChildRange children(int v) const
  {
    assert(0 <= v && v < n);
    return ChildRange(this, in[v] + 1, out[v] + 1);
  }

  // 辺 (u, v) が存在するとして、(親側, 子側)
  pair<int, int> parent_child(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    return depth(u) < depth(v) ? pair{u, v} : pair{v, u};
  }
  // 長さ n の配列。v 番目には v が子側になる辺の情報が格納される。根は未定義。
  template <class P, class EdgeInfo>
  vc<EdgeInfo> reordered_edge_info(const vc<P> &es, const vc<EdgeInfo> &edge_info) const
  {
    assert(SZ(es) == n - 1 && SZ(edge_info) >= n - 1);
    vc<EdgeInfo> res(n);
    repi(i, n - 1)
    {
      auto [u, v] = es[i];
      int eid = depth(u) < depth(v) ? v : u;
      res[eid] = edge_info[i];
    }
    return res;
  }

  int lca(int u, int v) const
  {
    static_assert(need_lca);
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    if (u == v)
      return u;
    int l = in[u], r = in[v];
    if (l > r)
      swap(l, r);
    return second(rmq.prod(l, r));
  }

  template <class I = ll>
  I dist(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    return depth(u) + depth(v) - 2 * depth(lca(u, v));
  }

  // u が v の祖先か
  bool is_ancestor(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    return in[u] <= in[v] && out[v] <= out[u];
  }
};
