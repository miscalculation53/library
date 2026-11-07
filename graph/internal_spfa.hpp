#pragma once

#include "graph.hpp"
#include "../ds/my_deque.hpp"

namespace internal
{
// 初期距離と重複のない処理開始頂点を渡す。親辺の変更は update(v, edge) へ通知する。
// stop_on_negative_cycle=true では負閉路の検出時に false を返して終了する。
template <bool stop_on_negative_cycle, bool directed, class Cost, class Dist, class Update>
bool spfa_relax(const Graph<directed, Cost> &g, vc<Dist> &dists, const vc<int> &sources,
                const Dist &infty, const Update &update)
{
  const int n = g.size();
  MyDeque<int> que;
  que.reserve(n);
  vc<unsigned char> queued(n, false);
  vc<int> length(n, 0);
  for (int v : sources) que.push_back(v), queued[v] = true;
  bool finite = true;
  while (!que.empty())
  {
    const int v = que.front();
    que.pop_front();
    queued[v] = false;
    fec(e : g.out_edges(v))
    {
      const Dist nd = !stop_on_negative_cycle && dists[v] == -infty ? -infty : dists[v] + e.cost;
      if (!(nd < dists[e.to])) continue;
      if constexpr (stop_on_negative_cycle)
        if (length[v] + 1 >= n) return false;
      if (!stop_on_negative_cycle && (dists[v] == -infty || length[v] + 1 >= n))
      {
        finite = false;
        dists[e.to] = -infty;
        update(e.to, Edge<Cost>{});
      }
      else
      {
        dists[e.to] = nd;
        length[e.to] = length[v] + 1;
        update(e.to, e);
      }
      if (!queued[e.to]) que.push_back(e.to), queued[e.to] = true;
    }
  }
  return finite;
}
}
