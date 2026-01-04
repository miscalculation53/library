#define PROBLEM "https://yukicoder.me/problems/no/2045"

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
#include "math/modint/modint.hpp"
using mint = modint998244353;

#include "algo/perm_cycle.hpp"
#include "math/prime/sieve/linear_sieve.hpp"

void init() {}

void main2()
{
  LL(N, x, y);
  if (x == 1 && y == 1)
    PRINTRETURN(1);
  if (x == 1 || y == 1)
    PRINTRETURN(2);
  auto P = permid(N), Q = permid(N);
  reverse(P.begin(), P.begin() + x);
  reverse(Q.end() - y, Q.end());
  auto R = permuted(P, Q);
  auto C = PermCycle(R).cycles;
  vl E(N + 1, 0);
  fec(c : C)
  {
    auto fac = LinearSieve::factorize(c.size());
    fec(pp : fac) chmax(E.at(pp.p), pp.e);
  }
  mint ans = 2;
  rep(p, N + 1) ans *= mint(p).pow(E.at(p));
  PRINT(ans);
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
