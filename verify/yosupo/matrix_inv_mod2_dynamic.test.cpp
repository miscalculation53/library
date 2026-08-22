#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix_mod_2"

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
  LL(N);
  VEC(string, N, A_);
  DynamicMatrixMod2 A(A_);
  auto [ok, B] = A.inv();
  if (!ok)
    PRINT(-1);
  else
    PRINT(B.to_vstr());
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
