#define PROBLEM "https://judge.yosupo.jp/problem/vertex_set_path_composite"

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

#include "graph/tree/euler_tour_path_prod.hpp"
#include "algebra/affine_function.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(pll, N, AB);
  VEC(pll, N - 1, UV);
  PathProd<OppositeGroup<GroupAffineFunction<mint>>> G(N, UV, randrange(0, N), AB);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(p, c, d);
      G.set(p, {c, d});
    }
    else if (t == 1)
    {
      LL(u, v, x);
      auto [c, d] = G.prod(u, v);
      PRINT(c * x + d);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
