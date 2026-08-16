#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/graph.hpp"

// Test focus: the compact graph storage preserves the Edge API, edge ids,
// self-loops, degree/matrix helpers, and erasable graph behavior.
template <bool directed, class Cost, bool erasable = false>
struct InspectGraph : Graph<directed, Cost, erasable>
{
  using Graph<directed, Cost, erasable>::Graph;
  static constexpr int stored_edge_size() { return sizeof(typename Graph<directed, Cost, erasable>::GE); }
};

int main()
{
  static_assert(InspectGraph<true, ll>::stored_edge_size() == 16);
  static_assert(InspectGraph<true, ll, true>::stored_edge_size() == sizeof(Edge<ll>));

  {
    InspectGraph<true, bool> g(0, vc<pair<int, int>>{});
    assert(g.size<int>() == 0 && g.num_of_edges<int>() == 0 && g.edges().empty());
  }
  {
    vc<tuple<int, int, ll>> es = {{2, 0, 7}, {2, 2, 5}, {2, 0, 3}, {0, 1, 4}};
    InspectGraph<true, ll> g(3, es);
    auto row = g.out_edges(2);
    assert(row.size() == 3);
    for (auto &e : row) assert(e.from == 2);
    auto arcs = g.out_arcs(2);
    static_assert(sizeof(remove_cv_t<remove_reference_t<decltype(arcs[0])>>) == 16);
    assert(arcs[0].to == 0 && arcs[0].cost == 7 && arcs[0].index == 0);
    assert(row[0].from == 2 && row[0].to == 0 && row[0].cost == 7 && row[0].index == 0);
    assert(row[1].from == 2 && row[1].to == 2 && row[1].cost == 5 && row[1].index == 1);
    assert(row.back().index == 2);
    assert((g.out_vertices<int>(2) == vc<int>{0, 2, 0}));
    assert((g.indegs<int>() == vc<int>{2, 1, 1}));
    assert((g.outdegs<int>() == vc<int>{1, 0, 3}));
    assert((g.adj_matrix_ecnt<int>() == vvc<int>{{0, 1, 0}, {0, 0, 0}, {2, 0, 1}}));
    auto adj = g.adj_list();
    assert(adj[2].size() == 3 && adj[2][0].from == 2 && adj[2][0].index == 0);
    auto all = g.edges();
    assert(all.size() == es.size());
    sort(ALL(all), [](cauto &a, cauto &b) { return a.index < b.index; });
    repi(i, all.size()) assert(all[i].index == i);

    auto rg = rev_graph(g);
    auto rev = rg.edges();
    vc<tuple<int, int, ll>> got;
    fec(e : rev) got.eb(e.from, e.to, e.cost);
    vc<tuple<int, int, ll>> expected;
    fec([ u, v, w ] : es) expected.eb(v, u, w);
    sort(ALL(got)), sort(ALL(expected));
    assert(got == expected);
  }
  {
    InspectGraph<true, ll> g(3, vc<pair<int, int>>{{0, 1}, {2, 1}}, 9);
    auto es = g.edges();
    assert(es.size() == 2);
    fec(e : es) assert(e.cost == 9);
  }
  {
    vc<tuple<int, int, ll>> es = {{2, 0, 7}, {1, 1, 5}, {0, 2, 3}};
    InspectGraph<false, ll> g(3, es);
    assert(g.num_of_edges<int>() == 3);
    assert((g.indegs<int>() == vc<int>{2, 1, 2}));
    assert((g.outdegs<int>() == vc<int>{2, 1, 2}));
    assert((g.adj_matrix_ecnt<int>() == vvc<int>{{0, 0, 2}, {0, 1, 0}, {2, 0, 0}}));
    auto all = g.edges();
    assert(all.size() == es.size());
    fec(e : all) assert(e.from <= e.to);
  }
  {
    InspectGraph<false, ll, true> g(3, vc<pair<int, int>>{{0, 1}, {1, 2}, {1, 1}});
    g.erase_edge(1);
    assert(g.num_of_edges<int>() == 2);
    assert((g.out_vertices<int>(0) == vc<int>{1}));
    assert((g.out_vertices<int>(1) == vc<int>{0, 1}));
    assert(g.out_edges(2).empty());
    assert(g.edges().size() == 2);
  }
  PRINT("Hello World");
}
