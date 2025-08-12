#define PROBLEM "https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence"

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
#include "math/fps/bmbm.hpp"
using mint = modint998244353;
using fps = FormalPowerSeries<mint>;

void init() {}

void main2()
{
  LL(d, k);
  VEC(mint, d, a);
  VEC(mint, d, c);
  c.insert(c.begin(), 0);
  PRINT(linear_recurrence(a, c, k));
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
