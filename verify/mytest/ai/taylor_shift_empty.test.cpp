#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/taylor_shift.hpp"

// Test focus: shifting the zero-length polynomial remains zero-length.
int main()
{
  using mint = modint998244353;
  assert(taylor_shift(FormalPowerSeries<mint>{}, mint(3)).empty());
  PRINT("Hello World");
}
