#define PROBLEM "https://atcoder.jp/contests/abc365/tasks/abc365_f"

#include "algebra/algebra_base.hpp"
#include "algebra/clamp_add_cost.hpp"
#include "ds/segtree/segtree.hpp"

int main()
{
  INT(N);
  using M = MonoidClampAddCost<ll, 1, 1>;
  vc<M::S> a;
  repi(i, N)
  {
    LL(l, r);
    a.push_back(M::S::clamp(l, r));
  }
  SegmentTree<OppositeMonoid<M>> seg(a);
  INT(Q);
  repi(q, Q)
  {
    INT(sx);
    LL(sy);
    INT(tx);
    LL(ty);
    if (sx > tx) swap(sx, tx), swap(sy, ty);
    const auto [y, cost] = seg.prod(sx, tx)(sy);
    PRINT(ll(tx - sx) + cost + abs(y - ty));
  }
}
