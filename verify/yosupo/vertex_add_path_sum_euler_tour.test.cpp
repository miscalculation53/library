#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

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

#include "graph/tree/euler_tour_path_sum.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  VEC(pll, N - 1, UV);
  PathSum<GroupAddSub<ll>> G(N, UV, randrange(0, N), A);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, x);
      G.add(p, x);
    }
    else if (t == 1)
    {
      LL(u, v);
      PRINT(G.sum(u, v));
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
