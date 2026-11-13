#define PROBLEM "https://judge.yosupo.jp/problem/general_weighted_matching"

#include "graph/general_weighted_matching.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vc<tuple<int, int, ll>> edges(m);
  for (auto &[u, v, w] : edges) cin >> u >> v >> w;
  const auto matching = general_weighted_matching(GraphUndirected<ll>(n, edges));
  ll total = 0;
  for (int id : matching) total += get<2>(edges[id]);
  cout << matching.size() << ' ' << total << '\n';
  for (int id : matching)
  {
    const auto [u, v, w] = edges[id];
    cout << min(u, v) << ' ' << max(u, v) << '\n';
  }
}
