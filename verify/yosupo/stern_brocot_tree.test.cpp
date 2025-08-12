#define PROBLEM "https://judge.yosupo.jp/problem/stern_brocot_tree"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"
#include "math/sbt.hpp"

void init() {}

void main2()
{
  STR(S);
  if (S == "ENCODE_PATH")
  {
    INT(a, b);
    SBTNode<int> node(a, b);
    auto path = node.encode_path();
    PRINT(path.size(), path);
  }
  else if (S == "DECODE_PATH")
  {
    using P = pair<char, int>;
    INT(k);
    VEC(P, k, path);
    SBTNode<int> node(path);
    PRINT(node.num(), node.den());
    dump(node);
  }
  else if (S == "LCA")
  {
    INT(a, b, c, d);
    SBTNode<int> node1(a, b), node2(c, d);
    auto lca = node1.lca(node2);
    PRINT(lca.num(), lca.den());
  }
  else if (S == "ANCESTOR")
  {
    INT(k, a, b);
    SBTNode<int> node(a, b);
    int dep = node.depth();
    if (!(0 <= k && k <= dep))
      PRINT(-1);
    else
    {
      node.ascend(dep - k);
      PRINT(node.num(), node.den());
    }
  }
  else if (S == "RANGE")
  {
    INT(a, b);
    SBTNode<int> node(a, b);
    PRINT(node.p, node.q, node.r, node.s);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
