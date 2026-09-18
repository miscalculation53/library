#pragma once

#include "apsp.hpp"
#include "../ds/topk_array.hpp"

/**
 * @brief 最小閉路
 * @docs docs/graph/minimum_cycle.md
 */

template <bool is_directed, class Cost, auto infty = nullptr>
struct MinimumCycle
{
  using Dist = typename ShortestPath<is_directed, Cost, infty>::Dist;
  using Result = pair<Dist, vc<Edge<Cost>>>;

private:
  using SP = ShortestPath<is_directed, Cost, infty>;
  using AP = AllPairsShortestPath<is_directed, Cost, infty>;
  const Graph<is_directed, Cost> &g;

  // s に接続する各辺について、最初に別の辺を通って戻る最短経路を求める。
  void update_edge_costs(int s, vc<Dist> &res) const
  {
    using Candidate = pair<Dist, int>;
    struct SameFirstEdge
    {
      bool operator()(const Candidate &a, const Candidate &b) const
      {
        return a.second == b.second;
      }
    };
    using Best = TopKArray<Candidate, 2, less<Candidate>, SameFirstEdge>;
    pql<tuple<Dist, int, int>> que;
    fec(e : g.out_edges(s))
    {
      if (e.to == s) res[e.index] = e.cost;
      else que.emplace(e.cost, e.index, e.to);
    }
    if (que.empty()) return;
    vc<Best> best(g.size());
    while (!que.empty())
    {
      auto [d, first_edge, v] = que.top();
      que.pop();
      // 距離・辺番号順に確定し、異なる最初の辺を持つ経路を 2 種類まで残す。
      if (!best[v].add({d, first_edge})) continue;
      fec(e : g.out_edges(v))
      {
        if (e.to == s) continue;
        Dist nd = add(d, e.cost);
        if (nd < inf()) que.emplace(nd, first_edge, e.to);
      }
    }
    fec(e : g.out_edges(s)) if (e.to != s)
    {
      repi(i, best[e.to].size())
      {
        auto [d, first_edge] = best[e.to].get(i);
        if (first_edge == e.index) continue;
        res[e.index] = add(d, e.cost);
        break;
      }
    }
  }

  static constexpr decltype(auto) inf() { return resolved_infty<Dist, infty>(); }

  static Dist add(const Dist &a, const Dist &b)
  {
    if (b > Dist(0) && a >= inf() - b) return inf();
    if (b < Dist(0) && a <= -inf() - b) return -inf();
    return a + b;
  }

  // 最短路木の根 s を通る候補のうち、最小のものを返す。
  pair<Dist, Edge<Cost>> from_source(int s, const vc<Dist> &ds,
                                    const vc<Edge<Cost>> &prv) const
  {
    fec(d : ds) if (d == -inf()) return {-inf(), {}};
    vc<int> branch;
    if constexpr (!is_directed)
    {
      const int n = g.size();
      branch.assign(n, -1);
      vvc<int> children(n);
      repi(v, n) if (prv[v].index != -1) children[prv[v].from].eb(v);
      vc<int> order{s};
      for (int i = 0; i < int(order.size()); i++)
      {
        int v = order[i];
        fec(to : children[v])
        {
          branch[to] = v == s ? prv[to].index : branch[v];
          order.eb(to);
        }
      }
    }
    Dist ans = inf();
    Edge<Cost> closing;
    fec(e : g.edges())
    {
      if constexpr (is_directed)
      {
        if (e.to != s || ds[e.from] == inf()) continue;
        if (chmin(ans, add(ds[e.from], e.cost))) closing = e;
      }
      else
      {
        if (ds[e.from] == inf() || ds[e.to] == inf()) continue;
        if (prv[e.from].index == e.index || prv[e.to].index == e.index) continue;
        // s の自己ループ、または根から異なる最初の辺を使う 2 経路を結ぶ辺。
        if (branch[e.from] == branch[e.to] && !(e.from == s && e.to == s)) continue;
        if (chmin(ans, add(add(ds[e.from], ds[e.to]), e.cost))) closing = e;
      }
    }
    return {ans, closing};
  }

  static vc<Edge<Cost>> path(int s, int t, const vc<Edge<Cost>> &prv)
  {
    vc<Edge<Cost>> res;
    while (t != s)
    {
      const auto &e = prv[t];
      assert(e.index != -1);
      res.eb(e);
      t = e.from;
    }
    reverse(res.begin(), res.end());
    return res;
  }

  static Result restore(int s, Dist cost, const Edge<Cost> &closing,
                        const vc<Edge<Cost>> &prv)
  {
    if (cost == inf() || cost == -inf()) return {cost, {}};
    auto cycle = path(s, closing.from, prv);
    cycle.eb(closing);
    if constexpr (!is_directed)
    {
      auto right = rev_path(path(s, closing.to, prv));
      cycle.insert(cycle.end(), right.begin(), right.end());
    }
    return {cost, cycle};
  }

public:
  explicit MinimumCycle(const Graph<is_directed, Cost> &g) : g(g) {}
  MinimumCycle(const Graph<is_directed, Cost> &&) = delete;

  // グラフ全体の最小閉路 (重み, 辺列)
  // 閉路がなければ (infty, 空)
  // 負閉路があるならば (-infty, 空)
  Result solve() const
  {
    AP ap(g);
    if (ap.negative_cycle()) return {-inf(), {}};
    Dist ans = inf();
    int source = -1;
    Edge<Cost> closing;
    vc<Edge<Cost>> best_prev;
    bool ok = ap.for_each_source([&](int s, const auto &ds, const auto &prv)
    {
      auto [cost, edge] = from_source(s, ds, prv);
      if (chmin(ans, cost)) source = s, closing = edge, best_prev = prv;
    });
    if (!ok) return {-inf(), {}};
    return restore(source, ans, closing, best_prev);
  }

  // 頂点 s を通る最小閉路
  // (重み, 辺列)
  // 辺列は s から始まる
  Result solve_vertex(int s) const
  {
    assert(0 <= s && s < g.size());
    if constexpr (!is_directed) fec(e : g.edges()) assert(e.cost >= Dist(0));
    SP sp(g);
    const auto ds = sp.solve(s);
    const auto prv = sp.prev_edges();
    auto [cost, closing] = from_source(s, ds, prv);
    return restore(s, cost, closing, prv);
  }

  // 辺 eid を通る最小閉路
  // (重み, 辺列)
  // 辺列の先頭が指定辺となる
  Result solve_edge(int eid) const
  {
    assert(0 <= eid && eid < g.num_of_edges());
    Edge<Cost> closing;
    vc<Edge<Cost>> remaining;
    fec(e : g.edges())
    {
      if constexpr (!is_directed) assert(e.cost >= Dist(0));
      if (e.index == eid) closing = e;
      else remaining.eb(e);
    }
    if (closing.from == closing.to)
    {
      if (closing.cost < Dist(0)) return {-inf(), {}};
      return {closing.cost, {closing}};
    }
    Graph<is_directed, Cost> without(g.size(), remaining);
    SP sp(without);
    const auto ds = sp.solve(closing.to, closing.from);
    fec(d : ds) if (d == -inf()) return {-inf(), {}};
    if (ds[closing.from] == inf()) return {inf(), {}};
    const Dist cost = add(ds[closing.from], closing.cost);
    if (cost < Dist(0)) return {-inf(), {}};
    if (cost == inf()) return {inf(), {}};
    auto back = sp.path(closing.from);
    vc<Edge<Cost>> cycle{closing};
    for (auto &e : back)
    {
      e.index = remaining[e.index].index;
      cycle.eb(e);
    }
    return {cost, cycle};
  }

  // 各辺に対し、その辺を含む最小閉路の重み
  // 負閉路がある場合は全要素 -infty
  vc<Dist> edge_costs() const
  {
    const int n = g.size();
    vc<Dist> res(g.num_of_edges(), inf());
    if constexpr (is_directed)
    {
      AP ap(g);
      if (ap.negative_cycle()) return vc<Dist>(g.num_of_edges(), -inf());
      const auto &ds = ap.solve();
      if (ap.negative_cycle()) return vc<Dist>(g.num_of_edges(), -inf());
      fec(e : g.edges()) if (ds[e.to][e.from] != inf())
        res[e.index] = add(e.cost, ds[e.to][e.from]);
    }
    else
    {
      fec(e : g.edges()) assert(e.cost >= Dist(0));
      repi(s, n) update_edge_costs(s, res);
    }
    return res;
  }

  // 各頂点に対し、その頂点を含む最小閉路の重み
  // 負閉路がある場合は全要素 -infty
  vc<Dist> vertex_costs() const
  {
    vc<Dist> res(g.size(), inf());
    AP ap(g);
    if (ap.negative_cycle()) return vc<Dist>(g.size(), -inf());
    bool ok = ap.for_each_source([&](int s, const auto &ds, const auto &prv)
    {
      res[s] = from_source(s, ds, prv).first;
    });
    if (!ok) return vc<Dist>(g.size(), -inf());
    return res;
  }
};
