#define PROBLEM "https://atcoder.jp/contests/joisc2019/tasks/joisc2019_i"

#include "algebra/algebra_base.hpp"
#include "algebra/clamp_add_cost.hpp"
#include "ds/segtree/segtree.hpp"

int main()
{
  INT(N, Q);
  using M = MonoidClampAddCost<ll, 0, 1>;
  using Both = NormalAndOppositeMonoid<M>;
  vc<Both::S> a;
  repi(i, N - 1)
  {
    LL(l, r);
    a.emplace_back(l, r - 1, 1);
  }
  SegmentTree<Both> seg(a);
  repi(i, Q)
  {
    INT(t);
    if (t == 1)
    {
      INT(p);
      LL(l, r);
      seg.set(p - 1, Both::S(l, r - 1, 1));
    }
    else
    {
      INT(a);
      LL(b);
      INT(c);
      LL(d);
      const auto f = seg.prod(min(a, c) - 1, max(a, c) - 1);
      const auto [y, leaps] = (a < c ? f.opposite : f.normal)(b);
      PRINT(leaps + max(y - d, 0LL));
    }
  }
}
