#define PROBLEM "https://yukicoder.me/problems/no/1168"

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

#include "algo/digit_sum.hpp"

void init() {}

void main2()
{
  LL(N);
  rep(_, 100) N = digit_sum(N);
  PRINT(N);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
