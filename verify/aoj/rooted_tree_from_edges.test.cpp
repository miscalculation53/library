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
  vc<vc<int>> C(N);
  rep(i, N)
  {
    LL(v, k);
    VEC(int, k, cs);
    C[v] = cs;
    fe(c : cs) root ^= c, UV.eb(v, c);
  }
  RootedTree G(N, UV, root);
  rep(v, N)
  {
    WRITE("node ", v, ": parent = ", v == G.root() ? -1 : G.parent(v), ", depth = ", G.depth(v), ", ");
    if (v == G.root())
      WRITE("root");
    else if (G.children(v).empty())
      WRITE("leaf");
    else
      WRITE("internal node");
    WRITE(", [");
    auto chi = G.children(v);
    assert(sorted(chi.to_v()) == sorted(C[v]));
    rep(i, C[v].size())
    {
      if (i)
        WRITE(", ");
      WRITE(C[v][i]);
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
