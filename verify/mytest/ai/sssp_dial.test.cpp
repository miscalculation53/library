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
    Graph<true, ll> g_input(n, es);
    ShortestPath g(g_input);
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
    Graph<true, ll> g_input(n, es);
    ShortestPath g(g_input);
    assert(g.solve(0) == g.dijkstra(0));
  }
  // 重みが大きいときも、計算量の比較で積をあふれさせずに Dijkstra を選ぶ。
  {
    const i128 w = i128(1) << 123;
    Graph<true, i128> g_input(128, vc<tuple<int, int, i128>>{{0, 1, w}, {0, 2, w / 2}});
    ShortestPath g(g_input);
    auto ds = g.solve(0);
    assert(ds == g.dijkstra(0));
    assert(ds[1] == w && ds[2] == w / 2);
  }
  PRINT("Hello World");
}
