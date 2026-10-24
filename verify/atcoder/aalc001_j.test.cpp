#define PROBLEM "https://atcoder.jp/contests/aalc001/tasks/aalc001_j"

#include "algebra/acted_monoid/affine_sum.hpp"
#include "ds/segtree/sparse_lazy_segtree.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int q;
  cin >> q;
  using AM = ActedMonoidAffineSum<ull>;
  SparseLazySegmentTree<AM> seg(1'000'000'000'000'000'000LL, 0);
  while (q--)
  {
    int t;
    ll l, r;
    cin >> t >> l >> r;
    --l;
    if (t == 1)
      seg.apply(l, r, {-1ull, 1ull});
    else
      cout << seg.prod(l, r).val << '\n';
  }
}
