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

#include "graph/tree/rooted_tree.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  VEC(pll, N - 1, UV);
  RootedTree G(N, UV, randrange(0, N));
  FenwickTree<GroupAddSub<ll>> fw(G.reordered_vertex_info(A));
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, x);
      fw.add(G.preorder(p), x);
    }
    else if (t == 1)
    {
      LL(u, v);
      ll sm = 0;
      G.path_query(u, v, [&](ll l, ll r, bool)
                   { sm += fw.sum(l, r); });
      PRINT(sm);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
