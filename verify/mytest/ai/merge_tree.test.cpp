#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/tree/merge_tree.hpp"
#include "graph/tree/add_virtual_root.hpp"

// The oracle explicitly tracks the current history root of each original vertex.
int main()
{
  MergeTree empty;
  assert(empty.parents().empty());
  MergeTree merges(4);
  vl weight(4, 0);
  auto merge = [&](int u, int v, ll w)
  {
    int node = merges.merge(u, v);
    if (node != -1)
    {
      assert(node == SZ<int>(weight));
      weight.eb(w);
    }
    return node;
  };
  assert(merge(0, 1, 2) == 4);
  assert(merge(2, 3, 5) == 5);
  assert(merge(1, 2, 8) == 6);
  assert(merge(0, 3, 10) == -1);
  auto par = merges.parents();
  assert(par == vc<int>({4, 4, 5, 5, 6, 6, -1}));
  RootedTree tree(par.size(), par);
  assert(weight[tree.lca(0, 3)] == 8);
  assert(weight[tree.lca(0, 1)] == 2);
  assert(tree.children(0).empty());
  for (int child : tree.children(4)) assert(child == 0 || child == 1);
  par[0] = -1;
  assert(merges.parents()[0] == 4);

  mt19937 rng(3756);
  for (int n : {0, 1, 2, 7, 30, 64, 500})
  {
    MergeTree builder(n);
    vc<int> current(n), expected(n, -1);
    iota(ALL(current), 0);
    if (n > 0) repi(iter, 500)
    {
      int u = rng() % n, v = rng() % n;
      int a = current[u], b = current[v];
      int node = builder.merge(u, v);
      if (a == b) assert(node == -1);
      else
      {
        assert(node == SZ<int>(expected));
        expected[a] = expected[b] = node;
        expected.eb(-1);
        for (int &root : current) if (root == a || root == b) root = node;
      }
    }
    auto parents = builder.parents();
    assert(parents == expected);
    assert(n == 0 || SZ<int>(parents) <= 2 * n - 1);
    int virtual_root = parents.size();
    auto rooted = add_virtual_root(virtual_root, parents);
    int root_count = 0;
    for (int child : rooted.children(virtual_root))
    {
      assert(parents[child] == -1);
      root_count++;
    }
    assert(root_count == SZ<int>(set<int>(ALL(current))));
    repi(v, virtual_root)
    {
      assert(rooted.parent(v) == (parents[v] == -1 ? virtual_root : parents[v]));
      assert(parents[v] == -1 || parents[v] > v);
      int child_count = 0;
      for (int child : rooted.children(v))
      {
        assert(child < v && parents[child] == v);
        child_count++;
      }
      assert(child_count == (v < n ? 0 : 2));
    }
  }

  const int n = 200000;
  MergeTree chain(n);
  repi(v, 1, n) assert(chain.merge(0, v) == n + v - 1);
  auto parents = chain.parents();
  RootedTree rooted(parents.size(), parents);
  assert(rooted.root() == 2 * n - 2);
  assert(rooted.la(0, n - 1) == rooted.root());
  PRINT("Hello World");
}
