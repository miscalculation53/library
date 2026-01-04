#define PROBLEM "https://judge.yosupo.jp/problem/many_factorials"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/fps/factorial.hpp"
using mint = modint998244353;

Factorial<mint> fac;

void init() {}

void main2()
{
  LL(N);
  PRINT(fac.factorial(N));
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
