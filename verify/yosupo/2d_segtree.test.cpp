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
#include "ds/2d/sparse/segtree.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(tlll, N, XYW);
  vtlll pts = XYW;
  vc<tuple<ll, ll, ll, ll, ll>> qs;
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(x, y, w);
      pts.eb(x, y, 0);
      qs.eb(0, x, y, w, -1);
    }
    else if (t == 1)
    {
      LL(lx, ly, rx, ry);
      qs.eb(1, lx, rx, ly, ry);
    }
  }

  SegmentTree2DSparse<MonoidAdd<ll>, ll> seg(pts);
  rep(qid, Q)
  {
    cauto &q = qs[qid];
    ll t = get<0>(q);
    if (t == 0)
    {
      auto [_, x, y, w, __] = q;
      seg.modify(qid, [&](ll &val)
                 { val += w; });
    }
    else if (t == 1)
    {
      auto [_, lx, rx, ly, ry] = q;
      PRINT(seg.sum(lx, rx, ly, ry));
    }
    dump(seg.content());
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
