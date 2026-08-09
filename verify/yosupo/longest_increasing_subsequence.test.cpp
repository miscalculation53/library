#define PROBLEM "https://judge.yosupo.jp/problem/longest_increasing_subsequence"

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
  VEC(ll, N, A);

  LIS lis(A);

  ll len = lis.len_of_lis();
  vl ans;
  rep(i, len)
  {
    ans.eb(lis.candidates(i).front());
  }
  PRINT(len);
  PRINT(ans);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
