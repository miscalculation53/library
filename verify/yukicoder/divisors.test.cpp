#define PROBLEM "https://yukicoder.me/problems/no/888"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/prime/factorize.hpp"

void init() {}

void main2()
{
  LL(N);
  auto ds = divisors(factorize(N));
  PRINT(SUM(ds));
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
