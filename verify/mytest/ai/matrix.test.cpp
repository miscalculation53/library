#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix.hpp"

// Test focus: subtraction and determinant also work over a floating-point field.
int main()
{
  using F = FieldAddSubMulDiv<double>;
  Matrix<F> a(vvc<double>{{3, 1}, {2, 4}}), b(vvc<double>{{1, 2}, {3, 4}});
  a -= b;
  assert(a == vvc<double>({{2, -1}, {-1, 0}}));
  assert(Matrix<F>(vvc<double>{{3, 1}, {2, 4}}).det() == 10);
  PRINT("Hello World");
}
