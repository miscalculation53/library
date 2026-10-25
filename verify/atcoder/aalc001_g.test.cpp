#define PROBLEM "https://atcoder.jp/contests/aalc001/tasks/aalc001_g"

#include "algebra/acted_monoid/add_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

int main()
{
  LL(N, Q);
  using mint = modint998244353;
  using AM = ActedMonoidAddPowerSum<mint, 4>;
  VEC(mint, N, a);
  LazySegmentTree<AM> seg(a);
  repi(q, Q)
  {
    LL(t, l, r, x);
    --l;
    if (t == 1)
      seg.apply(l, r, mint(x));
    else
      PRINT(seg.prod(l, r).power_sum[x]);
  }
}
