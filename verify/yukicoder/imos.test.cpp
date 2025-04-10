#define PROBLEM "https://yukicoder.me/problems/no/2154"

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

#include "ds/cumulative_sum/imos.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(pll, M, LR);
  offset(LR, pll{0, -1});
  Imos imos(N);
  fec([ l, r ] : LR) imos.add(r, l, 1);
  PRINTV(reversed(imos.content()));
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
