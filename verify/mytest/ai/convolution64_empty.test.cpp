#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/convolution/convolution64.hpp"

// Test focus: either empty operand produces an empty convolution.
int main()
{
  assert(convolution64({}, {}) == vc<ull>{});
  assert(convolution64({}, {1}) == vc<ull>{});
  assert(convolution64({1}, {}) == vc<ull>{});
  PRINT("Hello World");
}
