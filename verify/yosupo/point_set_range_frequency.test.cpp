#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_frequency"

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
#include "ds/query/point_set_range_frequency.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  PointSetRangeFrequency rf(A);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(k, v);
      rf.point_set(k, v);
    }
    else if (t == 1)
    {
      LL(l, r, x);
      rf.range_frequency(l, r, x);
    }
  }
  PRINTV(rf.run());
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
