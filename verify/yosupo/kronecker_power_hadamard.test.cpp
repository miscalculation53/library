#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"

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

#include "math/set/kronecker_power.hpp"

void hadamard(vc<mint> &A)
{
  kronecker_power_array_destructive<RingAddSubMul<mint>, 2>({{{1, 1}, {1, -1}}}, A);
}

void init() {}

void main2()
{
  LL(N);
  VEC(mint, 1 << N, A, B);
  hadamard(A), hadamard(B);
  dump(A, B);
  rep(i, 1 << N) A.at(i) *= B.at(i);
  hadamard(A);
  mint div = mint(1 << N).inv();
  rep(i, 1 << N) A.at(i) *= div;
  PRINT(A);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
