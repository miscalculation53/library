#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"

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
  LL(N, M);
  VEC2(mint, N, M, A_);
  VEC(mint, N, b);
  Matrix<FieldAddSubMulDiv<mint>> A(A_);
  auto [ok, sol, basis] = A.solve(b);
  if (!ok)
    PRINTRETURN(-1);
  PRINT(basis.size());
  PRINT(sol);
  PRINTV(basis);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
