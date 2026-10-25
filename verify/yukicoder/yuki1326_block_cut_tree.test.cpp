#define PROBLEM "https://yukicoder.me/problems/no/1326"

#define SINGLE_TESTCASE
#ifndef LOCAL
#define FAST_IO
#endif

#include "tree/block_cut_tree.hpp"
#include "tree/add_virtual_root.hpp"

void init() {}
void main2()
{
  INT(n, m);
  VEC(pll, m, es);
  for (auto &[u, v] : es) --u, --v;
  const BlockCutTree bct(GraphUndirected<>(n, es));
  vc<pair<int, int>> forest;
  for (const auto &e : bct.forest().edges()) forest.eb(e.from, e.to);
  const auto tree = add_virtual_root(bct.size(), forest);
  vc<int> prefix(tree.size());
  for (int i = 1; i < tree.size(); ++i)
  {
    const int v = tree.preorder_select(i);
    prefix[v] = prefix[tree.parent(v)] + bct.is_articulation_node(v);
  }
  INT(q);
  while (q--)
  {
    INT(u, v);
    --u, --v;
    if (u == v)
    {
      PRINT(0);
      continue;
    }
    const int x = bct.node_id(u), y = bct.node_id(v), z = tree.lca(x, y);
    const int on_path = prefix[x] + prefix[y] - 2 * prefix[z]
                     + (z < bct.size() && bct.is_articulation_node(z));
    PRINT(on_path - bct.is_articulation_node(x) - bct.is_articulation_node(y));
  }
}
void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
