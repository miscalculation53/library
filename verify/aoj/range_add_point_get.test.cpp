#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_E"

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

#include "ds/fenwick_tree/range_add_point_get.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, Q);
  vl A(N);
  rep(i, N) A.at(i) = randint(0, 1000);
  RangeAddPointGet<GroupAddSub<ll>> fw(A);
  dump(A);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(l, r, x);
      l--;
      fw.add(l, r, x);
    }
    else if (t == 1)
    {
      LL(i);
      i--;
      PRINT(fw.get(i) - A.at(i));
    }
    dump(fw.content());
  }
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
