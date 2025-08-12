#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_B&lang=ja"

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

#include "graph/sssp.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, M, S);
  VEC(tlll, M, UVW);
  ShortestPath<true, ll> G(N, UVW);
  auto dists = G.solve(S);
  if (MIN(dists) == -INF)
    PRINTRETURN("NEGATIVE CYCLE");
  rep(v, N)
  {
    if (dists.at(v) == INF)
      PRINT("INF");
    else
      PRINT(dists.at(v));
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
