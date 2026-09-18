#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/sssp.hpp"
#include "graph/scc.hpp"
#include "graph/connected_components.hpp"
#include "graph/contraction.hpp"
#include "graph/subgraphs.hpp"
#include "graph/cycle_detection.hpp"
#include "graph/triangles.hpp"
#include "graph/cliques.hpp"

using Key = tuple<int, int, int>;

template <bool directed, bool erasable>
void storage(int n, const vc<pair<int, int>> &uv, mt19937 &rng)
{
  Graph<directed, void, erasable> g(n, uv);
  vc<bool> active(uv.size(), true);
  auto check = [&]
  {
    vvc<Key> expected(n);
    vc<Key> all;
    vvc<int> matrix(n, vc<int>(n));
    vc<int> indeg(n), outdeg(n);
    repi(i, uv.size()) if (active[i])
    {
      auto [u, v] = uv[i];
      expected[u].eb(u, v, i);
      if constexpr (!directed)
      {
        if (u != v) expected[v].eb(v, u, i);
        if (u > v) swap(u, v);
      }
      all.eb(u, v, i);
    }
    auto adj = g.adj_list();
    repi(u, n)
    {
      auto row = g.out_edges(u);
      static_assert(is_same_v<decltype(row[0]), Edge<void>>);
      vc<Key> actual;
      vc<int> vertices;
      int j = 0;
      for (const auto &e : row)
      {
        assert(e.from == u && e.cost == 1 && int(e) == e.to);
        actual.eb(e.from, e.to, e.index);
        vertices.eb(e.to);
        assert(row.at(j).index == e.index && adj[u][j].index == e.index);
        matrix[u][e.to]++, indeg[e.to]++, outdeg[u]++, j++;
      }
      assert(row.template size<size_t>() == expected[u].size() && row.empty() == expected[u].empty());
      if (!row.empty())
      {
        assert(row.begin()->from == u);
        assert(row.front().index == get<2>(actual.front()));
        assert(row.back().index == get<2>(actual.back()));
      }
      assert(vertices == g.template out_vertices<int>(u));
      sort(ALL(actual)), sort(ALL(expected[u]));
      assert(actual == expected[u]);
    }
    vc<Key> actual;
    fec(e : g.edges()) actual.eb(e.from, e.to, e.index);
    sort(ALL(actual)), sort(ALL(all));
    assert(actual == all && g.template num_of_edges<size_t>() == all.size());
    assert(matrix == g.template adj_matrix_ecnt<int>());
    assert(indeg == g.template indegs<int>() && outdeg == g.template outdegs<int>());
  };
  check();
  if constexpr (erasable)
  {
    vc<int> order(uv.size());
    iota(ALL(order), 0);
    shuffle(ALL(order), rng);
    fec(i : order) { g.erase_edge(i); active[i] = false; check(); }
  }
}

template <bool directed>
void algorithms(int n, const vc<pair<int, int>> &uv)
{
  Graph<directed> g(n, uv);
  vvc<ll> dist(n, vc<ll>(n, INF));
  repi(i, n) dist[i][i] = 0;
  fec([u, v] : uv)
  {
    chmin(dist[u][v], 1LL);
    if constexpr (!directed) chmin(dist[v][u], 1LL);
  }
  repi(k, n) repi(i, n) repi(j, n)
    if (dist[i][k] != INF && dist[k][j] != INF) chmin(dist[i][j], dist[i][k] + dist[k][j]);
  ShortestPath sp(g);
  static_assert(is_same_v<typename decltype(sp)::Dist, ll>);
  repi(s, n)
  {
    assert(sp.solve(s) == dist[s]);
    vc<ll> counts(n);
    counts[s] = 1;
    repi(d, n) repi(u, n) if (dist[s][u] == d)
      fec(e : g.out_edges(u)) if (dist[s][e.to] == d + 1) counts[e.to] += counts[u];
    assert(sp.template count_paths<ll>() == counts);
    repi(t, n)
    {
      auto path = sp.path(t);
      static_assert(is_same_v<decltype(path), vc<Edge<void>>>);
      if (dist[s][t] == INF) { assert(path.empty()); continue; }
      assert(path.size() == size_t(dist[s][t]));
      int v = s;
      fec(e : path)
      {
        assert(e.from == v && e.cost == 1);
        auto [a, b] = uv[e.index];
        if constexpr (!directed) if (a != e.from) swap(a, b);
        assert(a == e.from && b == e.to);
        v = e.to;
      }
      assert(v == t);
    }
  }
  if (n)
  {
    assert(sp.bfs01(0) == dist[0]);
    assert(sp.dijkstra(0) == dist[0]);
    assert(sp.dijkstra_dense(0) == dist[0]);
    assert(sp.dial(0, 1) == dist[0]);
    assert(sp.bellman_ford(0) == dist[0]);
    assert(sp.solve(0, n - 1)[n - 1] == dist[0][n - 1]);
    assert(sp.path(n - 1).size() == (dist[0][n - 1] == INF ? 0 : size_t(dist[0][n - 1])));
    ShortestPath<directed, void, 1000> finite(g);
    auto ds = finite.solve(0);
    repi(t, n) assert(ds[t] == (dist[0][t] == INF ? 1000 : dist[0][t]));
  }
  if constexpr (directed)
  {
    auto ids = scc(g);
    repi(u, n) repi(v, n)
      assert((ids[u] == ids[v]) == (dist[u][v] != INF && dist[v][u] != INF));
    fec([u, v] : uv) assert(ids[u] <= ids[v]);
    auto rg = rev_graph(g);
    auto mat = g.template adj_matrix_ecnt<int>();
    auto rmat = rg.template adj_matrix_ecnt<int>();
    repi(u, n) repi(v, n) assert(mat[u][v] == rmat[v][u]);
  }
  else
  {
    auto ids = connected_component_ids(g);
    repi(u, n) repi(v, n) assert((ids[u] == ids[v]) == (dist[u][v] != INF));
  }
  // 単位重み版ともサイクルの有無を比較し、復元した辺を入力と照合する。
  Graph<directed, ll> weighted(n, uv);
  auto [found, cycle] = cycle_detection(g);
  assert(found == cycle_detection(weighted).first);
  assert(found == !cycle.empty());
  vc<bool> used(uv.size());
  repi(i, cycle.size())
  {
    auto e = cycle[i];
    assert(e.to == cycle[(i + 1) % cycle.size()].from && !used[e.index]);
    auto [u, v] = uv[e.index];
    if constexpr (!directed) if (u != e.from) swap(u, v);
    assert(u == e.from && v == e.to && e.cost == 1);
    used[e.index] = true;
  }
}

template <bool directed>
void transformations()
{
  Graph<directed> g(5, vc<pair<int, int>>{{0, 1}, {0, 1}, {1, 1}, {1, 2}, {2, 3}, {3, 2}});
  vc<int> ids{0, 0, 1, 1, 2};
  auto [gs, vids] = subgraphs(g, ids);
  assert((vids == vc<int>{0, 1, 0, 1, 0}));
  assert(gs.size() == 3 && gs[0].num_of_edges() == 3 && gs[1].num_of_edges() == 2);
  assert(gs[2].size() == 1 && gs[2].edges().empty());
  auto contracted = contracted_graph(g, ids);
  assert(contracted.size() == 3 && contracted.num_of_edges() == 1);
  auto e = contracted.edges().front();
  assert(e.from == 0 && e.to == 1 && e.cost == 1);
  Graph<directed> empty(0, vc<pair<int, int>>{});
  assert(subgraphs(empty, vc<int>{}).first.empty());
  assert(contracted_graph(empty, vc<int>{}).size() == 0);
}

void enumeration(mt19937 &rng)
{
  const int n = 7;
  vc<pair<int, int>> uv;
  vvc<bool> adj(n, vc<bool>(n));
  repi(u, n) repi(v, u + 1, n) if (rng() % 2)
    uv.eb(u, v), adj[u][v] = adj[v][u] = true;
  GraphUndirected<> g(n, uv);
  vc<int> expected, got, triples;
  repi(mask, 1, 1 << n)
  {
    bool ok = true;
    repi(u, n) repi(v, u + 1, n)
      if ((mask >> u & 1) && (mask >> v & 1) && !adj[u][v]) ok = false;
    if (ok) expected.eb(mask);
  }
  cliques(g, [&](const auto &vs)
  {
    int mask = 0;
    fec(v : vs) mask |= 1 << v;
    got.eb(mask);
  });
  sort(ALL(got));
  assert(got == expected);
  triangles(g, [&](int u, int v, int w) { triples.eb((1 << u) | (1 << v) | (1 << w)); });
  got.clear();
  fec(mask : expected) if (__builtin_popcount(mask) == 3) got.eb(mask);
  sort(ALL(triples));
  assert(triples == got);
}

int main()
{
  static_assert(is_same_v<GraphDirected<>, Graph<true, void>>);
  static_assert(Edge<>::cost == 1);
  static_assert(is_same_v<decltype(Edge<>::cost), const ll>);
  static_assert(!is_assignable_v<decltype((declval<Edge<>&>().cost)), int>);
  static_assert(!is_constructible_v<GraphDirected<>, int, vc<pair<int, int>>, int>);
  static_assert(!is_constructible_v<GraphDirected<>, int, vc<tuple<int, int, int>>>);
  GraphDirected<> from_edges(3, vc<Edge<>>{{1, 2, 99}, {0, 1, 42}});
  assert(from_edges.out_edges(1).front().index == 0);
  assert(from_edges.out_edges(0).front().index == 1);
  auto path = rev_path(vc<Edge<>>{{0, 1, 7}, {1, 2, 8}});
  assert(path[0].from == 2 && path[0].to == 1 && path[0].index == 8 && path[0].cost == 1);
  assert(path[1].from == 1 && path[1].to == 0 && path[1].index == 7);
  transformations<true>();
  transformations<false>();
  mt19937 rng(72193);
  repi(iter, 80)
  {
    int n = iter % 10;
    vc<pair<int, int>> uv;
    if (n) repi(i, rng() % 25) uv.eb(rng() % n, rng() % n);
    storage<true, false>(n, uv, rng);
    storage<false, false>(n, uv, rng);
    storage<true, true>(n, uv, rng);
    storage<false, true>(n, uv, rng);
    algorithms<true>(n, uv);
    algorithms<false>(n, uv);
    enumeration(rng);
  }
  PRINT("Hello World");
}
