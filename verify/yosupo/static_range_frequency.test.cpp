#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "ds/coordinate_compression.hpp"
#include "ds/group_index.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  CoordinateCompression cc(A);
  GroupIndex grp(compressed(A));
  rep(val, cc.size()) dump(val, grp.idxs(val));
  rep(_, Q)
  {
    LL(l, r, x);
    auto v = cc.get_id(x);
    dump(v, l, r, grp.lt_cnt(v, r), grp.lt_cnt(v, l));
    PRINT(grp.in_cnt(v, l, r));
  }
  dump(grp.to_vv() | cp::index());
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
