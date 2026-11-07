#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"

int main()
{
  mt19937_64 rng(20261106);
  repi(trial, 1200)
  {
    const int n = 1 + rng() % 60;
    vc<tuple<int, int, ll>> edges;
    vc<ll> potential(n);
    for (auto &h : potential) h = ll(rng() % 2001) - 1000;
    repi(i, rng() % (6 * n + 1))
    {
      int u = rng() % n, v = rng() % n;
      ll w = trial & 1 ? ll(rng() % 20) + potential[v] - potential[u] : ll(rng() % 21) - 10;
      edges.eb(u, v, w);
    }
    const GraphDirected<ll> g(n, edges);
    ShortestPath sp(g);
    vc<ll> starts;
    repi(i, rng() % (n + 1)) starts.eb(rng() % n);
    const auto expected = sp.bellman_ford(starts);
    assert(sp.spfa(starts) == expected);
    repi(t, n)
    {
      auto path = sp.path_vertex(t);
      auto es = sp.path_edge(t);
      if (expected[t] == INF || expected[t] == -INF) assert(!path && !es);
      else
      {
        assert(path && es && int(path->size()) <= n && path->size() == es->size() + 1);
        assert(find(starts.begin(), starts.end(), path->front()) != starts.end());
        assert(path->back() == t);
        ll cost = 0;
        repi(i, es->size())
        {
          const auto &e = (*es)[i];
          assert(e.from == (*path)[i] && e.to == (*path)[i + 1]);
          cost += e.cost;
        }
        assert(cost == expected[t]);
      }
    }
    const int source = rng() % n;
    assert(sp.spfa(source) == sp.bellman_ford(source));
  }
  {
    const int n = 20000;
    vc<tuple<int, int, ll>> edges;
    for (int v = 1; v < n; v++) edges.eb(v, v - 1, -1);
    const GraphDirected<ll> g(n, edges);
    ShortestPath sp(g);
    const auto d = sp.spfa(n - 1);
    repi(v, n) assert(d[v] == v - n + 1);
    assert(sp.path_vertex(0)->size() == size_t(n));
  }
  {
    const GraphDirected<ll> g(7, vc<tuple<int, int, ll>>{
        {0, 4, 2}, {0, 1, 0}, {1, 2, 0}, {2, 3, 0}, {3, 1, -1},
        {3, 5, 0}, {5, 6, 0}});
    ShortestPath sp(g);
    assert((sp.spfa(0) == vc<ll>{0, -INF, -INF, -INF, 2, -INF, -INF}));
    assert(sp.path_vertex(4) == optional<vc<int>>({{0, 4}}));
    assert(sp.spfa(vc<ll>{4, 4}) == sp.bellman_ford(4));
  }
  {
    const int n = 250;
    vc<tuple<int, int, ll>> edges;
    for (int v = n - 1; v >= 1; v--) edges.eb(0, v, 0);
    for (int v = 1; v + 1 < n; v++) edges.eb(v, v + 1, -1);
    const GraphDirected<ll> g(n, edges);
    ShortestPath sp(g);
    const auto d = sp.spfa(0);
    assert(sp.bellman_ford(0) == d);
    repi(v, n) assert(d[v] == (v == 0 ? 0 : 1 - v));
  }
  {
    const i128 w = i128(1) << 100;
    const GraphDirected<i128> g(3, vc<tuple<int, int, i128>>{{0, 1, -w}, {1, 2, -w}});
    ShortestPath sp(g);
    assert((sp.spfa(vc<ll>{0, 1}) == vc<i128>{0, -w, -2 * w}));
  }
  cout << "Hello World\n";
}
