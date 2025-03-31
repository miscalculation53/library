#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

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
// using mint = modint998244353;
// using mint = modint1000000007;
// using mint = static_modint<1000000000>;
using mint = modint;
#include "math/modint/binomial.hpp"
using bi = Binomial<mint>;

void init() {}

void main2()
{
  LL(T, M);
  mint::set_mod(200003);
  bi::reserve(ipow(10, 3));
  mint::set_mod(M);
  bi::reserve(ipow(10, 6));
  rep(_, T)
  {
    LL(N, K);
    PRINT(bi::C(N, K));
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
