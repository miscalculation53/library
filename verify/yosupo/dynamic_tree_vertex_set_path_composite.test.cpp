#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite"

#ifndef LOCAL
#define FAST_IO
#endif

#include "algebra/affine_function.hpp"
#include "math/modint/modint.hpp"
#include "tree/link_cut_tree.hpp"

int main()
{
  using M = OppositeMonoid<MonoidAffineFunction<modint998244353>>;
  INT(n, q);
  vc<M::S> values;
  values.reserve(n);
  for (int i = 0; i < n; ++i)
  {
    INT(a, b);
    values.emplace_back(a, b);
  }
  LinkCutTree<M> tree(values);
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
      INT(p, a, b);
      tree.set(p, {a, b});
    }
    else
    {
      INT(u, v, x);
      auto [a, b] = *tree.prod(u, v);
      PRINT((a * x + b).val());
    }
  }
}
