#define PROBLEM "https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum"

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
#include "algebra/algebra_basic_ops.hpp"
#include "ds/2d/offline/static_rectangle_add_rectangle_sum.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  StaticRectangleAddRectangleSum<GroupAddSub<mint>, ll> rs;
  rep(_, N)
  {
    LL(lx, ly, rx, ry, w);
    rs.rectangle_add(lx, rx, ly, ry, w);
  }
  rep(_, Q)
  {
    LL(lx, ly, rx, ry);
    rs.rectangle_sum(lx, rx, ly, ry);
  }
  auto ans = rs.run();
  PRINTV(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
