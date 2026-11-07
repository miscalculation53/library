#define PROBLEM "https://atcoder.jp/contests/abc263/tasks/abc263_g"

#include "graph/b_matching.hpp"
#include "math/prime/large/primality_test.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vc<ll> a(n), b(n);
  for (int i = 0; i < n; i++) cin >> a[i] >> b[i];
  vc<pair<int, int>> es;
  for (int i = 0; i < n; i++) for (int j = i; j < n; j++)
    if (is_prime(a[i] + a[j])) es.eb(i, j);
  ll answer = 0;
  for (auto [id, count] : b_matching(GraphUndirected<>(n, es), b, nullopt))
  {
    (void)id;
    answer += count;
  }
  cout << answer << '\n';
}
