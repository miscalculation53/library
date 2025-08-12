#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_G"

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
#include "math/algebra/acted_monoid/add_sum.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, Q);
  vl vec(N, 0);
  LazySegmentTree<ActedMonoidAddSum<ll>> seg(ALL(vec));
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(l, r, x);
      l--;
      seg.apply(l, r, x);
    }
    else if (t == 1)
    {
      LL(l, r);
      l--;
      PRINT(seg.prod(l, r).val);
    }
    dump(seg.content());
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
