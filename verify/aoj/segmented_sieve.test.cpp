#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2858"

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
#include "math/prime/sieve/segmented_sieve.hpp"

void init() {}

void main2()
{
  LL(L, R);
  vl cnt(R - L + 1, 0);
  vl val(R - L + 1);
  rep(x, L, R + 1) val.at(x - L) = x;
  segmented_sieve(L, R, [&](ll p, ll x)
                  {
                    ll &y = val.at(x - L);
                    while (y % p == 0)
                    {
                      y /= p;
                      cnt.at(x - L)++;
                    } 
                  });
  ll ans = 0;
  rep(x, L, R + 1)
  {
    if (val.at(x - L) != 1)
      cnt.at(x - L)++;
    if (LinearSieve::is_prime(cnt.at(x - L)))
      ans++;
  }
  rep(x, L, R + 1) dump(x, cnt.at(x - L));
  PRINT(ans);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
