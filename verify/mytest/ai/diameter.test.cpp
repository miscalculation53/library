#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "tree/diameter.hpp"
#include "tree/euler_tour.hpp"

// 全始点から独立に距離と経路を求め、直径・中心・仮想中心を確認する。
template <class Cost>
vvc<Cost> all_distances(int n, const vc<pair<int, int>> &es, const vc<Cost> &costs)
{
  vvc<pair<int, Cost>> adj(n);
  repi(i, es.size())
  {
    auto [u, v] = es[i];
    adj[u].eb(v, costs[i]), adj[v].eb(u, costs[i]);
  }
  vvc<Cost> ds(n, vc<Cost>(n));
  repi(s, n)
  {
    vc<int> par(n, -1), que{s};
    par[s] = s;
    for (int i = 0; i < int(que.size()); ++i)
    {
      int v = que[i];
      for (auto [to, cost] : adj[v]) if (to != par[v])
      {
        par[to] = v;
        ds[s][to] = ds[s][v] + cost;
        que.eb(to);
      }
    }
  }
  return ds;
}

template <class D, class Cost>
void check_diameter(const D &d, const vvc<Cost> &ds)
{
  const int n = ds.size();
  Cost length = 0;
  vc<Cost> eccentricities(n);
  repi(u, n) repi(v, n)
  {
    length = max(length, ds[u][v]);
    eccentricities[u] = max(eccentricities[u], ds[u][v]);
  }
  auto [a, b] = d.endpoints();
  assert(d.length() == length && ds[a][b] == length);
  const auto path = d.path_vertex();
  assert(!path.empty() && path.front() == a && path.back() == b);
  const auto radius = *min_element(ALL(eccentricities));
  vc<int> centers;
  repi(v, n) if (eccentricities[v] == radius) centers.eb(v);
  assert(d.centers() == centers);
  const auto groups = d.endpoint_groups();
  vc<int> membership(n, -1);
  int previous = -1;
  repi(i, groups.size())
  {
    const auto &group = groups[i];
    assert(!group.empty() && previous < group.front());
    assert(is_sorted(ALL(group)));
    previous = group.front();
    for (int v : group)
    {
      assert(0 <= v && v < n && membership[v] == -1);
      membership[v] = i;
    }
  }
  repi(v, n) assert((membership[v] != -1) == (eccentricities[v] == length));
  repi(u, n) repi(v, u + 1, n)
    assert((membership[u] != -1 && membership[v] != -1 && membership[u] != membership[v]) == (ds[u][v] == length));
}

void check_unweighted(int n, const vc<pair<int, int>> &es, mt19937 &rng)
{
  const Diameter d(n, es);
  static_assert(is_same_v<decltype(d.length()), ll>);
  const auto ds = all_distances(n, es, vc<ll>(n - 1, 1));
  check_diameter(d, ds);
  assert(ll(d.path_vertex().size()) == d.length() + 1);
  const auto cs = d.centers();
  const auto centered = d.virtual_center_tree();
  const int root = centered.root();
  assert(centered.size() == n + d.length() % 2);
  if (cs.size() == 1) assert(root == cs[0]);
  else
  {
    assert(root == n);
    assert(centered.parent(cs[0]) == n && centered.parent(cs[1]) == n);
    assert(centered.children(root).to_v().size() == 2);
  }
  ll height = 0;
  repi(v, n) height = max(height, centered.depth(v));
  assert(height == (d.length() + 1) / 2);
  vc<int> color(n);
  repi(v, n) color[v] = rng() % 4;
  ll expected = 0, actual = 0;
  repi(u, n) repi(v, u + 1, n)
  {
    const bool endpoints = centered.depth(u) == height && centered.depth(v) == height
        && centered.lca(u, v) == root;
    assert(endpoints == (ds[u][v] == d.length()));
    if (color[u] == color[v] && ds[u][v] == d.length()) ++expected;
    // 仮想中心を通るパスでは、奇数直径の中央辺の分割で長さが1増える。
    assert(centered.dist(u, v) == ds[u][v] + (cs.size() == 2 && centered.lca(u, v) == root));
  }
  array<ll, 4> seen{};
  for (int child : centered.children(root))
  {
    array<ll, 4> count{};
    repi(i, centered.preorder(child), centered.postorder(child))
    {
      const int v = centered.preorder_select(i);
      if (centered.depth(v) == height) ++count[color[v]];
    }
    repi(c, 4) actual += count[c] * seen[c], seen[c] += count[c];
  }
  assert(actual == expected);

  const RootedTree base(n, es, d.endpoints().first);
  const auto old_parents = base.parents();
  for (int rt : {0, n - 1, int(rng() % n)})
  {
    const auto tree = base.rerooted(rt);
    const RootedTree expected_tree(n, es, rt);
    assert(tree.parents() == expected_tree.parents());
    assert(base.parents() == old_parents);
    const EulerTour tour(n, es, rt);
    const EulerTour<false> simple(n, es, rt);
    repi(u, n)
    {
      assert(tree.depth(u) == ds[rt][u]);
      assert(tree.path_vertex_to_root(u) == tour.path_vertex_to_root(u));
      assert(tree.path_vertex_to_root(u) == simple.path_vertex_to_root(u));
      repi(v, n)
      {
        const auto path = tree.path_vertex(u, v);
        assert(path.front() == u && path.back() == v && ll(path.size()) == ds[u][v] + 1);
        assert(path == tour.path_vertex(u, v) && path == simple.path_vertex(u, v));
        assert(tree.lca(u, v) == expected_tree.lca(u, v));
        // ABC267 F: より遠い直径端点へ進むことで距離 k の頂点を得る。
        auto [a, b] = d.endpoints();
        const int target = ds[u][a] >= ds[u][b] ? a : b;
        for (int k : {0, int(ds[u][target]), int(ds[u][target]) + 1})
        {
          const auto answer = tree.jump(u, target, k);
          assert(bool(answer) == (k <= max(ds[u][a], ds[u][b])));
          if (answer) assert(ds[u][*answer] == k);
        }
      }
    }
  }
  auto copied = d;
  auto moved = std::move(copied);
  assert(moved.endpoints() == d.endpoints() && moved.centers() == cs);
}

template <class I, class Cost>
void check_tuple_input(const Diameter<Cost> &expected, int n, const vc<pair<int, int>> &es, const vc<Cost> &costs)
{
  vc<tuple<I, I, Cost>> triples;
  repi(i, es.size()) triples.eb(I(es[i].first), I(es[i].second), costs[i]);
  const auto &input = triples;
  const Diameter deduced(n, input);
  static_assert(is_same_v<remove_cv_t<decltype(deduced)>, Diameter<Cost>>);
  const Diameter<Cost> explicit_cost(n, vc<tuple<I, I, Cost>>(triples));
  triples.clear();
  for (const auto *d : {&deduced, &explicit_cost})
  {
    assert(d->endpoints() == expected.endpoints());
    assert(d->length() == expected.length());
    assert(d->path_vertex() == expected.path_vertex());
    assert(d->centers() == expected.centers());
    assert(d->endpoint_groups() == expected.endpoint_groups());
    const auto [tree, costs2] = d->virtual_center_tree();
    const auto [expected_tree, expected_costs2] = expected.virtual_center_tree();
    assert(tree.parents() == expected_tree.parents() && costs2 == expected_costs2);
  }
}

template <class Cost>
void check_weighted(int n, const vc<pair<int, int>> &es, vc<Cost> costs)
{
  const Diameter d(n, es, costs);
  static_assert(is_same_v<decltype(d.length()), Cost>);
  const auto ds = all_distances(n, es, costs);
  check_diameter(d, ds);
  check_tuple_input<ll>(d, n, es, costs);
  Cost path_length = 0;
  const auto path = d.path_vertex();
  repi(i, 1, path.size())
  {
    bool found = false;
    repi(e, es.size())
    {
      auto [u, v] = es[e];
      if ((u == path[i - 1] && v == path[i]) || (v == path[i - 1] && u == path[i]))
        path_length += costs[e], found = true;
    }
    assert(found);
  }
  assert(path_length == d.length());
}

int main()
{
  mt19937 rng(482221267);
  repi(n, 1, 25) repi(trial, 9)
  {
    vc<int> labels(n);
    iota(ALL(labels), 0);
    shuffle(ALL(labels), rng);
    vc<pair<int, int>> es;
    repi(v, 1, n)
    {
      const int p = trial % 3 == 0 ? v - 1 : trial % 3 == 1 ? 0 : rng() % v;
      es.eb(labels[p], labels[v]);
      if (rng() & 1) swap(es.back().first, es.back().second);
    }
    shuffle(ALL(es), rng);
    check_unweighted(n, es, rng);
    vc<ll> costs(n - 1);
    repi(i, n - 1) costs[i] = trial % 3 == 0 ? 0 : trial % 3 == 1 ? 1 : rng() % 11;
    check_weighted(n, es, costs);
  }
  const vc<pair<int, int>> es{{0, 1}, {1, 2}, {1, 3}, {3, 4}};
  check_weighted<int>(5, es, {0, 2, 0, 3});
  check_weighted<i128>(5, es, {i128(1) << 90, 2, 3, i128(1) << 91});
  check_weighted<u128>(5, es, {u128(1) << 90, 2, 3, u128(1) << 91});
  check_weighted<double>(5, es, {0.5, 1.25, 0.0, 2.5});
  const vc<ll> costs{0, 2, 0, 3};
  const Diameter weighted(5, es, costs);
  check_tuple_input<int>(weighted, 5, es, costs);
  check_tuple_input<unsigned>(weighted, 5, es, costs);
  check_tuple_input<i128>(weighted, 5, es, costs);
  check_tuple_input<u128>(weighted, 5, es, costs);
  const Diameter tuple_edges(3, vc<tuple<ll, ll>>{{0, 1}, {1, 2}});
  assert(tuple_edges.centers() == vc<int>{1});
  const Diameter array_edges(3, vc<array<int, 2>>{{0, 1}, {1, 2}}, vc<int>{2, 5});
  assert(array_edges.length() == 7 && array_edges.centers() == vc<int>{1});
  for (int n : {200000, 200001})
  {
    vc<pair<int, int>> edges;
    repi(v, 1, n) edges.eb(v - 1, v);
    const Diameter d(n, edges);
    const auto tree = d.virtual_center_tree();
    auto [a, b] = d.endpoints();
    assert(d.length() == n - 1);
    assert(tree.depth(a) == n / 2 && tree.depth(b) == n / 2);
    assert(tree.size() == n + (n - 1) % 2);
    assert(RootedTree(n, edges, a).rerooted(n / 3).depth(0) == n / 3);
  }
  cout << "Hello World" << endl;
}
