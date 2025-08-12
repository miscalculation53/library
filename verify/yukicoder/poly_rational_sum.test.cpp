#define PROBLEM "https://yukicoder.me/problems/no/1857"

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
#include "math/fps/fps.hpp"
using mint = modint998244353;
using bi = Binomial<mint>;
using fps = FormalPowerSeries<mint>;
#include "math/fps/rational_sum.hpp"

void init() {}

void main2()
{
  LL(N, S);
  VEC(mint, N, P);
  fem(p : P) p /= S;
  vc<pair<fps, fps>> fs(N);
  rep(i, N) fs.at(i) = {{0, P.at(i) * P.at(i)}, {1, P.at(i)}};
  auto f = rational_sum(fs);
  dump(fs, f);
  mint ans = 0;
  rep(k, N + 1)
  {
    ans += (k + 1) * bi::fac(k) * f.first.at(k);
    dump(k, f.first.at(k), bi::fac(k) * f.first.at(k));
  }
  PRINT(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
