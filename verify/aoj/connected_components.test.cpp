#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_D"

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

#include "graph/connected_components.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, M);
  VEC(pll, M, AB);
  Graph<ll> G(N, AB, false);
  dump(G.edges(), G.adj_list());
  auto cid = connected_component_ids(G);
  dump(cid);
  LL(Q);
  rep(_, Q)
  {
    LL(s,t);
    PRINT(cid.at(s) == cid.at(t) ? "yes" : "no");
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
