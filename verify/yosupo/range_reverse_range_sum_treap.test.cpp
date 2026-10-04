#define PROBLEM "https://judge.yosupo.jp/problem/range_reverse_range_sum"

#include "ds/bbst/sequence.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<ll> a(n);
  for (auto &x : a) cin >> x;
  SequenceTree<MonoidAdd<ll>> t(a);
  while (q--)
  {
    int type, l, r;
    cin >> type >> l >> r;
    if (type == 0) t.reverse(l, r);
    else cout << t.prod(l, r) << '\n';
  }
}
