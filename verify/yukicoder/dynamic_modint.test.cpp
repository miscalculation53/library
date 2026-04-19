#define PROBLEM "https://yukicoder.me/problems/no/1550"

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
// using mint = static_modint32<1000000000>;
using mint = modint;

void init() {}

void main2()
{
  mint::set_mod(1000000007);
  IN(mint, N);
  PRINT(N);
  dump(N);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
