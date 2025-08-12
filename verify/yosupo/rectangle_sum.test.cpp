#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"

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
#include "ds/2d/offline/rectangle_sum.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(tlll, N, XYW);
  VEC(tllll, Q, LXLYRXRY);

  RectangleSum<GroupAddSub<ll>, ll> rs;
  fec([ x, y, w ] : XYW) rs.point_add(x, y, w);
  fec([ lx, ly, rx, ry ] : LXLYRXRY) rs.rectangle_sum(lx, rx, ly, ry);
  auto res = rs.run();
  PRINTV(res);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
