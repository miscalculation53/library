#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_2_64"

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

#include "math/convolution/convolution64.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(ull, N, A);
  VEC(ull, M, B);
  auto C = convolution64(A, B);
  PRINT(C);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
