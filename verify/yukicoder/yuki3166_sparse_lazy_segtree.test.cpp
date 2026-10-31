#define PROBLEM "https://yukicoder.me/problems/no/3166"

#define MULTI_TESTCASE

#ifndef LOCAL
#define FAST_IO
#endif

#include "template/template_all_but_modint.hpp"
#include "ds/segtree/sparse_lazy_segtree.hpp"
#include "algebra/acted_monoid/add_min.hpp"

void init() {}

void main2()
{
  LL(N, L, K);
  VEC(ll, N, X);
  auto judge = [&](ll P)
  {
    if (P < 0) return false;
    SparseLazySegmentTree<ActedMonoidAddMin<ll>> seg(L, 0);
    for (ll x : X)
    {
      ll l = x - P, r = x + P;
      if (l < 0) seg.apply(safemod(l, L), L, 1);
      seg.apply(max(0LL, l), min(L, r), 1);
      if (r > L) seg.apply(0, r % L, 1);
    }
    return seg.all_prod() >= K;
  };
  PRINT(expsearch(judge, 0).first);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
