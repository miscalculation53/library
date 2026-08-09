#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"

// Test focus: dense Dijkstra keeps unreachable vertices at INF, and shortest-path
// counting handles ties and zero-weight edges.
int main()
{
  {
    ShortestPath<true, ll> g(3, vc<pair<int, int>>{{0, 1}});
    auto ds = g.dijkstra_dense(0, 2);
    assert(ds[0] == 0 && ds[1] == 1 && ds[2] == INF);
  }
  {
    vc<tuple<int, int, ll>> es = {{0, 1, 2}, {0, 2, 1}, {2, 1, 1}, {1, 3, 1}};
    ShortestPath<true, ll> g(4, es);
    g.solve(0);
    assert((g.count_paths<ll>() == vc<ll>{1, 2, 1, 2}));
  }
  {
    ShortestPath<true, ll> g(2, vc<tuple<int, int, ll>>{{0, 1, 0}});
    g.solve(0);
    assert((g.count_paths<ll>() == vc<ll>{1, 1}));
  }
  PRINT("Hello World");
}
