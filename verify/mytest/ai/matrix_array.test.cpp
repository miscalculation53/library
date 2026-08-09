#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix_array.hpp"

// Test focus: rectangular fixed-size multiplication and in-place subtraction.
int main()
{
  using F = FieldAddSubMulDiv<ll>;
  array<array<ll, 3>, 2> av{{{{1, 2, 3}}, {{4, 5, 6}}}};
  array<array<ll, 2>, 3> bv{{{{1, 2}}, {{3, 4}}, {{5, 6}}}};
  MatrixArray<F, 2, 3> a(av);
  MatrixArray<F, 3, 2> b(bv);
  auto c = a * b;
  assert(c[0][0] == 22 && c[0][1] == 28);
  assert(c[1][0] == 49 && c[1][1] == 64);
  MatrixArray<F, 2, 3> d(av);
  d -= a;
  repi(i, 2) repi(j, 3) assert(d[i][j] == 0);
  PRINT("Hello World");
}
