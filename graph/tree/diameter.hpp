#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "rooted_tree.hpp"

/**
 * @brief 木の直径
 * @docs docs/graph/tree/diameter.md
 */

// 深さに基づいた直径を求める
// (a, b, ga)
// a, b: 端点、ga: a を根とする根つき木
template <class I = ll, class Cost, bool need_dist>
tuple<I, I, RootedTree<Cost, need_dist>> tree_diameter_by_depth(const RootedTree<Cost, need_dist> &g)
{
  I a = ARGMAX(GEN_VEC(g.size(), i, g.depth(i)));
  auto ga = g.rerooted_tree(a);
  I b = ARGMAX(GEN_VEC(g.size(), i, ga.depth(i)));
  return {a, b, ga};
}

// コストに基づいた直径を求める (コストは非負)
// (a, b, ga)
// a, b: 端点、ga: a を根とする根つき木
template <class I = ll, class Cost>
tuple<I, I, RootedTree<Cost, true>> tree_diameter_by_cost(const RootedTree<Cost, true> &g)
{
  I a = ARGMAX(GEN_VEC(g.size(), i, g.dist(i)));
  auto ga = g.rerooted_tree(a);
  I b = ARGMAX(GEN_VEC(g.size(), i, ga.dist(i)));
  return {a, b, ga};
}
