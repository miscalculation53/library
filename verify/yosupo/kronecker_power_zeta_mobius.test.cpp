#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_and_convolution"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;

#include "math/set/kronecker_power.hpp"

void zeta(vc<mint> &A)
{
  kronecker_power_destructive<RingAddSubMul<mint>, 2>({{{1, 0}, {1, 1}}}, A);
}
void mobius(vc<mint> &A)
{
  kronecker_power_destructive<RingAddSubMul<mint>, 2>({{{1, 0}, {-1, 1}}}, A);
}

void init() {}

void main2()
{
  LL(N);
  VEC(mint, 1 << N, A, B);
  reverse(ALL(A)), reverse(ALL(B));
  zeta(A), zeta(B);
  dump(A, B);
  rep(i, 1 << N) A.at(i) *= B.at(i);
  mobius(A);
  reverse(ALL(A));
  PRINT(A);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
