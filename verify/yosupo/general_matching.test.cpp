#define PROBLEM "https://judge.yosupo.jp/problem/general_matching"

#include "graph/general_matching.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vc<pair<int, int>> es(m);
  for (auto &[u, v] : es) cin >> u >> v;
  const auto matching = general_matching(GraphUndirected<>(n, es));
  cout << matching.size() << '\n';
  for (auto [u, v] : matching) cout << u << ' ' << v << '\n';
}
