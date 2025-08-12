#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection_undirected"

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

#include "graph/cycle_detection.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(pll, M, UV);
  GraphUndirected<ll> G(N, UV);
  auto [ok, cycle] = cycle_detection(G);
  if (!ok)
    PRINTRETURN(-1);
  ll L = cycle.size();
  PRINT(L);
  vl resv(L), rese(L);
  rep(i, L)
  {
    resv.at(i) = cycle.at(i).from;
    rese.at(i) = cycle.at(i).index;
  }
  PRINT(resv);
  PRINT(rese);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
