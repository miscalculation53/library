#define PROBLEM "https://yukicoder.me/problems/447"

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

#include "math/crt.hpp"

void init() {}

void main2()
{
  LL(x1, y1, x2, y2, x3, y3);
  auto [ok, r, m] = crt(array{x1, x2, x3}, array{y1, y2, y3});
  if (!ok)
    PRINTRETURN(-1);
  if (r == 0)
    r += m;
  PRINT(r);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
