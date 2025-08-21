#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations_mod_2"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
// #define FAST_IO
#define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/linalg/matrix_mod2.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(string, N, A_);
  MatrixMod2<4096> A(A_);
  STR(b_);
  bitset<4096> b;
  rep(i, N) b.set(i, b_.at(i) - '0');
  auto [ok, sol, basis] = A.solve(b);
  if (!ok)
    PRINT(-1);
  else
  {
    PRINT(basis.size());
    string sol_(M, '0');
    rep(j, M) if (sol.test(j)) sol_.at(j) = '1';
    vc<string> basis_(basis.size(), string(M, '0'));
    rep(i, basis.size()) rep(j, M) if (basis.at(i).test(j)) basis_.at(i).at(j) = '1';
    PRINT(sol_);
    PRINTV(basis_);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
