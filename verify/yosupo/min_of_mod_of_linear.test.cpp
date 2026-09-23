#define PROBLEM "https://judge.yosupo.jp/problem/min_of_mod_of_linear"

#include "math/mod_of_linear/min_of_mod_of_linear.hpp"

int main()
{
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--)
  {
    ll n, m, a, b;
    cin >> n >> m >> a >> b;
    cout << min_of_mod_of_linear(n, m, a, b) << '\n';
  }
}
