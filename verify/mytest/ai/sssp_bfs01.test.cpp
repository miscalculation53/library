#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"

// Test focus: the two-vector 01 BFS matches Dijkstra for zero and one common positive
// weight, including stale registrations, target cutoff, and path restoration.
int main()
{
  mt19937 rng(123456789);
  repi(tc, 500)
  {
    const int n = 2 + rng() % 50;
    const ll positive_cost = 1 + rng() % 10;
    vc<tuple<int, int, ll>> es;
    repi(_, rng() % (6 * n + 1))
      es.eb(rng() % n, rng() % n, rng() & 1 ? 0 : positive_cost);

    ShortestPath<true, ll> g(n, es);
    int s = rng() % n;
    auto expected = g.dijkstra(s);
    assert(g.bfs01(s) == expected);
    assert(g.solve(s) == expected);

    int t = rng() % n;
    auto partial = g.bfs01(s, t);
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
  PRINT("Hello World");
}
