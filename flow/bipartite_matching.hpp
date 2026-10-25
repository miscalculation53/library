#pragma once

#include "../graph/graph.hpp"

/**
 * @brief 二部最大マッチング
 * @docs docs/flow/bipartite_matching.md
 */

// 色 0 -> 色 1 の頂点対を返す。重みは使わない。
template <class Cost, bool is_erasable, class Color>
vc<pair<int, int>> bipartite_matching(const GraphUndirected<Cost, is_erasable> &g, const vc<Color> &color)
{
  const int n = g.template size<int>();
  assert(int(color.size()) == n);
  int count[2] = {};
  for (int v = 0; v < n; v++)
  {
    assert(color[v] == 0 || color[v] == 1);
    if (!g.out_edges(v).empty()) count[color[v] == 1]++;
  }
  const bool side = count[0] > count[1];
  const int k = count[side];
  if (k == 0) return {};
  vc<int> left;
  left.reserve(k);
  for (int v = 0; v < n; v++)
    if ((color[v] == 1) == side && !g.out_edges(v).empty()) left.eb(v);

  vc<int> mate_left(k, -1), mate_right(n, -1);
  int matched = 0;
  auto result = [&]
  {
    vc<pair<int, int>> res;
    res.reserve(matched);
    for (int u = 0; u < k; u++) if (mate_left[u] != -1)
    {
      const int v = left[u], w = mate_left[u];
      if (!side) res.eb(v, w);
      else res.eb(w, v);
    }
    return res;
  };
  // 長さ 1 の増加路を先に処理し、小さい側が全て埋まれば直ちに返す。
  for (int u = 0; u < k; u++) for (const auto &e : g.out_edges(left[u]))
  {
    assert((color[e.to] == 1) != side);
    if (mate_right[e.to] != -1) continue;
    mate_left[u] = e.to, mate_right[e.to] = u;
    matched++;
    break;
  }
  if (matched == k) return result();

  {
    // 残次数が小さい頂点を優先し、長い交互路の少ない初期対応を作る。
    vc<int> degree(n), leaves(n), twos(n);
    int leaf_front = 0, leaf_back = 0, two_front = 0, two_back = 0;
    for (int v = 0; v < n; v++)
    {
      degree[v] = g.out_edges(v).size();
      if (degree[v] == 1) leaves[leaf_back++] = v;
      if (degree[v] == 2) twos[two_back++] = v;
    }
    fill(ALL(mate_left), -1);
    fill(ALL(mate_right), -1);
    for (int u = 0; u < k; u++) mate_right[left[u]] = u;
    matched = 0;
    int next_vertex = 0;
    while (true)
    {
      int v = -1;
      while (leaf_front < leaf_back && v == -1)
      {
        const int x = leaves[leaf_front++];
        if (degree[x] == 1) v = x;
      }
      while (two_front < two_back && v == -1)
      {
        const int x = twos[two_front++];
        if (degree[x] == 2) v = x;
      }
      if (v == -1)
      {
        while (next_vertex < n && degree[next_vertex] <= 0) next_vertex++;
        if (next_vertex == n) break;
        v = next_vertex++;
      }
      int w = -1;
      for (const auto &e : g.out_edges(v)) if (degree[e.to] > 0)
        if (w == -1 || degree[e.to] < degree[w]) w = e.to;
      assert(w != -1);
      int a = v, b = w;
      if ((color[a] == 1) != side) swap(a, b);
      const int u = mate_right[a];
      mate_left[u] = b, mate_right[b] = u;
      matched++;
      degree[v] = degree[w] = -1;
      for (int x : {v, w}) for (const auto &e : g.out_edges(x)) if (degree[e.to] > 0)
      {
        degree[e.to]--;
        if (degree[e.to] == 1) leaves[leaf_back++] = e.to;
        if (degree[e.to] == 2) twos[two_back++] = e.to;
      }
    }
  }
  if (matched == k) return result();

  CSR<int> adj;
  {
    // 多重辺を除き、小さい側から出る行先だけを連続配置する。
    vc<int> seen(n, -1), degree(k);
    for (int u = 0; u < k; u++) for (const auto &e : g.out_edges(left[u]))
    {
      assert((color[e.to] == 1) != side);
      if (seen[e.to] == u) continue;
      seen[e.to] = u, degree[u]++;
    }
    adj = CSR<int>(degree);
    fill(ALL(seen), -1);
    auto &data = adj.get_elist();
    for (int u = 0; u < k; u++)
    {
      int i = adj.offset(u);
      for (const auto &e : g.out_edges(left[u])) if (seen[e.to] != u)
        seen[e.to] = u, data[i++] = e.to;
    }
  }

  vc<int> level(k), iter(k), que(k), path;
  bool refined = false;
  while (matched < k)
  {
    fill(ALL(level), -1);
    int front = 0, back = 0, shortest = -1;
    for (int u = 0; u < k; u++) if (mate_left[u] == -1)
      level[u] = 0, que[back++] = u;
    // 最短の増加路に現れる左側の頂点まで、交互路の層を作る。
    while (front < back && shortest == -1)
    {
      const int u = que[front++];
      for (int v : adj[u])
      {
        const int w = mate_right[v];
        if (w == -1) { shortest = level[u] + 1; break; }
        if (level[w] == -1) level[w] = level[u] + 1, que[back++] = w;
      }
    }
    if (shortest == -1) break;
    if (!refined)
    {
      refined = true;
      // 完全に失敗した根の閉領域を共有し、任意長の増加路を先に減らす。
      vc<unsigned char> closed(k);
      fill(ALL(level), -1);
      const ll limit = 64LL * adj.get_elist().size();
      ll work = 0;
      for (int root = 0; root < k && work < limit; root++)
        if (mate_left[root] == -1 && !closed[root])
      {
        path.clear();
        path.eb(root);
        level[root] = root, iter[root] = 0;
        back = 0;
        que[back++] = root;
        while (!path.empty() && work < limit)
        {
          const int u = path.back();
          const auto row = adj[u];
          bool descended = false, augmented = false;
          for (int &i = iter[u]; i < int(row.size()) && work < limit;)
          {
            work++;
            const int v = row[i++], w = mate_right[v];
            if (w == -1)
            {
              int next = v;
              for (int j = int(path.size()) - 1; j >= 0; j--)
              {
                const int p = path[j], old = mate_left[p];
                mate_left[p] = next, mate_right[next] = p;
                next = old;
              }
              matched++;
              augmented = true;
              break;
            }
            if (!closed[w] && level[w] != root)
            {
              level[w] = root, iter[w] = 0;
              que[back++] = w;
              path.eb(w);
              descended = true;
              break;
            }
          }
          if (augmented) break;
          if (!descended)
          {
            if (iter[u] < int(row.size())) break;
            path.pop_back();
          }
        }
        // スタックが尽きた場合だけ、全到達集合を閉領域に加える。
        if (path.empty()) for (int i = 0; i < back; i++) closed[que[i]] = true;
      }
      if (matched == k) return result();
      continue;
    }
    fill(ALL(iter), 0);
    for (int root = 0; root < k; root++) if (mate_left[root] == -1)
    {
      path.clear();
      path.eb(root);
      // 明示的なスタックで、最短増加路の極大な頂点素集合を求める。
      while (!path.empty())
      {
        const int u = path.back();
        const auto row = adj[u];
        bool descended = false, augmented = false;
        for (int &i = iter[u]; i < int(row.size());)
        {
          const int v = row[i++], w = mate_right[v];
          if (w == -1)
          {
            if (level[u] + 1 != shortest) continue;
            int next = v;
            for (int j = int(path.size()) - 1; j >= 0; j--)
            {
              const int p = path[j], old = mate_left[p];
              mate_left[p] = next, mate_right[next] = p;
              level[p] = -1;
              next = old;
            }
            matched++;
            augmented = true;
            break;
          }
          if (level[w] == level[u] + 1 && level[w] < shortest)
          {
            path.eb(w);
            descended = true;
            break;
          }
        }
        if (augmented) break;
        if (!descended) level[u] = -1, path.pop_back();
      }
    }
  }

  return result();
}
