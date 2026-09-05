#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "graph/sssp.hpp"
#include "math/bigint.hpp"
#include "math/rational.hpp"
#include "utils/resolved_value.hpp"

using BI = BigInteger<>;
using R = Rational<BI>;

int factory_calls = 0;

BI bigint_infty()
{
  factory_calls++;
  return BI("1000000000000000000000000000000");
}

// Test focus: direct constants remain constexpr while function providers construct a heavy value once.
void test_resolve()
{
  static_assert(resolved_value<ll, 123>() == 123);
  const BI &a = resolved_value<BI, bigint_infty>();
  const BI &b = resolved_value<BI, bigint_infty>();
  assert(a == BI("1000000000000000000000000000000"));
  assert(&a == &b && factory_calls == 1);
  assert((MonoidMin<BI, bigint_infty>::e() == a));
}

// Test focus: ShortestPath accepts a Rational<BigInteger> infinity provider and preserves unreachable vertices.
void test_shortest_path()
{
  vc<tuple<int, int, R>> edges{{0, 1, R(BI(1), BI(2))}, {1, 2, R(BI(2), BI(3))}};
  ShortestPath<true, R, numeric_limits<R>::infinity> graph(4, edges);
  auto dist = graph.dijkstra(0);
  assert(dist[0] == R(0));
  assert(dist[1] == R(BI(1), BI(2)));
  assert(dist[2] == R(BI(7), BI(6)));
  assert(dist[3] == numeric_limits<R>::infinity());
}

int main()
{
  test_resolve();
  test_shortest_path();
  cout << "Hello World" << endl;
}
