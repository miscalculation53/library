#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/dynamic_matrix_mod2.hpp"

// Test focus: runtime-sized rows support [] assignment, addition, and exponentiation.
int main()
{
  DynamicMatrixMod2 a(2, 2, 1);
  a[0][1] = true;
  assert(a[0][1]);
  assert(!a[1][0]);

  auto zero = a + a;
  assert(zero[0].none() && zero[1].none());

  auto cube = a.pow(3);
  assert(cube.to_vstr() == vc<string>({"11", "01"}));
  PRINT("Hello World");
}
