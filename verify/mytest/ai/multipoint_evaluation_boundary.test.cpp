#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/multipoint_evaluation.hpp"

// Test focus: zero evaluation points and the single-point product-tree boundary.
int main()
{
  using mint = modint998244353;
  using F = FormalPowerSeries<mint>;
  assert(multipoint_evaluation(F{1, 2}, {}) == vc<mint>{});
  assert(multipoint_evaluation(F{1, 2}, {3}) == vc<mint>{7});
  PRINT("Hello World");
}
