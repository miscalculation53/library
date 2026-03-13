#define PROBLEM "https://yukicoder.me/problems/no/2338"

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

void init()
{
  oj(mt.seed(random_device()()));
}

#include "ds/group_index.hpp"
#include "ds/2d/offline/static_rectangle_add_point_get.hpp"

void main2()
{
  LL(N, M, Q);
  using ps = pair<ll, string>;
  VEC(ps, N, PS);
  auto [P, S] = top(PS);
  VEC(pll, Q, LR);
  offset(LR, pll{-1, 0});

  vl X(N);
  rep(i, N) X[i] = 2 * P[i] + (S[i] == "AC");
  GroupIndex grp(X);
  StaticRectangleAddPointGet<GroupAddSub<ll>, ll> AC, WA;
  rep(i, N)
  {
    ll prvAC = grp.lt_max(2 * P[i] + 1, i);
    ll nxtAC = grp.gt_min(2 * P[i] + 1, i);
    dump(i, prvAC, nxtAC);
    if (S[i] == "AC")
      AC.rectangle_add(prvAC + 1, i + 1, i + 1, N + 1, 1);
    if (S[i] == "WA")
      WA.rectangle_add(prvAC + 1, i + 1, nxtAC + 1, N + 1, 1);
  }

  fec([ l, r ] : LR) AC.point_get(l, r),
  WA.point_get(l, r);
  auto ansAC = AC.run(), ansWA = WA.run();
  rep(q, Q) PRINT(ansAC[q], ansWA[q]);
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
