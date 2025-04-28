#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"

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
#include "ds/coordinate_compression.hpp"
#include "ds/fenwick_tree/fenwick_tree_01.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  VEC(pll, Q, TX);
  auto [T, X] = top(TX);

  CoordinateCompression cc(concat(A, X));

  FenwickTree01 fw(cc.size());
  fec(a : A) fw.set(cc.get_id(a), 1);
  fe([t, x] : TX)
  {
    if (t == 0)
      fw.set(cc.get_id(x), 1);
    else if (t == 1)
      fw.set(cc.get_id(x), 0);
    else if (t == 2)
    {
      x--;
      ll ans = fw.kth_in_set(x);
      if (ans == cc.size())
        PRINT(-1);
      else
        PRINT(cc.get_val(ans));
    }
    else if (t == 3)
    {
      PRINT(fw.sum(cc.get_id(x) + 1));
    }
    else if (t == 4)
    {
      ll i = fw.leq_max_in_set(cc.get_id(x));
      if (0 <= i && i < fw.size_of_set())
      {
        ll ans = fw.kth_in_set(i);
        PRINT(cc.get_val(ans));
      }
      else
        PRINT(-1);
    }
    else if (t == 5)
    {
      ll i = fw.geq_min_in_set(cc.get_id(x));
      if (0 <= i && i < fw.size_of_set())
      {
        ll ans = fw.kth_in_set(i);
        PRINT(cc.get_val(ans));
      }
      else
        PRINT(-1);
    }
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
