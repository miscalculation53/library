#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"

#include "flow/bipartite_matching.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int l, r, m;
  cin >> l >> r >> m;
  vc<pair<int, int>> es(m);
  for (auto &[u, v] : es) cin >> u >> v, v += l;
  GraphUndirected<> g(l + r, es);
  vc<bool> color(l + r);
  for (int v = l; v < l + r; v++) color[v] = true;
  const auto matching = bipartite_matching(g, color);
  cout << matching.size() << '\n';
  for (auto [u, v] : matching) cout << u << ' ' << v - l << '\n';
}
