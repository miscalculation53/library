#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "tree/rooted_tree.hpp"

// BFS と部分木サイズの集計から、構築結果を独立に確認する。
void check(const vc<pair<int, int>> &es, int root)
{
  const int n = es.size() + 1;
  vvc<int> adj(n);
  for (auto [u, v] : es) adj[u].push_back(v), adj[v].push_back(u);
  vc<int> par(n, -1), depth(n), size(n, 1), order{root};
  par[root] = root;
  for (int i = 0; i < n; ++i)
    for (int c : adj[order[i]]) if (par[c] == -1)
    {
      par[c] = order[i], depth[c] = depth[order[i]] + 1;
      order.push_back(c);
    }
  for (int i = n - 1; i > 0; --i) size[par[order[i]]] += size[order[i]];
  const RootedTree from_edges(n, es, root);
  par[root] = -1;
  const RootedTree from_parents(n, par);
  for (const RootedTree *tree : {&from_edges, &from_parents})
  {
    assert(tree->root() == root && tree->parents() == par);
    vc<int> seen(n);
    for (int v = 0; v < n; ++v)
    {
      const int in = tree->preorder(v), out = tree->postorder(v);
      assert(tree->depth(v) == depth[v]);
      assert(out - in == size[v] && 0 <= in && out <= n);
      assert(tree->preorder_select(in) == v && !seen[in]++);
      int next = in + 1, biggest = 0, children = 0;
      const int heavy = tree->heavy_child(v);
      for (int c : tree->children(v))
      {
        assert(par[c] == v && tree->preorder(c) == next);
        assert(tree->head(c) == (c == heavy ? tree->head(v) : c));
        next = tree->postorder(c), biggest = max(biggest, size[c]);
        ++children;
      }
      assert(next == out);
      assert(children == int(adj[v].size()) - (v != root));
      assert((heavy == -1) == (children == 0));
      if (heavy != -1) assert(size[heavy] == biggest);
      if (v != root) assert(tree->parent(v) == par[v]);
    }
    const auto [brackets, vertices] = tree->dfs_ordered_vertices();
    assert(vertices.size() == es.size() * 2 + 1 && vertices.front() == root);
    vc<int> stack{root};
    for (int i = 0; i < int(brackets.size()); ++i)
    {
      if (brackets[i] == '(')
      {
        assert(par[vertices[i + 1]] == stack.back());
        stack.push_back(vertices[i + 1]);
      }
      else
      {
        stack.pop_back();
        assert(!stack.empty() && vertices[i + 1] == stack.back());
      }
    }
    assert(stack == vc<int>{root});

    mt19937 rng(713 + root);
    auto climb = [&](int v, int k)
    {
      if (k > depth[v]) return -1;
      while (k--) v = par[v];
      return v;
    };
    for (int trial = 0; trial < min(n, 64); ++trial)
    {
      const int u = rng() % n, v = rng() % n;
      int a = u, b = v;
      while (depth[a] > depth[b]) a = par[a];
      while (depth[b] > depth[a]) b = par[b];
      while (a != b) a = par[a], b = par[b];
      assert(tree->lca(u, v) == a);
      const int length = depth[u] + depth[v] - 2 * depth[a];
      for (int k : {0, int(rng() % (length + 2)), length, length + 1})
      {
        const int expected = k > length ? -1 : k <= depth[u] - depth[a]
            ? climb(u, k) : climb(v, length - k);
        assert(tree->jump(u, v, k) == expected);
        assert(tree->la(u, k) == climb(u, k));
      }
    }
  }
}

int main()
{
  mt19937 rng(531084);
  for (int n : {1, 2, 3, 17, 100, 1000})
    for (int trial = 0; trial < 30; ++trial)
    {
      vc<int> labels(n);
      iota(labels.begin(), labels.end(), 0);
      shuffle(labels.begin(), labels.end(), rng);
      vc<pair<int, int>> es;
      for (int v = 1; v < n; ++v)
      {
        const int p = trial % 3 == 0 ? v - 1 : trial % 3 == 1 ? 0 : rng() % v;
        es.emplace_back(labels[v], labels[p]);
        if (rng() & 1) swap(es.back().first, es.back().second);
      }
      shuffle(es.begin(), es.end(), rng);
      check(es, rng() % n);
    }
  for (int shape = 0; shape < 3; ++shape)
  {
    const int n = 300000;
    vc<pair<int, int>> es;
    for (int v = 1; v < n; ++v)
      es.emplace_back(shape == 0 ? v - 1 : shape == 1 ? 0 : (v - 1) / 2, v);
    shuffle(es.begin(), es.end(), rng);
    check(es, shape == 0 ? n - 1 : n / 3);
  }
  cout << "Hello World" << endl;
}
