#define PROBLEM "https://atcoder.jp/contests/abc482/tasks/abc482_g"

#include "graph/b_matching.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--)
  {
    int n, m;
    cin >> n >> m;
    vc<int> a(n);
    for (int &x : a) cin >> x;
    vc<pair<int, int>> es(m);
    for (auto &[u, v] : es) cin >> u >> v, --u, --v;
    cout << b_matching(GraphUndirected<>(n, es), a).size() << '\n';
  }
}
