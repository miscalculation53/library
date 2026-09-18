#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"

#include "convex/li_chao_tree.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  LiChaoTree<ll> tree(-1'000'000'000LL, 1'000'000'001LL);
  tree.reserve(n + q);
  auto add = [&]()
  {
    ll l, r, a, b;
    cin >> l >> r >> a >> b;
    tree.add_segment(l, r, a, b);
  };
  for (int i = 0; i < n; ++i) add();
  while (q--)
  {
    int t;
    cin >> t;
    if (t == 0) add();
    else
    {
      ll x;
      cin >> x;
      auto [value, line] = tree.query(x);
      if (line.id == -1) cout << "INFINITY\n";
      else cout << value << '\n';
    }
  }
}
