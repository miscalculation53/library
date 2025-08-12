#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"

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
  LL(N, M);
  VEC2(mint, N, M, A_);
  VEC(mint, N, b_);
  const int MX = local_oj(4, 500);
  MatrixArray<FieldAddSubMulDiv<mint>, MX, MX> A;
  rep(i, N) rep(j, M) A.at(i).at(j) = A_.at(i).at(j);
  array<mint, MX> b{};
  rep(i, N) b.at(i) = b_.at(i);
  auto [ok, sol, basis] = A.solve(b);
  if (!ok)
    PRINTRETURN(-1);
  dump(ok, sol, basis);
  while (!basis.empty())
  {
    bool popped = false;
    rep(j, M, MX)
    {
      if (basis.back().at(j) != 0)
      {
        basis.pop_back();
        popped = true;
        break;
      }
    }
    if (!popped)
      break;
  }
  dump(ok, sol, basis);
  PRINT(basis.size());
  rep(j, M) WRITE(sol.at(j), " ");
  WRITE("\n");
  fec(b : basis)
  {
    rep(j, M) WRITE(b.at(j), " ");
    WRITE("\n");
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
