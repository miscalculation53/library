#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/convolution/convolution_ll.hpp"

// Test focus: either empty operand produces an empty signed convolution.
int main()
{
  assert(convolution_4e18({}, {}) == vc<ll>{});
  assert(convolution_4e18({}, {1}) == vc<ll>{});
  assert(convolution_4e18({1}, {}) == vc<ll>{});
  PRINT("Hello World");
}
