#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"

int main()
{
  mt19937_64 rng(937518);
  for (int trial = 0; trial < 500; trial++)
  {
    const int n = 1 + rng() % 100, m = rng() % 1000;
    vc<tuple<int, int, ll>> es;
    for (int i = 0; i < m; i++) es.eb(rng() % n, rng() % n, rng() % 100000);
    GraphDirected<ll> g(n, es);
    ShortestPath sp(g);
    const int s = rng() % n, t = rng() % n;
    const auto expected = sp.dijkstra(s);
    assert(sp.dijkstra_fibonacci(s) == expected);
    const auto actual = sp.dijkstra_fibonacci(s, t);
    assert(actual[t] == expected[t]);
    const auto path = sp.path(t);
    if (expected[t] != INF)
    {
      ll cost = 0;
      int u = s;
      for (const auto &e : *path) assert(e.from == u), u = e.to, cost += e.cost;
      assert(u == t && cost == expected[t]);
    }
    else assert(!path);
  }
  const GraphDirected<ll> large(3, vc<tuple<int, int, ll>>{{0, 1, 3'000'000'000'000'000'000LL},
      {1, 2, 8'000'000'000'000'000'000LL}, {0, 2, 7}});
  ShortestPath sp(large);
  const auto result = sp.dijkstra_fibonacci(0);
  assert(result[1] == 3'000'000'000'000'000'000LL && result[2] == 7);
  const GraphUndirected<> unweighted(4, vc<pair<int, int>>{{0, 1}, {1, 2}});
  ShortestPath unit(unweighted);
  assert(unit.dijkstra_fibonacci(0) == unit.bfs(0));
  cout << "Hello World\n";
}
