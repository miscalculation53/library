#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/subgraphs.hpp"

// Test focus: partitioning an empty graph produces no subgraph or vertex mapping.
int main()
{
  GraphDirected<bool> g(0, vc<pair<int, int>>{});
  auto [gs, ids] = subgraphs(g, vc<int>{});
  assert(gs.empty() && ids.empty());
  PRINT("Hello World");
}
