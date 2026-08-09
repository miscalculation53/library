#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/convolution/convolution_many.hpp"

// Test focus: explicit truncation of one factor and an empty factor sequence.
int main()
{
  using mint = modint998244353;
  using F = vc<mint>;
  assert(convolution_many(vc<F>{F{1, 2, 3}}, 2) == F({1, 2}));
  assert(convolution_many(vc<F>{}, 0).empty());
  PRINT("Hello World");
}
