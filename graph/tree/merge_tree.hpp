#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../ds/uf/uf.hpp"

/**
 * @brief 成分の併合履歴を表す二分木
 * @docs docs/graph/tree/merge_tree.md
 */

struct MergeTree
{
private:
  UnionFind<UFDataEmpty<>> uf;
  vc<int> top, par;

public:
  explicit MergeTree(int n = 0)
  {
    assert(n >= 0);
    uf.reset(n);
    top.resize(n);
    iota(ALL(top), 0);
    par.reserve(size_t(n) * 2);
    par.assign(n, -1);
  }

  vc<int> parents() const { return par; }

  bool same(int u, int v) { return uf.same(u, v); }
  // 元の頂点 u, v の成分をマージする
  // 新しい親の番号 (マージ済みなら -1) を返す
  // 新しくマージされたら木に頂点が増える
  int merge(int u, int v)
  {
    int x = uf.leader(u), y = uf.leader(v);
    if (x == y) return -1;
    int a = top[x], b = top[y], z = par.size();
    par.eb(-1);
    par[a] = par[b] = z;
    int r = uf.merge(x, y);
    top[r] = z;
    return z;
  }
};
