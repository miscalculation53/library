#define PROBLEM "https://atcoder.jp/contests/abc308/tasks/abc308_h"

#include "graph/minimum_cycle.hpp"

int main()
{
  LL(N, M);
  VEC(tlll, M, es);
  for (auto &[u, v, w] : es) --u, --v;
  GraphUndirected<ll> g(N, es);
  MinimumCycle mc(g);
  ll ans = INF;
  repi(s, N)
  {
    if (g.out_edges(s).size() < 3) continue;
    auto [cost, cycle] = mc.solve_vertex(s);
    if (cost == INF) continue;
    const auto first = cycle.front(), last = cycle.back();
    fec(e : g.out_edges(s))
      if (e.index != first.index && e.index != last.index) chmin(ans, cost + e.cost);
    for (const auto &tail : {first, last})
    {
      vc<tuple<ll, ll, ll>> remaining;
      repi(i, M) if (i != tail.index) remaining.eb(es[i]);
      GraphUndirected<ll> without(N, remaining);
      auto next = MinimumCycle(without).solve_vertex(s).first;
      if (next != INF) chmin(ans, next + tail.cost);
    }
  }
  PRINT(ans == INF ? -1 : ans);
}
