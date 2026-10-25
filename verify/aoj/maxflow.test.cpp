#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A"

#include "flow/maxflow.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  MaxFlow<ll> g(n);
  while (m--)
  {
    int u, v;
    ll c;
    cin >> u >> v >> c;
    g.add_edge(u, v, c);
  }
  cout << g.flow(0, n - 1) << '\n';
}
