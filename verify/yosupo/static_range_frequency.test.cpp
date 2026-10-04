#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

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

#include "ds/group_index.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  GroupIndex grp(A);
  rep(_, Q)
  {
    LL(l, r, x);
    dump(x, l, r, grp.lt_cnt(x, r), grp.lt_cnt(x, l));
    PRINT(grp.in_cnt(x, l, r));
  }
  dump(grp.to_vv() | cp::index());
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
