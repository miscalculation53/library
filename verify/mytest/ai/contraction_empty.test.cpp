#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/contraction.hpp"

// Test focus: contracting an empty graph is defined and stays empty.
int main()
{
  GraphDirected<bool> g(0, vc<pair<int, int>>{});
  auto h = contracted_graph(g, vc<int>{});
  assert(h.size() == 0 && h.num_of_edges() == 0);
  PRINT("Hello World");
}
