#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series_sparse"

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
using fps = FormalPowerSeries<mint>;

void init() {}

void main2()
{
  LL(N, K);
  fps f(N);
  rep(_, K)
  {
    LL(i);
    IN(mint, a);
    f.at(i) = a;
  }
  fps g = f.exp();
  PRINT(g);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
