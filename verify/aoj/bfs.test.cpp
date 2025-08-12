#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_C&lang=ja"

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
  LL(N);
  vpll UV;
  rep(_, N)
  {
    LL(u, k);
    VEC(ll, k, v);
    u--;
    offset(v, -1);
    fec(vi : v) UV.eb(u, vi);
  }

  ShortestPath<true, ll> G(N, UV);
  auto dists = G.solve(0);
  rep(i, N) PRINT(i + 1, dists.at(i) == INF ? -1 : dists.at(i));
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
