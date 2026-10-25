#pragma once

#include "maxflow.hpp"
#include "bipartite_matching.hpp"
#include "../graph/scc.hpp"
#include "../graph/contraction.hpp"
#include "../graph/connected_components.hpp"

/**
 * @brief 有向グラフの頂点・辺のパス分割とパス・ウォーク被覆
 * @docs docs/flow/path_cover.md
 */

namespace path_cover_detail
{
template <class Edge>
vc<int> weak_components(int n, const vc<Edge> &es)
{
  vc<pair<int, int>> undirected;
  undirected.reserve(es.size());
  for (const auto &e : es) undirected.eb(e.from, e.to);
  return connected_component_ids<int>(GraphUndirected<>(n, undirected));
}

struct CoverFlow
{
  int count;
  vc<int> starts, use;
};

// 既知の実行可能流から t -> s へ流し戻し、流量を最小化する。
template <class Edge>
CoverFlow cover_flow(int n, const vc<Edge> &es, bool vertices)
{
  const int m = es.size(), bound = vertices ? n : m;
  if (bound == 0) return {0, vc<int>(n), vc<int>(m)};
  const int s = vertices ? 2 * n : n, t = s + 1;
  MaxFlow<int> flow(t + 1);
  vc<int> starts(n, vertices ? 1 : 0), ends = starts;
  if (!vertices)
  {
    for (const auto &e : es) starts[e.from]++, starts[e.to]--;
    for (int v = 0; v < n; v++) ends[v] = max(0, -starts[v]), starts[v] = max(0, starts[v]);
  }
  vc<int> source_edge(n), original(m);
  int initial = 0;
  for (int v = 0; v < n; v++)
  {
    initial += starts[v];
    if (vertices) flow.add_edge(v, n + v, bound - 1); // 下限 1 を除いた残余容量。
    source_edge[v] = flow.add_edge(s, v, bound);
    flow.change_edge(source_edge[v], bound, starts[v]);
    const int id = flow.add_edge(vertices ? n + v : v, t, bound);
    flow.change_edge(id, bound, ends[v]);
  }
  for (int i = 0; i < m; i++)
    original[i] = flow.add_edge(vertices ? n + es[i].from : es[i].from, es[i].to,
                                vertices ? bound : bound - 1);
  CoverFlow result{initial - flow.flow(t, s, initial), vc<int>(n), vc<int>(m)};
  for (int v = 0; v < n; v++) result.starts[v] = flow.get_edge(source_edge[v]).flow;
  for (int i = 0; i < m; i++) result.use[i] = int(!vertices) + flow.get_edge(original[i]).flow;
  return result;
}

template <bool vertices, class E>
auto restore(int n, const vc<E> &es, CoverFlow state)
{
  using Path = conditional_t<vertices, pair<int, vc<E>>, vc<E>>;
  vc<pair<int, int>> entries;
  entries.reserve(es.size());
  for (int i = 0; i < int(es.size()); i++) entries.eb(es[i].from, i);
  CSR<int> out(n, entries);
  vc<int> iter(n);
  vc<Path> result;
  result.reserve(state.count);
  for (int root = 0; root < n; root++) for (int k = 0; k < state.starts[root]; k++)
  {
    vc<E> path;
    int v = root;
    while (true)
    {
      const auto row = out[v];
      while (iter[v] < int(row.size()) && state.use[row[iter[v]]] == 0) iter[v]++;
      if (iter[v] == int(row.size())) break;
      const int id = row[iter[v]];
      state.use[id]--;
      path.eb(es[id]);
      v = es[id].to;
    }
    if constexpr (vertices) result.eb(root, move(path));
    else result.eb(move(path));
  }
  for (int use : state.use) assert(use == 0); // DAG なので循環流が残らない。
  return result;
}
}

// (1) DAG の各頂点をちょうど 1 回ずつ使うパス分割。Hopcroft–Karp を使う。
// 頂点が対象のときは (始点, 辺列) を返し、長さ 0 のパスも表す。
template <class Cost, bool is_erasable>
vc<pair<int, vc<Edge<Cost>>>> dag_vertex_path_partition(const GraphDirected<Cost, is_erasable> &g)
{
  const int n = g.template size<int>();
  vc<pair<int, int>> es;
  es.reserve(g.template num_of_edges<int>());
  for (int v = 0; v < n; v++) for (const auto &e : g.out_edges(v)) es.eb(v, n + e.to);
  GraphUndirected<> bipartite(2 * n, es);
  vc<bool> color(2 * n);
  for (int v = n; v < 2 * n; v++) color[v] = true;
  const auto matching = bipartite_matching(bipartite, color);
  vc<int> next(n, -1), previous(n, -1), edge_position(n, -1);
  for (auto [u, v] : matching) next[u] = v - n, previous[v - n] = u;
  for (int v = 0; v < n; v++) if (next[v] != -1)
  {
    const auto row = g.out_edges(v);
    for (int i = 0; i < row.size(); i++) if (row[i].to == next[v]) { edge_position[v] = i; break; }
  }
  vc<pair<int, vc<Edge<Cost>>>> result;
  result.reserve(n - matching.size());
  int visited = 0;
  for (int root = 0; root < n; root++) if (previous[root] == -1)
  {
    vc<Edge<Cost>> path;
    for (int v = root; v != -1; v = next[v])
    {
      visited++;
      if (next[v] != -1) path.eb(g.out_edges(v)[edge_position[v]]);
    }
    result.eb(root, move(path));
  }
  assert(visited == n);
  return result;
}

template <class Cost, bool is_erasable>
int dag_vertex_path_partition_size(const GraphDirected<Cost, is_erasable> &g)
{ return dag_vertex_path_partition(g).size(); }

// (4) DAG の各辺をちょうど 1 回ずつ使う。次数だけで O(n+m)。
template <class Cost, bool is_erasable>
int dag_edge_path_partition_size(const GraphDirected<Cost, is_erasable> &g)
{
  const auto in = g.template indegs<int>(), out = g.template outdegs<int>();
  int result = 0;
  for (int v = 0; v < int(in.size()); v++) result += max(0, out[v] - in[v]);
  return result;
}

template <class Cost, bool is_erasable>
vvc<Edge<Cost>> dag_edge_path_partition(const GraphDirected<Cost, is_erasable> &g)
{
  const int n = g.template size<int>();
  const auto es = g.edges();
  const int m = es.size();
  vc<pair<int, int>> incoming, outgoing;
  incoming.reserve(m), outgoing.reserve(m);
  for (int i = 0; i < m; i++) incoming.eb(es[i].to, i), outgoing.eb(es[i].from, i);
  CSR<int> in(n, incoming), out(n, outgoing);
  vc<int> next(m, -1), previous(m, -1);
  for (int v = 0; v < n; v++)
    for (int i = 0; i < min(in[v].size(), out[v].size()); i++)
      next[in[v][i]] = out[v][i], previous[out[v][i]] = in[v][i];
  vvc<Edge<Cost>> result;
  int visited = 0;
  for (int root = 0; root < m; root++) if (previous[root] == -1)
  {
    vc<Edge<Cost>> path;
    for (int id = root; id != -1; id = next[id])
    {
      path.eb(es[id]), visited++;
    }
    result.eb(move(path));
  }
  assert(visited == m);
  return result;
}

// (2), (5) DAG の頂点・辺を 1 回以上使うパス被覆。
template <class Cost, bool is_erasable>
int dag_vertex_path_cover_size(const GraphDirected<Cost, is_erasable> &g)
{ return path_cover_detail::cover_flow(g.template size<int>(), g.edges(), true).count; }

template <class Cost, bool is_erasable>
vc<pair<int, vc<Edge<Cost>>>> dag_vertex_path_cover(const GraphDirected<Cost, is_erasable> &g)
{
  const auto es = g.edges();
  const int n = g.template size<int>();
  return path_cover_detail::restore<true>(n, es, path_cover_detail::cover_flow(n, es, true));
}

template <class Cost, bool is_erasable>
int dag_edge_path_cover_size(const GraphDirected<Cost, is_erasable> &g)
{ return path_cover_detail::cover_flow(g.template size<int>(), g.edges(), false).count; }

template <class Cost, bool is_erasable>
vvc<Edge<Cost>> dag_edge_path_cover(const GraphDirected<Cost, is_erasable> &g)
{
  const auto es = g.edges();
  const int n = g.template size<int>();
  return path_cover_detail::restore<false>(n, es, path_cover_detail::cover_flow(n, es, false));
}

// (3) 各頂点を 1 回以上使うウォーク被覆。強連結成分内を 1 頂点にする。
template <class Cost, bool is_erasable>
int vertex_walk_cover_size(const GraphDirected<Cost, is_erasable> &g)
{
  vc<pair<int, int>> es;
  es.reserve(g.template num_of_edges<int>());
  for (int v = 0; v < g.template size<int>(); v++)
    for (const auto &e : g.out_edges(v)) es.eb(v, e.to);
  GraphDirected<> unweighted(g.template size<int>(), es);
  const auto ids = scc<void, int>(unweighted);
  return dag_vertex_path_cover_size(contracted_graph(unweighted, ids));
}

// (6) 各辺を 1 回以上使うウォーク被覆。内部辺・多重辺をそのまま扱う。
template <class Cost, bool is_erasable>
int edge_walk_cover_size(const GraphDirected<Cost, is_erasable> &g)
{
  const int n = g.template size<int>();
  const auto es = g.edges();
  const auto state = path_cover_detail::cover_flow(n, es, false);
  const auto ids = path_cover_detail::weak_components(n, es);
  vc<bool> has_edge(n);
  for (const auto &e : es) has_edge[ids[e.from]] = true;
  vc<int> starts(n);
  for (int v = 0; v < n; v++) starts[ids[v]] += state.starts[v];
  int result = 0;
  for (int id = 0; id < n; id++) if (has_edge[id]) result += max(1, starts[id]);
  return result;
}

// (7) 一般の有向グラフでも、辺をちょうど 1 回ずつ使うウォーク分割は O(n+m)。
template <class Cost, bool is_erasable>
int edge_walk_partition_size(const GraphDirected<Cost, is_erasable> &g)
{
  const int n = g.template size<int>();
  const auto es = g.edges();
  const auto ids = path_cover_detail::weak_components(n, es);
  vc<int> excess(n), starts(n);
  vc<bool> has_edge(n);
  for (const auto &e : es)
    excess[e.from]++, excess[e.to]--, has_edge[ids[e.from]] = true;
  for (int v = 0; v < n; v++) starts[ids[v]] += max(0, excess[v]);
  int result = 0;
  for (int id = 0; id < n; id++) if (has_edge[id]) result += max(1, starts[id]);
  return result;
}

template <class Cost, bool is_erasable>
vvc<Edge<Cost>> edge_walk_partition(const GraphDirected<Cost, is_erasable> &g)
{
  const int n = g.template size<int>();
  const auto original = g.edges();
  const int m = original.size();
  const auto ids = path_cover_detail::weak_components(n, original);
  struct Arc { int from, to; };
  vc<Arc> es;
  es.reserve(2 * m);
  vc<int> excess(n), root(n, -1);
  for (const auto &e : original)
  {
    es.push_back({e.from, e.to});
    excess[e.from]++, excess[e.to]--;
    root[ids[e.from]] = e.from;
  }
  vc<pair<int, int>> surplus, deficit;
  for (int v = 0; v < n; v++)
  {
    if (excess[v] > 0) surplus.eb(ids[v], v);
    if (excess[v] < 0) deficit.eb(ids[v], v);
  }
  CSR<int> sources(n, surplus), sinks(n, deficit);
  for (int id = 0; id < n; id++)
  {
    int i = 0, j = 0;
    while (i < sources[id].size() && j < sinks[id].size())
    {
      const int u = sinks[id][j], v = sources[id][i];
      const int count = min(-excess[u], excess[v]);
      // 不足する出辺を仮想辺で補い、各成分を Euler 閉路にする。
      for (int k = 0; k < count; k++) es.push_back({u, v});
      excess[u] += count, excess[v] -= count;
      if (excess[u] == 0) j++;
      if (excess[v] == 0) i++;
    }
  }
  vc<pair<int, int>> entries;
  entries.reserve(es.size());
  for (int i = 0; i < int(es.size()); i++) entries.eb(es[i].from, i);
  CSR<int> out(n, entries);
  vc<int> iter(n), stack, edge_stack, circuit;
  vvc<Edge<Cost>> result;
  for (int r : root) if (r != -1)
  {
    stack.clear(), edge_stack.clear(), circuit.clear();
    stack.eb(r);
    while (!stack.empty())
    {
      const int v = stack.back();
      if (iter[v] < out[v].size())
      {
        const int id = out[v][iter[v]++];
        stack.eb(es[id].to), edge_stack.eb(id);
      }
      else
      {
        stack.pop_back();
        if (!edge_stack.empty()) circuit.eb(edge_stack.back()), edge_stack.pop_back();
      }
    }
    reverse(circuit.begin(), circuit.end());
    int split = -1;
    for (int i = 0; i < int(circuit.size()); i++) if (circuit[i] >= m) { split = i; break; }
    if (split == -1)
    {
      vc<Edge<Cost>> path;
      for (int id : circuit) path.eb(original[id]);
      result.eb(move(path));
    }
    else
    {
      vc<Edge<Cost>> path;
      int position = split;
      for (int k = 1; k <= int(circuit.size()); k++)
      {
        if (++position == int(circuit.size())) position = 0;
        const int id = circuit[position];
        if (id < m) path.eb(original[id]);
        else
        {
          assert(!path.empty());
          result.eb(move(path));
          path.clear();
        }
      }
    }
  }
  return result;
}
