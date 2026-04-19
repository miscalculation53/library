#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "rooted_tree.hpp"

/**
 * @brief DSU on Tree
 * @docs docs/graph/tree/dsu_on_tree.md
 */

// void add(v): 頂点 v を追加
// void reset(v): 頂点 v の部分木の頂点がすべて入った状態から始めて、初期状態に戻す
// void rem(v): 頂点 v の部分木についてのクエリにすべて答える
template <class Add, class Reset, class Rem>
void dsu_on_tree(const RootedTree &g, const Add &add, const Reset &reset, const Rem &rem)
{
  auto dfs = [&](auto dfs, int v, bool keep) -> void
  {
    fec(c : g.light_children(v)) dfs(dfs, c, false);
    if (g.heavy_child(v) != -1)
      dfs(dfs, g.heavy_child(v), true);
    add(v);
    fec(c : g.light_children(v)) repi(i, g.preorder(c), g.postorder(c)) add(g.preorder_select(i));
    rem(v);
    if (!keep)
      reset(v);
  };
  dfs(dfs, g.root(), true);
}
