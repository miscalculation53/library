#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank_mod_2"

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
#include "math/linalg/matrix_mod2.hpp"

template <size_t MAX_M = 1>
void solve(ll N, size_t M, const vstr &A_)
{
  if (M > MAX_M)
  {
    solve<MAX_M * 2>(N, M, A_);
    return;
  }
  MatrixMod2<MAX_M> A(A_);
  PRINT(A.rank());
}

void init() {}

void main2()
{
  LL(N, M);
  if (M == 0)
    PRINTRETURN(0);
  VEC(string, N, A_);
  solve(N, M, A_);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
