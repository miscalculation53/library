#pragma once

#include "../../ds/mo/mo.hpp"
#include "rooted_tree.hpp"

/**
 * @brief Mo's algorithm on tree
 * @docs docs/graph/tree/mo_on_tree.md
 */

// add(v): 頂点 v を現在の集合に追加する
// del(v): 頂点 v を現在の集合から削除する
// rem(qid): uvs[qid] のパスについて答えを記録する
// edge のときは辺属性を扱い、辺 (parent(v), v) を頂点 v で表す
template <class I, class Add, class Del, class Rem>
void mo_on_tree(const RootedTree &g, const vc<pair<I, I>> &uvs, const Add &add, const Del &del, const Rem &rem, bool edge = false)
{
  if (uvs.empty())
    return;
  const int n = g.size(), q = uvs.size();
  vc<int> in(n), out(n), tour(2 * n);
  repi(v, n)
  {
    // 行きがけと帰りがけに頂点を 1 回ずつ記録する。
    in[v] = 2 * g.preorder(v) - g.depth(v);
    out[v] = 2 * g.postorder(v) - g.depth(v) - 1;
    tour[in[v]] = tour[out[v]] = v;
  }

  vc<pair<int, int>> lrs(q);
  vc<int> extra(q, -1);
  repi(i, q)
  {
    auto [u, v] = uvs[i];
    assert(0 <= u && u < n && 0 <= v && v < n);
    if (in[u] > in[v])
      swap(u, v);
    int w = g.lca(u, v);
    if (u == w)
      lrs[i] = {in[u] + edge, in[v] + 1};
    else
    {
      lrs[i] = {out[u], in[v] + 1};
      if (!edge)
        extra[i] = w;
    }
  }

  vb active(n, false);
  auto flip = [&](int i, bool)
  {
    int v = tour[i];
    if (edge && v == g.root())
      return;
    if (active[v])
      del(v);
    else
      add(v);
    active[v] = !active[v];
  };
  auto answer = [&](int i)
  {
    // 区間に現れない LCA を、答えの取得時だけ追加する。
    if (extra[i] != -1)
      add(extra[i]);
    rem(i);
    if (extra[i] != -1)
      del(extra[i]);
  };
  mo(2 * n, lrs, flip, flip, answer);
}
