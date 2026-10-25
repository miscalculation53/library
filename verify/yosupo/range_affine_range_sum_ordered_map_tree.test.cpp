#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include "ds/bbst/lazy_ordered_map.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"
#include "math/modint/modint.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  using mint = modint998244353;
  int n, q;
  cin >> n >> q;
  LazyOrderedMapTree<int, ActedMonoidAffineSum<mint>> t;
  for (int i = 0; i < n; ++i)
  {
    mint x;
    cin >> x;
    t.insert(i, x);
  }
  while (q--)
  {
    int type, l, r;
    cin >> type >> l >> r;
    if (type == 0)
    {
      mint b, c;
      cin >> b >> c;
      t.apply_by_key(l, r, {b, c});
    }
    else cout << t.prod_by_key(l, r).val << '\n';
  }
}
