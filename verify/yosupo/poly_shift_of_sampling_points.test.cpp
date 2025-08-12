#define PROBLEM "https://judge.yosupo.jp/problem/shift_of_sampling_points_of_polynomial"

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
#include "math/fps/shift_of_sampling_points.hpp"

void init() {}

void main2()
{
  LL(N, M);
  IN(mint, c);
  VEC(mint, N, y);
  auto z = shift_of_sampling_points_many(y, c, M);
  PRINT(z);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
