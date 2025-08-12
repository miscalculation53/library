#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"

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
  LL(N, M, K);
  VEC2(mint, N, M, A_);
  VEC2(mint, M, K, B_);
  const int MX = local_oj(4, 1024);
  MatrixArray<FieldAddSubMulDiv<mint>, MX, MX> A, B;
  rep(i, N) rep(j, M) A.at(i).at(j) = A_.at(i).at(j);
  rep(i, M) rep(j, K) B.at(i).at(j) = B_.at(i).at(j);
  auto C = A * B;
  dump(A, B, C);
  rep(i, N)
  {
    rep(j, K) WRITE(C.at(i).at(j), " ");
    WRITE("\n");
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
