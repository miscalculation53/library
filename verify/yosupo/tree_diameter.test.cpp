#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"

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

#include "graph/tree/diameter.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC(tlll, N - 1, ABC);
  RootedTree<ll> G(N, ABC, 0);
  
  auto [a, b, ga] = tree_diameter_by_cost(G);
  auto path = G.path(b, a);
  auto path2 = ga.path_to_root(b);
  dump(path, path2);
  assert(path == path2);
  vl ans = concat(vl{b}, vl(ALL(path)));
  PRINT(ga.dist(b), SZ(ans));
  PRINT(ans);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
