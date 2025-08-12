#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_7_A&lang=ja"

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

#include "graph/tree/rooted_tree.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N);
  vpll UV;
  ll root = 0;
  rep(i, N) root ^= i;
  rep(_, N)
  {
    LL(v, k);
    VEC(ll, k, cs);
    fe(c : cs) root ^= c, UV.eb(v, c);
  }
  RootedTree<ll> G(N, UV, root);
  rep(v, N)
  {
    WRITE("node ", v, ": parent = ", G.parent(v), ", depth = ", G.depth(v), ", ");
    if (G.parent(v) == -1)
      WRITE("root");
    else if (G.children(v).empty())
      WRITE("leaf");
    else
      WRITE("internal node");
    WRITE(", [");
    auto chi = G.children(v);
    rep(i, chi.size())
    {
      if (i)
        WRITE(", ");
      WRITE(chi.at(i));
    }
    WRITE("]\n");
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
