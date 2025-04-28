#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

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
#include "ds/fenwick_tree/fenwick_tree_01.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  STR(T);
  FenwickTree01 fw(stov(T, '0'));
  dump(fw.content());
  rep(_, Q)
  {
    LL(c, k);
    if (c == 0)
      fw.set(k, 1);
    else if (c == 1)
      fw.set(k, 0);
    else if (c == 2)
      PRINT(fw.get(k));
    else if (c == 3)
    {
      ll i = fw.geq_min_in_set(k);
      PRINT(i < fw.size_of_set() ? fw.kth_in_set(i) : -1);
    }
    else if (c == 4)
    {
      ll i = fw.leq_max_in_set(k);
      PRINT(i >= 0 ? fw.kth_in_set(i) : -1);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
