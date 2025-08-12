#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2945"

// #define SINGLE_TESTCASE
// #define MULTI_TESTCASE
#define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "graph/sssp.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N);
  if (N == 0)
    exit(0);
  LL(A, B, C, D);
  A--, B--, C--, D--;
  VEC(pll, N + 1, XY);
  offset(XY, pll{-1, -1});
  auto [X, Y] = top(XY);

  ll M = max({A, B, C, D, MAX(X), MAX(Y)}) + 1;
  vtlll UVW;
  rep(i, M) rep(j, M)
  {
    if (i != M - 1)
    {
      ll u = i * M + j;
      ll v = (i + 1) * M + j;
      UVW.eb(u, v, A <= i + 1 && i + 1 <= C && B <= j && j <= D ? 0 : 1);
      UVW.eb(v, u, A <= i && i <= C && B <= j && j <= D ? 0 : 1);
    }
    if (j != M - 1)
    {
      ll u = i * M + j;
      ll v = i * M + (j + 1);
      UVW.eb(u, v, A <= i && i <= C && B <= j + 1 && j + 1 <= D ? 0 : 1);
      UVW.eb(v, u, A <= i && i <= C && B <= j && j <= D ? 0 : 1);
    }
  }
  ShortestPath<true, ll> G(M * M, UVW);

  ll ans = 0;
  rep(i, N)
  {
    ll s = X.at(i) * M + Y.at(i);
    ll t = X.at(i + 1) * M + Y.at(i + 1);
    ll d = G.solve(s, t).at(t);
    ans += d;
  }
  PRINT(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
