#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product_mod_2"

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
  LL(N, M, K);
  VEC(string, N, A_);
  VEC(string, M, B_);
  const int MAX = local_oj(4, 4096);
  MatrixMod2<MAX, MAX> A, B;
  rep(i, N) rep(j, M) A(i, j) = A_[i][j] == '1';
  rep(i, M) rep(j, K) B(i, j) = B_[i][j] == '1';
  auto C = A * B;
  rep(i, N) { rep(j, K) WRITE(char('0' + C(i, j))); WRITE("\n"); }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
