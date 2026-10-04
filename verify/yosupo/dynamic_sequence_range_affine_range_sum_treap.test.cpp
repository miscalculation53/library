#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"

#include "ds/bbst/lazy_sequence.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"
#include "math/modint/modint.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  using mint = modint998244353;
  int n, q;
  cin >> n >> q;
  vector<mint> a(n);
  for (auto &x : a) cin >> x;
  LazySequenceTree<ActedMonoidAffineSum<mint>> t(a);
  while (q--)
  {
    int type, l, r;
    cin >> type >> l;
    if (type == 0)
    {
      mint x;
      cin >> x;
      t.insert(l, x);
    }
    else if (type == 1) t.erase(l);
    else
    {
      cin >> r;
      if (type == 2) t.reverse(l, r);
      else if (type == 3)
      {
        mint b, c;
        cin >> b >> c;
        t.apply(l, r, {b, c});
      }
      else cout << t.prod(l, r).val << '\n';
    }
  }
}
