#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/uniform_matroid.hpp"
#include "matroid/partition_matroid.hpp"
#include "matroid/graphic_matroid.hpp"
#include "matroid/bicircular_matroid.hpp"
#include "matroid/transversal_matroid.hpp"
#include "matroid/binary_matroid.hpp"

template <class F, class Predicate>
void check_representation(const Matrix<F> &m, const Predicate &independent, bool exact = true)
{
  const int n = m.template shape<int>().second;
  for (int mask = 0; mask < (1 << n); mask++)
  {
    vc<int> ids;
    for (int e = 0; e < n; e++) if (mask >> e & 1) ids.push_back(e);
    auto a = m.select_columns(ids);
    const bool actual = a.template rank<int>() == int(ids.size());
    const bool expected = independent(mask);
    assert(!actual || expected);
    if (exact) assert(actual == expected);
  }
}

bool graph_independent(int vertices, const vc<pair<int, int>> &edges, int mask, bool bicircular)
{
  vc<int> component(vertices), vertex_count(vertices), edge_count(vertices);
  iota(ALL(component), 0);
  for (int e = 0; e < int(edges.size()); e++) if (mask >> e & 1)
  {
    const auto [u, v] = edges[e];
    const int a = component[u], b = component[v];
    if (a == b && !bicircular) return false;
    for (int &c : component) if (c == b) c = a;
  }
  for (int c : component) vertex_count[c]++;
  for (int e = 0; e < int(edges.size()); e++) if (mask >> e & 1)
    edge_count[component[edges[e].first]]++;
  for (int v = 0; v < vertices; v++) if (edge_count[v] > vertex_count[v]) return false;
  return true;
}

bool transversal_independent(const vvc<int> &adjacency, int mask)
{
  auto dfs = [&](auto self, int e, int used) -> bool
  {
    if (e == int(adjacency.size())) return true;
    if (!(mask >> e & 1)) return self(self, e + 1, used);
    for (int v : adjacency[e])
      if (!(used >> v & 1) && self(self, e + 1, used | (1 << v))) return true;
    return false;
  };
  return dfs(dfs, 0, 0);
}

template <class F>
void random_representations(bool exact)
{
  using S = typename F::S;
  mt19937_64 rng(20261104);
  auto sample = [&]() { return S(rng() % S::mod()); };
  auto zero = []() { return F::e0(); };
  for (int trial = 0; trial < 100; trial++)
  {
    const int n = rng() % 9, vertices = 1 + rng() % 7;
    vc<pair<int, int>> edges;
    for (int e = 0; e < n; e++) edges.emplace_back(rng() % vertices, rng() % vertices);
    const auto graphic = graphic_linear_representation<F>(vertices, edges);
    assert(graphic.template shape<int>() == make_pair(vertices, n));
    check_representation(graphic, [&](int mask)
    { return graph_independent(vertices, edges, mask, false); });

    const auto bicircular = bicircular_linear_representation<F>(vertices, edges, sample);
    check_representation(bicircular, [&](int mask)
    { return graph_independent(vertices, edges, mask, true); }, exact);
    check_representation(bicircular_linear_representation<F>(vertices, edges, zero), [&](int mask)
    { return graph_independent(vertices, edges, mask, true); }, false);

    vvc<int> adjacency(n);
    const int right = rng() % 7;
    for (auto &list : adjacency) for (int v = 0; v < right; v++)
    {
      if (rng() % 2) list.push_back(v);
      if (rng() % 3 == 0) list.push_back(v);
    }
    check_representation(transversal_linear_representation<F>(right, adjacency, sample), [&](int mask)
    { return transversal_independent(adjacency, mask); }, exact);
    check_representation(transversal_linear_representation<F>(right, adjacency, zero), [&](int mask)
    { return transversal_independent(adjacency, mask); }, false);
  }
}

template <class UInt>
void binary_representations()
{
  using S = static_modint32<2>;
  using F = FieldAddSubMulDiv<S>;
  constexpr int width = numeric_limits<UInt>::digits;
  vc<UInt> vectors{UInt(1), UInt(1) << (width - 1), UInt(0), UInt(1),
                   (UInt(1) << (width - 1)) ^ UInt(1)};
  vc<UInt> xor_sum(1 << vectors.size());
  for (int mask = 1; mask < int(xor_sum.size()); mask++)
  {
    const int e = __builtin_ctz(unsigned(mask));
    xor_sum[mask] = xor_sum[mask ^ (1 << e)] ^ vectors[e];
  }
  const auto m = binary_linear_representation<F>(vectors);
  assert(m.shape().first == width);
  check_representation(m, [&](int mask)
  {
    for (int sub = mask; sub; sub = (sub - 1) & mask) if (xor_sum[sub] == 0) return false;
    return true;
  });
}

int main()
{
  using S = static_modint32<998244353>;
  using F = FieldAddSubMulDiv<S>;
  using F2 = FieldAddSubMulDiv<static_modint32<2>>;
  const vvc<S> source{{1, 0}, {0, 1}, {1, 1}};
  LinearMatroid<F> copied(2, source);
  assert(copied.columns() == source && copied.columns().data() != source.data());
  auto movable = source;
  const auto *outer_storage = movable.data();
  const auto *inner_storage = movable[0].data();
  LinearMatroid<F> moved(2, std::move(movable));
  assert(moved.columns() == source && moved.columns().data() == outer_storage);
  assert(moved.columns()[0].data() == inner_storage);
  copied.build({0, 1});
  moved.build({0, 1});
  assert(copied.circuit(2) == moved.circuit(2));
  check_representation(graphic_linear_representation<F>(0, {}), [](int) { return true; });
  check_representation(bicircular_linear_representation<F>(0, {}, [] { return S(0); }),
                       [](int) { return true; });
  check_representation(transversal_linear_representation<F>(0, {{}, {}}, [] { return S(0); }),
                       [](int mask) { return mask == 0; });
  check_representation(partition_linear_representation<F>({}, {}, {}), [](int) { return true; });
  check_representation(binary_linear_representation<F2>(vc<u128>{}), [](int) { return true; });

  for (int n = 0; n <= 8; n++) for (int r = 0; r <= n + 2; r++)
  {
    vc<S> points(n);
    for (int e = 0; e < n; e++) points[e] = S(e);
    auto m = uniform_linear_representation<F>(r, points);
    assert(m.shape().first == min(r, n));
    check_representation(m, [&](int mask) { return __builtin_popcount(unsigned(mask)) <= r; });
  }

  mt19937 rng(20261105);
  for (int trial = 0; trial < 200; trial++)
  {
    const int n = rng() % 9, k = 1 + rng() % 5;
    vc<int> group(n), capacity(k), group_count(k);
    vc<S> points(n);
    for (int &c : capacity) c = rng() % 7;
    for (int e = 0; e < n; e++)
    {
      group[e] = rng() % k;
      points[e] = S(group_count[group[e]]++);
    }
    auto m = partition_linear_representation<F>(group, capacity, points);
    int dimension = 0;
    for (int g = 0; g < k; g++) dimension += min(capacity[g], group_count[g]);
    assert(m.shape().first == dimension);
    check_representation(m, [&](int mask)
    {
      vc<int> count(k);
      for (int e = 0; e < n; e++) if (mask >> e & 1) count[group[e]]++;
      for (int g = 0; g < k; g++) if (count[g] > capacity[g]) return false;
      return true;
    });
  }
  random_representations<F>(true);
  random_representations<F2>(false);
  binary_representations<uint32_t>();
  binary_representations<uint64_t>();
  binary_representations<u128>();

  int calls = 0;
  auto sample = [&]() { calls++; return S(calls); };
  const auto duplicate = transversal_linear_representation<F>(2, {{0, 0, 0}, {0, 1, 1}}, sample);
  assert(calls == 3);
  assert(duplicate.column(0) == vc<S>({1, 0}));
  assert(duplicate.column(1) == vc<S>({2, 3}));
  auto make_sample = []
  {
    return [next = make_unique<int>(0)]() mutable { return S(++*next); };
  };
  const vvc<int> duplicate_adjacency{{0, 0, 0}, {0, 1, 1}};
  auto noncopy_sample = make_sample();
  assert(transversal_linear_representation<F>(2, duplicate_adjacency, noncopy_sample) ==
         duplicate);
  assert(transversal_linear_representation<F>(2, CSR<int>(duplicate_adjacency), make_sample()) ==
         duplicate);
  assert(transversal_linear_representation<F>(2, {{0, 0, 0}, {0, 1, 1}}, make_sample()) ==
         duplicate);
  const GraphUndirected<> duplicate_graph(4, vc<pair<int, int>>{
      {0, 1}, {0, 1}, {0, 1}, {2, 1}, {2, 3}, {2, 3}});
  const vc<int> duplicate_color{0, 1, 0, 1};
  assert(transversal_linear_representation<F>(duplicate_graph, duplicate_color, make_sample()) ==
         duplicate);
  assert(TransversalMatroid(duplicate_graph, duplicate_color).linear_representation<F>(make_sample()) ==
         duplicate);
  assert(bicircular_linear_representation<F>(2, {{0, 0}, {0, 1}}, make_sample()) ==
         duplicate);
  calls = 0;
  const auto loop = bicircular_linear_representation<F>(1, {{0, 0}}, sample);
  assert(calls == 1 && loop[0][0] == S(1));

  UniformMatroid uniform(4, 2);
  PartitionMatroid partition({0, 0, 1}, {1, 1});
  GraphicMatroid graphic(3, {{0, 1}, {1, 2}, {2, 0}});
  uniform.build({0, 1});
  partition.build({0, 2});
  graphic.build({0, 1});
  assert(uniform.linear_representation<F>({0, 1, 2, 3}) ==
         uniform_linear_representation<F>(2, vc<S>{0, 1, 2, 3}));
  assert(partition.linear_representation<F>({0, 1, 0}) ==
         partition_linear_representation<F>({0, 0, 1}, {1, 1}, vc<S>{0, 1, 0}));
  assert(graphic.linear_representation<F>() ==
         graphic_linear_representation<F>(3, {{0, 1}, {1, 2}, {2, 0}}));
  TransversalMatroid transversal(2, {{0, 0}, {0, 1}});
  transversal.build({0, 1});
  calls = 0;
  auto member_transversal = transversal.linear_representation<F>(sample);
  assert(calls == 3);
  calls = 0;
  assert(member_transversal ==
         transversal_linear_representation<F>(2, {{0, 0}, {0, 1}}, sample));
  BicircularMatroid bicircular(2, {{0, 0}, {0, 1}});
  bicircular.build({0, 1});
  calls = 0;
  auto member_bicircular = bicircular.linear_representation<F>(sample);
  calls = 0;
  assert(member_bicircular ==
         bicircular_linear_representation<F>(2, {{0, 0}, {0, 1}}, sample));

  vc<DynamicBitset> wide(4, DynamicBitset(193));
  wide[0].set(192), wide[1].set(64);
  wide[2] = wide[0] ^ wide[1];
  BinaryMatroid<DynamicBitset> dynamic(wide);
  dynamic.build({0, 1});
  assert(dynamic.circuit(2) == optional<vc<int>>({0, 1}));
  assert(dynamic.circuit(3)->empty());
  auto dynamic_representation = dynamic.linear_representation<F2>();
  assert(dynamic_representation.shape().first == 193);
  assert(dynamic_representation == binary_linear_representation<F2>(wide));
  check_representation(dynamic_representation, [](int mask)
  {
    return !(mask & 8) && __builtin_popcount(unsigned(mask)) <= 2;
  });
  cout << "Hello World\n";
}
