#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/tree/mo_on_tree.hpp"

// BFS on the input graph supplies paths independently of RootedTree and LCA.
vc<int> bfs_parents(const vvc<int> &adj, int root)
{
  vc<int> par(adj.size(), -1), que{root};
  par[root] = root;
  repi(i, adj.size())
    for (int to : adj[que[i]])
      if (par[to] == -1)
        par[to] = que[i], que.push_back(to);
  return par;
}

template <class I>
void check_queries(const vc<pair<int, int>> &es, int root, const vc<pair<I, I>> &uvs)
{
  const int n = es.size() + 1, q = uvs.size();
  vvc<int> adj(n);
  for (auto [u, v] : es)
    adj[u].push_back(v), adj[v].push_back(u);
  auto par = bfs_parents(adj, root);
  vvc<int> from(n);
  repi(v, n) from[v] = bfs_parents(adj, v);
  RootedTree g(n, es, root), from_par(n, par);

  for (const RootedTree *tree : {&g, &from_par})
    for (bool edge : {false, true})
    {
      vc<int> active(n), cnt(5), seen(q);
      int distinct = 0, updates = 0;
      auto add = [&](int v)
      {
        assert(0 <= v && v < n && active[v] == 0);
        assert(!edge || v != root);
        updates++;
        active[v] = 1;
        distinct += (cnt[v % 5]++ == 0);
      };
      auto del = [&](int v)
      {
        assert(0 <= v && v < n && active[v] == 1);
        assert(!edge || v != root);
        updates++;
        active[v] = 0;
        distinct -= (--cnt[v % 5] == 0);
      };
      auto rem = [&](int qid)
      {
        assert(0 <= qid && qid < q && seen[qid]++ == 0);
        auto [u, v] = uvs[qid];
        vc<int> expected(n), expected_cnt(5);
        if (!edge)
          expected[u] = 1;
        for (int x = v; x != u; x = from[u][x])
        {
          int p = from[u][x];
          expected[edge && par[p] == x ? p : x] = 1;
        }
        assert(active == expected);
        repi(x, n) expected_cnt[x % 5] += expected[x];
        assert(cnt == expected_cnt);
        int expected_distinct = 0;
        for (int c : expected_cnt) expected_distinct += (c != 0);
        assert(distinct == expected_distinct);
      };
      if (edge)
        mo_on_tree(*tree, uvs, add, del, rem, true);
      else
        mo_on_tree(*tree, uvs, add, del, rem);
      for (int count : seen) assert(count == 1);
      if (uvs.empty())
        assert(updates == 0 && active == vc<int>(n) && distinct == 0);
    }
}

void test_small_trees()
{
  mt19937 rng(1729);
  repi(n, 1, 25) repi(shape, 3)
  {
    vc<int> labels = permid<int>(n);
    shuffle(labels.begin(), labels.end(), rng);
    vc<pair<int, int>> es, uvs;
    repi(v, 1, n)
    {
      int p = shape == 0 ? v - 1 : shape == 1 ? 0 : rng() % v;
      es.emplace_back(labels[p], labels[v]);
      if (rng() % 2) swap(es.back().first, es.back().second);
    }
    shuffle(es.begin(), es.end(), rng);
    repi(u, n) repi(v, n) uvs.emplace_back(u, v);
    uvs.emplace_back(labels.back(), labels.front());
    uvs.emplace_back(labels.back(), labels.front());
    shuffle(uvs.begin(), uvs.end(), rng);
    for (int root : {labels.front(), labels.back(), int(rng() % n)})
    {
      check_queries(es, root, uvs);
      check_queries(es, root, vc<pair<int, int>>{});
      check_queries(es, root, vc<pair<ll, ll>>{{root, root}});
      check_queries(es, root, vc<pair<ll, ll>>{{labels.back(), labels.front()}});
    }
  }
}

void test_long_chain()
{
  const int n = 200000, root = n / 2;
  vc<pair<int, int>> es;
  repi(v, 1, n) es.emplace_back(v - 1, v);
  RootedTree g(n, es, root);
  vc<pair<ll, ll>> uvs{{0, n - 1}, {n - 1, 0}, {root, root}, {0, 0}, {n - 1, n - 1}};
  mt19937 rng(314159);
  repi(i, 100) uvs.emplace_back(rng() % n, rng() % n);
  for (bool edge : {false, true})
  {
    vc<int> active(n), seen(uvs.size());
    ll sum = 0;
    auto add = [&](int v)
    {
      assert(active[v] == 0);
      active[v] = 1;
      sum += v + 1;
    };
    auto del = [&](int v)
    {
      assert(active[v] == 1);
      active[v] = 0;
      sum -= v + 1;
    };
    auto rem = [&](int qid)
    {
      assert(seen[qid]++ == 0);
      auto [u, v] = uvs[qid];
      if (u > v) swap(u, v);
      ll expected = (u + v + 2) * (v - u + 1) / 2;
      if (edge) expected -= clamp<ll>(root, u, v) + 1;
      assert(sum == expected);
    };
    mo_on_tree(g, uvs, add, del, rem, edge);
    for (int count : seen) assert(count == 1);
  }
}

int main()
{
  test_small_trees();
  test_long_chain();
  cout << "Hello World" << endl;
}
