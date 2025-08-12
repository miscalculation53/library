#define PROBLEM "https://judge.yosupo.jp/problem/rational_approximation"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/sbt.hpp"

void init() {}

void main2()
{
  INT(N, x, y);
  auto judge = [&](int num, int den) -> bool
  {
    return ll(num) * ll(y) <= ll(den) * ll(x);
  };
  auto judge2 = [&](int num, int den) -> bool
  {
    return ll(num) * ll(y) >= ll(den) * ll(x);
  };
  auto node = sbt_search<int>(judge, N);
  dump(node);
  int p = node.p, q = node.q;
  int r, s;
  if (judge2(p, q))
    r = p, s = q;
  else
    r = node.r, s = node.s;
  PRINT(p, q, r, s);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
