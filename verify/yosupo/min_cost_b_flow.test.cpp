#define PROBLEM "https://judge.yosupo.jp/problem/min_cost_b_flow"

#include "flow/min_cost_b_flow.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vc<ll> b(n);
  for (auto &v : b) cin >> v;
  MinCostBFlow<ll, ll, i128> g(b);
  for (int i = 0; i < m; i++)
  {
    int s, t;
    int l, u, c;
    cin >> s >> t >> l >> u >> c;
    g.add_edge(s, t, l, u, c);
  }
  const auto cost = g.flow();
  if (!cost) { cout << "infeasible\n"; return 0; }
  cout << *cost << '\n';
  for (auto p : g.potential()) cout << p << '\n';
  for (auto e : g.edges()) cout << e.flow << '\n';
}
