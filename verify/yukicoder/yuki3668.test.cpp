#define PROBLEM "https://yukicoder.me/problems/no/3668"

#include "flow/maxflow.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m, s, t;
  cin >> n >> m >> s >> t;
  MaxFlow<ll> g(n);
  while (m--)
  {
    int u, v;
    ll c;
    cin >> u >> v >> c;
    g.add_edge(u - 1, v - 1, c);
  }
  cout << g.flow(s - 1, t - 1) << '\n';
}
