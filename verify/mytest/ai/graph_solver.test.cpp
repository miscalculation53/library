#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/minimum_cycle.hpp"

template <class Solver, class G>
void check_construction()
{
  static_assert(is_constructible_v<Solver, G &>);
  static_assert(is_constructible_v<Solver, const G &>);
  static_assert(!is_constructible_v<Solver, G &&>);
  static_assert(!is_constructible_v<Solver, const G &&>);
}

template <class Cost>
vc<int> edge_ids(const vc<Edge<Cost>> &path)
{
  vc<int> ids;
  fec(e : path) ids.eb(e.index);
  return ids;
}

template <bool directed, class Cost = ll>
void shared_graph()
{
  using G = Graph<directed, Cost>;
  const G g = []
  {
    if constexpr (is_void_v<Cost>)
      return G(5, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}, {2, 3}});
    else
      return G(5, vc<tuple<int, int, ll>>{{0, 1, 2}, {1, 2, 3}, {2, 0, 4}, {2, 3, 1}});
  }();
  ShortestPath sp(g), other(g);
  AllPairsShortestPath ap(g);
  const MinimumCycle mc(g);
  check_construction<decltype(sp), G>();
  check_construction<decltype(ap), G>();
  check_construction<MinimumCycle<directed, Cost>, G>();
  static_assert(is_same_v<decltype(sp), ShortestPath<directed, Cost>>);
  static_assert(is_same_v<decltype(ap), AllPairsShortestPath<directed, Cost>>);
  static_assert(!is_base_of_v<G, decltype(sp)>);
  static_assert(!is_base_of_v<decltype(sp), decltype(ap)>);
  static_assert(!is_base_of_v<decltype(sp), MinimumCycle<directed, Cost>>);

  // 同じグラフに対する再計算を交互に行っても、他の計算結果を保つ。
  auto ds = sp.solve(0);
  const vl expected = is_void_v<Cost> ? vl{0, 1, directed ? 2 : 1, directed ? 3 : 2, INF}
                                     : vl{0, 2, directed ? 5 : 4, directed ? 6 : 5, INF};
  assert(ds == expected);
  const auto &saved = sp;
  auto path = edge_ids(saved.path(3));
  auto counts = saved.template count_paths<ll>();
  other.solve(2);
  const auto &matrix = ap.solve();
  assert(matrix[0] == ds);
  const auto ap_path = edge_ids(ap.path(0, 3));
  assert(ap.for_each_source([&](int s, const auto &row, const auto &)
  {
    assert(row == matrix[s] && edge_ids(ap.path(0, 3)) == ap_path);
  }));
  assert(edge_ids(saved.path(3)) == path && saved.template count_paths<ll>() == counts);
  const ll length = is_void_v<Cost> ? 3 : 9;
  assert(mc.solve_vertex(0).first == length);
  assert(mc.solve_edge(0).first == length);
  assert(mc.solve().first == length);
  assert((mc.edge_costs() == vl{length, length, length, INF}));
  assert((mc.vertex_costs() == vl{length, length, length, INF, INF}));
  assert(matrix[0] == ds && edge_ids(saved.path(3)) == path);
  assert(ap.johnson()[0] == ds);
  assert(saved.template count_paths<ll>() == counts);
  sp.solve(3);
  assert(ap.path(0, 3).size() == path.size());
  assert(g.num_of_edges() == 4 && g.out_edges(0).front().cost == (is_void_v<Cost> ? 1 : 2));
}

constexpr ll unweighted_infinity() { return 10000000000LL; }

template <bool directed, auto infty>
void unweighted_custom_infinity()
{
  Graph<directed> g(5, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}});
  AllPairsShortestPath<directed, void, infty> ap(g);
  const MinimumCycle<directed, void, infty> mc(g);
  static_assert(is_same_v<typename decltype(ap)::Dist, ll>);
  static_assert(is_same_v<typename decltype(mc)::Result, pair<ll, vc<Edge<void>>>>);
  const ll inf = unweighted_infinity();
  for (const auto &ds : {ap.solve(), ap.repeated_sssp(), ap.floyd_warshall(), ap.johnson()})
  {
    assert((ds[0] == vl{0, 1, directed ? 2 : 1, inf, inf}));
    assert(!ap.negative_cycle());
    assert(ap.path(0, 3).empty() && ap.prev_edges(0)[3].index == -1);
  }
  assert((mc.edge_costs() == vl{3, 3, 3}));
  assert((mc.vertex_costs() == vl{3, 3, 3, inf, inf}));
  assert(mc.solve().first == 3 && mc.solve_edge(0).first == 3);
  assert(mc.solve_vertex(3).first == inf && mc.solve_vertex(3).second.empty());
  const auto saved = ap.solve();
  int rows = 0;
  assert(ap.for_each_source([&](int s, const auto &ds, const auto &prev)
  {
    assert(s == rows++ && ds == saved[s]);
    static_assert(is_same_v<decay_t<decltype(prev)>, vc<Edge<void>>>);
  }));
  assert(rows == 5);
  g = Graph<directed>(2, vc<pair<int, int>>{{0, 1}});
  assert(ap.solve().size() == 2 && !ap.negative_cycle());
  assert(mc.solve().first == inf && mc.solve().second.empty());
  assert(mc.solve_edge(0).first == inf && mc.solve_edge(0).second.empty());
  assert((mc.edge_costs() == vl{inf}));
  assert((mc.vertex_costs() == vl{inf, inf}));
}

void negative_edges()
{
  const GraphDirected<ll> g(4, vc<tuple<int, int, ll>>{
      {0, 1, -3}, {1, 2, 5}, {2, 0, 1}});
  ShortestPath sp(g);
  AllPairsShortestPath ap(g);
  const MinimumCycle mc(g);
  auto ds = sp.solve(0);
  assert((ds == vl{0, -3, 2, INF}));
  assert(ap.johnson()[0] == ds);
  assert(ap.path(0, 2).front().cost == -3);
  assert(mc.solve_vertex(0).first == 3);
  assert(mc.solve_edge(0).first == 3);
  assert(mc.solve().first == 3);
  assert(sp.path(2).front().cost == -3 && g.out_edges(0).front().cost == -3);
}

int main()
{
  shared_graph<false>();
  shared_graph<true>();
  shared_graph<false, void>();
  shared_graph<true, void>();
  unweighted_custom_infinity<false, 10000000000LL>();
  unweighted_custom_infinity<true, 10000000000LL>();
  unweighted_custom_infinity<false, unweighted_infinity>();
  unweighted_custom_infinity<true, unweighted_infinity>();
  negative_edges();
  PRINT("Hello World");
}
