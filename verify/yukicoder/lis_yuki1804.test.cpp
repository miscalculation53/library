#define PROBLEM "https://yukicoder.me/problems/no/1804"

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

#include "algo/lis.hpp"

void init() {}

void main2()
{
  LL(N);
  VEC(ll, N, P);

  LIS lis(P);
  vl ans;
  rep(i, lis.len_of_lis())
  {
    auto cand = lis.candidates(i);
    if (SZ(cand) == 1)
      ans.eb(P[cand.front()]);
  }
  if (ans.empty())
  {
    PRINT(ans.size());
    PRINT(" ");
  }
  else
  {
    PRINT(ans.size());
    PRINT(ans);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
