#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"

#include "convex/li_chao_tree.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  LiChaoTree<ll> tree(-1'000'000'000LL, 1'000'000'001LL);
  tree.reserve(n + q);
  for (int i = 0; i < n; ++i)
  {
    ll a, b;
    cin >> a >> b;
    tree.add_line(a, b);
  }
  while (q--)
  {
    int t;
    cin >> t;
    if (t == 0)
    {
      ll a, b;
      cin >> a >> b;
      tree.add_line(a, b);
    }
    else
    {
      ll x;
      cin >> x;
      cout << tree.query(x).first << '\n';
    }
  }
}
