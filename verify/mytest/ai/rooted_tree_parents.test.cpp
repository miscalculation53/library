#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/tree/rooted_tree.hpp"

int main()
{
  const vc<pair<int, int>> edges = {{0, 1}, {1, 2}, {1, 3}};
  const RootedTree tree(4, edges, 0);
  assert(tree.parents() == vc<int>({-1, 0, 1, 1}));
  assert(RootedTree(4, edges, 2).parents() == vc<int>({1, 2, -1, 1}));
  assert(RootedTree(4, vl{0, 0, 1, 1}).parents() == tree.parents());
  assert(RootedTree(1, vc<int>{0}).parents() == vc<int>({-1}));
  auto copy = tree.parents();
  copy[1] = 3;
  assert(tree.parents() == vc<int>({-1, 0, 1, 1}));

  mt19937 rng(9217);
  for (int n : {1, 2, 7, 30, 100}) repi(trial, 20)
  {
    vc<int> order(n), input(n, -1), expected(n, -1);
    iota(ALL(order), 0);
    shuffle(ALL(order), rng);
    int root = order[0];
    input[root] = trial % 2 ? root : -7;
    vc<pair<int, int>> es;
    repi(i, 1, n)
    {
      int v = order[i], p = order[rng() % i];
      input[v] = expected[v] = p;
      es.eb(v, p);
      if (rng() % 2) swap(es.back().first, es.back().second);
    }
    shuffle(ALL(es), rng);
    const RootedTree from_par(n, input), from_edges(n, es, root);
    assert(from_par.parents() == expected && from_edges.parents() == expected);
    const RootedTree rebuilt(n, from_edges.parents());
    assert(rebuilt.root() == root && rebuilt.parents() == expected);
    repi(v, n) assert(rebuilt.depth(v) == from_edges.depth(v));
  }

#ifdef LOCAL
  cp::options::es_style = cp::types::es_style_t::no_es;
  cp::options::max_line_width = 1000;
  cp::options::cont_indent_style = cp::types::cont_indent_style_t::minimal;
  string text = cp::export_var(tree);
  assert(text.find("RootedTree") != string::npos);
  assert(text.find("size()= 4") != string::npos);
  assert(text.find("root()= 0") != string::npos);
  assert(text.find("parents()= " + cp::export_var(tree.parents())) != string::npos);
  assert(cp::export_var(vc<RootedTree>{tree}).find("parents()=") != string::npos);
  dump(tree);
  dump(tree.parents() | cp::index());
#endif
  PRINT("Hello World");
}
