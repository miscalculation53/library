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
  assert(SZ(es) == n - 1 && SZ(costs) == n - 1);
  assert(all_of(ALL(costs), [](const Cost &x) { return x >= 0; }));
  auto weighted_depths = [&](const RootedTree &t)
  {
    vc<Cost> values(n, 0);
    repi(i, n - 1)
    {
      auto [u, v] = es[i];
      values[t.parent_child(u, v).second] = costs[i];
    }
    return t.weighted_depths(move(values));
  };
  RootedTree g(n, es, 0);
  vc<Cost> wds0 = weighted_depths(g);
  int a = ARGMAX(GEN_VEC(n, i, wds0[i]));
  RootedTree ga(n, es, a);
  vc<Cost> wdsa = weighted_depths(ga);
  int b = ARGMAX(GEN_VEC(n, i, wdsa[i]));
  return {a, b, ga, wdsa};
}
