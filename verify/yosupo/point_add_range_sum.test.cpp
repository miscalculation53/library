#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

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
#include "ds/fenwick_tree.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);

  FenwickTree<GroupAddSub<ll>> fw(A);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, x);
      fw.add(p, x);
    }
    else if (t == 1)
    {
      LL(l, r);
      PRINT(fw.sum(l, r));
    }
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
