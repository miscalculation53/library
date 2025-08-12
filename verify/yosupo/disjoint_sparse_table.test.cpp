#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

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

#include "ds/static_range/disjoint_sparse_table.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  DisjointSparseTable<MonoidMin<ll>> spt(A);
  rep(_, Q)
  {
    LL(l, r);
    PRINT(spt.prod(l, r));
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
