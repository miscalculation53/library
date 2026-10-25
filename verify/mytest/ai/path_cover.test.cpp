#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/path_cover.hpp"

int cover_dp(int bits, const vc<unsigned> &paths, bool partition)
{
  vc<int> dp(1U << bits, 1000000);
  dp[0] = 0;
  for (unsigned mask = 0; mask < dp.size(); mask++)
    for (unsigned path : paths) if (!partition || !(mask & path))
      dp[mask | path] = min(dp[mask | path], dp[mask] + 1);
  return dp.back();
}

template <class G>
pair<vc<unsigned>, vc<unsigned>> dag_paths(const G &g)
{
  const auto es = g.edges();
  vvc<int> out(g.template size<int>());
  for (int i = 0; i < int(es.size()); i++) out[es[i].from].eb(i);
  vc<unsigned> vertices, edges;
  auto dfs = [&](auto &self, int v, unsigned vm, unsigned em) -> void
  {
    vertices.eb(vm), edges.eb(em);
    for (int id : out[v]) self(self, es[id].to, vm | 1U << es[id].to, em | 1U << id);
  };
  for (int v = 0; v < g.template size<int>(); v++) dfs(dfs, v, 1U << v, 0);
  return {vertices, edges};
}

template <class G>
vc<unsigned> walk_masks(const G &g, bool vertices, bool partition)
{
  const int n = g.template size<int>();
  const auto es = g.edges();
  const int bits = vertices ? n : int(es.size()), masks = 1U << bits;
  vvc<int> out(n);
  for (int i = 0; i < int(es.size()); i++) out[es[i].from].eb(i);
  vc<bool> seen(size_t(n) * masks), possible(masks);
  vc<pair<int, unsigned>> queue;
  for (int v = 0; v < n; v++)
  {
    const unsigned mask = vertices ? 1U << v : 0;
    seen[size_t(v) * masks + mask] = true, queue.eb(v, mask);
  }
  for (int i = 0; i < int(queue.size()); i++)
  {
    const auto [v, mask] = queue[i];
    possible[mask] = true;
    for (int id : out[v])
    {
      if (!vertices && partition && (mask >> id & 1)) continue;
      const int to = es[id].to;
      const unsigned next = mask | 1U << (vertices ? to : id);
      const size_t index = size_t(to) * masks + next;
      if (!seen[index]) seen[index] = true, queue.eb(to, next);
    }
  }
  vc<unsigned> result;
  for (int mask = 0; mask < masks; mask++) if (possible[mask]) result.eb(mask);
  return result;
}

template <bool vertices, class G, class Paths>
void check_paths(const G &g, const Paths &paths, bool partition, bool simple)
{
  const int n = g.template size<int>();
  map<int, typename G::E> edge;
  for (const auto &e : g.edges()) edge[e.index] = e;
  vc<int> used(n);
  map<int, int> used_edges;
  for (const auto &p : paths)
  {
    const auto &path = [&]() -> const vc<typename G::E> &
    {
      if constexpr (vertices) return p.second;
      else return p;
    }();
    int v;
    if constexpr (vertices) v = p.first;
    else
    {
      assert(!path.empty());
      v = path.front().from;
    }
    set<int> seen;
    auto visit = [&](int vertex)
    {
      assert(0 <= vertex && vertex < n);
      if (simple) assert(seen.insert(vertex).second);
      used[vertex]++;
    };
    visit(v);
    for (const auto &e : path)
    {
      assert(edge.count(e.index));
      const auto &original = edge.at(e.index);
      assert(e.from == v && e.from == original.from && e.to == original.to);
      assert(e.cost == original.cost);
      used_edges[e.index]++;
      v = e.to;
      visit(v);
    }
  }
  if constexpr (vertices)
    for (int count : used) assert(partition ? count == 1 : count >= 1);
  else
    for (const auto &[id, e] : edge) assert(partition ? used_edges[id] == 1 : used_edges[id] >= 1);
}

template <class G>
void check_dag(const G &g)
{
  const auto [vertices, edges] = dag_paths(g);
  const int n = g.template size<int>(), m = g.template num_of_edges<int>();
  const int vpart = cover_dp(n, vertices, true), vcover = cover_dp(n, vertices, false);
  const int epart = cover_dp(m, edges, true), ecover = cover_dp(m, edges, false);
  vc<pair<int, vc<typename G::E>>> vp = dag_vertex_path_partition(g), covered_vertices = dag_vertex_path_cover(g);
  vvc<typename G::E> ep = dag_edge_path_partition(g), ec = dag_edge_path_cover(g);
  assert(int(vp.size()) == vpart && dag_vertex_path_partition_size(g) == vpart);
  assert(int(covered_vertices.size()) == vcover && dag_vertex_path_cover_size(g) == vcover);
  assert(int(ep.size()) == epart && dag_edge_path_partition_size(g) == epart);
  assert(int(ec.size()) == ecover && dag_edge_path_cover_size(g) == ecover);
  check_paths<true>(g, vp, true, true), check_paths<true>(g, covered_vertices, false, true);
  check_paths<false>(g, ep, true, true), check_paths<false>(g, ec, false, true);
  assert(vertex_walk_cover_size(g) == vcover && edge_walk_cover_size(g) == ecover);
  assert(edge_walk_partition_size(g) == epart);
}

int main()
{
  mt19937 rng(20261016);
  for (int trial = 0; trial < 650; trial++)
  {
    const int n = rng() % 7, m = n < 2 ? 0 : rng() % 9;
    vc<int> order(n);
    iota(order.begin(), order.end(), 0), shuffle(order.begin(), order.end(), rng);
    vc<tuple<int, int, ll>> es;
    for (int i = 0; i < m; i++)
    {
      int u = rng() % n, v = (u + 1 + rng() % (n - 1)) % n;
      if (u > v) swap(u, v);
      es.eb(order[u], order[v], ll(rng()));
    }
    GraphDirected<ll, true> g(n, es);
    for (int i = 0; i < m; i++) if (rng() % 6 == 0) g.erase_edge(i);
    check_dag(g);
  }
  for (int trial = 0; trial < 650; trial++)
  {
    const int n = rng() % 7, m = n ? rng() % 9 : 0;
    vc<pair<int, int>> es;
    for (int i = 0; i < m; i++) es.eb(rng() % n, rng() % n);
    vc<tuple<int, int, ll>> weighted;
    for (auto [u, v] : es) weighted.eb(u, v, ll(rng()) - rng());
    GraphDirected<void, true> g(n, es);
    GraphDirected<ll, true> w(n, weighted);
    for (int i = 0; i < m; i++) if (rng() % 7 == 0) g.erase_edge(i), w.erase_edge(i);
    assert(vertex_walk_cover_size(g) == cover_dp(n, walk_masks(g, true, false), false));
    assert(edge_walk_cover_size(g) == cover_dp(g.template num_of_edges<int>(), walk_masks(g, false, false), false));
    const int partition = cover_dp(g.template num_of_edges<int>(), walk_masks(g, false, true), true);
    assert(edge_walk_partition_size(g) == partition);
    const vvc<Edge<void>> paths = edge_walk_partition(g);
    const vvc<Edge<ll>> weighted_paths = edge_walk_partition(w);
    assert(int(paths.size()) == partition && int(weighted_paths.size()) == partition);
    check_paths<false>(g, paths, true, false);
    check_paths<false>(w, weighted_paths, true, false);
  }
  {
    GraphDirected<> g(5, vc<pair<int, int>>{{0, 2}, {1, 2}, {2, 3}, {2, 4}});
    assert(dag_vertex_path_partition_size(g) == 3 && dag_vertex_path_cover_size(g) == 2);
    check_dag(g);
    GraphDirected<> h(4, vc<pair<int, int>>{{0, 1}, {0, 1}, {1, 2}, {2, 3}, {2, 3}});
    assert(dag_edge_path_partition_size(h) == 3 && dag_edge_path_cover_size(h) == 2);
    check_dag(h);
    GraphDirected<> isolated(3, vc<pair<int, int>>{});
    check_dag(isolated);
    GraphDirected<> k(3, vc<pair<int, int>>{{0, 0}, {1, 1}});
    assert(vertex_walk_cover_size(k) == 3 && edge_walk_cover_size(k) == 2);
    GraphDirected<> q(2, vc<pair<int, int>>{{0, 0}, {1, 1}, {0, 1}, {0, 1}});
    assert(vertex_walk_cover_size(q) == 1 && edge_walk_cover_size(q) == 2);
    GraphDirected<> fork(3, vc<pair<int, int>>{{0, 0}, {1, 1}, {2, 2}, {0, 2}, {1, 2}});
    assert(vertex_walk_cover_size(fork) == 2);
    assert(cover_dp(3, walk_masks(fork, true, false), false) == 2);
  }
  {
    const int n = 200000;
    vc<pair<int, int>> es;
    for (int v = 1; v < n; v++) es.eb(v - 1, v);
    GraphDirected<> g(n, es);
    const auto ids = scc<void, int>(g);
    for (int v = 0; v < n; v++) assert(ids[v] == v);
    assert(vertex_walk_cover_size(g) == 1 && dag_edge_path_partition_size(g) == 1);
  }
  cout << "Hello World\n";
}
