#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"

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

#include "ds/segtree/segtree_beats.hpp"
#include "algebra/acted_monoid/chmin_chmax_add_min_max_sum.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  INT(N, Q);
  VEC(ll, N, A);
  using AM = ActedMonoidChminChmaxAddMinMaxSum<ll>;
  using F = AM::F;
  SegmentTreeBeats<AM> seg(A);
  rep(_, Q)
  {
    INT(t, l, r);
    if (t == 0)
    {
      LL(x);
      seg.apply(l, r, F::chmin(x));
    }
    else if (t == 1)
    {
      LL(x);
      seg.apply(l, r, F::chmax(x));
    }
    else if (t == 2)
    {
      LL(x);
      seg.apply(l, r, F::add(x));
    }
    else
      PRINT(seg.prod(l, r).sum);
  }
}

void test()
{
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
