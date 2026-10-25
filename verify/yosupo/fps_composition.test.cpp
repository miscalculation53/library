#define PROBLEM "https://judge.yosupo.jp/problem/composition_of_formal_power_series_large"

#define SINGLE_TESTCASE

#ifndef LOCAL
#define FAST_IO
#endif

#include "template/template_all_but_modint.hpp"
#include "math/fps/composition.hpp"
using mint = modint998244353;
using fps = FormalPowerSeries<mint>;

void init() {}

void main2()
{
  LL(N);
  VEC(mint, N, f);
  VEC(mint, N, g);
  PRINT(composition(fps(f), fps(g), N));
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
