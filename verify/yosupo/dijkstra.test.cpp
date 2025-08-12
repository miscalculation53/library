#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

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
  LL(N, M, s, t);
  VEC(tlll, M, ABC);
  ShortestPath<true, ll> G(N, ABC);
  auto dists = G.solve(s, t);
  auto path = G.path(t);
  if (dists.at(t) == INF)
    PRINTRETURN(-1);
  PRINT(dists.at(t), path.size());
  fec(e : path) PRINT(e.from, e.to);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
