#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"

/**
 * @brief 強連結成分分解
 * @docs docs/graph/scc.md
 */

// 各頂点の成分番号を返す
// 縮約した後の DAG はトポロジカルソートされている
template <class Cost, class I = ll>
vc<I> scc(const GraphDirected<Cost> &g)
{
  const int n = g.size();
  if (g.num_of_edges() == 0)
  {
    vc<I> ids(n);
    repi(v, n) ids[v] = n - 1 - v;
    return ids;
  }
  vc<int> ord(n, -1), low(n), iter(n), stack, active;
  vc<I> ids(n);
  stack.reserve(n), active.reserve(n);
  int timer = 0, cnt = 0;
  auto enter = [&](int v)
  {
    ord[v] = low[v] = timer++;
    stack.eb(v), active.eb(v);
  };
  repi(root, n) if (ord[root] == -1)
  {
    enter(root);
    while (!stack.empty())
    {
      const int v = stack.back();
      const auto row = g.out_edges(v);
      if (iter[v] < row.size())
      {
        const int to = row[iter[v]++].to;
        if (ord[to] == -1) enter(to);
        else chmin(low[v], ord[to]);
        continue;
      }
      stack.pop_back();
      if (low[v] == ord[v])
      {
        while (true)
        {
          const int u = active.back();
          active.pop_back();
          ids[u] = cnt;
          // 成分が確定した頂点は訪問番号 n として扱う。
          ord[u] = n;
          if (u == v) break;
        }
        cnt++;
      }
      if (!stack.empty()) chmin(low[stack.back()], low[v]);
    }
  }
  for (auto &id : ids) id = cnt - 1 - id;
  return ids;
}
