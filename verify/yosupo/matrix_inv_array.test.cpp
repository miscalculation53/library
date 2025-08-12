#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"

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
#include "math/linalg/matrix_array.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC2(mint, N, N, A_);
  const int MX = local_oj(4, 500);
  MatrixArray<FieldAddSubMulDiv<mint>, MX, MX> A(1, 0);
  rep(i, N) rep(j, N) A.at(i).at(j) = A_.at(i).at(j);
  auto [ok, B] = A.inv();
  if (!ok)
    PRINT(-1);
  else
  {
    rep(i, N)
    {
      rep(j, N) WRITE(B.at(i).at(j), " ");
      WRITE("\n");
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
