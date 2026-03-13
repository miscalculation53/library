#define PROBLEM "https://yukicoder.me/problems/no/1145"

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
#include "math/fps/fps.hpp"
using mint = modint998244353;
using bi = Binomial<mint>;
using fps = FormalPowerSeries<mint>;
#include "math/fps/pow_sum.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(mint, N, A);
  auto ans = pow_sum(A, M + 1);
  ans.erase(ans.begin());
  PRINT(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
