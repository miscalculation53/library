#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"

// Test focus: dense Dijkstra keeps unreachable vertices at INF, and shortest-path
// counting handles ties and zero-weight edges.
int main()
{
  {
    Graph<true, ll> g_input(3, vc<pair<int, int>>{{0, 1}});
    ShortestPath g(g_input);
    auto ds = g.dijkstra_dense(0, 2);
    assert(ds[0] == 0 && ds[1] == 1 && ds[2] == INF);
  }
  {
    vc<tuple<int, int, ll>> es = {{0, 1, 2}, {0, 2, 1}, {2, 1, 1}, {1, 3, 1}};
    Graph<true, ll> g_input(4, es);
    ShortestPath g(g_input);
    g.solve(0);
    assert((g.count_paths<ll>() == vc<ll>{1, 2, 1, 2}));
  }
  {
    Graph<true, ll> g_input(2, vc<tuple<int, int, ll>>{{0, 1, 0}});
    ShortestPath g(g_input);
    g.solve(0);
    assert((g.count_paths<ll>() == vc<ll>{1, 1}));
  }
  {
    GraphDirected<ll> input(6, vc<tuple<int, int, ll>>{
        {0, 1, 2}, {1, 2, -3}, {2, 1, 1}, {2, 3, 0}, {0, 4, 7}});
    ShortestPath sp(input);
    assert((sp.solve(0) == vl{0, -INF, -INF, -INF, 7, INF}));
    auto prev = sp.prev_edges();
    for (int t : {1, 2, 3, 5})
      assert(sp.path(t).empty() && prev[t].index == -1);
    const auto finite = sp.path(4);
    assert(finite.size() == 1 && finite[0].index == 4 && finite[0].cost == 7);
    assert(sp.solve(1)[1] == -INF && sp.path(1).empty());
    assert((sp.solve(4) == vl{INF, INF, INF, INF, 0, INF}));
  }
  PRINT("Hello World");
}
