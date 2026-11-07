#pragma once

#include "graph.hpp"
#include "../ds/uf/uf.hpp"

/**
 * @brief 一般グラフ最大マッチング（Micali–Vazirani）
 * @docs docs/graph/general_matching.md
 */

//! 数学的手法: https://arxiv.org/abs/2012.03582v1 （Sections 2–4, 8）
namespace internal
{
struct MicaliVazirani
{
  struct Petal
  {
    int root[2], root_bud[2], base, parent[2], port[2];
    int parity;
  };
  int n, search_id = 0;
  vc<pair<int, int>> edges;
  CSR<int> adj;
  vc<int> mate, mate_edge, low, high, label, pred_count;
  vc<int> stamp, color, cursor, parent[2], port[2];
  vc<int> petal_of, tree_parent, tree_port, tree_side;
  vc<unsigned char> state, removed;
  vvc<int> level, bridge, pred, succ;
  vc<Petal> petals;
  UnionFind<UFDataEmpty<>> uf;

  MicaliVazirani(int n, const vc<pair<int, int>> &es)
      : n(n), edges(es), mate(n, -1), mate_edge(n, -1), low(n), high(n),
        label(n), pred_count(n), stamp(n), color(n), cursor(n),
        petal_of(n), tree_parent(n), tree_port(n), tree_side(n),
        state(es.size()), removed(n), level(n), bridge(n), pred(n), succ(n), uf(n)
  {
    vc<int> degree(n);
    for (auto [u, v] : edges) degree[u]++, degree[v]++;
    adj = CSR<int>(degree);
    fill(ALL(degree), 0);
    for (int i = 0; i < int(edges.size()); i++)
    {
      auto [u, v] = edges[i];
      adj[u][degree[u]++] = i, adj[v][degree[v]++] = i;
    }
    for (int s = 0; s < 2; s++) parent[s].resize(n), port[s].resize(n);
  }

  int other(int e, int v) const { return edges[e].first ^ edges[e].second ^ v; }
  int bud(int v) { return label[uf.leader(v)]; }
  int distance(int v, int parity) const
  { return (low[v] & 1) == parity ? low[v] : high[v]; }

  void schedule(int e)
  {
    if (state[e] != 2) return;
    auto [u, v] = edges[e];
    const int parity = mate[u] == v;
    const int a = distance(u, parity), b = distance(v, parity);
    if (a == n || b == n || a + b + 1 >= 2 * n) return;
    bridge[(a + b) / 2].eb(e);
    state[e] = 3;
  }

  // 戻り値は共通の bottleneck。異なる自由頂点に達した場合は -1。
  int ddfs(int e, vc<int> &visited, int (&center)[2])
  {
    auto [u, v] = edges[e];
    center[0] = bud(u), center[1] = bud(v);
    if (center[0] == center[1]) return center[0];
    ++search_id;
    visited.clear();
    for (int s = 0; s < 2; s++)
    {
      int x = center[s];
      stamp[x] = search_id, color[x] = s, cursor[x] = 0;
      parent[s][x] = port[s][x] = -1;
      visited.eb(x);
    }
    int barrier = center[0];
    auto step = [&](int s, int stop, bool meet)
    {
      const int a = center[s];
      while (cursor[a] < int(pred[a].size()))
      {
        const int x = pred[a][cursor[a]++];
        if (removed[x]) continue;
        const int b = bud(x);
        if (removed[b]) continue;
        if (stamp[b] == search_id && !(meet && b == center[1 - s])) continue;
        parent[s][b] = a, port[s][b] = x;
        center[s] = b;
        if (stamp[b] != search_id)
        {
          stamp[b] = search_id, color[b] = s, cursor[b] = 0;
          visited.eb(b);
        }
        return true;
      }
      if (a == stop) return false;
      assert(parent[s][a] != -1);
      center[s] = parent[s][a];
      return true;
    };
    while (true)
    {
      if (center[0] != center[1])
      {
        if (low[center[0]] == 0 && low[center[1]] == 0) return -1;
        int s = low[center[0]] >= low[center[1]] ? 0 : 1;
        const bool moved = step(s, s == 0 ? barrier : bud(v), true);
        assert(moved);
        (void)moved;
        continue;
      }
      const int b = center[0];
      bool escaped = false;
      color[b] = 1;
      if (b != barrier)
      {
        center[0] = parent[0][b];
        while (true)
        {
          if (low[center[0]] <= low[b]) { escaped = true; break; }
          if (!step(0, barrier, false)) break;
        }
      }
      if (escaped) continue;
      center[0] = barrier = b, color[b] = 0;
      if (parent[1][b] != -1)
      {
        center[1] = parent[1][b];
        while (true)
        {
          if (low[center[1]] <= low[b]) { escaped = true; break; }
          if (!step(1, bud(v), false)) break;
        }
      }
      if (!escaped) return b;
    }
  }

  vc<int> augmenting_path(int e, const int (&center)[2])
  {
    struct Job { int kind, v, target, parity, p, side; bool reverse; };
    vc<Job> jobs;
    vc<pair<int, int>> path_edges;
    const int current = petals.size();
    auto [u, v] = edges[e];
    Petal now{{u, v}, {bud(u), bud(v)}, -1, {-1, -1}, {-1, -1}, int(mate[u] == v)};
    petals.eb(now);
    auto push_edge = [&](int a, int b, bool reverse)
    { jobs.eb(Job{2, a, b, 0, 0, 0, reverse}); };
    auto push_path = [&](int a, int b, int parity, bool reverse)
    { if (a != b) jobs.eb(Job{0, a, b, parity, 0, 0, reverse}); };
    auto push_tree = [&](int a, int p, int side, bool reverse)
    { jobs.eb(Job{1, a, 0, 0, p, side, reverse}); };
    push_tree(center[1], current, 1, false);
    push_edge(u, v, false);
    push_tree(center[0], current, 0, true);
    while (!jobs.empty())
    {
      const Job j = jobs.back();
      jobs.pop_back();
      if (j.kind == 2)
      {
        path_edges.eb(j.reverse ? j.target : j.v, j.reverse ? j.v : j.target);
        continue;
      }
      if (j.kind == 1)
      {
        const auto &p = petals[j.p];
        if (j.v == p.root_bud[j.side])
        {
          push_path(p.root[j.side], j.v, p.parity, j.reverse);
          continue;
        }
        int a, x;
        if (j.p == current) a = parent[j.side][j.v], x = port[j.side][j.v];
        else if (j.v == p.base) a = p.parent[j.side], x = p.port[j.side];
        else a = tree_parent[j.v], x = tree_port[j.v];
        assert(a != -1);
        if (!j.reverse)
        {
          push_path(x, j.v, 1 - (low[a] & 1), false);
          push_edge(a, x, false);
          push_tree(a, j.p, j.side, false);
        }
        else
        {
          push_tree(a, j.p, j.side, true);
          push_edge(a, x, true);
          push_path(x, j.v, 1 - (low[a] & 1), true);
        }
        continue;
      }
      if (j.parity == (low[j.v] & 1))
      {
        int x = -1;
        for (int w : pred[j.v]) if (!removed[w]) { x = w; break; }
        assert(x != -1);
        if (!j.reverse)
        {
          push_path(x, j.target, 1 - j.parity, false);
          push_edge(j.v, x, false);
        }
        else
        {
          push_edge(j.v, x, true);
          push_path(x, j.target, 1 - j.parity, true);
        }
        continue;
      }
      const int id = petal_of[j.v], s = tree_side[j.v];
      assert(id != -1);
      const auto &p = petals[id];
      if (!j.reverse)
      {
        push_path(p.base, j.target, 0, false);
        push_tree(p.base, id, 1 - s, false);
        push_edge(p.root[s], p.root[1 - s], false);
        push_tree(j.v, id, s, true);
      }
      else
      {
        push_tree(j.v, id, s, false);
        push_edge(p.root[s], p.root[1 - s], true);
        push_tree(p.base, id, 1 - s, true);
        push_path(p.base, j.target, 0, true);
      }
    }
    petals.pop_back();
    vc<int> path{center[0]};
    for (auto [a, b] : path_edges)
    {
      assert(path.back() == a);
      path.eb(b);
    }
    assert(path.back() == center[1]);
    return path;
  }

  bool phase()
  {
    fill(ALL(low), n), fill(ALL(high), n);
    fill(ALL(state), 0), fill(ALL(removed), 0), fill(ALL(pred_count), 0);
    fill(ALL(petal_of), -1);
    petals.clear();
    uf.reset(n);
    iota(ALL(label), 0);
    for (int v = 0; v < n; v++)
    {
      level[v].clear(), bridge[v].clear(), pred[v].clear(), succ[v].clear();
    }
    for (int v = 0; v < n; v++) if (mate[v] == -1) low[v] = 0, level[0].eb(v);
    bool augmented = false;
    vc<int> visited, erase;
    for (int i = 0; i < n; i++)
    {
      auto scan = [&](int u, int e)
      {
        if (state[e]) return;
        const int v = other(e, u);
        if (low[v] >= i + 1)
        {
          if (low[v] == n) low[v] = i + 1, level[i + 1].eb(v);
          pred[v].eb(u), succ[u].eb(v), pred_count[v]++;
          state[e] = 1;
        }
        else state[e] = 2, schedule(e);
      };
      for (int u : level[i])
      {
        if (i & 1)
        {
          if (mate_edge[u] != -1) scan(u, mate_edge[u]);
        }
        else for (int e : adj[u]) if (mate[u] != other(e, u)) scan(u, e);
      }
      for (int e : bridge[i])
      {
        auto [u, v] = edges[e];
        if (removed[u] || removed[v] || bud(u) == bud(v)) continue;
        int center[2];
        const int b = ddfs(e, visited, center);
        if (b == -1)
        {
          const auto path = augmenting_path(e, center);
          assert(int(path.size()) == 2 * i + 2);
          for (int j = 0; j < int(path.size()); j += 2)
          {
            const int a = path[j], z = path[j + 1];
            mate[a] = z, mate[z] = a;
          }
          erase.clear();
          for (int a : path) if (!removed[a]) removed[a] = 1, erase.eb(a);
          for (int j = 0; j < int(erase.size()); j++)
            for (int z : succ[erase[j]]) if (!removed[z] && --pred_count[z] == 0)
              removed[z] = 1, erase.eb(z);
          augmented = true;
          continue;
        }
        Petal p{{u, v}, {bud(u), bud(v)}, b,
                {parent[0][b], parent[1][b]}, {port[0][b], port[1][b]}, int(mate[u] == v)};
        const int id = petals.size();
        petals.eb(p);
        for (int x : visited) if (x != b)
        {
          petal_of[x] = id, tree_side[x] = color[x];
          tree_parent[x] = parent[color[x]][x], tree_port[x] = port[color[x]][x];
          high[x] = 2 * i + 1 - low[x];
          if (high[x] < n) level[high[x]].eb(x);
          if (low[x] & 1) for (int f : adj[x]) schedule(f);
        }
        for (int x : visited) if (x != b) uf.merge(b, x);
        label[uf.leader(b)] = b;
      }
      if (augmented) break;
    }
    if (augmented)
    {
      fill(ALL(mate_edge), -1);
      for (int e = 0; e < int(edges.size()); e++)
      {
        auto [u, v] = edges[e];
        if (mate[u] == v) mate_edge[u] = mate_edge[v] = e;
      }
    }
    return augmented;
  }

  vc<pair<int, int>> solve(const vc<pair<int, int>> &initial)
  {
    for (auto [u, v] : initial)
    {
      assert(0 <= u && u < n && 0 <= v && v < n && u != v);
      assert(mate[u] == -1 && mate[v] == -1);
      int found = -1;
      for (int e : adj[u]) if (other(e, u) == v) { found = e; break; }
      assert(found != -1);
      mate[u] = v, mate[v] = u, mate_edge[u] = mate_edge[v] = found;
    }
    for (int e = 0; e < int(edges.size()); e++)
    {
      auto [u, v] = edges[e];
      if (mate[u] == -1 && mate[v] == -1)
        mate[u] = v, mate[v] = u, mate_edge[u] = mate_edge[v] = e;
    }
    while (phase()) {}
    vc<pair<int, int>> res;
    for (int u = 0; u < n; u++) if (u < mate[u]) res.eb(u, mate[u]);
    return res;
  }
};
}

// 初期マッチングから開始し、最大マッチングを頂点対の配列で返す。
template <class Cost, bool is_erasable>
vc<pair<int, int>> general_matching(const GraphUndirected<Cost, is_erasable> &g,
                                  const vc<pair<int, int>> &initial = {})
{
  vc<pair<int, int>> es;
  es.reserve(g.num_of_edges());
  for (auto e : g.edges()) if (e.from != e.to) es.eb(e.from, e.to);
  return internal::MicaliVazirani(g.template size<int>(), es).solve(initial);
}
