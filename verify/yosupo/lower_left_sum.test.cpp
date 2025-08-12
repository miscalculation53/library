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

  LowerLeftSum<GroupAddSub<ll>, ll> rs;
  fec([ x, y, w ] : XYW) rs.point_add(x, y, w);
  fec([ lx, ly, rx, ry ] : LXLYRXRY)
  {
    rs.rectangle_sum(lx, ly);
    rs.rectangle_sum(lx, ry);
    rs.rectangle_sum(rx, ly);
    rs.rectangle_sum(rx, ry);
  }
  auto res = rs.run();
  dump(res);
  rep(q, Q)
  {
    ll ans = res.at(4 * q) - res.at(4 * q + 1) - res.at(4 * q + 2) + res.at(4 * q + 3);
    PRINT(ans);
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
