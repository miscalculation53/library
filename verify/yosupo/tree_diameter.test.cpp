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

#include "tree/diameter.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC(tlll, N - 1, ABC);
  UNZIP(ABC, A, B, C);
  ZIP(AB, A, B);
  RootedTree G(N, AB, 0);

  Diameter diameter(N, AB, C);
  auto [a, b] = diameter.endpoints();
  const RootedTree Ga(N, AB, a);
  auto path = G.path_vertex(b, a);
  auto path2 = Ga.path_vertex_to_root(b);
  dump(path, path2);
  assert(path == path2);
  PRINT(diameter.length(), SZ(path));
  PRINT(path);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
