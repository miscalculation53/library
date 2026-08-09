#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/pow_sum.hpp"

// Test focus: requesting zero coefficients returns an empty sequence.
int main()
{
  using mint = modint998244353;
  assert(pow_sum(vc<mint>{1, 2}, 0).empty());
  PRINT("Hello World");
}
