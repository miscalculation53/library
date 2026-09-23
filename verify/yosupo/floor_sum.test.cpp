#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"

#include "math/mod_of_linear/floor_sum.hpp"

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
    cout << floor_sum<i128>(n, m, a, b) << '\n';
  }
}
