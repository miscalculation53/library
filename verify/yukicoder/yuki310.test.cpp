#define PROBLEM "https://yukicoder.me/problems/no/310"

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
using mint = modint1000000007;
#include "graph/matrix_tree.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, M);
  VEC(pll, M, AB);
  offset(AB, pll{-1, -1});
  vvc<ll> G(N, vc<ll>(N, 1));
  fec([ a, b ] : AB) G.at(a).at(b) = 0;
  PRINT(count_eularian_trails<mint>(G));
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
