#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum_large_array"

#define SINGLE_TESTCASE

#ifndef LOCAL
#define FAST_IO
#endif

#include "template/template_all_but_modint.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;

#include "ds/segtree/sparse_lazy_segtree.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, Q);
  using AM = ActedMonoidAffineSum<mint>;
  SparseLazySegmentTree<AM> seg(N, AM::S(0));
  seg.reserve(1 << 20);
  rep(_, Q)
  {
    LL(t, l, r);
    if (t == 0)
    {
      LL(b, c);
      seg.apply(l, r, {b, c});
    }
    else
      PRINT(seg.prod(l, r).val);
  }
}

void test()
{
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
