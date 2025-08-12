#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../graph.hpp"
#include "ds/my_queue.hpp"

/**
 * @brief 根つき木クラス
 * @docs docs/graph/tree/rooted_tree.md
 */

// コンストラクタは
// - n, es, root
// - g, root
// - p
// のいずれか
template <class Cost, bool need_dist = true>
struct RootedTree
{
  using E = Edge<Cost>;

protected:
  int n, root_;

  vc<E> par;  // par[v] は v から親に向かう辺
  CSR<E> chi;

  // bfs_ordered_eid: 辺番号を BFS 順に並べた順列
  vc<int> bfs_ordered_eid;

  vc<int> dep;
  vc<Cost> dis;
  vc<int> siz;

  void calc_siz()
  {
    siz.assign(n, 1);
    repi(j, n - 2, -1, -1)
    {
      const E &e = get_edge(bfs_ordered_eid[j]);
      siz[e.from] += siz[e.to];
    }
  }

public:
  RootedTree() {}
  // p[i] は i の親の頂点番号 (根は -1) を渡す
  template <class I>
  RootedTree(const vc<I> &p, const Cost &dflt_cost = 1) : n(p.size())
  {
    // par, chi を計算する
    par.resize(n);
    root_ = -1;
    vc<pair<int, E>> edges(n - 1);
    for (int i = 0, j = 0; i < n; i++)
    {
      if (p[i] < 0)
      {
        assert(root_ == -1 && "There are more than two roots");
        par[i] = E(i, -1, dflt_cost, -1);
        root_ = i;
      }
      else
      {
        assert(j < n - 1 && "There is no root");
        par[i] = E(i, p[i], dflt_cost, j);
        edges[j] = {p[i], E(p[i], i, dflt_cost, j)};
        j++;
      }
    }
    chi = CSR<E>(n, edges);

    // bfs_ordered_eid, dep(, dist) を計算する
    bfs_ordered_eid.reserve(n - 1);
    MyQueue<int> que;
    que.push(root_);
    dep.assign(n, 0);
    if constexpr (need_dist)
      dis.assign(n, 0);
    while (!que.empty())
    {
      int v = que.front();
      que.pop();
      fec(e : chi[v])
      {
        bfs_ordered_eid.eb(e.index);
        dep[e.to] = dep[e.from] + 1;
        if constexpr (need_dist)
          dis[e.to] = dis[e.from] + e.cost;
        que.push(e.to);
      }
    }

    // siz を計算する
    calc_siz();
  }
  // g は木であることを想定、もとの g の index はそのまま保持される
  template <bool is_directed>
  RootedTree(const Graph<is_directed, Cost> &g, int root)
  : n(g.size()), root_(root)
  {
    // par, chi, bfs_ordered_eid, dep(, dist) を計算する
    par.resize(n);
    par[root] = E(root, -1, {}, -1);
    vc<pair<int, E>> edges(n - 1);
    MyQueue<int> que;
    que.push(root);
    dep.assign(n, 0);
    if constexpr (need_dist)
      dis.assign(n, 0);
    while (!que.empty())
    {
      int v = que.front();
      que.pop();
      fec(e : g.out_edges(v))
      {
        if (par[e.from] == e.to)
          continue;
        par[e.to] = e.rev();
        edges[e.index] = {e.from, e};
        bfs_ordered_eid.eb(e.index);
        dep[e.to] = dep[e.from] + 1;
        if constexpr (need_dist)
          dis[e.to] = dis[e.from] + e.cost;
        que.push(e.to);
      }
    }
    chi = CSR<E>(n, edges);

    // siz を計算する
    calc_siz();
  }

  template <class I>
  RootedTree(int n, const vc<pair<I, I>> &es, int root)
  : RootedTree(GraphUndirected<Cost>(n, es), root) {}
  template <class I>
  RootedTree(int n, const vc<tuple<I, I, Cost>> &es, int root)
  : RootedTree(GraphUndirected<Cost>(n, es), root) {}

  // 頂点数を返す
  template <class I = ll>
  I size() const { return n; }

  // 根を返す
  int root() const { return root_; }

  // v から親に向かう辺として返す (根なら to == -1)
  const E &parent(int v) const
  {
    assert(0 <= v && v < n);
    return par[v];
  }
  // v から子に向かう辺の集合
  auto children(int v) const
  {
    assert(0 <= v && v < n);
    return chi[v];
  }
  // v の深さ
  template <class I = ll>
  I depth(int v) const
  {
    assert(0 <= v && v < n);
    return dep[v];
  }
  // 根から v への距離 (重みつき)
  // need_dist == true が必要
  Cost dist(int v) const
  {
    static_assert(need_dist);
    assert(0 <= v && v < n);
    return dis[v];
  }
  // v を根とする部分木のサイズ (v も含む)
  template <class I = ll>
  I subtree_size(int v) const
  {
    assert(0 <= v && v < n);
    return siz[v];
  }
  // 辺 (u, v) が存在するとして、これを切ったときの
  // u 側、v 側それぞれの連結成分サイズ
  template <class I = ll>
  pair<I, I> cut_and_subtree_size(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    if (dep[u] < dep[v])
      return {n - siz[v], siz[v]};
    else
      return {siz[u], n - siz[u]};
  }

  // 辺番号から辺を取得する
  // 辺は親から子
  const E &get_edge(int eid) const { return chi.find_by_eid(eid); }

  // すべての辺を返す
  // 辺は親から子
  vc<E> edges() const
  {
    vc<E> res(n - 1);
    repi(i, n - 1) res[i] = get_edge(i);
    return res;
  }

  // v から根までのパスを (辺の vector として) 返す
  vc<E> path_to_root(int v) const
  {
    assert(0 <= v && v < n);
    vc<E> res;
    res.reserve(dep[v]);
    while (v != root_)
    {
      res.eb(par[v]);
      v = par[v].to;
    }
    return res;
  }

  // u から v までのパスを (辺の vector として) 返す
  vc<E> path(int u, int v) const
  {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    vc<E> pu, pv;
    while (u != v)
    {
      if (dep[u] > dep[v])
      {
        const E &e = parent(u);
        pu.eb(e);
        u = e.to;
      }
      else
      {
        const E &e = parent(v);
        pv.eb(e);
        v = e.to;
      }
    }
    return concat(pu, rev_path(pv));
  }

  // BFS 順に頂点を並べた、長さ n の列を返す
  template <class I = ll>
  vc<I> bfs_ordered_vertices() const
  {
    vc<I> res(n);
    res[0] = root_;
    repi(i, n - 1) res[i + 1] = get_edge(bfs_ordered_eid[i]).to;
    return res;
  }

  // BFS 順に辺を並べた、長さ n-1 の列を返す
  // 辺は親から子
  vc<E> bfs_ordered_edges() const
  {
    vc<E> res(n - 1);
    repi(i, n - 1) res[i] = get_edge(bfs_ordered_eid[i]);
    return res;
  }

  // 帰るのも含めた DFS 順に辺を並べた、長さ 2(n-1) の列を返す
  vc<E> dfs_ordered_edges() const
  {
    vc<E> res(2 * (n - 1));
    vc<int> dp(n, 0);
    fec(eid : bfs_ordered_eid)
    {
      const E &e = get_edge(eid);
      dp[e.to] = dp[e.from] + 1;
      res[dp[e.to] - 1] = e;
      dp[e.from] += 2 * siz[e.to];
      res[dp[e.from] - 1] = e.rev();
    }
    return res;
  }

  // グラフや根つき木を作るための辺の vc<tuple>
  template <class I = ll>
  vc<tuple<I, I, Cost>> edges_vt() const
  {
    vc<tuple<I, I, Cost>> es(n - 1);
    repi(i, n - 1)
    {
      const E &e = get_edge(i);
      es[i] = {e.from, e.to, e.cost};
    }
    return es;
  }

  // 新たに根を付け替えた根つき木を返す
  RootedTree<Cost, need_dist> rerooted_tree(int new_root) const
  {
    return RootedTree<Cost, need_dist>(n, edges_vt(), new_root);
  }

  // Graph として返す
  template <bool is_directed>
  Graph<is_directed, Cost> to_graph() const
  {
    return Graph<is_directed, Cost>(n, edges_vt());
  }
};
