#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/tree/rooted_tree.hpp"
#include "graph/tree/euler_tour_path_sum.hpp"
#include "graph/tree/euler_tour_path_prod.hpp"
#include "graph/tree/diameter.hpp"
#include "graph/tree/rerooting.hpp"

// A finite noncommutative group checks path order without numerical overflow.
struct PermutationGroup
{
  using S = array<int, 3>;
  static S e() { return {0, 1, 2}; }
  static S op(const S &a, const S &b) { return {a[b[0]], a[b[1]], a[b[2]]}; }
  static S inv(const S &a)
  {
    S res;
    repi(i, 3) res[a[i]] = i;
    return res;
  }
};

struct DistanceSum
{
  using S = pair<ll, ll>;
  static S e() { return {0, 0}; }
  static S op(const S &a, const S &b) { return {a.first + b.first, a.second + b.second}; }
};

struct ValueWithoutDefault
{
  int value;
  explicit ValueWithoutDefault(int value) : value(value) {}
};

// The reference paths use BFS on the input adjacency list, independently of HLD/LCA.
vc<int> bfs_parents(const vvc<int> &adj, int root)
{
  vc<int> par(adj.size(), -1), que{root};
  par[root] = root;
  for (int i = 0; i < int(que.size()); i++)
  {
    int v = que[i];
    for (int to : adj[v])
      if (par[to] == -1)
        par[to] = v, que.push_back(to);
  }
  return par;
}

void test_conversion_types()
{
  const vc<pair<int, int>> es{{0, 1}, {2, 1}};
  RootedTree tree(3, es, 2);
  EulerTour<false> tour(3, es, 2);
  const vc<tuple<ll, ll, ll>> weights{{1, 2, -4}, {0, 1, 7}};
  assert((tree.edge_to_vertex_values(weights) == vl{7, -4, 0}));
  assert((tour.edge_to_vertex_values(weights, 99LL) == vl{7, -4, 99}));
  assert((tree.weighted_depths(tree.edge_to_vertex_values(weights)) == vl{3, -4, 0}));
  const vc<tuple<int, int, bool>> flags{{1, 0, true}, {2, 1, false}};
  assert((tree.edge_to_vertex_values(flags, true) == vb{true, false, true}));
  assert((tour.edge_to_vertex_values(flags, true) == vb{true, false, true}));
  const vc<tuple<int, int, ValueWithoutDefault>> values{
    {0, 1, ValueWithoutDefault(8)}, {2, 1, ValueWithoutDefault(9)}};
  auto a = tree.edge_to_vertex_values(values, ValueWithoutDefault(10));
  auto b = tour.edge_to_vertex_values(values, ValueWithoutDefault(10));
  repi(v, 3) assert(a[v].value == 8 + v && b[v].value == 8 + v);
}

void test_random_trees()
{
  using Add = GroupAddSub<ll>;
  using G = PermutationGroup;
  mt19937 rng(1729);
  auto random_value = [&]()
  {
    auto res = G::e();
    shuffle(res.begin(), res.end(), rng);
    return res;
  };
  repi(n, 1, 22) repi(trial, 6)
  {
    vc<pair<int, int>> es;
    vvc<int> adj(n);
    repi(v, 1, n)
    {
      int p = rng() % v;
      adj[p].push_back(v), adj[v].push_back(p);
      es.emplace_back(p, v);
      if (rng() % 2) swap(es.back().first, es.back().second);
    }
    shuffle(es.begin(), es.end(), rng);
    int root = rng() % n;
    auto par = bfs_parents(adj, root);
    RootedTree tree(n, es, root);
    EulerTour<> tour(n, es, root);
    EulerTour<false> tour_without_lca(n, par);
    vc<tuple<int, int, ll>> weighted_edges;
    vl costs, edge_values(n, 777);
    for (auto [u, v] : es)
    {
      ll cost = rng() % 11;
      costs.push_back(cost);
      weighted_edges.emplace_back(v, u, cost);
      edge_values[par[u] == v ? u : v] = cost;
    }
    shuffle(weighted_edges.begin(), weighted_edges.end(), rng);
    assert(tree.edge_to_vertex_values(weighted_edges, 777LL) == edge_values);
    assert(tour.edge_to_vertex_values(weighted_edges, 777LL) == edge_values);
    assert(tour_without_lca.edge_to_vertex_values(weighted_edges, 777LL) == edge_values);
    auto ordered = tree.reordered_vertex_values(edge_values);
    repi(v, n) assert(ordered[tree.preorder(v)] == edge_values[v]);

    vl all_distance_sums(n);
    ll diameter = 0;
    repi(u, n)
    {
      auto from_u = bfs_parents(adj, u);
      repi(v, n)
      {
        ll expected = 0;
        for (int x = v; x != u; x = from_u[x])
        {
          int p = from_u[x];
          expected += edge_values[par[x] == p ? x : p];
        }
        ll actual = 0;
        tree.path_query(u, v, [&](int l, int r, bool)
        {
          repi(i, l, r) actual += ordered[i];
        }, true);
        assert(actual == expected);
        all_distance_sums[u] += expected;
        diameter = max(diameter, expected);
      }
    }
    auto [a, b, diameter_tree, depths] = tree_diameter_weighted(n, es, costs);
    assert(diameter_tree.root() == a && depths[b] == diameter);
    auto rerooted = rerooting<DistanceSum>(n, es,
      [&](DistanceSum::S x, int eid) { return DistanceSum::S{x.first + costs[eid] * x.second, x.second}; },
      [](DistanceSum::S x, int) { return DistanceSum::S{x.first, x.second + 1}; }, false);
    repi(v, n) assert(rerooted[v] == make_pair(all_distance_sums[v], ll(n)));

    for (bool edge : {false, true})
    {
      vl sums(n);
      vc<G::S> products(n);
      repi(v, n) sums[v] = ll(rng() % 21) - 10, products[v] = random_value();
      // Nonidentity root input must be ignored in edge mode.
      sums[root] = 123;
      products[root] = {1, 2, 0};
      PathSum<Add> path_sum(n, es, root, sums, edge);
      PathSum<Add, false> root_sum(n, par, sums, edge);
      PathProd<G> path_prod(n, es, root, products, edge);
      PathProd<G, false> root_prod(n, par, products, edge);
      if (edge) sums[root] = 0, products[root] = G::e();
      repi(round, 4)
      {
        repi(v, n)
        {
          assert(path_sum.get(v) == sums[v] && root_sum.get(v) == sums[v]);
          assert(path_prod.get(v) == products[v] && root_prod.get(v) == products[v]);
        }
        repi(u, n)
        {
          auto from_u = bfs_parents(adj, u);
          repi(v, n)
          {
            vc<int> path{v};
            while (path.back() != u) path.push_back(from_u[path.back()]);
            reverse(path.begin(), path.end());
            ll sum = 0;
            auto prod = G::e();
            repi(i, edge ? 1 : 0, path.size())
            {
              int x = path[i];
              if (edge && par[path[i - 1]] == x) x = path[i - 1];
              sum += sums[x];
              prod = G::op(prod, products[x]);
            }
            assert(path_sum.sum(u, v) == sum);
            assert(path_prod.prod(u, v) == prod);
            if (u == root)
            {
              assert(path_sum.sum(v) == sum && root_sum.sum(v) == sum);
              assert(path_prod.prod(v) == prod && root_prod.prod(v) == prod);
            }
          }
        }
        repi(v, n)
        {
          if (edge && v == root) continue;
          ll x = ll(rng() % 21) - 10;
          if (round % 2)
            sums[v] = x, path_sum.set(v, x), root_sum.set(v, x);
          else
            sums[v] += x, path_sum.add(v, x), root_sum.add(v, x);
          products[v] = random_value();
          path_prod.set(v, products[v]), root_prod.set(v, products[v]);
        }
      }
    }
  }
}

void test_empty_initial_values()
{
  using G = GroupAddSub<ll>;
  const vc<int> par{1, 1, 0};
  const vc<pair<int, int>> es{{1, 0}, {0, 2}};
  PathSum<G> vertex_sum(3, par);
  PathProd<G> vertex_prod(3, es, 1);
  vertex_sum.set(1, 7), vertex_prod.set(1, 7);
  assert(vertex_sum.sum(2) == 7 && vertex_prod.prod(2) == 7);
  using RMQ = LinearRMQ<MonoidMin<ull>>;
  PathSum<G, true, FenwickTree<G>, RMQ> edge_sum(3, par, {}, true);
  PathProd<G, true, SegmentTree<G>, SegmentTree<OppositeGroup<G>>, RMQ> edge_prod(3, es, 1, {}, true);
  edge_sum.add(0, 5), edge_prod.set(0, 5);
  assert(edge_sum.sum(1, 2) == 5 && edge_prod.prod(1, 2) == 5);
  assert(edge_sum.sum(0, 2) == 0 && edge_prod.prod(0, 2) == 0);
  PathSum<G, false> root_sum(3, es, 1, {}, true);
  PathProd<G, false> root_prod(3, par, {}, true);
  root_sum.set(2, 8), root_prod.set(2, 8);
  assert(root_sum.sum(2) == 8 && root_prod.prod(2) == 8);
}

int main()
{
  test_conversion_types();
  test_random_trees();
  test_empty_initial_values();
  cout << "Hello World" << endl;
}
