#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/tree/rooted_tree.hpp"

// Both sides of the boundary match a linear walk, including the absent-side sentinels.
int main()
{
  mt19937 rng(3756);
  for (int n : {1, 2, 7, 30, 100}) repi(trial, 20)
  {
    vc<int> order(n), p(n, -1);
    iota(ALL(order), 0);
    shuffle(ALL(order), rng);
    repi(i, 1, n) p[order[i]] = order[rng() % i];
    RootedTree tree(n, p);
    repi(iter, 500)
    {
      int v = rng() % n, limit = int(rng() % (n + 2)) - 1;
      int child = -1, parent = v;
      while (parent != -1 && tree.depth(parent) > limit)
      {
        child = parent;
        parent = p[parent];
      }
      int calls = 0;
      auto actual = tree.first_ancestor(v, [&](int x) { calls++; return tree.depth(x) <= limit; });
      assert((actual == pair<int, int>{child, parent}));
      if (child != -1) assert(tree.depth(child) > limit);
      if (parent != -1) assert(tree.depth(parent) <= limit);
      if (child != -1 && parent != -1) assert(p[child] == parent);
      int log = 0;
      while ((1 << log) < n) log++;
      assert(calls <= 2 * log + 3);
      assert((tree.first_ancestor(v, [](int) { return true; }) == pair<int, int>{-1, v}));
      assert((tree.first_ancestor(v, [](int) { return false; }) == pair<int, int>{order[0], -1}));
    }
  }
  int n = 200000;
  vc<int> p(n);
  repi(i, n - 1) p[i] = i + 1;
  p.back() = -1;
  RootedTree chain(n, p);
  repi(iter, 1000)
  {
    int v = rng() % n, threshold = rng() % (n + 1), calls = 0;
    auto found = chain.first_ancestor(v, [&](int x) { calls++; return x >= threshold; });
    pair<int, int> expected;
    if (threshold == n) expected = {n - 1, -1};
    else if (threshold <= v) expected = {-1, v};
    else expected = {threshold - 1, threshold};
    assert(found == expected);
    assert(calls <= 40);
  }
  PRINT("Hello World");
}
