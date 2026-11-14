#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum"

#ifndef LOCAL
#define FAST_IO
#endif

#include "tree/link_cut_tree.hpp"

int main()
{
  INT(n, q);
  VEC(ll, n, values);
  LinkCutTree<MonoidAdd<ll>> tree(values);
  for (int i = 1; i < n; ++i)
  {
    INT(u, v);
    tree.link(u, v);
  }
  while (q--)
  {
    INT(type);
    if (type == 0)
    {
      INT(u, v, w, x);
      tree.cut(u, v);
      tree.link(w, x);
    }
    else if (type == 1)
    {
      INT(p);
      LL(x);
      tree.set(p, tree.get(p) + x);
    }
    else
    {
      INT(u, v);
      PRINT(*tree.prod(u, v));
    }
  }
}
