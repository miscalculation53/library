#define PROBLEM "https://judge.yosupo.jp/problem/counting_eulerian_circuits"

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
#include "graph/matrix_tree.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(pll, M, UV);
  GraphDirected<bool> G(N, UV);
  PRINT(count_eularian_circuits<mint>(G.adj_matrix_ecnt()));
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
