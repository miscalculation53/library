#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"

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

#include "graph/tree/diameter.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC(tlll, N - 1, ABC);
  UNZIP(ABC, A, B, C);
  ZIP(AB, A, B);
  RootedTree G(N, AB, 0);

  auto [a, b, Ga, Da] = tree_diameter_weighted(N, AB, C);
  auto path = G.path(b, a);
  auto path2 = Ga.path_to_root(b);
  dump(path, path2);
  assert(path == path2);
  PRINT(Da[b], SZ(path));
  PRINT(path);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
