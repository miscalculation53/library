#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/contraction.hpp"

// Test focus: contraction handles an empty graph and preserves directed/undirected
// adjacency while removing loops and parallel edges without materializing Edge copies.
int main()
{
  {
    GraphDirected<bool> g(0, vc<pair<int, int>>{});
    auto h = contracted_graph(g, vc<int>{});
    assert(h.size() == 0 && h.num_of_edges() == 0);
  }
  {
    GraphDirected<bool> g(4, vc<pair<int, int>>{{0, 1}, {0, 2}, {1, 3}, {2, 0}, {3, 1}});
    auto h = contracted_graph(g, vc<int>{0, 0, 1, 1});
    assert((h.adj_matrix_ecnt<int>() == vvc<int>{{0, 1}, {1, 0}}));
  }
  {
    GraphUndirected<bool> g(4, vc<pair<int, int>>{{0, 1}, {0, 2}, {1, 3}, {2, 3}});
    auto h = contracted_graph(g, vc<int>{0, 0, 1, 1});
    assert(h.num_of_edges<int>() == 1);
    assert((h.adj_matrix_ecnt<int>() == vvc<int>{{0, 1}, {1, 0}}));
  }
  PRINT("Hello World");
}
