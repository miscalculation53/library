#define PROBLEM "https://yukicoder.me/problems/no/2290"

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
  UnionFind<UFDataEverything<>> uf(N);
  rep(_, Q)
  {
    LL(t);
    if (t == 1)
    {
      LL(u, v);
      u--, v--;
      uf.merge(u, v);
    }
    else if (t == 2)
    {
      LL(w);
      w--;
      ll u = uf.gdat.min_leader, v = uf.gdat.max_leader;
      if (u == v)
        PRINT(-1);
      else
        PRINT(uf.same(w, u) ? v + 1 : u + 1);
    }
  }
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
