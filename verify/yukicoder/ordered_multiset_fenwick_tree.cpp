#define PROBLEM "https://yukicoder.me/problems/no/649"

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
#include "ds/fenwick_tree.hpp"

void init() {}

void main2()
{
  LL(Q, K);
  K--;

  vl vals;
  vc<pll> queries(Q);
  rep(q, Q)
  {
    LL(t);
    if (t == 1)
    {
      LL(v);
      vals.eb(v);
      queries.at(q) = {t, v};
    }
    else if (t == 2)
    {
      queries.at(q) = {t, -1};
    }
  }
  CoordinateCompression cc(vals);
  FenwickTree<GroupAddSub<ll>> fw(cc.size());
  fec([t, v] : queries)
  {
    if (t == 1)
    {
      ll i = cc.get_id(v);
      fw.add(i, 1);
    }
    else if (t == 2)
    {
      ll i = fw.kth_of_multiset(K);
      if (i == fw.size())
        PRINT(-1);
      else
      {
        PRINT(cc.get_val(i));
        fw.add(i, -1);
      }
    }
  }
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
