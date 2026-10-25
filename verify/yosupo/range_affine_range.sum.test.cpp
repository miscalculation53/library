#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

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

#include "math/modint/modint.hpp"
using mint = modint998244353;

#include "ds/segtree/lazy_segtree.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, Q);
  VEC(mint, N, A);
  LazySegmentTree<ActedMonoidAffineSum<mint>> seg(ALL(A));
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(l, r, b, c);
      seg.apply(l, r, {b, c});
    }
    else if (t == 1)
    {
      LL(l, r);
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
