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

#include "template/template_all.hpp"
using mint = modint998244353;
// using mint = modint1000000007;
// using mint = static_modint<1000000000>;
// using mint = modint;
using bi = Binomial<mint>;

#include "graph/minimum_cycle.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(T);
  LL(N, M);
  VEC(tlll, M, UVW);
  offset(UVW, tlll{-1, -1, 0});
  if (T == 0)
  {
    GraphUndirected<ll> G(N, UVW);
    MinimumCycle mc(G);
    auto ans = mc.solve();
    dump(ans);
    PRINT(ans.first == INF ? -1 : ans.first);
  }
  else
  {
    GraphDirected<ll> G(N, UVW);
    MinimumCycle mc(G);
    auto ans = mc.solve();
    dump(ans);
    PRINT(ans.first == INF ? -1 : ans.first);
  }
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
