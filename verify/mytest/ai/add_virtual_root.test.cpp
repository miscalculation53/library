#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/tree/add_virtual_root.hpp"

// Compare the resulting HLD tree with explicit parent walks.
void check_tree(const RootedTree &tree, const vc<int> &par, mt19937 &rng)
{
  int n = par.size() - 1;
  assert(tree.size() == n + 1 && tree.root() == n);
  repi(v, n) assert(tree.parent(v) == par[v]);
  repi(iter, 100)
  {
    int u = rng() % (n + 1), v = rng() % (n + 1);
    vc<int> path;
    for (int x = u; x != -1; x = par[x]) path.eb(x);
    int expected = v;
    while (find(ALL(path), expected) == path.end()) expected = par[expected];
    assert(tree.lca(u, v) == expected);
  }
}

void test_examples()
{
  mt19937 rng(1);
  vc<int> par = {-1, 0, -1, 2, 2};
  auto tree = add_virtual_root(5, par);
  check_tree(tree, vc<int>{5, 0, 5, 2, 2, -1}, rng);
  assert(tree.lca(3, 4) == 2 && tree.lca(1, 3) == 5);
  vc<pair<int, int>> edges = {{1, 0}, {3, 2}, {2, 4}};
  check_tree(add_virtual_root(5, edges), vc<int>{5, 0, 5, 2, 2, -1}, rng);
  vc<tuple<ll, ll>> tuples = {{1, 0}, {3, 2}, {2, 4}};
  check_tree(add_virtual_root(5, tuples), vc<int>{5, 0, 5, 2, 2, -1}, rng);
  vc<array<unsigned, 2>> arrays = {{{1, 0}}, {{3, 2}}, {{2, 4}}};
  check_tree(add_virtual_root(5, arrays), vc<int>{5, 0, 5, 2, 2, -1}, rng);
  vl wide = {2, LLONG_MIN, 2};
  check_tree(add_virtual_root(3, wide), vc<int>{2, 3, 3, -1}, rng);
  vc<unsigned> unsigned_par = {2, 1, 2};
  check_tree(add_virtual_root(3, unsigned_par), vc<int>{2, 3, 3, -1}, rng);
  vc<unsigned char> narrow(300, 0);
  vc<int> narrow_expected(301, 0);
  narrow_expected[0] = 300, narrow_expected[300] = -1;
  check_tree(add_virtual_root(300, narrow), narrow_expected, rng);
  check_tree(add_virtual_root(0, vc<int>{}), vc<int>{-1}, rng);
  check_tree(add_virtual_root(0, vc<pair<int, int>>{}), vc<int>{-1}, rng);
  check_tree(add_virtual_root(1, vc<int>{0}), vc<int>{1, -1}, rng);
  check_tree(add_virtual_root(4, vc<pair<int, int>>{}), vc<int>{4, 4, 4, 4, -1}, rng);
}

void test_random_forests()
{
  mt19937 rng(5719);
  for (int n : {1, 2, 7, 30, 100}) repi(trial, 40)
  {
    vc<int> order(n), par(n, -1);
    iota(ALL(order), 0);
    shuffle(ALL(order), rng);
    vc<pair<int, int>> edges;
    vvc<int> adj(n);
    repi(i, n)
    {
      int v = order[i];
      if (i == 0 || rng() % 4 == 0) par[v] = rng() % 2 ? v : -3;
      else
      {
        int p = order[rng() % i];
        par[v] = p;
        edges.eb(p, v);
        if (rng() % 2) swap(edges.back().first, edges.back().second);
        adj[p].eb(v), adj[v].eb(p);
      }
    }
    shuffle(ALL(edges), rng);
    vc<int> expected(n + 1, -1);
    repi(v, n) expected[v] = par[v] < 0 || par[v] == v ? n : par[v];
    check_tree(add_virtual_root(n, par), expected, rng);

    // Reorient each component from its minimum vertex, independently of the input roots.
    vc<bool> seen(n);
    fill(ALL(expected), -1);
    repi(root, n) if (!seen[root])
    {
      vc<int> stack{root};
      expected[root] = n;
      seen[root] = true;
      while (!stack.empty())
      {
        int v = stack.back();
        stack.pop_back();
        for (int to : adj[v]) if (!seen[to])
        {
          seen[to] = true;
          expected[to] = v;
          stack.eb(to);
        }
      }
    }
    check_tree(add_virtual_root(n, edges), expected, rng);
  }
}

void test_long_chain()
{
  const int n = 200000;
  vc<int> par(n);
  vc<pair<int, int>> edges;
  repi(v, n - 1) par[v] = v + 1, edges.eb(v + 1, v);
  par[n - 1] = n - 1;
  auto from_par = add_virtual_root(n, par);
  assert(from_par.root() == n && from_par.depth(0) == n);
  assert(from_par.la(0, n - 1) == n - 1 && from_par.la(0, n) == n);
  auto from_edges = add_virtual_root(n, edges);
  assert(from_edges.root() == n && from_edges.depth(n - 1) == n);
  assert(from_edges.la(n - 1, n - 1) == 0 && from_edges.la(n - 1, n) == n);
}

int main()
{
  test_examples();
  test_random_forests();
  test_long_chain();
  PRINT("Hello World");
}
