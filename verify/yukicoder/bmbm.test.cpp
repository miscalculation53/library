#define PROBLEM "https://yukicoder.me/problems/no/215"

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
#include "math/fps/bmbm.hpp"
#include "ds/cumulative_sum/imos.hpp"
using mint = modint1000000007;
using fps = FormalPowerSeries<mint>;

// 出目が A のサイコロを K 個振るときの出目の和ごとの場合の数
vc<mint> f(const vl &A, ll K)
{
  ll N = A.size(), M = MAX(A);
  // dp(k, n, s) := k 個振って、直前に使ったのが n で、和が s
  auto dp = dvec({K + 1, N, M * K + 1}, mint(0));
  dp.at(0).at(0).at(0) = 1;
  rep(k, K) rep(n, N) rep(s, M * K + 1)
  {
    rep(nn, n, N)
    {
      ll ns = s + A.at(nn);
      if (ns <= M * K)
        dp.at(k + 1).at(nn).at(ns) += dp.at(k).at(n).at(s);
    }
  }
  vc<mint> res(M * K + 1);
  rep(n, N) rep(s, M * K + 1) res.at(s) += dp.at(K).at(n).at(s);
  return res;
}

void init() {}

void main2()
{
  LL(N, P, C);
  auto A = f({2, 3, 5, 7, 11, 13}, P);
  auto B = f({4, 6, 8, 9, 10, 12}, C);
  dump(A | cp::index(), B | cp::index());
  fps f = convolution(A, B);
  dump(f | cp::index());
  const ll MAX_N = 20000;
  fps g = (fps{1} - f).inv(MAX_N);
  // dump(g | cp::index());
  Imos<GroupAddSub<mint>> imos(MAX_N);
  rep(i, MAX_N) rep(j, SZ(f))
  {
    // i ではゴールでないが i+j はゴール: (i, i+j]
    imos.add(i + 1, min(MAX_N, i + j + 1), g.at(i) * f.at(j));
  }
  auto anss = imos.content();
  // dump(anss | cp::index());
  mint ans = bmbm(anss, N);
  PRINT(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
