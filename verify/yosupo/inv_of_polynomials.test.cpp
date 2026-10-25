#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_polynomials"
#include "math/fps/half_gcd.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  FormalPowerSeries<modint998244353> a(n), b(m);
  for (auto &x : a) cin >> x;
  for (auto &x : b) cin >> x;
  const auto c = polynomial_inv_mod(a, b);
  if (!c) cout << -1 << '\n';
  else
  {
    cout << c->size() << '\n';
    for (auto x : *c) cout << x << ' ';
    cout << '\n';
  }
}
