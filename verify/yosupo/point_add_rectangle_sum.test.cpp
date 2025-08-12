#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"

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
#include "ds/2d/offline/point_add_rectangle_sum.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(tlll, N, XYW);
  PointAddRectangleSum<GroupAddSub<ll>, ll> rs;
  fec([ x, y, w ] : XYW) rs.point_add(x, y, w);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(x, y, w);
      rs.point_add(x, y, w);
    }
    else if (t == 1)
    {
      LL(lx, ly, rx, ry);
      rs.rectangle_sum(lx, rx, ly, ry);
    }
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
