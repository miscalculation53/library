#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"

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
#include "convex/convex_hull_trick.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  ConvexHullTrick cht;
  repi(i, N)
  {
    LL(a, b);
    cht.add_line(a, b);
  }
  repi(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(a, b);
      cht.add_line(a, b);
    }
    else
    {
      LL(x);
      PRINT(cht.query(x));
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
