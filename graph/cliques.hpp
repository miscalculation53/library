#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"

/**
 * @brief クリーク列挙
 * @docs docs/graph/triangles.md
 */

// すべての (空でない) クリーク vc<I> C に対して f(C) を実行
// クリークの個数はたかだか 2^{sqrt(2m)} 個
// 計算量は O(2^{sqrt(2m)}*n)
template <class I = ll, class Cost, bool is_erasable, class F>
void cliques(const GraphUndirected<Cost, is_erasable> &g, const F &f)
{
  const int n = g.size(), m = g.num_of_edges();
  const int b = sqrt(2 * m);
  vc<int> id(n, -1);
  // 2^vs をすべて調べる (v は必ず含む頂点)
  auto check = [&](const vc<int> &vs, int v)
  {
    const int k = vs.size();
    int j = -1;
    repi(i, k)
    {
      id[vs[i]] = i;
      if (vs[i] == v)
        j = i;
    }
    vc<int> sub(k);
    repi(i, k)
    {
      bset(sub[i], i);
      fec(e : g.out_edges(vs[i])) if (id[e.to] >= 0) bset(sub[i], id[e.to]);
    }
    repi(bit, 1 << k)
    {
      if (j >= 0 && !btest(bit, j))
        continue;
      int tmp = bit;
      repi(i, k) if (btest(bit, i)) tmp &= sub[i];
      if (tmp == bit)
      {
        vc<I> clq;
        clq.reserve(popcount(bit));
        repi(i, k) if (btest(bit, i)) clq.eb(vs[i]);
        if (!clq.empty())
          f(clq);
      }
    }
    fec(v : vs) id[v] = -1;
  };
  while (true)
  {
    repi(v, n + 1)
    {
      if (v == n)
      {
        vc<int> vs;
        repi(v, n) if (id[v] != -2) vs.eb(v);
        check(vs, -1);
        return;
      }
      if (id[v] == -2)
        continue;
      vc<int> vs = {v};
      vs.reserve(g.out_edges(v).size() + 1);
      fec(e : g.out_edges(v)) if (id[e.to] != -2) vs.eb(e.to);
      sortunique(vs);
      const int k = vs.size();
      if (k < b)
      {
        check(vs, v);
        id[v] = -2;
        break;
      }
    }
  }
}
