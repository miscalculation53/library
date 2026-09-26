#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/tree/rooted_tree.hpp"

using Segment = tuple<int, int, bool>;

// 入力の隣接リストから BFS でパスを求め、HLD の区間を展開した列と比較する。
vc<int> bfs_parents(const vvc<int> &adj, int root)
{
  vc<int> par(adj.size(), -1), que{root};
  par[root] = root;
  for (int i = 0; i < int(que.size()); ++i)
    for (int to : adj[que[i]])
      if (par[to] == -1)
        par[to] = que[i], que.push_back(to);
  return par;
}

vc<int> expand(const RootedTree &tree, const RootedTree::PathRange &range)
{
  vc<int> res;
  for (auto [l, r, isrev] : range)
  {
    assert(0 <= l && l < r && r <= tree.size());
    assert(tree.head(tree.preorder_select(l)) == tree.head(tree.preorder_select(r - 1)));
    if (isrev)
      for (int i = r - 1; i >= l; --i) res.push_back(tree.preorder_select(i));
    else
      for (int i = l; i < r; ++i) res.push_back(tree.preorder_select(i));
  }
  return res;
}

void check_path(const RootedTree &tree, int u, int v,
                const vc<int> &from_u, const vc<int> &parent)
{
  vc<int> path{v};
  while (path.back() != u) path.push_back(from_u[path.back()]);
  reverse(path.begin(), path.end());
  for (bool edge : {false, true})
  {
    vc<int> expected;
    if (edge)
    {
      for (int i = 1; i < int(path.size()); ++i)
      {
        int a = path[i - 1], b = path[i];
        expected.push_back(parent[a] == b ? a : b);
      }
    }
    else
      expected = path;

    const auto range = tree.path_query(u, v, edge);
    assert(expand(tree, range) == expected);
    assert(range.empty() == (edge && u == v));
    assert(range.size() == distance(range.begin(), range.end()));
    assert(range.size() <= 2 * int(bit_width(tree.size())));
    assert((range.to_v() == vc<Segment>(range.begin(), range.end())));
    if (!range.empty())
    {
      auto it = range.begin();
      auto copy = it;
      assert(*it++ == *copy);
      assert(it == ++copy);
    }

    // コピーした範囲と複数の走査を独立に扱える。
    const auto saved = range;
    auto other = tree.path_query(v, u, edge);
    other = tree.path_query(u, u);
    assert(expand(tree, saved) == expected);
    assert((expand(tree, other) == vc<int>{u}));
    for (const auto &segment : range)
    {
      const auto before = segment;
      for (auto [l, r, isrev] : tree.path_query(v, u))
      {
        assert(l < r);
        (void)isrev;
      }
      assert(segment == before);
    }
    int visited = 0;
    for (auto segment : range)
    {
      assert(segment == *range.begin());
      ++visited;
      break;
    }
    assert(visited == !range.empty());
    assert(expand(tree, range) == expected);
  }
}

void check_tree(int n, const vc<pair<int, int>> &es, int root, mt19937 &rng, bool all_pairs)
{
  vvc<int> adj(n);
  for (auto [u, v] : es)
    adj[u].push_back(v), adj[v].push_back(u);
  const auto parent = bfs_parents(adj, root);
  const RootedTree tree(n, es, root), from_parent(n, parent);
  for (int i = 0; i < (all_pairs ? n : 20); ++i)
  {
    const int u = all_pairs ? i : int(rng() % n);
    const auto from_u = bfs_parents(adj, u);
    for (int j = 0; j < (all_pairs ? n : 4); ++j)
    {
      const int v = all_pairs ? j : (j == 0 ? u : int(rng() % n));
      check_path(tree, u, v, from_u, parent);
      check_path(from_parent, u, v, from_u, parent);
    }
  }
}

int main()
{
  mt19937 rng(20260926);
  for (int n = 1; n <= 32; ++n)
    for (int shape = 0; shape < 6; ++shape)
    {
      vc<int> labels(n);
      iota(labels.begin(), labels.end(), 0);
      shuffle(labels.begin(), labels.end(), rng);
      vc<pair<int, int>> es;
      for (int v = 1; v < n; ++v)
      {
        int p = shape == 0 ? v - 1 : shape == 1 ? 0 : shape == 2 ? (v - 1) / 2 : int(rng() % v);
        es.emplace_back(labels[p], labels[v]);
        if (rng() % 2) swap(es.back().first, es.back().second);
      }
      shuffle(es.begin(), es.end(), rng);
      check_tree(n, es, int(rng() % n), rng, true);
    }

  for (int shape = 0; shape < 3; ++shape)
  {
    const int n = 65535;
    vc<pair<int, int>> es;
    for (int v = 1; v < n; ++v)
      es.emplace_back(shape == 0 ? v - 1 : shape == 1 ? 0 : (v - 1) / 2, v);
    check_tree(n, es, 0, rng, false);
  }

  // 区間列は木の寿命にも依存しない。
  const auto saved = RootedTree(3, vc<int>{-1, 0, 1}).path_query(2, 0);
  assert((saved.to_v() == vc<Segment>{{0, 3, true}}));
  cout << "Hello World" << endl;
}
