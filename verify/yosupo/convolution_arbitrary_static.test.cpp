#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

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
using mint = modint1000000007;

#include "math/convolution/convolution.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(mint, N, A);
  VEC(mint, M, B);
  auto C = convolution(A, B);
  PRINT(C);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
