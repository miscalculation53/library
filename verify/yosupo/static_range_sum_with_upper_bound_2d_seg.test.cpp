#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum_with_upper_bound"

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

void init() {}

#include "ds/cumulative_sum/cumulative_sum.hpp"
#include "ds/2d/sparse/segtree.hpp"

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);

  vc<tuple<ll, ll, pll>> XYW(N);
  rep(i, N)
  {
    XYW.at(i) = {i, A.at(i), {1, A.at(i)}};
  }

  SegmentTree2DSparse<GroupAddSub<pll>, ll, CumulativeSum> seg(XYW);
  rep(_, Q)
  {
    LL(l, r, x);
    auto [c, s] = seg.sum(l, r, 0, x + 1);
    PRINT(c, s);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
