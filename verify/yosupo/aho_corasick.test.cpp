#define PROBLEM "https://judge.yosupo.jp/problem/aho_corasick"

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

#include "string/aho_corasick.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N);
  VEC(string, N, S);
  vvl A(N);
  rep(i, N) A.at(i) = stov(S.at(i), 'a');
  AhoCorasick<26> aho(A);
  PRINT(aho.trie.num_of_vertices());
  rep(i, 1, aho.trie.num_of_vertices())
  {
    PRINT(aho.trie.parent(i), aho.failure(i));
  }
  vl V(N);
  rep(i, N)
  {
    ll v = 0;
    fec(a : A.at(i)) v = aho.trie.child(v, a);
    V.at(i) = v;
  }
  PRINT(V);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
