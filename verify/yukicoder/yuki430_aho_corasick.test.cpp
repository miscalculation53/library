#define PROBLEM "https://yukicoder.me/problems/no/430"

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
#include "graph/tree/rooted_tree.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  STR(S_);
  vl S = stov(S_, 'A');
  LL(M);
  VEC(string, M, T_);
  vvl T(M);
  rep(i, M) T.at(i) = stov(T_.at(i), 'A');
  AhoCorasick<26> aho(T);
  ll N = aho.trie.num_of_vertices();
  rep(i, N) dump(i, vtos(aho.trie.to_string(i), 'A'));

  vl cnt(N, 0);
  ll v = 0;
  fec(c : S)
  {
    v = aho.next(v, c);
    cnt.at(v)++;
    dump(v);
  }
  dump(cnt | cp::index());

  RootedTree<bool, false> G(GEN_VEC(N, i, aho.failure(i)));
  fec(e : reversed(G.bfs_ordered_edges()))
  {
    cnt.at(e.from) += cnt.at(e.to);
  }

  ll ans = 0;
  fec(t : T)
  {
    ll v = 0;
    fec(c : t) v = aho.trie.child(v, c);
    dump(vtos(t, 'A'), cnt.at(v));
    ans += cnt.at(v);
  }
  PRINT(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
