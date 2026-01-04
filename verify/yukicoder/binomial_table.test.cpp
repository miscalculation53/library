#define PROBLEM "https://yukicoder.me/problems/no/129"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/modint/modint.hpp"
// using mint = modint998244353;
// using mint = modint1000000007;
using mint = static_modint<1000000000>;
// using mint = modint;
#include "math/modint/binomial_table.hpp"
using bi = BinomialTable<mint>;

void init() {}

void main2()
{
  LL(N, M);
  ll x = N / (1000 * M);
  N -= 1000 * M * x;
  N = N / 1000;
  PRINT(bi::C(M, min(M, N)));
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
