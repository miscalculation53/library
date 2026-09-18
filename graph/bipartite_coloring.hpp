#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"
#include "../ds/my_queue.hpp"

/**
 * @brief 二部グラフ判定・$2$ 彩色・奇閉路の復元
 * @docs docs/graph/bipartite_coloring.md
 */

// (is_bipartite, coloring, cycle)
// coloring は各要素が 0, 1 の vector
template <class Cost, class I = ll>
tuple<bool, vc<I>, vc<Edge<Cost>>> bipartite_coloring(const GraphUndirected<Cost> &g)
{
  const int n = g.size();
  vc<I> res(n, -1);
  vc<Edge<Cost>> prv(n);
  MyQueue<int> que;
  repi(sv, n)
  {
    if (res[sv] != -1)
      continue;
    res[sv] = 0, que.push(sv);
    while (!que.empty())
    {
      int v = que.front();
      que.pop();
      fec(e : g.out_edges(v))
      {
        const int nv = e.to;
        if (res[nv] == -1)
        {
          res[nv] = res[v] ^ 1;
          prv[nv] = e;
          que.push(nv);
        }
        else if (res[nv] == res[v])
        {
          vc<Edge<Cost>> cycle, right;
          int a = v, b = nv;
          while (a != b)
          {
            cycle.eb(prv[a]);
            right.eb(prv[b].rev());
            a = prv[a].from;
            b = prv[b].from;
          }
          reverse(ALL(cycle));
          cycle.eb(e);
          cycle.insert(cycle.end(), ALL(right));
          return {false, {}, move(cycle)};
        }
      }
    }
  }
  return {true, move(res), {}};
}
