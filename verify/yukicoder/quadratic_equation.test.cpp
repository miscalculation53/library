#define PROBLEM "https://yukicoder.me/problems/no/550"

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

#include "math/quadratic_equation_integer.hpp"

void init() {}

void main2()
{
  LL(a, b, c);
  auto f = [&](i128 x) -> i128
  {
    return x * x * x + a * x * x + b * x + c;
  };
  auto judge = [&](i128 x)
  { return f(x) <= 0; };
  ll p = binsearch(judge, ipow(-10, 9), ipow(10, 9)).first;
  auto [cnt, sol] = quadratic_equation_integer(1, a + p, b + a * p + p * p);
  auto [q, r] = sol;
  PRINT(sorted(vl{p, q, r}));
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
