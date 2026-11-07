#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"

template <bool directed, class Cost, class Starts>
vc<ll> reference(const Graph<directed, Cost> &g, const Starts &starts)
{
  const int n = g.size();
  vvc<ll> d(n, vc<ll>(n, INF));
  repi(v, n) d[v][v] = 0;
  repi(v, n) for (const auto &e : g.out_edges(v)) chmin(d[v][e.to], ll(e.cost));
  repi(k, n) repi(u, n) repi(v, n)
    if (d[u][k] != INF && d[k][v] != INF) chmin(d[u][v], d[u][k] + d[k][v]);
  vc<ll> result(n, INF);
  for (auto s : starts) repi(v, n) chmin(result[v], d[s][v]);
  repi(k, n) if (d[k][k] < 0)
  {
    bool reachable = false;
    for (auto s : starts) reachable |= d[s][k] != INF;
    if (reachable) repi(v, n) if (d[k][v] != INF) result[v] = -INF;
  }
  return result;
}

template <class Solver, class Starts>
void check_path(const Solver &sp, const Starts &starts, int t, ll distance)
{
  const auto edges = sp.path_edge(t);
  const auto vertices = sp.path_vertex(t);
  if (distance == INF || distance == -INF)
  {
    assert(!edges && !vertices);
    assert(sp.prev_edges()[t].index == -1);
    return;
  }
  assert(edges && vertices && vertices->size() == edges->size() + 1);
  assert(find(starts.begin(), starts.end(), vertices->front()) != starts.end());
  assert(vertices->back() == t);
  ll sum = 0;
  repi(i, edges->size())
  {
    const auto &e = (*edges)[i];
    assert(e.from == (*vertices)[i] && e.to == (*vertices)[i + 1]);
    sum += e.cost;
  }
  assert(sum == distance);
  assert(sp.prev_edges()[vertices->front()].index == -1);
}

template <bool directed, class Cost, class Starts>
void check_algorithms(const Graph<directed, Cost> &g, const Starts &starts, int mode)
{
  const auto expected = reference(g, starts);
  const int n = g.size();
  ShortestPath sp(g);
  auto check = [&](auto run)
  {
    assert(run(-1) == expected);
    repi(t, n) check_path(sp, starts, t, expected[t]);
    if (n)
    {
      for (int t : {0, n - 1})
      {
        assert(run(t)[t] == expected[t]);
        check_path(sp, starts, t, expected[t]);
      }
    }
  };
  check([&](int t) { return sp.solve(starts, t); });
  check([&](int) { return sp.bellman_ford(starts); });
  check([&](int) { return sp.spfa(starts); });
  if (mode < 0) return;
  check([&](int t) { return sp.dijkstra(starts, t); });
  check([&](int t) { return sp.dijkstra_dense(starts, t); });
  check([&](int t) { return sp.dijkstra_fibonacci(starts, t); });
  if constexpr (is_integral_ext<typename decltype(sp)::Dist>)
    check([&](int t) { return sp.dial(starts, 9, t); });
  if (mode <= 1) check([&](int t) { return sp.bfs01(starts, t); });
  if (mode == 0) check([&](int t) { return sp.bfs(starts, t); });
}

template <bool directed>
void randomized(mt19937 &rng)
{
  for (int mode = -1; mode < 3; mode++) repi(trial, 100)
  {
    const int n = 1 + rng() % 18;
    vc<tuple<int, int, ll>> edges;
    repi(i, rng() % (4 * n + 1))
    {
      int u = rng() % n, v = rng() % n;
      ll w = mode < 0 ? ll(rng() % 11) - 5 : mode == 0 ? 1 : mode == 1 ? (rng() % 2) * 7 : rng() % 10;
      edges.eb(u, v, w);
    }
    const Graph<directed, ll> g(n, edges);
    vc<int> starts;
    repi(i, rng() % (2 * n + 1)) starts.eb(rng() % n);
    check_algorithms(g, starts, mode);
    const vc<ll> wide(starts.begin(), starts.end());
    check_algorithms(g, wide, mode);
    if (mode == 0)
    {
      vc<pair<int, int>> unweighted;
      for (auto [u, v, w] : edges) unweighted.eb(u, v);
      const Graph<directed> unit(n, unweighted);
      check_algorithms(unit, wide, mode);
    }
  }
}

int main()
{
  mt19937 rng(614372);
  randomized<true>(rng);
  randomized<false>(rng);
  {
    const GraphDirected<ll> g(5, vc<tuple<int, int, ll>>{
        {0, 1, 0}, {0, 2, 2}, {1, 2, 2}, {2, 3, 1}});
    ShortestPath sp(g), shared(g);
    const vc<ll> starts{0, 1, 1};
    assert((sp.solve(starts) == vc<ll>{0, 0, 2, 3, INF}));
    assert((sp.count_paths<ll>() == vc<ll>{1, 2, 3, 3, 0}));
    assert(sp.path_vertex(1) == optional<vc<int>>({{1}}));
    assert(sp.path_edge(1)->empty());
    auto copied = sp;
    auto moved = move(copied);
    assert(moved.path_vertex(3) == sp.path_vertex(3));
    assert(moved.count_paths<ll>() == sp.count_paths<ll>());
    assert((shared.solve(1) == vc<ll>{INF, 0, 2, 3, INF}));
    assert((sp.count_paths<ll>() == vc<ll>{1, 2, 3, 3, 0}));
    assert((sp.solve(vc<int>{}) == vc<ll>(5, INF)));
    assert((sp.count_paths<ll>() == vc<ll>(5, 0)));
    assert(!sp.path_vertex(0) && !sp.path_edge(3));
    assert(sp.prev_edges()[0].index == -1);
    assert((sp.solve(4LL) == vc<ll>{INF, INF, INF, INF, 0}));
    assert((sp.count_paths<ll>() == vc<ll>{0, 0, 0, 0, 1}));
    check_algorithms(g, vc<unsigned>{0, 1, 0}, 2);
    check_algorithms(g, vc<i128>{0, 1, 1}, 2);
  }
  {
    const GraphDirected<ll> g(4, vc<tuple<int, int, ll>>{{0, 1, -2}, {1, 2, 3}, {0, 2, 1}});
    ShortestPath sp(g);
    const vc<ll> starts{0, 1, 1};
    assert((sp.solve(starts) == vc<ll>{0, -2, 1, INF}));
    assert((sp.count_paths<ll>() == vc<ll>{1, 1, 2, 0}));
    assert(sp.path_vertex(1) == optional<vc<int>>({{0, 1}}));
    assert((sp.solve(vc<int>{1}) == sp.solve(1)));
  }
  {
    const GraphDirected<ll> g(6, vc<tuple<int, int, ll>>{
        {0, 1, 3}, {2, 3, -2}, {3, 2, 1}, {3, 4, 2}});
    check_algorithms(g, vc<int>{0}, -1);
    check_algorithms(g, vc<ll>{0, 2, 2}, -1);
    check_algorithms(g, vc<int>{}, -1);
  }
  {
    const GraphDirected<> g(0, vc<pair<int, int>>{});
    check_algorithms(g, vc<int>{}, 0);
    ShortestPath sp(g);
    assert(sp.solve(vc<ll>{}).empty());
    assert(sp.count_paths<ll>().empty());
  }
  {
    const GraphDirected<int> g(4, vc<tuple<int, int, int>>{{0, 1, 2}, {2, 1, 1}});
    ShortestPath<true, int, 1000> sp(g);
    const vc<ll> starts{0, 2};
    assert((sp.solve(starts) == vc<int>{0, 1, 0, 1000}));
    assert((sp.count_paths<ll>() == vc<ll>{1, 1, 1, 0}));
  }
  {
    const GraphDirected<i128> g(4, vc<tuple<int, int, i128>>{
        {0, 1, i128(1) << 80}, {2, 1, i128(1) << 79}});
    ShortestPath sp(g);
    const vc<i128> starts{0, 2, 0};
    const auto d = sp.solve(starts);
    assert(d[0] == 0 && d[1] == (i128(1) << 79) && d[2] == 0);
    assert(sp.dijkstra_fibonacci(starts) == d);
    assert(sp.path_vertex(1) == optional<vc<int>>({{2, 1}}));
  }
  {
    const GraphDirected<double> g(4, vc<tuple<int, int, double>>{
        {0, 1, 2.5}, {2, 1, 1.5}});
    ShortestPath sp(g);
    const vc<ll> starts{0, 2};
    const auto d = sp.solve(starts);
    assert(d[1] == 1.5 && sp.dijkstra(starts) == d && sp.dijkstra_fibonacci(starts) == d);
  }
  {
    const int n = 100;
    vc<tuple<int, int, ll>> edges;
    repi(v, n - 1) edges.eb(v, v + 1, v % 4);
    const GraphDirected<ll> g(n, edges);
    ShortestPath sp(g);
    const vc<ll> starts{0, 25, 50, 99};
    const auto expected = reference(g, starts);
    assert(sp.solve(starts) == expected && sp.dial(starts, 3) == expected);
  }
  repi(trial, 200)
  {
    const int n = 1 + rng() % 18;
    vc<tuple<int, int, ll>> edges;
    repi(u, n) for (int v = u + 1; v < n; v++)
      if (rng() % 4 == 0) edges.eb(u, v, ll(rng() % 11) - 5);
    const GraphDirected<ll> g(n, edges);
    vc<ll> starts;
    repi(i, rng() % (2 * n + 1)) starts.eb(rng() % n);
    const auto expected = reference(g, starts);
    vc<ll> counts(n);
    vc<bool> seen(n);
    for (auto s : starts) if (!seen[s])
    {
      seen[s] = true;
      vc<ll> d(n, INF), ways(n);
      d[s] = 0, ways[s] = 1;
      repi(u, n) if (d[u] != INF)
        for (const auto &e : g.out_edges(u))
        {
          const ll next = d[u] + e.cost;
          if (next < d[e.to]) d[e.to] = next, ways[e.to] = ways[u];
          else if (next == d[e.to]) ways[e.to] += ways[u];
        }
      repi(v, n) if (d[v] != INF && d[v] == expected[v]) counts[v] += ways[v];
    }
    ShortestPath sp(g);
    assert(sp.solve(starts) == expected);
    assert(sp.count_paths<ll>() == counts);
    assert(sp.spfa(starts) == expected);
    assert(sp.count_paths<ll>() == counts);
  }
  cout << "Hello World\n";
}
