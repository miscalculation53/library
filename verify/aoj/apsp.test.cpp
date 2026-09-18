#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_C"

#include "graph/apsp.hpp"

int main()
{
  LL(N, M);
  VEC(tlll, M, es);
  Graph<true, ll> g_input(N, es);
  AllPairsShortestPath g(g_input);
  if (g.negative_cycle())
  {
    PRINT("NEGATIVE CYCLE");
    return 0;
  }
  const auto &dist = g.solve();
  repi(s, N)
  {
    vc<string> row;
    repi(t, N) row.eb(dist[s][t] == INF ? "INF" : to_string(dist[s][t]));
    PRINTV(row);
  }
}
