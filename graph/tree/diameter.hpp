#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "rooted_tree.hpp"

/**
 * @brief 木の直径
 * @docs docs/graph/tree/diameter.md
 */

// (a, b, ga)
// a, b: 端点、ga: a を根とする根つき木
template <class Pair>
tuple<int, int, RootedTree> tree_diameter(int n, const vc<Pair> &es)
{
  RootedTree g(n, es, 0);
  int a = ARGMAX(GEN_VEC(n, i, g.depth(i)));
  RootedTree ga(n, es, a);
  int b = ARGMAX(GEN_VEC(n, i, ga.depth(i)));
  return {a, b, ga};
}

// (a, b, ga, wdsa)
// a, b: 端点、ga: a を根とする根つき木、wdsa: a を根としたときの各頂点の重みつき深さ
template <class Pair, class Cost>
tuple<int, int, RootedTree, vc<Cost>> tree_diameter_weighted(int n, const vc<Pair> &es, const vc<Cost> &costs)
{
  assert(MIN(costs) >= 0);
  RootedTree g(n, es, 0);
  vc<Cost> wds0 = g.weighted_depths(g.reordered_edge_info(es, costs));
  int a = ARGMAX(GEN_VEC(n, i, wds0[i]));
  RootedTree ga(n, es, a);
  vc<Cost> wdsa = ga.weighted_depths(ga.reordered_edge_info(es, costs));
  int b = ARGMAX(GEN_VEC(n, i, wdsa[i]));
  return {a, b, ga, wdsa};
}
