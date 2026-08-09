#define PROBLEM "https://judge.yosupo.jp/problem/two_square_sum"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/two_square_sum.hpp"

void init() {}

void main2()
{
  LL(N);
  auto AB = two_square_sum(N);
  rep(i, SZ(AB)) AB.eb(AB[i].second, AB[i].first);
  sortunique(AB);
  PRINT(AB.size());
  PRINTV(AB);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
