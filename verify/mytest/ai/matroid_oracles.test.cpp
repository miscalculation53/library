#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/uniform_matroid.hpp"
#include "matroid/partition_matroid.hpp"
#include "matroid/graphic_matroid.hpp"
#include "matroid/bicircular_matroid.hpp"
#include "matroid/transversal_matroid.hpp"
#include "matroid/laminar_matroid.hpp"
#include "matroid/binary_matroid.hpp"

template <class Oracle, class Predicate>
void check(Oracle oracle, const Predicate &independent)
{
  const int n = oracle.size();
  static_assert(is_same_v<decltype(as_const(oracle).circuit(0)), optional<vc<int>>>);
  for (int mask = 0; mask < (1 << n); mask++) if (independent(mask))
  {
    vc<int> s;
    for (int e = 0; e < n; e++) if (mask >> e & 1) s.push_back(e);
    oracle.build(s);
    for (int e = 0; e < n; e++) if (!(mask >> e & 1))
    {
      const auto circuit = as_const(oracle).circuit(e);
      if (independent(mask | (1 << e))) assert(!circuit);
      else
      {
        assert(circuit);
        vc<int> expected;
        for (int f : s) if (independent((mask ^ (1 << f)) | (1 << e)))
          expected.push_back(f);
        auto actual = *circuit;
        sort(ALL(actual));
        assert(actual == expected);
      }
    }
    auto copied = oracle;
    auto moved = move(copied);
    reverse(ALL(s));
    moved.build(s);
    for (int e = 0; e < n; e++) if (!(mask >> e & 1))
    {
      auto a = oracle.circuit(e), b = moved.circuit(e);
      assert(bool(a) == bool(b));
      if (a) { sort(ALL(*a)), sort(ALL(*b)); assert(a == b); }
    }
  }
  oracle.build({});
}

bool graph_independent(int vertices, const vc<pair<int, int>> &edges, int mask, bool bicircular)
{
  vc<int> component(vertices);
  iota(ALL(component), 0);
  for (int e = 0; e < int(edges.size()); e++) if (mask >> e & 1)
  {
    auto [u, v] = edges[e];
    int a = component[u], b = component[v];
    if (a == b && !bicircular) return false;
    for (int &c : component) if (c == b) c = a;
  }
  vc<int> vertex_count(vertices), edge_count(vertices);
  for (int c : component) vertex_count[c]++;
  for (int e = 0; e < int(edges.size()); e++) if (mask >> e & 1)
    edge_count[component[edges[e].first]]++;
  for (int v = 0; v < vertices; v++) if (edge_count[v] > vertex_count[v]) return false;
  return true;
}

bool transversal_independent(const vvc<int> &adjacency, int mask)
{
  auto dfs = [&](auto self, int u, int used) -> bool
  {
    if (u == int(adjacency.size())) return true;
    if (!(mask >> u & 1)) return self(self, u + 1, used);
    for (int v : adjacency[u])
      if (!(used >> v & 1) && self(self, u + 1, used | (1 << v))) return true;
    return false;
  };
  return dfs(dfs, 0, 0);
}

template <class UInt>
void binary_cases()
{
  constexpr int width = numeric_limits<UInt>::digits;
  mt19937_64 rng(20261103 + width);
  check(BinaryMatroid<UInt>({}), [](int) { return true; });
  for (int trial = 0; trial < 150; trial++)
  {
    const int n = rng() % 9;
    vc<UInt> vectors(n);
    for (UInt &x : vectors)
      for (int b = 0; b < 6; b++) if (rng() % 2)
        x ^= UInt(1) << ((width - 1 - b * 5) % width);
    // 全部分集合の XOR を列挙して独立性を判定する。
    vc<UInt> xor_sum(1 << n);
    vc<bool> independent(1 << n, true);
    for (int mask = 1; mask < (1 << n); mask++)
    {
      const int b = __builtin_ctz(unsigned(mask));
      xor_sum[mask] = xor_sum[mask ^ (1 << b)] ^ vectors[b];
      for (int sub = mask; sub; sub = (sub - 1) & mask)
        if (xor_sum[sub] == UInt(0)) independent[mask] = false;
    }
    check(BinaryMatroid<UInt>(vectors), [&](int mask) { return independent[mask]; });
  }
  // 全ビットを使う基底と、その全係数が 1 の依存関係。
  vc<UInt> vectors;
  vc<int> s;
  for (int b = 0; b < width; b++) vectors.push_back(UInt(1) << b), s.push_back(b);
  vectors.push_back(UInt(~UInt(0)));
  vectors.push_back(UInt(0));
  vectors.push_back(UInt(1) << (width - 1));
  BinaryMatroid<UInt> m(vectors);
  m.build(s);
  assert(m.circuit(width)->size() == width);
  assert(m.circuit(width + 1)->empty());
  assert(*m.circuit(width + 2) == vc<int>{width - 1});
  s.pop_back();
  m.build(s);
  assert(!m.circuit(width - 1));
  assert(!m.circuit(width));
}

int main()
{
  binary_cases<uint32_t>();
  binary_cases<uint64_t>();
  binary_cases<u128>();
  for (int n = 0; n <= 9; n++) for (int r = 0; r <= n + 1; r++)
    check(UniformMatroid(n, r), [&](int mask) { return __builtin_popcount(unsigned(mask)) <= r; });

  check(PartitionMatroid({}, {}), [](int) { return true; });
  check(GraphicMatroid(0, {}), [](int) { return true; });
  check(BicircularMatroid(0, {}), [](int) { return true; });
  check(TransversalMatroid(0, {}), [](int) { return true; });
  check(TransversalMatroid(0, {{}, {}, {}}), [](int mask) { return mask == 0; });
  check(LaminarMatroid({}, {}, {}), [](int) { return true; });

  mt19937 rng(20261102);
  for (int trial = 0; trial < 250; trial++)
  {
    const int n = rng() % 10, k = 1 + rng() % 6;
    vc<int> group(n), capacity(k);
    for (int &x : group) x = rng() % k;
    for (int &x : capacity) x = rng() % 5;
    check(PartitionMatroid(group, capacity), [&](int mask)
    {
      vc<int> count(k);
      for (int e = 0; e < n; e++) if (mask >> e & 1) count[group[e]]++;
      for (int g = 0; g < k; g++) if (count[g] > capacity[g]) return false;
      return true;
    });

    const int vertices = 1 + rng() % 8;
    vc<pair<int, int>> edges;
    for (int e = 0; e < n; e++) edges.emplace_back(rng() % vertices, rng() % vertices);
    check(GraphicMatroid(vertices, edges), [&](int mask)
    { return graph_independent(vertices, edges, mask, false); });
    check(BicircularMatroid(vertices, edges), [&](int mask)
    { return graph_independent(vertices, edges, mask, true); });

    const int right = rng() % 7;
    vvc<int> adjacency(n);
    for (auto &list : adjacency) for (int v = 0; v < right; v++)
    {
      if (rng() % 2) list.push_back(v);
      if (rng() % 4 == 0) list.push_back(v);
    }
    check(TransversalMatroid(right, adjacency), [&](int mask)
    { return transversal_independent(adjacency, mask); });

    vc<int> parent(k, -1);
    for (int v = 1; v < k; v++) if (rng() % 3) parent[v] = rng() % v;
    check(LaminarMatroid(parent, capacity, group), [&](int mask)
    {
      vc<int> count(k);
      for (int e = 0; e < n; e++) if (mask >> e & 1)
        for (int v = group[e]; v != -1; v = parent[v]) count[v]++;
      for (int v = 0; v < k; v++) if (count[v] > capacity[v]) return false;
      return true;
    });
  }

  // 深い木・孤立点・ループを含む端点のケース。
  constexpr int n = 100000;
  vc<pair<int, int>> edges;
  vc<int> s;
  for (int v = 0; v + 1 < n; v++) edges.emplace_back(v, v + 1), s.push_back(v);
  edges.emplace_back(n - 1, 0);
  edges.emplace_back(0, 0);
  GraphicMatroid graphic(n + 1, edges);
  graphic.build(s);
  assert(graphic.circuit(n - 1)->size() == n - 1);
  assert(graphic.circuit(n)->empty());
  BicircularMatroid bicircular(n + 1, edges);
  bicircular.build(s);
  assert(!bicircular.circuit(n - 1));
  s.push_back(n - 1);
  bicircular.build(s);
  assert(bicircular.circuit(n)->size() == n);
  cout << "Hello World\n";
}
