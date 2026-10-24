#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/scc.hpp"

int main()
{
  assert(scc(GraphDirected<>(0, vc<pair<int, int>>{})).empty());
  const auto isolated = scc(GraphDirected<>(1000, vc<pair<int, int>>{}));
  repi(v, 1000) assert(isolated[v] == 999 - v);
  const int n = 200000;
  vc<pair<int, int>> edges;
  repi(v, 1, n) edges.eb(v - 1, v);
  GraphDirected<> chain(n, edges);
  auto ids = scc(chain);
  repi(v, n) assert(ids[v] == v);
  edges.eb(n - 1, 0);
  GraphDirected<> cycle(n, edges);
  assert(scc(cycle) == vl(n, 0));

  using Weight = array<ll, 8>;
  vc<tuple<int, int, Weight>> weighted;
  for (auto [u, v] : vc<pair<int, int>>{{0, 1}, {1, 0}, {1, 2}, {2, 3}, {3, 2}, {0, 1}, {2, 2}})
    weighted.eb(u, v, Weight{});
  GraphDirected<Weight> g(5, weighted);
  const auto small = scc<Weight, unsigned char>(g);
  assert(small[0] == small[1] && small[2] == small[3]);
  assert(small[1] < small[2] && small[4] != small[0] && small[4] != small[2]);
  cout << "Hello World" << endl;
}
