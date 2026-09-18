#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"

/**
 * @brief クリーク列挙
 * @docs docs/graph/cliques.md
 */

// すべての (空でない) クリーク vc<I> C に対して f(C) を実行
// クリークの個数はたかだか 2^{sqrt(2m)} 個
// 計算量は O(2^{sqrt(2m)}*n)
template <class I = ll, class Cost, bool is_erasable, class F>
void cliques(const GraphUndirected<Cost, is_erasable> &g, const F &f)
{
  const int n = g.size(), m = g.num_of_edges();
  int b = 1;
  while (1LL * b * b < 2LL * m)
    b++;
  vc<int> id(n, -1);
  // vs のクリークをすべて調べる (v >= 0 なら v は必ず含む)
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
    vvc<char> adj(k, vc<char>(k, false));
    repi(i, k)
    {
      adj[i][i] = true;
      fec(e : g.out_edges(vs[i])) if (id[e.to] >= 0) adj[i][id[e.to]] = true;
    }
    vc<int> cur;
    auto dfs = [&](auto dfs, int i) -> void
    {
      if (i == k)
      {
        if (!cur.empty())
          f(GEN_VEC(cur.size(), h, I(vs[cur[h]])));
        return;
      }
      if (i != j)
        dfs(dfs, i + 1);
      if (all_of(ALL(cur), [&](int h) { return adj[i][h]; }))
      {
        cur.eb(i);
        dfs(dfs, i + 1);
        cur.pop_back();
      }
    };
    dfs(dfs, 0);
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
      if (k <= b)
      {
        check(vs, v);
        id[v] = -2;
        break;
      }
    }
  }
}
