#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank_mod_2"

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
#include "math/linalg/matrix_mod2.hpp"

void init() {}

void main2()
{
  LL(N, M);
  if (M == 0)
    PRINTRETURN(0);
  VEC(string, N, A_);
  const int MAX = local_oj(4, 4096);
  MatrixMod2<MAX, MAX> A;
  rep(i, N) rep(j, M) A(i, j) = A_[i][j] == '1';
  PRINT(A.rank());
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
