#define PROBLEM "https://judge.yosupo.jp/problem/unionfind_with_potential_non_commutative_group"

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

#include "ds/uf/uf_potential.hpp"
#include "algebra/matmul22.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  UnionFindPotential<GroupMatMul22<mint>> uf(N);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(u, v, a, b, c, d);
      PRINT(uf.merge(v, u, {{a, b, c, d}}));
    }
    else if (t == 1)
    {
      LL(u, v);
      if (uf.same(u, v))
        PRINT(uf.diff(v, u));
      else
        PRINT(-1);
    }
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
