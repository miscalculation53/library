#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"

#include "ds/bbst/ordered_map.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  OrderedMapTree<ll, MonoidAdd<ll>> t;
  for (int i = 0; i < n; ++i)
  {
    ll x;
    cin >> x;
    t.insert(x, x);
  }
  while (q--)
  {
    int type;
    ll x;
    cin >> type >> x;
    if (type == 0) t.insert(x, x);
    else if (type == 1) t.erase(x);
    else if (type == 2) cout << (x <= t.size() ? t.get_by_order(x - 1).first : -1) << '\n';
    else if (type == 3) cout << t.upper_order_of_key(x) << '\n';
    else if (type == 4)
    {
      int k = t.upper_order_of_key(x) - 1;
      cout << (k >= 0 ? t.get_by_order(k).first : -1) << '\n';
    }
    else
    {
      int k = t.order_of_key(x);
      cout << (k < t.size() ? t.get_by_order(k).first : -1) << '\n';
    }
  }
}
