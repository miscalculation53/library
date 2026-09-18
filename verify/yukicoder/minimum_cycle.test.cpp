#define PROBLEM "https://yukicoder.me/problems/no/1320"

#include "graph/minimum_cycle.hpp"

int main()
{
  LL(T, N, M);
  VEC(tlll, M, es);
  for (auto &[u, v, w] : es) --u, --v;
  ll ans;
  if (T == 0)
  {
    GraphUndirected<ll> g(N, es);
    ans = MinimumCycle(g).solve().first;
  }
  else
  {
    GraphDirected<ll> g(N, es);
    ans = MinimumCycle(g).solve().first;
  }
  PRINT(ans == INF ? -1 : ans);
}
