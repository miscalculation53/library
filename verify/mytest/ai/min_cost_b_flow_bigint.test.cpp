#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/min_cost_b_flow.hpp"
#include "math/bigint.hpp"

int main()
{
  const ll hi = numeric_limits<ll>::max();
  using Big = BigInteger<>;
  MinCostBFlow<ll, ll, Big> g(1);
  for (int i = 0; i < 3; i++) g.add_edge(0, 0, hi, hi, hi);
  assert(g.flow().value() == Big(hi) * hi * 3);
  MinCostBFlow<ll, i128, Big> h(4);
  h.add_edge(0, 1, hi, hi), h.add_edge(1, 2, hi, hi), h.add_edge(2, 3, hi, hi);
  assert(h.flow().has_value());
  const auto slope = h.slope(0, 3, hi);
  assert(slope.back().first == hi && slope.back().second == Big(hi) * hi * 3);
  MinCostBFlow<ll, i128, Big> interval(4);
  interval.set_supply(0, 1, hi), interval.set_supply(3, -hi, -1);
  interval.add_edge(0, 1, hi, -i128(hi));
  interval.add_edge(1, 2, hi, -i128(hi));
  interval.add_edge(2, 3, hi, -i128(hi));
  assert(interval.flow().value() == -Big(hi) * hi * 3);
  assert(interval.get_supply(0) == hi && interval.get_supply(3) == -hi);
  assert(interval.flow(decltype(interval)::Algorithm::network_simplex).value() == -Big(hi) * hi * 3);
  assert(h.flow(decltype(h)::Algorithm::network_simplex).value() == Big(hi) * hi * 3);
  assert(h.flow(3, 0, hi).second == -Big(hi) * hi * 3);
  cout << "Hello World\n";
}
