#define PROBLEM "https://judge.yosupo.jp/problem/scc"

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

#include "graph/scc.hpp"
#include "ds/group_index.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(pll, M, UV);
  GraphDirected<ll> G(N, UV);
  auto ids = scc(G);
  dump(ids);
  GroupIndex grp(ids);
  PRINT(MAX(ids) + 1);
  rep(i, MAX(ids) + 1)
  {
    auto v = grp.idxs(i).to_v();
    PRINT(v.size(), v);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
