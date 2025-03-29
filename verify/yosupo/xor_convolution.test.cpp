#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/set/xor_convolution.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;

void init() {}

void main2()
{
  LL(N);
  VEC(mint, 1 << N, A, B);
  auto C = xor_convolution<FieldAddSubMulDiv<mint>>(A, B);
  PRINT(C);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
