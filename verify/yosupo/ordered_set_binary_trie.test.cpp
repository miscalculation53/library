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
#include "ds/binary_trie.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  BinaryTrie<30> bt;
  fec(a : A) bt.set(a, 1);
  rep(_, Q)
  {
    LL(t, x);
    if (t == 0)
      bt.set(x, 1);
    else if (t == 1)
      bt.set(x, 0);
    else if (t == 2)
    {
      x--;
      if (x < bt.all_sum())
        PRINT(bt.kth_by(x, identity{}));
      else
        PRINT(-1);
    }
    else if (t == 3)
      PRINT(bt.leq_sum(x));
    else if (t == 4)
    {
      ll val = bt.leq_max_by(x, identity{});
      if (val != -1)
        PRINT(val);
      else
        PRINT(-1);
    }
    else if (t == 5)
    {
      ll val = bt.geq_min_by(x, identity{});
      if (val != -1)
        PRINT(val);
      else
        PRINT(-1);
    }
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
