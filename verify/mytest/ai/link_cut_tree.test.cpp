#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/acted_monoid/affine_sum.hpp"
#include "algebra/acted_monoid/upd_min.hpp"
#include "tree/link_cut_tree.hpp"

struct WordMonoid
{
  using S = string;
  static S op(const S &a, const S &b) { return a + b; }
  static S e() { return {}; }
};

struct WordAssignment : WordMonoid
{
  using F = char;
  static S mapping(F f, const S &x) { return f ? string(x.size(), f) : x; }
  static F composition(F f, F g) { return f ? f : g; }
  static F id() { return 0; }
};

struct CountedValue
{
  int value;
  explicit CountedValue(int value) : value(value) {}
  CPP_DUMP_DEFINE_DATA(value)
};

struct CountedMonoid
{
  using S = CountedValue;
  static S op(const S &a, const S &b) { return S(a.value + b.value); }
  static S e() { return S(0); }
};

struct ForestModel
{
  vc<int> parent;
  vc<string> vertex;
  vvc<string> edge;
  CPP_DUMP_DEFINE_DATA(parent, vertex, edge)

  explicit ForestModel(int n) : parent(n, -1), vertex(n), edge(n, vc<string>(n)) {}

  vc<int> path(int from, int to) const
  {
    vc<int> prev(parent.size(), -1), todo{from};
    prev[from] = from;
    for (int i = 0; i < int(todo.size()); ++i)
      for (int v = 0; v < int(parent.size()); ++v)
        if (prev[v] == -1 && (parent[todo[i]] == v || parent[v] == todo[i]))
        {
          prev[v] = todo[i];
          todo.push_back(v);
        }
    if (prev[to] == -1) return {};
    vc<int> result;
    for (int v = to; v != from; v = prev[v]) result.push_back(v);
    result.push_back(from);
    reverse(result.begin(), result.end());
    return result;
  }

  int root(int v) const
  {
    while (parent[v] != -1) v = parent[v];
    return v;
  }

  void reroot(int v)
  {
    int prev = -1;
    while (v != -1)
    {
      swap(parent[v], prev);
      swap(v, prev);
    }
  }

  optional<int> lca(int u, int v) const
  {
    if (root(u) != root(v)) return nullopt;
    auto a = path(root(u), u), b = path(root(v), v);
    int j = 0;
    while (j + 1 < int(min(a.size(), b.size())) && a[j + 1] == b[j + 1]) ++j;
    return a[j];
  }
};

void check_random(int n, int seed)
{
  mt19937 rng(seed);
  ForestModel model(n);
  for (int v = 0; v < n; ++v) model.vertex[v] = string(1, char('a' + v % 6));
  LazyLinkCutTree<WordAssignment> vertices(model.vertex);
  LazyEdgeLinkCutTree<WordAssignment> edges(n);
  auto letter = [&] { return char('a' + rng() % 6); };
  for (int i = 0; i < 3500; ++i)
  {
    int u = rng() % n, v = rng() % n;
    if (model.parent[u] != -1 && rng() % 4 == 0) v = model.parent[u];
    auto path = model.path(u, v);
    const bool same = !path.empty();
    const bool adjacent = path.size() == 2;
    auto [a, b] = pair{letter(), letter()};
    switch (rng() % 11)
    {
      case 0:
        assert(vertices.link(u, v) == !same);
        assert(edges.link(u, v, string(1, a), string(1, b)) == !same);
        if (!same)
        {
          model.reroot(u);
          model.parent[u] = v;
          model.edge[u][v] = a;
          model.edge[v][u] = b;
        }
        break;
      case 1:
        assert(vertices.cut(u, v) == adjacent);
        assert(edges.cut(u, v) == adjacent);
        if (adjacent) model.parent[model.parent[u] == v ? u : v] = -1;
        break;
      case 2:
        assert(vertices.cut_parent(u) == (model.parent[u] != -1));
        assert(edges.cut_parent(u) == (model.parent[u] != -1));
        model.parent[u] = -1;
        break;
      case 3:
        vertices.reroot(u);
        edges.reroot(u);
        model.reroot(u);
        break;
      case 4:
        vertices.set(u, string(1, a));
        model.vertex[u] = a;
        assert(edges.set(u, v, string(1, b)) == adjacent);
        if (adjacent) model.edge[u][v] = b;
        break;
      case 5:
        assert(edges.set(u, v, string(1, a), string(1, b)) == adjacent);
        if (adjacent) model.edge[u][v] = a, model.edge[v][u] = b;
        break;
      case 6:
        assert(vertices.apply(u, v, a) == same);
        for (int x : path) model.vertex[x] = a;
        break;
      case 7:
        if (i % 2 == 0)
        {
          b = a;
          assert(edges.apply(u, v, a) == same);
        }
        else assert(edges.apply(u, v, a, b) == same);
        for (int j = 1; j < int(path.size()); ++j)
        {
          model.edge[path[j - 1]][path[j]] = a;
          model.edge[path[j]][path[j - 1]] = b;
        }
        break;
      case 8:
        assert(vertices.same(u, v) == same && edges.same(u, v) == same);
        assert(vertices.lca(u, v) == model.lca(u, v) && edges.lca(u, v) == model.lca(u, v));
        break;
      default:
        assert(vertices.get(u) == model.vertex[u]);
        assert(edges.get(u, v) == (adjacent ? optional<string>(model.edge[u][v]) : nullopt));
        break;
    }

    u = rng() % n, v = rng() % n;
    path = model.path(u, v);
    const int length = int(path.size()) - 1;
    const int k = rng() % (n + 1);
    optional<int> distance, jump, first_vertex;
    optional<pair<int, int>> first_edge;
    optional<string> vertex_product, edge_product;
    if (length >= 0)
    {
      distance = length;
      if (k <= length) jump = path[k];
      vertex_product = "", edge_product = "";
      for (int j = 0; j <= length; ++j)
      {
        *vertex_product += model.vertex[path[j]];
        if (!first_vertex && model.vertex[path[j]][0] == a) first_vertex = path[j];
        if (j == 0) continue;
        auto [s, t] = pair{path[j - 1], path[j]};
        *edge_product += model.edge[s][t];
        if (!first_edge && model.edge[s][t][0] == a) first_edge = {s, t};
      }
    }
    const auto contains = [&](const string &s) { return s.find(a) != string::npos; };
    assert(vertices.prod(u, v) == vertex_product && edges.prod(u, v) == edge_product);
    assert(vertices.first_vertex(u, v, contains) == first_vertex);
    assert(edges.first_edge(u, v, contains) == first_edge);
    assert(vertices.dist(u, v) == distance && edges.dist(u, v) == distance);
    assert(vertices.jump(u, v, i128(k)) == jump && edges.jump(u, v, u128(k)) == jump);
    assert(vertices.root(u) == model.root(u) && edges.root(u) == model.root(u));
    optional<int> parent = model.parent[u] == -1 ? nullopt : optional<int>(model.parent[u]);
    assert(vertices.parent(u) == parent && edges.parent(u) == parent);
  }
}

void check_boundaries()
{
  LinkCutTree<> empty(0);
  assert(empty.size() == 0);
  LinkCutTree<CountedMonoid> counts(vc<int>{1, 2});
  assert(counts.link(0, 1) && counts.prod(0, 1)->value == 3);
  EdgeLinkCutTree<CountedMonoid> edge_count(2);
  assert(edge_count.link(0, 1, CountedValue(4)) && edge_count.prod(1, 0)->value == 4);

  LinkCutTree<WordMonoid> vertices({"A", "B", "C", "D"});
  assert(vertices.link(3, 2) && vertices.link(2, 1) && vertices.link(1, 0));
  assert(vertices.prod(0, 3) == "ABCD" && vertices.prod(3, 0) == "DCBA");
  assert(!vertices.jump(0, 3, -(i128(1) << 100)));
  assert(!vertices.jump(0, 3, (u128(1) << 100) + 1));
  assert(vertices.first_vertex(3, 0, [](const string &) { return true; }) == 3);
  int calls = 0;
  try
  {
    vertices.first_vertex(3, 0, [&](const string &)
    {
      if (++calls == 2) throw runtime_error("predicate");
      return true;
    });
  }
  catch (const runtime_error &)
  {
    assert(vertices.root(3) == 0 && vertices.prod(0, 3) == "ABCD");
  }
  assert(calls == 2);
  auto copy = vertices;
  assert(copy.cut(1, 2) && !copy.prod(3, 0) && vertices.prod(3, 0) == "DCBA");

  LazyEdgeLinkCutTree<ActedMonoidUpdMax<int>> edges(4);
  assert(edges.link(0, 1, 1, 9));
  assert(edges.link(1, 2, 2, 8));
  assert(edges.link(2, 3, 3, 7));
  assert(edges.apply(0, 3, 4, 5));
  assert(edges.apply(3, 3, 50));
  assert(edges.prod(3, 3) == ActedMonoidUpdMax<int>::e());
  assert(edges.prod(0, 3) == 4 && edges.prod(3, 0) == 5);
  assert(edges.cut(1, 2) && !edges.apply(0, 3, 70, 80));
  assert(edges.link(1, 2, 10, 11));
  assert(edges.get(1, 2) == 10 && edges.get(2, 1) == 11);
  assert(edges.get(0, 1) == 4 && edges.get(3, 2) == 5);
  auto edge_copy = edges;
  assert(edges.cut(2, 3) && edge_copy.get(3, 2) == 5);
  assert(edges.link(3, 2, 6) && edges.cut_parent(3));
  assert(edge_copy.get(3, 2) == 5);
  assert(!edge_copy.first_edge(3, 3, [](int) { return true; }));
  assert((edge_copy.first_edge(3, 0, [](int) { return true; }) == pair{3, 2}));
  LazyEdgeLinkCutTree<ActedMonoidUpdMax<int>> moved = std::move(edges);
  assert(edges.size() == 0 && moved.link(3, 2, 12));
  edges = std::move(moved);
  assert(moved.size() == 0 && edges.get(2, 3) == 12);

  LazyLinkCutTree<ActedMonoidAffineSum<ll>> affine({1, 2, 3});
  assert(affine.link(0, 1) && affine.link(1, 2));
  assert(affine.apply(0, 2, {2, 1}) && affine.apply(2, 1, {3, 4}));
  assert(affine.prod(2, 0)->val == 47);
}

void check_long_path()
{
  const int n = 200000;
  LinkCutTree<> tree(n);
  for (int v = 1; v < n; ++v) assert(tree.link(v, v - 1));
  assert(tree.dist(n - 1, 0) == n - 1);
  assert(tree.jump(n - 1, 0, n / 2) == n / 2 - 1);
  assert(tree.parent(n - 1) == n - 2);
  assert(tree.root(n - 1) == 0);
}

int main()
{
  check_boundaries();
  for (int n : {1, 2, 9, 30})
    for (int seed = 0; seed < 10; ++seed)
      check_random(n, 267804 + seed);
  check_long_path();
  PRINT("Hello World");
}
