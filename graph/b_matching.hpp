#pragma once

#include "general_matching.hpp"
#include "../flow/maxflow.hpp"

/**
 * @brief 一般グラフの b-matching・f-factor
 * @docs docs/graph/b_matching.md
 */

//! 数学的手法: https://alexander-karzanov.net/Publications/93_path_probl_skew.pdf （1993, Sections 2.3–2.6）。
//! 予備 PDF: https://ftpmirror.your.org/pub/misc/bitsavers/pdf/stanford/Stanford_CS_TR_Collection_2025-12-12/OCR/CS-TR-93-1489-ocr.pdf
namespace internal
{
struct SkewRegularPath
{
  int n, pairs, source, generation = 0;
  const vc<pair<int, int>> &arc;
  CSR<int> outgoing;
  UnionFind<UFDataEmpty<>> uf;
  vc<int> top, entry, side, sample, forest_parent, stamp, color;
  vvc<int> cycle, cycle_node;
  vc<unsigned char> deleted;
  vc<int> queue;

  SkewRegularPath(int n, const vc<pair<int, int>> &arc)
      : n(n), pairs(n / 2), arc(arc), uf(pairs), top(pairs),
        entry(pairs, -1), side(pairs, -1), sample(pairs), forest_parent(pairs, -1),
        stamp(pairs), color(pairs), cycle(pairs), cycle_node(pairs), deleted(arc.size())
  {
    iota(ALL(top), 0), iota(ALL(sample), 0);
    vc<int> degree(n);
    for (auto [u, v] : arc) { (void)v; degree[u]++; }
    outgoing = CSR<int>(degree);
    fill(ALL(degree), 0);
    for (int a = 0; a < int(arc.size()); a++) outgoing[arc[a].first][degree[arc[a].first]++] = a;
  }

  int component(int v) { return top[uf.leader(v / 2)]; }
  int head(int a)
  {
    const int t = component(arc[a].second);
    return 2 * t + (t < pairs ? arc[a].second % 2 : a != entry[t]);
  }
  int tail(int a) { return head(a ^ 1) ^ 1; }
  void enqueue(int v) { for (int a : outgoing[v]) queue.eb(a); }

  int common_ancestor(int a, int b)
  {
    ++generation;
    int v[2] = {a, b};
    while (v[0] != -1 || v[1] != -1)
      for (int s = 0; s < 2; s++) if (v[s] != -1)
      {
        const int t = v[s];
        if (stamp[t] == generation)
        {
          assert(color[t] != s);
          return t;
        }
        stamp[t] = generation, color[t] = s;
        v[s] = entry[t] == -1 ? -1 : tail(entry[t]) / 2;
      }
    assert(false);
    return -1;
  }

  vc<int> restore(vc<int> path)
  {
    vc<unsigned char> active(entry.size(), true);
    vvc<int> ancestors(pairs);
    auto root = [&](int v)
    {
      auto &st = ancestors[v / 2];
      if (st.empty())
      {
        int t = v / 2;
        while (true)
        {
          st.eb(t);
          if (forest_parent[t] == -1 || !active[forest_parent[t]]) break;
          t = forest_parent[t];
        }
      }
      while (!active[st.back()]) st.pop_back();
      return st.back();
    };
    auto current_head = [&](int a)
    {
      const int t = root(arc[a].second);
      return 2 * t + (t < pairs ? arc[a].second % 2 : a != entry[t]);
    };
    auto current_tail = [&](int a) { return current_head(a ^ 1) ^ 1; };
    list<int> expanded(ALL(path));
    auto previous = expanded.begin(), next = previous;
    if (next == expanded.end()) return {};
    ++next;
    while (next != expanded.end())
    {
      const int a = *previous, b = *next;
      if (arc[a].second == arc[b].first) { previous = next++; continue; }
      const int t = root(arc[a].second);
      assert(t == root(arc[b].first) && t >= pairs);
      const bool reflect = a != entry[t];
      assert(!reflect || b == (entry[t] ^ 1));
      const int in = reflect ? b ^ 1 : a, out = reflect ? a ^ 1 : b;
      active[t] = false;
      const int target = current_tail(out), start = current_head(in);
      vc<int> insert;
      bool found = start == target;
      for (int direction = 0; direction < 2 && !found; direction++)
      {
        insert.clear();
        for (int i = 0; i < int(cycle[t].size()); i++)
        {
          const int e = direction == 0 ? cycle[t][i] : cycle[t][cycle[t].size() - 1 - i] ^ 1;
          insert.eb(e);
          const int reached = direction == 0 ? cycle_node[t][i + 1]
              : cycle_node[t][cycle[t].size() - 1 - i] ^ 1;
          if (reached == target) { found = true; break; }
        }
      }
      assert(found);
      if (reflect)
      {
        reverse(ALL(insert));
        for (int &e : insert) e ^= 1;
      }
      if (!insert.empty()) next = expanded.insert(next, ALL(insert));
    }
    return vc<int>(ALL(expanded));
  }

  optional<vc<int>> run(int s)
  {
    source = s / 2;
    side[source] = s % 2;
    enqueue(s);
    for (int cursor = 0; cursor < int(queue.size()); cursor++)
    {
      const int a = queue[cursor];
      if (deleted[a]) continue;
      const int x = tail(a), y = head(a), u = x / 2, v = y / 2;
      if (side[u] != x % 2) continue;
      if (u == v && u >= pairs) continue;
      if (side[v] == y % 2) continue;
      if (side[v] == -1)
      {
        assert(v < pairs);
        side[v] = y % 2, entry[v] = a;
        enqueue(arc[a].second);
        continue;
      }
      const int lca = common_ancestor(u, v);
      vc<int> left, right, children{lca};
      for (int t = u; t != lca; t = tail(entry[t]) / 2)
        left.eb(entry[t]), children.eb(t);
      for (int t = v; t != lca; t = tail(entry[t]) / 2)
        right.eb(entry[t]), children.eb(t);
      vc<int> route;
      for (auto it = left.rbegin(); it != left.rend(); ++it) route.eb(*it);
      route.eb(a);
      for (int e : right) route.eb(e ^ 1);
      if (lca == source) return restore(move(route));
      const int t = entry.size();
      entry.eb(entry[lca]), side.eb(0), sample.eb(sample[lca]);
      forest_parent.eb(-1), stamp.eb(0), color.eb(0);
      vc<int> route_nodes{tail(route[0])};
      for (int e : route) route_nodes.eb(head(e));
      cycle_node.eb(move(route_nodes));
      cycle.eb(move(route));
      for (int e : cycle.back()) deleted[e] = deleted[e ^ 1] = true;
      for (int child : children)
      {
        if (child < pairs) enqueue(2 * child + (side[child] ^ 1));
        forest_parent[child] = t;
        uf.merge(sample[lca], sample[child]);
      }
      top[uf.leader(sample[lca])] = t;
    }
    return nullopt;
  }
};
}

//! 数学的手法: https://arxiv.org/abs/math/0304290v2 （Sections 2.1, 3, 5）。
namespace internal
{
template <class Cap>
struct GoldbergKarzanovBMatching
{
  int n, m;
  const vc<pair<int, int>> &edges;
  const vc<Cap> &b, &capacity;

  GoldbergKarzanovBMatching(int n, const vc<pair<int, int>> &edges,
                           const vc<Cap> &b, const vc<Cap> &capacity)
      : n(n), m(edges.size()), edges(edges), b(b), capacity(capacity) {}

  void round(vc<Cap> &twice)
  {
    vc<pair<int, int>> es = edges;
    vc<Cap> slack(n);
    for (int v = 0; v < n; v++) slack[v] = 2 * b[v];
    for (int e = 0; e < m; e++)
      slack[es[e].first] -= twice[e], slack[es[e].second] -= twice[e];
    for (int v = 0; v < n; v++) es.eb(v, n), twice.eb(slack[v]);
    vc<int> degree(n + 1);
    for (int e = 0; e < int(es.size()); e++) if (twice[e] % 2)
      degree[es[e].first]++, degree[es[e].second]++;
    CSR<int> adj(degree);
    fill(ALL(degree), 0);
    for (int e = 0; e < int(es.size()); e++) if (twice[e] % 2)
    {
      adj[es[e].first][degree[es[e].first]++] = e;
      adj[es[e].second][degree[es[e].second]++] = e;
    }
    struct OddCycle { vc<int> edge, vertex; bool active; };
    vc<OddCycle> pending;
    vc<int> at(n + 1, -1), cursor(n + 1), position(n + 1, -1);
    vc<unsigned char> used(es.size());
    auto adjust = [&](const vc<int> &walk)
    {
      for (int i = 0; i < int(walk.size()); i++)
        if (i % 2) twice[walk[i]]++;
        else twice[walk[i]]--;
    };
    auto process = [&](vc<int> walk, vc<int> vertices)
    {
      if (walk.size() % 2 == 0) { adjust(walk); return; }
      int shared = -1, other = -1;
      for (int i = 0; i < int(vertices.size()); i++) if (at[vertices[i]] != -1)
      { shared = i, other = at[vertices[i]]; break; }
      if (other == -1)
      {
        const int id = pending.size();
        for (int v : vertices) at[v] = id;
        pending.push_back({move(walk), move(vertices), true});
        return;
      }
      auto &old = pending[other];
      int pivot = find(ALL(old.vertex), vertices[shared]) - old.vertex.begin();
      rotate(walk.begin(), walk.begin() + shared, walk.end());
      for (int i = 0; i < int(old.edge.size()); i++)
        walk.eb(old.edge[(pivot + i) % old.edge.size()]);
      adjust(walk);
      old.active = false;
      for (int v : old.vertex) at[v] = -1;
    };
    for (int start = 0; start <= n; start++)
    {
      vc<int> path{start}, path_edges;
      position[start] = 0;
      while (!path.empty())
      {
        const int v = path.back();
        int &i = cursor[v];
        while (i < int(adj[v].size()) && used[adj[v][i]]) i++;
        if (i == int(adj[v].size()))
        {
          assert(path.size() == 1);
          position[v] = -1;
          break;
        }
        const int e = adj[v][i++], w = es[e].first ^ es[e].second ^ v;
        used[e] = true;
        path_edges.eb(e);
        if (position[w] == -1)
        {
          position[w] = path.size();
          path.eb(w);
          continue;
        }
        const int pos = position[w];
        process(vc<int>(path_edges.begin() + pos, path_edges.end()),
                vc<int>(path.begin() + pos, path.end()));
        for (int j = pos + 1; j < int(path.size()); j++) position[path[j]] = -1;
        path.resize(pos + 1), path_edges.resize(pos);
      }
    }
    for (const auto &c : pending) if (c.active)
    {
      assert(find(ALL(c.vertex), n) == c.vertex.end());
      adjust(c.edge);
    }
    twice.resize(m);
    for (Cap &x : twice) { assert(x % 2 == 0); x /= 2; }
  }

  vc<Cap> solve()
  {
    const int s = 2 * n, t = s + 1;
    MaxFlow<Cap> flow(t + 1);
    for (int e = 0; e < m; e++)
    {
      auto [u, v] = edges[e];
      flow.add_edge(2 * u, 2 * v + 1, capacity[e]);
      flow.add_edge(2 * v, 2 * u + 1, capacity[e]);
    }
    for (int v = 0; v < n; v++)
      flow.add_edge(s, 2 * v, b[v]), flow.add_edge(2 * v + 1, t, b[v]);
    const Cap upper = flow.flow(s, t) / 2;
    vc<Cap> selected(m);
    for (int e = 0; e < m; e++)
      selected[e] = flow.get_edge(2 * e).flow + flow.get_edge(2 * e + 1).flow;
    bool integral = true;
    for (Cap x : selected) if (x % 2) { integral = false; break; }
    if (integral)
    {
      for (Cap &x : selected) x /= 2;
      return selected;
    }
    round(selected);
    Cap total = 0;
    for (Cap x : selected) total += x;
    if (total == upper) return selected;

    vc<pair<int, int>> network;
    vc<Cap> residual;
    auto add = [&](int from, int to, Cap cap, Cap used)
    {
      network.eb(from, to), network.eb(to ^ 1, from ^ 1);
      network.eb(to, from), network.eb(from ^ 1, to ^ 1);
      residual.eb(cap - used), residual.eb(cap - used), residual.eb(used), residual.eb(used);
    };
    vc<Cap> degree(n);
    for (int e = 0; e < m; e++)
    {
      const auto [u, v] = edges[e];
      add(2 * u, 2 * v + 1, capacity[e], selected[e]);
      degree[u] += selected[e], degree[v] += selected[e];
    }
    for (int v = 0; v < n; v++) add(s, 2 * v, b[v], degree[v]);
    while (total < upper)
    {
      vc<pair<int, int>> split;
      vc<int> image;
      for (int a = 0; a < int(network.size()); a += 2)
        for (int copy = 0; copy < (residual[a] >= 2 ? 2 : int(residual[a])); copy++)
        {
          split.eb(network[a]), split.eb(network[a + 1]);
          image.eb(a), image.eb(a + 1);
        }
      const auto path = SkewRegularPath(t + 1, split).run(s);
      if (!path) break;
      vc<int> count(network.size() / 2);
      for (int a : *path) count[image[a] / 2]++;
      Cap delta = upper - total;
      for (int a : *path) delta = min<Cap>(delta, residual[image[a]] / count[image[a] / 2]);
      assert(delta > 0);
      for (int a : *path)
      {
        const int e = image[a];
        residual[e] -= delta, residual[e ^ 1] -= delta;
        residual[e ^ 2] += delta, residual[e ^ 3] += delta;
      }
      total += delta;
    }
    for (int e = 0; e < m; e++) selected[e] = residual[4 * e + 2];
    return selected;
  }
};

template <bool unit_edges = false, class Cost, bool erasable, class Capacity, class GetCapacity>
vc<pair<int, Capacity>> b_matching_impl(
    const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &capacity,
    const GetCapacity &get_capacity)
{
  static_assert(is_integral_ext<Capacity>);
  using Work = conditional_t<(sizeof(Capacity) < sizeof(ll)), ll, Capacity>;
  const int n = g.template size<int>();
  assert(int(capacity.size()) == n);
  for (int v = 0; v < n; v++)
    if constexpr (is_signed_ext<Capacity>) assert(capacity[v] >= 0);
  vc<int> vertex(n, -1), index;
  vc<pair<int, int>> edges;
  vc<Work> bounded(n), edge_capacity;
  int vertices = 0;
  for (const auto &e : g.edges())
  {
    const int u = e.from, v = e.to;
    const Capacity limit = get_capacity(e.index);
    if constexpr (is_signed_ext<Capacity>) assert(limit >= 0);
    Work c = min<Work>(limit, capacity[u]);
    c = min<Work>(c, capacity[v]);
    if (u == v) c = min<Work>(c, Work(capacity[u]) / 2);
    if (c == 0) continue;
    if (vertex[u] == -1) vertex[u] = vertices++;
    if (vertex[v] == -1) vertex[v] = vertices++;
    edges.eb(vertex[u], vertex[v]), index.eb(e.index);
    if constexpr (!unit_edges) edge_capacity.eb(c);
    bounded[u] += min<Work>(c, Work(capacity[u]) - bounded[u]);
    bounded[v] += min<Work>(c, Work(capacity[v]) - bounded[v]);
  }
  if (edges.empty()) return {};
  vc<Work> b(vertices);
  for (int v = 0; v < n; v++) if (vertex[v] != -1) b[vertex[v]] = bounded[v];
  vc<pair<int, Capacity>> selected;
  if constexpr (unit_edges)
  {
    if (all_of(ALL(b), [](Work c) { return c <= 1; }))
    {
      const auto matching = general_matching(GraphUndirected<>(vertices, edges));
      vc<int> mate(vertices, -1);
      for (auto [u, v] : matching) mate[u] = v, mate[v] = u;
      for (int e = 0; e < int(edges.size()); e++)
      {
        const auto [u, v] = edges[e];
        if (mate[u] == v) selected.eb(index[e], Capacity(1)), mate[u] = mate[v] = -1;
      }
      return selected;
    }
  }
  auto collect = [&](const auto &count)
  {
    for (int e = 0; e < int(edges.size()); e++) if (count[e] != 0)
      selected.eb(index[e], Capacity(count[e]));
  };
  if constexpr (unit_edges)
    collect(GoldbergKarzanovBMatching<int>(vertices, edges,
        vc<int>(ALL(b)), vc<int>(edges.size(), 1)).solve());
  else collect(GoldbergKarzanovBMatching<Work>(vertices, edges, b, edge_capacity).solve());
  return selected;
}
}

// 各辺の使用本数を圧縮して返す。自己ループ一本は頂点容量を二つ使う。
template <class Cost, bool erasable, class Capacity>
vc<pair<int, Capacity>> b_matching(
    const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &capacity,
    const vc<Capacity> &edge_capacity)
{
  return internal::b_matching_impl(g, capacity, [&](int id)
  {
    assert(0 <= id && id < int(edge_capacity.size()));
    return edge_capacity[id];
  });
}

// 各辺を高々一度使う。
template <class Cost, bool erasable, class Capacity>
vc<pair<int, Capacity>> b_matching(
    const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &capacity)
{
  return internal::b_matching_impl<true>(g, capacity, [](int) { return Capacity(1); });
}

// 辺容量に nullopt を渡すと、各辺を任意回使える。
template <class Cost, bool erasable, class Capacity>
vc<pair<int, Capacity>> b_matching(
    const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &capacity, nullopt_t)
{
  return internal::b_matching_impl(g, capacity,
      [](int) { return numeric_limits<Capacity>::max(); });
}

// 各頂点の次数が demand と一致する辺集合を、元の辺番号で返す。
template <class Cost, bool is_erasable, class Demand>
optional<vc<int>> f_factor(const GraphUndirected<Cost, is_erasable> &g, const vc<Demand> &demand)
{
  const int n = g.template size<int>();
  assert(int(demand.size()) == n);
  vc<int> degree(n);
  for (const auto &e : g.edges())
  {
    assert(e.from != e.to);
    degree[e.from]++, degree[e.to]++;
  }
  int total = 0;
  bool possible = true;
  for (int v = 0; v < n; v++)
  {
    if constexpr (Demand(-1) < Demand(0)) assert(demand[v] >= 0);
    if (u128(demand[v]) > unsigned(degree[v])) possible = false;
    else total += int(demand[v]);
  }
  if (!possible || total % 2) return nullopt;
  const auto matching = b_matching(g, demand);
  if (int(matching.size()) != total / 2) return nullopt;
  vc<int> selected;
  selected.reserve(matching.size());
  for (const auto &e : matching) selected.eb(e.first);
  return selected;
}
