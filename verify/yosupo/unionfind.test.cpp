#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "ds/uf/uf.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  UnionFind<UFDataEmpty<>> uf(N);
  rep(_, Q)
  {
    LL(t, u, v);
    if (t == 0)
      uf.merge(u, v);
    else if (t == 1)
      PRINT(uf.same(u, v));
  }
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
