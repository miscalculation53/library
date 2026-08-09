#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"

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

#include "ds/segtree/dual_segtree.hpp"
#include "math/algebra/acted_monoid/affine_sum.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, Q);
  VEC(mint, N, A);
  DualSegmentTree<ActedMonoidAffineSum<mint>> seg(ALL(A));
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
      LL(i);
      PRINT(seg.get(i).val);
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
