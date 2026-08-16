#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/subgraphs.hpp"

// Test focus: partitioning handles an empty graph and preserves weighted undirected
// edges, including reversed input orientation and self-loops, in each subgraph.
int main()
{
  {
    GraphDirected<bool> g(0, vc<pair<int, int>>{});
    auto [gs, ids] = subgraphs(g, vc<int>{});
    assert(gs.empty() && ids.empty());
  }
  {
    vc<tuple<int, int, ll>> es = {{2, 0, 7}, {2, 2, 5}, {1, 3, 11}, {0, 1, 13}};
    GraphUndirected<ll> g(4, es);
    auto [gs, ids] = subgraphs(g, vc<int>{0, 1, 0, 1});
    assert((ids == vc<int>{0, 0, 1, 1}));
    assert(gs.size() == 2);
    auto es0 = gs[0].edges(), es1 = gs[1].edges();
    vc<tuple<int, int, ll>> got0, got1;
    fec(e : es0) got0.eb(e.from, e.to, e.cost);
    fec(e : es1) got1.eb(e.from, e.to, e.cost);
    sort(ALL(got0)), sort(ALL(got1));
    assert((got0 == vc<tuple<int, int, ll>>{{0, 1, 7}, {1, 1, 5}}));
    assert((got1 == vc<tuple<int, int, ll>>{{0, 1, 11}}));
  }
  PRINT("Hello World");
}
