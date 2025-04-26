#define PROBLEM "https://yukicoder.me/problems/no/1183"

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

#include "algo/rle.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N);
  VEC(ll, N, A, B);
  vl C(N);
  rep(i, N) C.at(i) = A.at(i) ^ B.at(i);
  auto runs = rle(C);
  ll ans = 0;
  fec(run : runs)
  {
    if (run.val == 1)
      ans++;
  }
  PRINT(ans);
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
