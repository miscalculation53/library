#define PROBLEM "https://yukicoder.me/problems/no/60"

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

#include "ds/cumulative_sum/imos_2d.hpp"

void init() {}

void main2()
{
  LL(N, K);
  VEC(tlll, N, XYHP);
  const ll M = 500;
  offset(XYHP, tlll{M, M, 0});
  Imos2D imos(2 * M + 2, 2 * M + 2);
  fec([ x, y, hp ] : XYHP) imos.add(x, x + 1, y, y + 1, hp);
  rep(_, K)
  {
    LL(ax, ay, w, h, d);
    ax += M, ay += M;
    imos.add(ax, min(2 * M + 2, ax + w + 1), ay, min(2 * M + 2, ay + h + 1), -d);
  }
  auto res = imos.content();
  ll ans = 0;
  rep(x, 2 * M + 2) rep(y, 2 * M + 2)
  {
    if (res.at(x).at(y) > 0)
      ans += res.at(x).at(y);
  }
  PRINT(ans);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
