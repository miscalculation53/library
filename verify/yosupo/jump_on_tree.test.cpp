#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"

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

#include "graph/tree/rooted_tree.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(pll, N - 1, AB);
  RootedTree G(N, AB, randrange(0, N));
  rep(_, Q)
  {
    LL(s, t, i);
    PRINT(G.jump(s, t, i));
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
