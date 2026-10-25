#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "ds/bbst/sequence.hpp"
#include "algebra/affine_function.hpp"
#include "math/modint/modint.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  using mint = modint998244353;
  using M = OppositeMonoid<MonoidAffineFunction<mint>>;
  int n, q;
  cin >> n >> q;
  vector<M::S> a;
  for (int i = 0; i < n; ++i)
  {
    mint b, c;
    cin >> b >> c;
    a.emplace_back(b, c);
  }
  SequenceTree<M> t(a);
  while (q--)
  {
    int type;
    cin >> type;
    if (type == 0)
    {
      int p;
      mint b, c;
      cin >> p >> b >> c;
      t.set(p, {b, c});
    }
    else
    {
      int l, r;
      mint x;
      cin >> l >> r >> x;
      auto f = t.prod(l, r);
      cout << f.b * x + f.c << '\n';
    }
  }
}
