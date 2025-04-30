#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"
#include "../ds/my_queue.hpp"

/**
 * @brief 連結成分への分解
 * @docs docs/graph/connected_components.md
 */

// 無向グラフを受け取り、各頂点の連結成分の番号を返す
template <class I = ll, class Cost, Cost dflt_cost>
vc<I> connected_component_ids(const Graph<Cost, dflt_cost> &g)
{
  const int n = g.size();
  vc<I> res(n, -1);
  int id = 0;
  MyQueue<int> que;
  repi(sv, n)
  {
    if (res[sv] != -1)
      continue;
    res[sv] = id;
    que.clear();
    que.push(sv);
    while (!que.empty())
    {
      int v = que.front();
      que.pop();
      fe(nv : g.out_edges(v))
      {
        if (res[nv] != -1)
          continue;
        res[nv] = id;
        que.push(nv);
      }
    }
    id++;
  }
  return res;
}
