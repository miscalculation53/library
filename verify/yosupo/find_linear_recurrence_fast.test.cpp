#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"
#include "math/fps/berlekamp_massey_fast.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vc<modint998244353> a(n);
  for (auto &x : a) cin >> x;
  auto c = berlekamp_massey_fast(a);
  cout << c.size() - 1 << '\n';
  for (int i = 1; i < (int)c.size(); ++i) cout << c[i] << ' ';
  cout << '\n';
}
