#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"

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

#include "ds/2d/offline/area_of_union_of_rectangles.hpp"

void init() {}

void main2()
{
  LL(N);
  AreaOfUnionOfRectangles ur;
  rep(_, N)
  {
    LL(lx, ly, rx, ry);
    ur.rectangle_add(lx, rx, ly, ry);
  }
  PRINT(ur.run());
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
