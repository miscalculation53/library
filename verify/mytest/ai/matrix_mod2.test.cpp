#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix_mod2.hpp"

// Test focus: exponentiation uses the correct identity and multiplication over GF(2).
int main()
{
  MatrixMod2<4> a(2, 2, 1);
  auto b = a.pow(3);
  assert((b.to_vvi() == vvc<ll>{{1, 0}, {0, 1}}));
  PRINT("Hello World");
}
