#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix_mod_2"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
// #define FAST_IO
#define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/linalg/matrix_mod2.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC(string, N, A_);
  const int MAX = local_oj(4, 4096);
  MatrixMod2<MAX, MAX> A(1);
  rep(i, N) rep(j, N) A(i, j) = A_[i][j] == '1';
  const auto B = A.inv();
  if (!B)
    PRINT(-1);
  else
    rep(i, N) { rep(j, N) WRITE(char('0' + (*B)(i, j))); WRITE("\n"); }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
