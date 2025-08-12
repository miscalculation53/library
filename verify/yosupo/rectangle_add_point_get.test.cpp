#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_add_point_get"

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
#include "ds/2d/offline/rectangle_add_point_get.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  RectangleAddPointGet<GroupAddSub<ll>, ll> rs;
  rep(_, N)
  {
    LL(lx, ly, rx, ry, w);
    rs.rectangle_add(lx, rx, ly, ry, w);
  }
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(lx, ly, rx, ry, w);
      rs.rectangle_add(lx, rx, ly, ry, w);
    }
    else if (t == 1)
    {
      LL(x, y);
      rs.point_get(x, y);
    }
  }
  PRINTV(rs.run());
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
