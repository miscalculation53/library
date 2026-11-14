#define PROBLEM "https://judge.yosupo.jp/problem/incremental_minimum_spanning_forest"

#ifndef LOCAL
#define FAST_IO
#endif

#include "tree/link_cut_tree.hpp"

struct MaximumEdge
{
  using S = pair<int, int>;
  static S op(const S &a, const S &b) { return max(a, b); }
  static S e() { return {-1, -1}; }
};

int main()
{
  INT(n, m);
  EdgeLinkCutTree<MaximumEdge> tree(n);
  vc<pair<int, int>> ends(m);
  for (int i = 0; i < m; ++i)
  {
    INT(u, v, w);
    ends[i] = {u, v};
    int removed = -1;
    if (auto current = tree.prod(u, v))
    {
      const auto [max_weight, edge] = *current;
      removed = i;
      if (max_weight > w)
      {
        tree.cut(ends[edge].first, ends[edge].second);
        removed = edge;
      }
    }
    if (removed != i) tree.link(u, v, {w, i});
    PRINT(removed);
  }
}
