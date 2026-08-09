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
#include "ds/2d/sparse/fenwick_tree.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(tlll, N, XYW);
  vpll pts;
  vc<tuple<ll, ll, ll, ll, ll>> qs;
  fec([ x, y, w ] : XYW)
  {
    pts.eb(x, y);
    qs.eb(0, x, y, w, -1);
  }
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(x, y, w);
      pts.eb(x, y);
      qs.eb(0, x, y, w, -1);
    }
    else if (t == 1)
    {
      LL(lx, ly, rx, ry);
      pts.eb(0, 0);
      qs.eb(1, lx, rx, ly, ry);
    }
  }

  FenwickTree2DSparse<GroupAddSub<ll>, ll> fw(pts);
  rep(qid, N + Q)
  {
    dump(fw.content());
    cauto &q = qs[qid];
    ll t = get<0>(q);
    if (t == 0)
    {
      auto [_, x, y, w, __] = q;
      fw.add(qid, w);
    }
    else if (t == 1)
    {
      auto [_, lx, rx, ly, ry] = q;
      PRINT(fw.sum(lx, rx, ly, ry));
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
