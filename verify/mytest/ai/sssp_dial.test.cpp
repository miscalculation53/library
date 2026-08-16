#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"

// Test focus: Dial's algorithm matches Dijkstra with zero and bounded integer weights,
// including repeated decreases, unreachable vertices, target cutoff, and path restoration.
int main()
{
  mt19937 rng(123456789);
  repi(tc, 300)
  {
    const int n = 2 + rng() % 40;
    const int max_cost = rng() % 10;
    vc<tuple<int, int, ll>> es;
    repi(_, rng() % (4 * n + 1))
    {
      int u = rng() % n, v = rng() % n;
      ll w = max_cost == 0 ? 0 : rng() % (max_cost + 1);
      es.eb(u, v, w);
    }
    ShortestPath<true, ll> g(n, es);
    int s = rng() % n;
    auto expected = g.dijkstra(s);
    auto actual = g.dial(s, max_cost);
    assert(actual == expected);

    int t = rng() % n;
    auto partial = g.dial(s, max_cost, t);
    assert(partial[t] == expected[t]);
    auto path = g.path(t);
    if (expected[t] == INF)
      assert(path.empty());
    else
    {
      ll sum = 0;
      int v = s;
      fec(e : path)
      {
        assert(e.from == v);
        v = e.to, sum += e.cost;
      }
      assert(v == t && sum == expected[t]);
    }
  }

  // Test focus: solve may select Dial on a sparse graph without changing its result.
  {
    const int n = 100;
    vc<tuple<int, int, ll>> es;
    repi(i, n - 1) es.eb(i, i + 1, i % 4);
    repi(i, 400) es.eb(i % n, (i * 37 + 11) % n, i % 4);
    ShortestPath<true, ll> g(n, es);
    assert(g.solve(0) == g.dijkstra(0));
  }
  PRINT("Hello World");
}
