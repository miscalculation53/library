#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"

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

#include "template/template_all.hpp"
using mint = modint998244353;

#include "math/set/subset_convolution.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC(mint, 1 << N, A, B);
  auto C = subset_convolution<RingAddSubMul<mint>>(A, B);
  PRINT(C);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
