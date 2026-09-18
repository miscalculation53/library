#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/apsp.hpp"

// 始点ごとの独立した Bellman–Ford を oracle にする。
template <bool directed>
vvl oracle(int n, const vc<tuple<int, int, ll>> &es)
{
  vvl ds(n, vl(n, INF));
  repi(s, n)
  {
    ds[s][s] = 0;
    repi(iter, n)
    {
      auto next = ds[s];
      for (auto [u, v, w] : es)
      {
        if (ds[s][u] != INF) chmin(next[v], ds[s][u] + w);
        if constexpr (!directed) if (ds[s][v] != INF) chmin(next[u], ds[s][v] + w);
      }
      if (next == ds[s]) break;
      if (iter == n - 1)
      {
        vb affected(n);
        repi(v, n) affected[v] = next[v] < ds[s][v];
        repi(_, n) for (auto [u, v, w] : es)
        {
          if (affected[u]) affected[v] = true;
          if constexpr (!directed) if (affected[v]) affected[u] = true;
        }
        repi(v, n) if (affected[v]) ds[s][v] = -INF;
        break;
      }
      ds[s].swap(next);
    }
  }
  return ds;
}

template <bool directed, class Cost = ll>
void check(int n, const vc<tuple<int, int, ll>> &es)
{
  auto expected = oracle<directed>(n, es);
  bool negative = false;
  repi(s, n) negative |= expected[s][s] == -INF;
  const auto g_input = [&]
  {
    if constexpr (is_void_v<Cost>)
    {
      vc<pair<int, int>> uv;
      for (auto [u, v, w] : es) { assert(w == 1); uv.eb(u, v); }
      return Graph<directed>(n, uv);
    }
    else return Graph<directed, Cost>(n, es);
  }();
  AllPairsShortestPath g(g_input);
  static_assert(is_same_v<typename decltype(g)::Matrix, vvc<ll>>);
  // 計算前の判定は距離行列を作らず、const オブジェクトからも呼べる。
  assert(as_const(g).negative_cycle() == negative);
  auto verify = [&](const auto &ds)
  {
    assert(g.negative_cycle() == negative);
    assert(ds == expected);
    repi(s, n)
    {
      const auto prev = g.prev_edges(s);
      repi(t, n)
      {
        auto path = g.path(s, t);
        static_assert(is_same_v<decltype(path), vc<Edge<Cost>>>);
        if (ds[s][t] == INF || ds[s][t] == -INF)
        {
          assert(path.empty() && prev[t].index == -1);
          continue;
        }
        int cur = s;
        ll sum = 0;
        vb used(n);
        used[s] = true;
        fec(e : path)
        {
          assert(e.from == cur && !used[e.to]);
          assert(0 <= e.index && e.index < int(es.size()));
          auto [u, v, w] = es[e.index];
          bool valid = u == e.from && v == e.to;
          if constexpr (!directed) valid |= v == e.from && u == e.to;
          assert(valid && w == e.cost && prev[e.to].index == e.index);
          used[e.to] = true;
          cur = e.to, sum += e.cost;
        }
        assert(cur == t && sum == ds[s][t]);
      }
    }
  };
  verify(g.solve());
  verify(g.floyd_warshall());
  const auto &saved = g.johnson();
  verify(saved);
  int rows = 0;
  auto consume = [&](int s, const auto &ds, const auto &prev)
  {
    assert(s == rows++ && ds == expected[s]);
    repi(t, n)
    {
      if (ds[t] == INF || ds[t] == -INF || s == t)
      {
        assert(prev[t].index == -1);
        continue;
      }
      ll sum = 0;
      int v = t, count = 0;
      while (v != s)
      {
        const auto &e = prev[v];
        assert(e.to == v && e.index >= 0 && ++count < n);
        auto [from, to, cost] = es[e.index];
        bool valid = e.from == from && e.to == to;
        if constexpr (!directed) valid |= e.from == to && e.to == from;
        assert(valid && e.cost == cost);
        sum += e.cost, v = e.from;
      }
      assert(sum == ds[t]);
    }
  };
  bool ok = g.for_each_source(consume);
  assert(ok == !negative && rows == n);
  verify(saved);
  bool nonnegative = true;
  for (auto [u, v, w] : es) nonnegative &= w >= 0;
  if (nonnegative) verify(g.repeated_sssp());
}

template <bool directed>
void tests()
{
  check<directed>(0, {});
  check<directed>(5, {});
  check<directed>(1, {{0, 0, -1}});
  check<directed>(2, {{0, 1, -3}});
  check<directed>(5, {{0, 1, 1}, {2, 3, -3}, {3, 2, 2}});
  // 負閉路の手前・先・別の枝・非連結成分で、有限値と両方の無限大が混在する。
  check<directed>(8, {{0, 1, 2}, {1, 2, -3}, {2, 1, 1}, {2, 3, 4},
                      {0, 4, 7}, {4, 5, 1}, {6, 0, 0}});
  // 疎グラフの事前判定と、Johnson から Floyd に切り替えた距離行列。
  check<directed>(80, {{0, 1, 2}, {1, 2, -3}, {2, 1, 1}, {2, 3, 4}, {0, 4, 7}});
  check<directed>(4, {{0, 1, 0}, {1, 2, 0}, {2, 0, 0}, {2, 3, 0}});
  check<directed>(4, {{0, 1, 9}, {0, 1, 2}, {1, 2, -2}, {2, 0, 1}, {3, 2, 7}});
  check<directed>(3, {{0, 1, 1000000000LL}, {1, 2, 1000000000LL}});
  mt19937 rng(20260908 + directed);
  repi(tc, 1200)
  {
    int n = 1 + rng() % 9, m = rng() % 45;
    vl h(n);
    for (auto &x : h) x = int(rng() % 21) - 10;
    vc<tuple<int, int, ll>> es;
    repi(i, m)
    {
      int u = rng() % n, v = rng() % n;
      ll w = tc % 4 == 0 ? 0 : ll(rng() % 11);
      if constexpr (directed) if (tc % 4 == 1) w += h[v] - h[u];
      if (tc % 4 == 2) w -= 4;
      es.eb(u, v, w);
    }
    check<directed>(n, es);
  }
}

template <bool directed>
void unweighted_tests()
{
  check<directed, void>(0, {});
  check<directed, void>(5, {});
  check<directed, void>(1, {{0, 0, 1}, {0, 0, 1}});
  check<directed, void>(2, {{0, 1, 1}});
  check<directed, void>(2, {{0, 1, 1}, {0, 1, 1}, {1, 0, 1}});
  check<directed, void>(7, {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}, {2, 3, 1}, {4, 5, 1}});
  mt19937 rng(20260913 + directed);
  repi(tc, 300)
  {
    int n = 1 + rng() % 7, m = rng() % 15;
    vc<tuple<int, int, ll>> es;
    repi(i, m) es.eb(rng() % n, rng() % n, 1);
    check<directed, void>(n, es);
  }
}

constexpr int int_inf() { return 1000000000; }

int main()
{
  tests<false>();
  tests<true>();
  unweighted_tests<false>();
  unweighted_tests<true>();
  Graph<true, int> g_input(3, vc<tuple<int, int, int>>{{0, 1, -5}, {1, 2, 2}});
  AllPairsShortestPath<true, int, int_inf> g(g_input);
  assert(!g.negative_cycle());
  assert(g.solve()[0][2] == -3 && !g.negative_cycle());
  // 事前判定は結果を保存しないため、solve 前のグラフ変更も反映する。
  GraphDirected<ll> pending_input(2, vc<tuple<int, int, ll>>{{0, 1, -1}});
  AllPairsShortestPath pending(pending_input);
  assert(!pending.negative_cycle());
  pending_input = GraphDirected<ll>(2, vc<tuple<int, int, ll>>{{0, 1, -1}, {1, 0, 0}});
  assert(pending.negative_cycle());
  pending_input = GraphDirected<ll>(2, vc<tuple<int, int, ll>>{{0, 1, -1}, {1, 0, 2}});
  assert(!pending.negative_cycle());
  assert((pending.johnson() == vvl{{0, -1}, {2, 0}}));
  assert(!pending.negative_cycle() && pending.path(0, 1).front().cost == -1);
  Graph<false, ll> h_input(3, vc<pair<int, int>>{{0, 1}, {1, 2}});
  AllPairsShortestPath h(h_input);
  assert(h.solve()[0][2] == 2);
  // 参照先を更新して再計算すると、負閉路の判定・距離・経路も更新される。
  h_input = Graph<false, ll>(1, vc<tuple<int, int, ll>>{{0, 0, -1}});
  assert((h.johnson() == vvl{{-INF}}) && h.negative_cycle());
  h_input = Graph<false, ll>(1, vc<pair<int, int>>{});
  assert(h.solve()[0][0] == 0 && !h.negative_cycle());
  // 負閉路の反復による距離の増幅を打ち切り、int でもオーバーフローを避ける。
  vc<tuple<int, int, int>> dense;
  repi(u, 24) repi(v, 24) if (u != v) dense.eb(u, v, -1000000);
  GraphDirected<int> dense_input(26, dense);
  AllPairsShortestPath<true, int, int_inf> bounded(dense_input);
  assert(bounded.negative_cycle());
  for (const auto &ds : {bounded.solve(), bounded.floyd_warshall(), bounded.johnson()})
  {
    assert(bounded.negative_cycle());
    repi(s, 26) repi(t, 26)
      assert(ds[s][t] == (s < 24 && t < 24 ? -int_inf() : s == t ? 0 : int_inf()));
  }
  PRINT("Hello World");
}
