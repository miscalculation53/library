#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/rational_sum.hpp"

// Test focus: the empty sum is represented canonically as 0 / 1.
int main()
{
  using mint = modint998244353;
  using F = FormalPowerSeries<mint>;
  auto [p, q] = rational_sum(vc<pair<F, F>>{});
  assert(p.empty());
  assert(q == F{1});
  PRINT("Hello World");
}
