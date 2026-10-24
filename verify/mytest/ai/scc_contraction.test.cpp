#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/scc.hpp"
#include "graph/contraction.hpp"

template <bool directed>
void check_contraction(int n, const vc<tuple<int, int, ll>> &es, const vc<short> &ids)
{
  Graph<directed, ll> g(n, es);
  const auto result = contracted_graph(g, ids);
  const int k = ids.empty() ? 0 : *max_element(ids.begin(), ids.end()) + 1;
  assert(result.template size<int>() == k);
  vvc<int> expected(k, vc<int>(k));
  for (auto [u, v, cost] : es) if (ids[u] != ids[v])
  {
    expected[ids[u]][ids[v]] = 1;
    if constexpr (!directed) expected[ids[v]][ids[u]] = 1;
  }
  assert(result.template adj_matrix_ecnt<int>() == expected);
  for (const auto &e : result.edges()) assert(e.cost == 1);
}

int main()
{
  mt19937 rng(20261016);
  for (int trial = 0; trial < 900; trial++)
  {
    const int n = rng() % 18, m = n ? rng() % 100 : 0;
    vc<tuple<int, int, ll>> es;
    for (int i = 0; i < m; i++) es.eb(rng() % n, rng() % n, ll(rng() % 101) - 50);
    GraphDirected<ll> g(n, es);
    const auto ids = scc<ll, int>(g);
    vvc<bool> reach(n, vc<bool>(n));
    for (int v = 0; v < n; v++) reach[v][v] = true;
    for (auto [u, v, cost] : es) reach[u][v] = true;
    for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
      if (reach[i][k] && reach[k][j]) reach[i][j] = true;
    for (int u = 0; u < n; u++) for (int v = 0; v < n; v++)
    {
      assert((ids[u] == ids[v]) == (reach[u][v] && reach[v][u]));
      if (reach[u][v] && !reach[v][u]) assert(ids[u] < ids[v]);
    }
    vc<short> labels(n);
    for (auto &v : labels) v = rng() % (n + 3);
    check_contraction<true>(n, es, labels);
    check_contraction<false>(n, es, labels);
  }
  cout << "Hello World\n";
}
