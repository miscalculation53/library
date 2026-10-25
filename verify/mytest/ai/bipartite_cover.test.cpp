#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/bipartite_cover.hpp"

template <class G>
optional<int> brute_edge_cover(const G &g)
{
  const int n = g.template size<int>();
  const auto es = g.edges();
  optional<int> best;
  for (unsigned mask = 0; mask < 1U << es.size(); mask++)
  {
    unsigned vertices = 0;
    for (int i = 0; i < int(es.size()); i++) if (mask >> i & 1)
      vertices |= 1U << es[i].from | 1U << es[i].to;
    if (vertices == (1U << n) - 1)
    {
      const int value = __builtin_popcount(mask);
      if (!best || value < *best) best = value;
    }
  }
  return best;
}

template <class G>
void check_edges(const G &g, const optional<vc<int>> &result)
{
  const auto expected = brute_edge_cover(g);
  assert(bool(result) == bool(expected));
  if (!result) return;
  assert(int(result->size()) == *expected);
  map<int, pair<int, int>> edge;
  for (const auto &e : g.edges()) edge[e.index] = {e.from, e.to};
  vc<bool> covered(g.template size<int>());
  set<int> selected;
  for (int id : *result)
  {
    assert(edge.count(id) && selected.insert(id).second);
    const auto [u, v] = edge[id];
    covered[u] = covered[v] = true;
  }
  for (bool v : covered) assert(v);
}

template <class G, class Color>
void check_bipartite(const G &g, const vc<Color> &color)
{
  const int n = g.template size<int>();
  const auto matching = bipartite_matching(g, color);
  const auto cover = bipartite_vertex_cover(g, color, matching);
  const auto independent = bipartite_independent_set(g, color, matching);
  vc<bool> selected(n), free(n);
  for (int v : cover) assert(!selected[v]), selected[v] = true;
  for (int v : independent) assert(!free[v]), free[v] = true;
  for (int v = 0; v < n; v++) assert(selected[v] != free[v]);
  const auto es = g.edges();
  int best = n;
  for (unsigned mask = 0; mask < 1U << n; mask++)
  {
    bool ok = true;
    for (const auto &e : es) if (!(mask >> e.from & 1) && !(mask >> e.to & 1)) ok = false;
    if (ok) best = min(best, __builtin_popcount(mask));
  }
  for (const auto &e : es) assert(selected[e.from] || selected[e.to]);
  assert(int(cover.size()) == best && int(independent.size()) == n - best);
  assert(bipartite_vertex_cover(g, color) == cover);
  assert(bipartite_independent_set(g, color) == independent);
  check_edges(g, bipartite_edge_cover(g, color));
  check_edges(g, minimum_edge_cover(g, matching));
}

int main()
{
  mt19937 rng(20261016);
  for (int trial = 0; trial < 800; trial++)
  {
    const int n = rng() % 10;
    vc<int> color(n);
    for (auto &c : color) c = rng() % 2;
    vc<tuple<int, int, ll>> es;
    for (int i = 0; i < 10 && n > 0; i++)
    {
      int u = rng() % n, v = rng() % n;
      if (color[u] != color[v]) es.eb(u, v, ll(rng()));
    }
    GraphUndirected<ll, true> g(n, es);
    for (int i = 0; i < int(es.size()); i++) if (rng() % 5 == 0) g.erase_edge(i);
    check_bipartite(g, color);
    check_bipartite(g, vc<bool>(color.begin(), color.end()));
    check_bipartite(g, vc<unsigned char>(color.begin(), color.end()));
  }
  for (int trial = 0; trial < 300; trial++)
  {
    const int n = rng() % 8, m = n ? rng() % 10 : 0;
    vc<pair<int, int>> es;
    for (int i = 0; i < m; i++) es.eb(rng() % n, rng() % n);
    GraphUndirected<> g(n, es);
    vc<pair<int, int>> current, matching;
    auto dfs = [&](auto &self, int id, unsigned used) -> void
    {
      if (id == m)
      {
        if (current.size() > matching.size()) matching = current;
        return;
      }
      self(self, id + 1, used);
      const auto [u, v] = es[id];
      if (u == v || (used >> u & 1) || (used >> v & 1)) return;
      current.eb(u, v);
      self(self, id + 1, used | 1U << u | 1U << v);
      current.pop_back();
    };
    dfs(dfs, 0, 0);
    check_edges(g, minimum_edge_cover(g, matching));
  }
  cout << "Hello World\n";
}
