#define PROBLEM "https://yukicoder.me/problems/no/187"

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

#include "math/crt.hpp"
#include "math/modint/modint.hpp"
using mint = modint1000000007;

void init() {}

void main2()
{
  LL(N);
  VEC(pll, N, XY);
  auto [X, Y] = top(XY);
  vc<int> X_(ALL(X)), Y_(ALL(Y));
  bool ok = pre_crt(X, Y);
  bool ok2 = pre_crt(X_, Y_);
  assert(ok == ok2);
  if (!ok)
    PRINTRETURN(-1);
  auto res = crt_mod<mint>(X, Y);
  auto [r, m] = res;
  auto res2 = crt_mod_dynamic_32<mint>(X, Y);
  assert(res == res2);
  if (MAX(X) == 0)
    PRINT(m);
  else
    PRINT(r);
  rep(i, N)
  {
    X.at(i) += randint(-ipow(10, 9), ipow(10, 9)) * Y.at(i);
  }
  auto res3 = crt_mod<mint>(X, Y);
  auto res4 = crt_mod_dynamic_32<mint>(X, Y);
  auto res5 = crt_mod<mint>(X_, Y_);
  auto res6 = crt_mod_dynamic_32<mint>(X_, Y_);
  assert(res == res3);
  assert(res == res4);
  assert(res == res5);
  assert(res == res6);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
