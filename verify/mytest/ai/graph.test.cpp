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

#ifdef LOCAL
struct DumpCost
{
  ll distance;
  string label;
  CPP_DUMP_DEFINE_DATA(distance, label);
};

template <class Cost>
void check_dump_edge(const Edge<Cost> &e, const string &cost_text)
{
  for (const auto &text : {cp::export_var(e), cp::export_var(vc<Edge<Cost>>{e})})
  {
    assert(text.find("from= " + to_string(e.from)) != string::npos);
    assert(text.find("to= " + to_string(e.to)) != string::npos);
    assert(text.find("cost= " + cost_text) != string::npos);
    assert(text.find("index= " + to_string(e.index)) != string::npos);
  }
}

void check_dump()
{
  cp::options::es_style = cp::types::es_style_t::no_es;
  cp::options::max_line_width = 1000;
  check_dump_edge(Edge<>(2, 1, 7), "1");
  check_dump_edge(Edge<bool>(2, 1, true, 7), "true");
  check_dump_edge(Edge<int>(2, 1, 4, 7), "4");
  check_dump_edge(Edge<ll>(2, 1, 10000000000LL, 7), "10000000000");
  check_dump_edge(Edge<double>(2, 1, 2.5, 7), "2.5");
  check_dump_edge(Edge<i128>(2, 1, i128(1) << 100, 7), "1267650600228229401496703205376");
  check_dump_edge(Edge<vc<int>>(2, 1, {4, 9}, 7), "[ 4, 9 ]");
  check_dump_edge(Edge<DumpCost>(2, 1, {17, "road"}, 7),
                  "DumpCost{ distance= 17, label= \"road\" }");
  const GraphDirected<double> g(3, vc<tuple<int, int, double>>{{2, 1, 2.5}});
  check_dump_edge(g.out_edges(2).front(), "2.5");
}
#endif

#if __cplusplus >= 202002L
template <class Cost = void>
using ErasableUndirected = GraphUndirected<Cost, true>;

void check_deduction()
{
  const vc<pair<ll, ll>> uv{{0, 1}, {1, 2}};
  const vc<tuple<ll, ll, ll>> uvw{{0, 1, 7}, {1, 2, 9}};
  GraphDirected unweighted(3, uv);
  GraphUndirected weighted(3, uvw);
  GraphDirected uniform(3, uv, 5LL);
  GraphUndirected fractional(3, uv, 0.5);
  GraphDirected from_edges(3, vc<Edge<int>>{{0, 1, 4, 42}});
  GraphUndirected from_unit_edges(3, vc<Edge<>>{{0, 1, 42}});
  static_assert(is_same_v<decltype(unweighted), GraphDirected<void>>);
  static_assert(is_same_v<decltype(weighted), GraphUndirected<ll>>);
  static_assert(is_same_v<decltype(uniform), GraphDirected<ll>>);
  static_assert(is_same_v<decltype(fractional), GraphUndirected<double>>);
  static_assert(is_same_v<decltype(from_edges), GraphDirected<int>>);
  static_assert(is_same_v<decltype(from_unit_edges), GraphUndirected<void>>);
  assert(unweighted.out_edges(0).front().cost == 1);
  assert(weighted.out_edges(1).back().cost == 9);
  fec(e : uniform.edges()) assert(e.cost == 5);
  fec(e : fractional.edges()) assert(e.cost == 0.5);
  assert(from_edges.edges().front().cost == 4 && from_edges.edges().front().index == 0);
  assert(from_unit_edges.edges().front().cost == 1 && from_unit_edges.edges().front().index == 0);

  GraphDirected empty(0, vc<pair<int, int>>{});
  GraphUndirected empty_weighted(0, vc<tuple<int, int, int>>{});
  static_assert(is_same_v<decltype(empty), GraphDirected<void>>);
  static_assert(is_same_v<decltype(empty_weighted), GraphUndirected<int>>);
  assert(empty.edges().empty() && empty_weighted.edges().empty());
  GraphUndirected copy(weighted);
  static_assert(is_same_v<decltype(copy), GraphUndirected<ll>>);
  assert(copy.out_edges(1).back().cost == 9);

  ErasableUndirected erasable(3, uv);
  ErasableUndirected erasable_weighted(3, uvw);
  static_assert(is_same_v<decltype(erasable), GraphUndirected<void, true>>);
  static_assert(is_same_v<decltype(erasable_weighted), GraphUndirected<ll, true>>);
  erasable.erase_edge(0);
  erasable_weighted.erase_edge(0);
  assert(erasable.num_of_edges() == 1 && erasable.edges().front().cost == 1);
  assert(erasable_weighted.num_of_edges() == 1 && erasable_weighted.edges().front().cost == 9);
}
#endif

int main()
{
#ifdef LOCAL
  check_dump();
#endif
#if __cplusplus >= 202002L
  check_deduction();
#endif
  static_assert(InspectGraph<true, ll>::stored_edge_size() == 16);
  static_assert(InspectGraph<true, bool>::stored_edge_size() == 12);
  static_assert(InspectGraph<true, void>::stored_edge_size() == 8);
  static_assert(InspectGraph<true, void, true>::stored_edge_size() == 12);
  static_assert(sizeof(Edge<void>) == 12);
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
    static_assert(is_same_v<decltype(row[0]), Edge<ll>>);
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
