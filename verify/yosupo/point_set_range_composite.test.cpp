#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

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
#include "ds/segtree/segtree.hpp"
#include "algebra/affine_function.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(pll, N, AB);
  UNZIP(AB, A, B);

  SegmentTree<OppositeMonoid<GroupAffineFunction<mint>>> seg(gen_vec(N, [&](ll i)
                                                                     { return GroupAffineFunction<mint>::S{A.at(i), B.at(i)}; }));
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, c, d);
      seg.set(p, {c, d});
    }
    else if (t == 1)
    {
      LL(l, r, x);
      auto [a, b] = seg.prod(l, r);
      PRINT(a * x + b);
    }
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
