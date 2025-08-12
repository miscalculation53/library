#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"

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
#include "math/fps/berlekamp_massey.hpp"
#include "math/modint/modint.hpp"
using mint = modint998244353;

void init() {}

void main2()
{
  LL(N);
  VEC(mint, N, a);
  auto c = berlekamp_massey<FieldAddSubMulDiv<mint>>(a);
  c.erase(c.begin());
  PRINT(c.size());
  PRINT(c);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
