#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations_mod_2"

#define SINGLE_TESTCASE

#ifndef LOCAL
#define FAST_CIO
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/linalg/dynamic_matrix_mod2.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(string, N, A_);
  DynamicMatrixMod2 A(A_);
  STR(b_);
  DynamicBitset b(N);
  repi(i, N) b.set(i, b_[i] - '0');
  auto [ok, sol, basis] = A.solve(b);
  if (!ok)
    PRINT(-1);
  else
  {
    PRINT(basis.size());
    string sol_(M, '0');
    repi(j, M) if (sol.test(j)) sol_[j] = '1';
    vc<string> basis_(basis.size(), string(M, '0'));
    repi(i, basis.size()) repi(j, M) if (basis[i].test(j)) basis_[i][j] = '1';
    PRINT(sol_);
    PRINTV(basis_);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
