#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/modint/modint_div.hpp"
#include "math/modint/modint.hpp"

// Test focus: equality compares the represented quotient, not just one component.
int main()
{
  using D = ModintDiv<modint998244353>;
  assert(D(2, 3) == D(2, 3));
  assert(D(2, 3) != D(2, 4));
  PRINT("Hello World");
}
