#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "utils/resolved_infty.hpp"
#include "convex/convex_hull_trick_dot_product.hpp"
#include "algebra/acted_monoid/add_min.hpp"
#include "algebra/acted_monoid/clamp_min_max.hpp"
#include "graph/apsp.hpp"
#include "graph/minimum_cycle.hpp"
#include "math/bigint.hpp"

constexpr i128 WIDE_INF = i128(INF) * INF;
static_assert(default_infty<ll>() == INF);
static_assert(default_infty<ull>() == ull(INF));
static_assert(default_infty<i128>() == WIDE_INF);
static_assert(default_infty<u128>() == u128(INF) * INF);
static_assert(default_infty<int>() == (1 << 30) - 1);
static_assert(default_infty<unsigned int>() == (1U << 30) - 1);
static_assert(default_infty<short>() == numeric_limits<short>::max() / 2);
static_assert(default_infty<signed char>() == numeric_limits<signed char>::max() / 2);
static_assert(default_infty<unsigned char>() == numeric_limits<unsigned char>::max() / 2);
static_assert(default_infty<int>() + default_infty<int>() <= numeric_limits<int>::max());
static_assert(default_infty<ll>() + default_infty<ll>() <= numeric_limits<ll>::max());
static_assert(WIDE_INF + WIDE_INF <= numeric_limits<i128>::max());
static_assert(MonoidMin<int>::e() == (1 << 30) - 1);
static_assert(MonoidMax<i128>::e() == -WIDE_INF);
static_assert(resolved_infty<i128, INF>() == i128(INF));
static_assert(resolved_value<int *, nullptr>() == nullptr);

int provider_calls = 0;
BigInteger<> explicit_big_infty()
{
  provider_calls++;
  return BigInteger<>("10000000000000000000000000000000000000000");
}

void test_rational_and_providers()
{
  using R = Rational<ll>;
  const R &inf = default_infty<R>();
  assert(inf.num == 1 && inf.den == 0);
  assert(&inf == &default_infty<R>());
  assert(MonoidMin<R>::e().is_infinite());
  assert(MonoidMax<R>::e().num == -1);
  assert(ActedMonoidAddMin<R>::mapping(R(3), inf).is_infinite());
  using Clamp = ActedMonoidClampMinMax<R>;
  auto unchanged = Clamp::mapping(Clamp::id(), Clamp::e());
  assert(unchanged.mn == inf && unchanged.mx == -inf);
  auto finite = resolved_infty<R, INF>();
  assert(finite.num == INF && finite.den == 1);
  auto explicit_inf = resolved_infty<R, R::infty>();
  assert(explicit_inf == inf);

  using BI = BigInteger<>;
  using BR = Rational<BI>;
  assert(default_infty<BR>().num == BI(1) && default_infty<BR>().den == BI(0));
  const BI &a = resolved_infty<BI, explicit_big_infty>();
  const BI &b = resolved_infty<BI, explicit_big_infty>();
  assert(&a == &b && provider_calls == 1);
  assert((MonoidMin<BI, explicit_big_infty>::e() == a));
  assert(isinf(default_infty<float>()) && default_infty<float>() > 0);
  assert(isinf(default_infty<double>()) && default_infty<double>() > 0);
  assert(isinf(default_infty<long double>()) && default_infty<long double>() > 0);
}

void test_graphs()
{
  const i128 large = i128(INF) + 100;
  vc<tuple<int, int, i128>> edges{{0, 1, large}, {1, 2, 3}};
  Graph<true, i128> graph_input(4, edges);
  ShortestPath graph(graph_input);
  auto dist = graph.dijkstra(0);
  assert(dist[2] == large + 3 && dist[3] == WIDE_INF);
  AllPairsShortestPath all(graph_input);
  const auto &matrix = all.floyd_warshall();
  assert(matrix[0][2] == large + 3 && matrix[0][3] == WIDE_INF);
  vc<tuple<int, int, i128>> cycle_edges{
      {0, 1, large}, {1, 2, 3}, {2, 0, 4}, {2, 3, 1}};
  Graph<false, i128> cycle_input(5, cycle_edges);
  MinimumCycle cycle(cycle_input);
  assert((cycle.edge_costs() == vc<i128>{large + 7, large + 7, large + 7, WIDE_INF}));
  assert((cycle.vertex_costs() == vc<i128>{large + 7, large + 7, large + 7, WIDE_INF, WIDE_INF}));

  using R = Rational<ll>;
  vc<tuple<int, int, R>> rational_edges{{0, 1, R(1, 2)}, {1, 2, R(2, 3)}};
  Graph<true, R> rational_graph_input(4, rational_edges);
  ShortestPath rational_graph(rational_graph_input);
  auto rational_dist = rational_graph.dijkstra(0);
  assert(rational_dist[2] == R(7, 6) && rational_dist[3].is_infinite());
  AllPairsShortestPath rational_all(rational_graph_input);
  const auto &rational_matrix = rational_all.floyd_warshall();
  assert(rational_matrix[0][2] == R(7, 6) && rational_matrix[0][3].is_infinite());
}

void test_cht_and_helpers()
{
  ConvexHullTrick<ll> cht;
  assert(cht.query(0).first == WIDE_INF && cht.query(0).second.b == INF);
  ConvexHullTrick<int> small;
  assert(small.query(0).first == INF && small.query(0).second.b == (1 << 30) - 1);
  small.add_line(1, 2);
  assert(small.query(3).first == 5);
  using R = Rational<ll>;
  ConvexHullTrick<R> rational;
  assert(rational.query(R(0)).first.is_infinite());
  rational.add_line(R(2), R(3));
  auto value = rational.query(R(1, 2)).first;
  assert(value.num == 4 * value.den);
  ConvexHullTrickDotProduct<ll> dot;
  assert(dot.min_query(0, 1).first == WIDE_INF);
  assert(dot.max_query(0, 1).first == -WIDE_INF);

  assert(vecget(vc<int>{1}, -1) == -((1 << 30) - 1));
  assert(vecget(vc<i128>{1}, 1) == WIDE_INF);
  assert(vecget(vc<R>{R(1)}, 1).is_infinite());
  const i128 trillion = 1'000'000'000'000LL;
  assert(pow_limited<i128>(trillion, 3) == trillion * trillion * trillion);
  assert(mul_limited<i128>(i128(INF), i128(INF)) == WIDE_INF);
  assert(mul_limited<int>(1'000'000, 1'000'000) == (1 << 30) - 1);
  assert(pow_limited<int>(10, 10) == (1 << 30) - 1);
}

int main()
{
  test_rational_and_providers();
  test_graphs();
  test_cht_and_helpers();
  cout << "Hello World" << endl;
}
