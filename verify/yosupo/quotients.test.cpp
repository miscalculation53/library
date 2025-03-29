#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_quotients"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/quotients.hpp"

void init() {}

void main2()
{
  LL(N);
  vl ans;
  fec([y, l, r] : quotients(N))
  {
    ans.push_back(y);
  }
  PRINT(ans.size());
  PRINT(ans);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
