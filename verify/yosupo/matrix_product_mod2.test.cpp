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
  MatrixMod2<4096> A(A_), B(B_);
  auto C = A * B;
  PRINTV(C.to_vstr());
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
