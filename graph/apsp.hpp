#pragma once

#include "sssp.hpp"

/**
 * @brief 全点対最短路
 * @docs docs/graph/apsp.md
 */

template <bool is_directed, class Cost, auto infty = nullptr>
struct AllPairsShortestPath
{
  using Dist = typename ShortestPath<is_directed, Cost, infty>::Dist;
  using Matrix = vvc<Dist>;

private:
  using SP = ShortestPath<is_directed, Cost, infty>;
  const Graph<is_directed, Cost> &g;
  static constexpr decltype(auto) inf() { return resolved_infty<Dist, infty>(); }
  Matrix dist;
  vvc<int> prev;
  vc<Edge<Cost>> by_id;
  bool negative = false, solved = false;

  enum class Method { repeated_sssp, floyd_warshall, johnson };

  Method select_method() const
  {
    if constexpr (is_void_v<Cost>) return Method::repeated_sssp;
    bool neg = false, multiple_positive = false, have_positive = false;
    Dist positive = 0;
    repi(v, g.size()) fec(e : g.out_edges(v))
    {
      neg |= e.cost < Dist(0);
      if (e.cost > Dist(0))
      {
        if (have_positive && e.cost != positive) multiple_positive = true;
        positive = e.cost, have_positive = true;
      }
    }
    if (!neg && !multiple_positive) return Method::repeated_sssp;
    const ll n = g.size(), m = g.num_of_edges();
    if (n * n < 16 * m) return Method::floyd_warshall;
    if (neg) return Method::johnson;
    return Method::repeated_sssp;
  }

  void init_distances()
  {
    const int n = g.size();
    dist.assign(n, vc<Dist>(n, inf()));
    prev.assign(n, vc<int>(n, -1));
  }
  void init(bool allocate_distances = true)
  {
    negative = false, solved = true;
    dist.clear(), prev.clear();
    if (allocate_distances) init_distances();
    by_id.resize(g.num_of_edges());
    fec(e : g.edges()) by_id[e.index] = e;
  }
  // 全頂点を始点とする Bellman–Ford。負閉路があれば nullopt。
  optional<vc<Dist>> potentials() const
  {
    const int n = g.size();
    vc<Dist> h(n, Dist(0));
    repi(iter, n)
    {
      auto next = h;
      bool changed = false;
      repi(v, n) fec(e : g.out_edges(v))
        if (chmin(next[e.to], h[v] + e.cost)) changed = true;
      if (!changed) break;
      if (iter == n - 1) return nullopt;
      h.swap(next);
    }
    return h;
  }
  Edge<Cost> ending_at(int id, int t) const
  {
    auto e = by_id[id];
    if constexpr (!is_directed) if (e.to != t) e = e.rev();
    assert(e.to == t);
    return e;
  }

public:
  explicit AllPairsShortestPath(const Graph<is_directed, Cost> &g) : g(g) {}
  AllPairsShortestPath(const Graph<is_directed, Cost> &&) = delete;

  // 距離行列
  // 到達不能は infty
  // 負閉路を経由して到達できる組は -infty
  const Matrix &solve()
  {
    const auto method = select_method();
    if (method == Method::floyd_warshall) return floyd_warshall();
    if (method == Method::johnson) return johnson();
    return repeated_sssp();
  }

  // f(s, 距離列, 親辺列) を全始点について呼ぶ
  // 負閉路があれば戻り値は false
  template <class F>
  bool for_each_source(const F &f) const
  {
    const auto method = select_method();
    if (method == Method::repeated_sssp)
    {
      SP sp(g);
      repi(s, g.size())
      {
        const auto ds = sp.solve(s);
        f(s, ds, sp.prev_edges());
      }
      return true;
    }
    AllPairsShortestPath ap(g);
    const auto &ds = method == Method::floyd_warshall ? ap.floyd_warshall() : ap.johnson();
    repi(s, g.size()) f(s, ds[s], ap.prev_edges(s));
    return !ap.negative_cycle();
  }

  const Matrix &repeated_sssp()
  {
    init();
    fec(e : by_id) assert(e.cost >= Dist(0));
    SP sp(g);
    repi(s, g.size())
    {
      dist[s] = sp.solve(s);
      const auto row = sp.prev_edges();
      repi(t, g.size()) prev[s][t] = row[t].index;
    }
    return dist;
  }

  const Matrix &floyd_warshall()
  {
    init();
    const int n = g.size();
    repi(v, n) dist[v][v] = 0;
    repi(v, n) fec(e : g.out_edges(v))
    {
      if (chmin(dist[v][e.to], e.cost)) prev[v][e.to] = e.index;
    }
    repi(k, n) repi(i, n)
    {
      if (dist[i][k] == inf()) continue;
      repi(j, n)
      {
        if (dist[k][j] == inf()) continue;
        // 負閉路のある入力でも、距離の増幅によるオーバーフローを避ける。
        const Dist &a = dist[i][k], &b = dist[k][j];
        Dist nd;
        if (a == -inf() || b == -inf()) nd = -inf();
        else if (b < Dist(0) && a < -inf() - b) nd = -inf();
        else if (b > Dist(0) && a > inf() - b) nd = inf();
        else nd = a + b;
        if (chmin(dist[i][j], nd)) prev[i][j] = prev[k][j];
      }
    }
    // s -> 負閉路 -> t と進める組だけを -infty にする。
    // infty かどうかは更新で変わらないため、到達性の行列は別に保存しない。
    repi(k, n) if (dist[k][k] < Dist(0))
    {
      negative = true;
      repi(s, n) if (dist[s][k] != inf())
        repi(t, n) if (dist[k][t] != inf())
          dist[s][t] = -inf(), prev[s][t] = -1;
    }
    return dist;
  }

  const Matrix &johnson()
  {
    if constexpr (is_void_v<Cost>)
      return repeated_sssp();
    else
    {
      init(false);
      const int n = g.size();
      const auto potential = potentials();
      if (!potential) return floyd_warshall();
      const auto &h = *potential;
      auto reweighted = by_id;
      for (auto &e : reweighted) e.cost = e.cost + h[e.from] - h[e.to];
      Graph<is_directed, Cost> reweighted_graph(n, reweighted);
      SP sp(reweighted_graph);
      init_distances();
      repi(s, n)
      {
        dist[s] = sp.dijkstra(s);
        const auto row = sp.prev_edges();
        repi(t, n)
        {
          if (dist[s][t] != inf()) dist[s][t] = dist[s][t] - h[s] + h[t];
          prev[s][t] = row[t].index;
        }
      }
      return dist;
    }
  }

  bool negative_cycle() const
  {
    if (solved) return negative;
    if constexpr (is_void_v<Cost>) return false;
    return !potentials().has_value();
  }

  vc<Edge<Cost>> prev_edges(int s) const
  {
    assert(solved && 0 <= s && s < g.size());
    vc<Edge<Cost>> res(g.size());
    repi(t, g.size()) if (prev[s][t] != -1) res[t] = ending_at(prev[s][t], t);
    return res;
  }

  vc<Edge<Cost>> path(int s, int t) const
  {
    const int n = g.size();
    assert(solved && 0 <= s && s < n && 0 <= t && t < n);
    vc<Edge<Cost>> res;
    if (dist[s][t] == inf() || dist[s][t] == -inf()) return res;
    while (t != s)
    {
      assert(prev[s][t] != -1 && int(res.size()) < n);
      auto e = ending_at(prev[s][t], t);
      res.eb(e);
      t = e.from;
    }
    reverse(res.begin(), res.end());
    return res;
  }
};
