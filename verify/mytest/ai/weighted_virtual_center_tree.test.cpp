#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "tree/diameter.hpp"

// 元の隣接リストを全始点から走査し、返された木の距離と端点の組を照合する。
template <class Cost>
void check(int n, const vc<pair<int, int>> &edges, const vc<Cost> &costs, mt19937 &rng)
{
  vvc<pair<int, Cost>> adj(n);
  repi(i, edges.size())
  {
    auto [u, v] = edges[i];
    adj[u].eb(v, costs[i]), adj[v].eb(u, costs[i]);
  }
  vvc<Cost> ds(n, vc<Cost>(n));
  Cost diameter = 0;
  repi(s, n)
  {
    vc<int> par(n, -1), que{s};
    par[s] = s;
    for (int i = 0; i < int(que.size()); ++i)
    {
      const int v = que[i];
      for (auto [to, cost] : adj[v]) if (to != par[v])
      {
        par[to] = v;
        ds[s][to] = ds[s][v] + cost;
        diameter = max(diameter, ds[s][to]);
        que.eb(to);
      }
    }
  }
  const Diameter d(n, edges, costs);
  auto [tree, costs2] = d.virtual_center_tree();
  static_assert(is_same_v<decltype(costs2), vc<Cost>>);
  vc<int> exact;
  repi(v, n)
  {
    const Cost eccentricity = *max_element(ALL(ds[v]));
    if (eccentricity + eccentricity == diameter) exact.eb(v);
  }
  const int root = tree.root();
  assert(tree.size() == n + int(exact.empty()));
  assert(root == (exact.empty() ? n : exact.front()));
  assert(costs2.size() == size_t(tree.size()) && costs2[root] == 0);
  map<pair<int, int>, Cost> remaining;
  repi(i, edges.size()) remaining[minmax(edges[i].first, edges[i].second)] = costs[i] + costs[i];
  vc<int> split;
  repi(v, tree.size()) if (v != root)
  {
    int parent = *tree.parent(v);
    if (parent == n) split.eb(v);
    else
    {
      const pair<int, int> key = minmax(parent, int(v));
      assert(remaining.count(key) && remaining.at(key) == costs2[v]);
      remaining.erase(key);
    }
  }
  if (root == n)
  {
    assert(split.size() == 2 && remaining.size() == 1);
    const pair<int, int> key = minmax(split[0], split[1]);
    assert(remaining.count(key) && remaining.at(key) == costs2[split[0]] + costs2[split[1]]);
    assert(costs2[split[0]] > 0 && costs2[split[1]] > 0);
  }
  else assert(remaining.empty());
  assert(d.length() == diameter);
  const auto depths2 = tree.weighted_depths(costs2);
  Cost maximum = 0;
  repi(v, n)
  {
    assert(costs2[v] >= 0);
    maximum = max(maximum, depths2[v]);
  }
  assert(maximum == diameter);
  vc<int> colors(n);
  repi(v, n) colors[v] = rng() % 5;
  ll expected = 0, actual = 0;
  const auto groups = d.endpoint_groups();
  vc<int> membership(n, -1);
  repi(i, groups.size()) for (int v : groups[i])
  {
    assert(membership[v] == -1);
    membership[v] = i;
  }
  repi(u, n) repi(v, n)
  {
    const int lca = tree.lca(u, v);
    const Cost distance2 = (depths2[u] - depths2[lca]) + (depths2[v] - depths2[lca]);
    assert(distance2 == ds[u][v] + ds[u][v]);
    if (u < v)
    {
      const bool endpoints = depths2[u] == diameter && depths2[v] == diameter && lca == root;
      if (exact.size() <= 1) assert(endpoints == (ds[u][v] == diameter));
      else if (endpoints) assert(ds[u][v] == diameter);
      assert((membership[u] != -1 && membership[v] != -1 && membership[u] != membership[v]) == (ds[u][v] == diameter));
      if (colors[u] == colors[v] && ds[u][v] == diameter) ++expected;
    }
  }
  array<ll, 5> seen{};
  for (const auto &group : groups)
  {
    array<ll, 5> count{};
    for (int v : group) ++count[colors[v]];
    repi(color, 5)
    {
      actual += seen[color] * count[color];
      seen[color] += count[color];
    }
  }
  assert(actual == expected);
  auto copy = d;
  auto moved = move(copy);
  auto [again, again_costs2] = moved.virtual_center_tree();
  assert(again.parents() == tree.parents() && again_costs2 == costs2);
}

void explicit_cases(mt19937 &rng)
{
  check<ll>(1, {}, {}, rng);
  check<ll>(2, {{0, 1}}, {3}, rng);
  check<ll>(3, {{0, 1}, {1, 2}}, {2, 8}, rng);
  check<ll>(4, {{0, 1}, {0, 2}, {0, 3}}, {4, 4, 2}, rng);
  // 中心の0コスト成分と、中心から離れた0コスト辺。
  const vc<pair<int, int>> es{{0, 1}, {1, 2}, {0, 3}, {1, 4}, {2, 5}, {2, 6}, {6, 7}, {7, 8}};
  check<ll>(9, es, {0, 0, 5, 5, 5, 2, 0, 3}, rng);
  check<ll>(9, es, vc<ll>(8, 0), rng);
  check<int>(9, es, {0, 0, 5, 5, 5, 2, 0, 3}, rng);
  check<i128>(3, {{0, 1}, {1, 2}}, {i128(1) << 90, (i128(1) << 91) + 1}, rng);
  check<u128>(3, {{0, 1}, {1, 2}}, {u128(1) << 90, (u128(1) << 91) + 1}, rng);
  check<double>(5, {{0, 1}, {1, 2}, {2, 3}, {2, 4}}, {0.0, 1.25, 0.5, 0.75}, rng);
  check<long double>(3, {{0, 1}, {1, 2}}, {0.25L, 0.5L}, rng);
  const Diameter tuple_edges(3, vc<tuple<ll, ll>>{{0, 1}, {1, 2}}, vc<ll>{2, 8});
  auto [tree, costs2] = tuple_edges.virtual_center_tree();
  assert(tree.root() == 3 && costs2 == vc<ll>({4, 6, 10, 0}));
  const Diameter array_edges(3, vc<array<int, 2>>{{0, 1}, {1, 2}}, vc<ll>{3, 3});
  auto [tree2, weights2] = array_edges.virtual_center_tree();
  assert(tree2.root() == 1 && tree2.parents() == vc<int>({1, -1, 1}));
  assert(weights2 == vc<ll>({6, 0, 6}));
}

int main()
{
  mt19937 rng(20261101);
  explicit_cases(rng);
  repi(n, 1, 26) repi(trial, 30)
  {
    vc<int> labels(n);
    iota(ALL(labels), 0);
    shuffle(ALL(labels), rng);
    vc<pair<int, int>> edges;
    repi(v, 1, n)
    {
      const int p = trial % 3 == 0 ? v - 1 : trial % 3 == 1 ? 0 : rng() % v;
      edges.eb(labels[p], labels[v]);
      if (rng() & 1) swap(edges.back().first, edges.back().second);
    }
    shuffle(ALL(edges), rng);
    vc<ll> costs(n - 1);
    repi(i, n - 1) costs[i] = trial % 5 == 0 ? 0 : trial % 5 == 1 ? 1 : rng() % 8;
    check(n, edges, costs, rng);
  }
  for (int shape = 0; shape < 4; ++shape)
  {
    const int n = 200000 + shape % 2;
    vc<pair<int, int>> edges;
    vc<ll> costs;
    repi(v, 1, n)
    {
      edges.eb(shape < 2 ? v - 1 : 0, v);
      costs.eb(shape == 2 ? 0 : shape == 3 ? v % 3 : 1);
    }
    const Diameter d(n, edges, costs);
    auto [tree, costs2] = d.virtual_center_tree();
    const int expected_root = shape == 0 ? n : shape == 1 ? n / 2 : 0;
    assert(tree.root() == expected_root && tree.size() == n + int(expected_root == n));
    const auto depths2 = tree.weighted_depths(costs2);
    assert(*max_element(ALL(depths2)) == d.length());
    auto [a, b] = d.endpoints();
    assert(depths2[a] == d.length() && depths2[b] == d.length());
    assert(tree.lca(a, b) == (a == b ? a : tree.root()));
  }
  cout << "Hello World" << endl;
}
