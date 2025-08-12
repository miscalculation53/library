#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

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

#include "ds/coordinate_compression.hpp"
#include "ds/mo.hpp"
#include "ds/fenwick_tree/inversion.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  VEC(pll, Q, LR);

  vpll vec = GEN_VEC(N, i, (pll{A.at(i), i}));
  A = compressed(vec);

  InversionSlider slider(A);
  vl ans(Q);
  auto rem = [&](ll q)
  { ans.at(q) = slider.inversion_num; };
  mo(N, LR, slider, rem);
  PRINTV(ans);
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
