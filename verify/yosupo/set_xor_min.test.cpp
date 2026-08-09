#define PROBLEM "https://judge.yosupo.jp/problem/set_xor_min"

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
  LL(Q);
  BinaryTrie<30> bt;
  rep(_, Q)
  {
    LL(t, x);
    if (t == 0)
    {
      if (bt.get(x) == 0)
        bt.set(x, 1);
    }
    else if (t == 1)
      bt.set(x, 0);
    else
      PRINT(bt.min_element_by(identity{}, x) ^ x);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
