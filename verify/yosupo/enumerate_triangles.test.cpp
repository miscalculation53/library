#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_triangles"

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

#include "graph/triangles.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, M);
  VEC(mint, N, X);
  VEC(pll, M, UV);
  GraphUndirected<ll> G(N, UV);
  mint ans = 0;
  triangles(G, [&](ll u, ll v, ll w)
            { dump(u, v, w); ans += X.at(u) * X.at(v) * X.at(w); });
  PRINT(ans);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
