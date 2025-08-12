#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
using mint = modint998244353;
#include "math/linalg/matrix.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC2(mint, N, N, A_);
  Matrix<FieldAddSubMulDiv<mint>> A(A_);
  PRINT(A.det());
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
