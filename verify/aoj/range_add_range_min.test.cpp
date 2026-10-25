#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_H"

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

#include "ds/segtree/lazy_segtree.hpp"
#include "algebra/acted_monoid/add_min.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, Q);
  LazySegmentTree<ActedMonoidAddMin<ll>> seg(vl(N, 0));
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(l, r, x);
      r++;
      seg.apply(l, r, x);
    }
    else if (t == 1)
    {
      LL(l, r);
      r++;
      PRINT(seg.prod(l, r));
    }
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
