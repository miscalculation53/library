#define PROBLEM "https://judge.yosupo.jp/problem/unionfind_with_potential"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;

#include "ds/uf/uf_potential.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  UnionFindPotential<GroupAddSub<mint>> uf(N);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(u, v, w);
      PRINT(uf.merge(v, u, w));
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
