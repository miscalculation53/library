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
  const int MAX = local_oj(4, 4096);
  MatrixMod2<MAX, MAX> A;
  rep(i, N) rep(j, M) A(i, j) = A_[i][j] == '1';
  STR(b_);
  bitset<MAX> b;
  rep(i, N) b.set(i, b_.at(i) - '0');
  const auto result = A.solve(b);
  if (!result)
    PRINT(-1);
  else
  {
    const auto &[sol, basis] = *result;
    vc<string> basis_;
    for (const auto &v : basis) {
      string row(M, '0');
      rep(j, M) if (v[j]) row[j] = '1';
      if (row.find('1') != string::npos) basis_.push_back(row);
    }
    PRINT(basis_.size());
    string sol_(M, '0');
    rep(j, M) if (sol.test(j)) sol_.at(j) = '1';
    PRINT(sol_);
    PRINTV(basis_);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
