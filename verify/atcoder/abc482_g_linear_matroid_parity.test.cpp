#define PROBLEM "https://atcoder.jp/contests/abc482/tasks/abc482_g"

#include "matroid/linear_matroid_parity.hpp"
#include "matroid/partition_matroid.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int tests;
  cin >> tests;
  while (tests--)
  {
    int n, m;
    cin >> n >> m;
    vc<int> capacity(n), group(2 * m);
    for (int &a : capacity) cin >> a;
    for (int e = 0; e < m; e++)
    {
      int u, v;
      cin >> u >> v;
      group[2 * e] = u - 1;
      group[2 * e + 1] = v - 1;
    }
    auto representation = PartitionMatroid(std::move(group), std::move(capacity)).sparse_linear_representation();
    cout << linear_matroid_parity_size(representation) << '\n';
  }
}
