#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/multipoint_evaluation_geo.hpp"

// Test focus: an empty polynomial evaluates to zero at every geometric point.
int main()
{
  using mint = modint998244353;
  assert(multipoint_evaluation_geo(FormalPowerSeries<mint>{}, 3, mint(2), mint(3)) == vc<mint>(3, 0));
  PRINT("Hello World");
}
