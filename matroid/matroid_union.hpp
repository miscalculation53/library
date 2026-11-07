#pragma once

#include "internal_exchange_graph.hpp"

/**
 * @brief マトロイド合併・分割
 * @docs docs/matroid/matroid_union.md
 */

// 最大個数の要素を、各マトロイドの独立集合へ重複なく割り当てる。
template <class M>
vvc<int> matroid_union(vc<M> matroids)
{
  const int k = matroids.size();
  if (k == 0) return {};
  const int n = matroids[0].size();
  for (const auto &matroid : matroids) assert(matroid.size() == n);
  vvc<int> parts(k);
  vc<int> owner(n, -1);
  while (true)
  {
    for (int i = 0; i < k; i++) matroids[i].build(parts[i]);
    vc<int> parent(n, -1), destination(n, -1), queue;
    for (int e = 0; e < n; e++) if (owner[e] == -1) parent[e] = e, queue.eb(e);
    int target = -1, final_part = -1;
    for (int head = 0; head < int(queue.size()) && target == -1; head++)
    {
      const int e = queue[head];
      for (int i = 0; i < k; i++) if (owner[e] != i)
      {
        auto circuit = matroids[i].circuit(e);
        if (!circuit) { target = e, final_part = i; break; }
        for (int f : *circuit) if (parent[f] == -1)
          parent[f] = e, destination[f] = i, queue.eb(f);
      }
    }
    if (target == -1) return parts;
    for (int e = target, next_part = final_part;; e = parent[e])
    {
      owner[e] = next_part;
      if (parent[e] == e) break;
      next_part = destination[e];
    }
    for (auto &part : parts) part.clear();
    for (int e = 0; e < n; e++) if (owner[e] != -1) parts[owner[e]].eb(e);
  }
}
