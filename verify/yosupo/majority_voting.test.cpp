#define PROBLEM "https://judge.yosupo.jp/problem/majority_voting"

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

#include "ds/segtree/segtree.hpp"
#include "math/algebra/majority_vote.hpp"
#include "ds/query/point_set_range_frequency.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  VEC(tlll, Q, qs);

  SegmentTree<MajorityVote<ll>> seg(ALL(A));
  vl ans, len;
  fec([t, q1, q2] : qs)
  {
    if (t == 0)
    {
      ll p = q1, v = q2;
      seg.set(p, v);
    }
    else if (t == 1)
    {
      ll l = q1, r = q2;
      ans.eb(seg.prod(l, r).cand);
      len.eb(r - l);
    }
  }

  PointSetRangeFrequency rf(A);
  {
    ll qi = 0;
    fec([ t, q1, q2 ] : qs)
    {
      if (t == 0)
      {
        ll p = q1, v = q2;
        rf.point_set(p, v);
      }
      else if (t == 1)
      {
        ll l = q1, r = q2;
        ll x = ans.at(qi++);
        rf.range_frequency(l, r, x);
      }
    }
  }
  auto res = rf.run();
  rep(qi, res.size()) if (res.at(qi) * 2 <= len.at(qi)) ans.at(qi) = -1;
  PRINTV(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
