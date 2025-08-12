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
  vl X;
  vc<tuple<ll, ll, ll, ll, ll>> qs;
  fec([ x, y, w ] : XYW)
  {
    qs.eb(0, x, y, w, -1);
    X.eb(x);
  }
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(x, y, w);
      qs.eb(0, x, y, w, -1);
      X.eb(x);
    }
    else if (t == 1)
    {
      LL(lx, ly, rx, ry);
      qs.eb(1, lx, rx, ly, ry);
    }
  }
  sortunique(X);
  vpll IY;
  fem(q : qs)
  {
    ll t = get<0>(q);
    if (t == 0)
    {
      auto &[_, x, y, w, __] = q;
      IY.eb(LB(X, x), y);
    }
  }

  FenwickTree2DSparse<GroupAddSub<ll>, ll> fw(IY);
  fec(q : qs)
  {
    dump(fw.content());
    ll t = get<0>(q);
    if (t == 0)
    {
      auto [_, x, y, w, __] = q;
      fw.add(LB(X, x), y, w);
    }
    else if (t == 1)
    {
      auto [_, lx, rx, ly, ry] = q;
      PRINT(fw.sum(LB(X, lx), LB(X, rx), ly, ry));
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
