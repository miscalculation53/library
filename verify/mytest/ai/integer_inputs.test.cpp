#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/matroid.hpp"
#include "flow/bipartite_cover.hpp"
#include "flow/dm_decomposition.hpp"
#include "flow/project_selection.hpp"
#include "graph/general_matching.hpp"
#include "ds/group_index.hpp"
#include "ds/flat_dvec.hpp"
#include "algebra/multivariate_power_sum.hpp"
#include "algo/subset_sum.hpp"

struct QualifiedPredicate
{
  int *const_calls, *mutable_calls;
  bool operator()(const vc<int> &) const { ++*const_calls; return true; }
  bool operator()(vc<int> &) const { ++*mutable_calls; return true; }
};

template <class I, class M>
void compare_build(M original, M generic)
{
  const int n = original.size();
  for (int mask = 0; mask < (1 << n); mask++)
  {
    original.build(vc<int>{});
    vc<int> selected;
    bool valid = true;
    for (int e = 0; e < n; e++) if (mask >> e & 1)
    {
      if (original.circuit(e)) { valid = false; break; }
      selected.push_back(e);
      original.build(selected);
    }
    if (!valid) continue;
    generic.build(vc<I>(ALL(selected)));
    for (int f = 0; f < n; f++) if (!(mask >> f & 1))
      assert(original.circuit(f) == generic.circuit(f));
  }
  generic.build({});
  auto copy = generic;
  auto moved = move(copy);
  for (int e = 0; e < n; e++)
  {
    original.build({});
    assert(original.circuit(e) == moved.circuit(e));
  }
}

template <class I>
void integer_cases()
{
  using F = FieldAddSubMulDiv<static_modint32<998244353>>;
  const vc<I> group{0, 0, 1, 1}, capacity{1, 2};
  PartitionMatroid partition(group, capacity);
  compare_build<I>(PartitionMatroid({0, 0, 1, 1}, {1, 2}), partition);
  compare_build<I>(UniformMatroid(4, 2), UniformMatroid(4, 2));
  const vc<pair<I, unsigned>> edges{{0, 1}, {1, 2}, {2, 0}, {0, 0}};
  compare_build<I>(GraphicMatroid(3, {{0, 1}, {1, 2}, {2, 0}, {0, 0}}), GraphicMatroid(3, edges));
  compare_build<I>(BicircularMatroid(3, {{0, 1}, {1, 2}, {2, 0}, {0, 0}}), BicircularMatroid(3, edges));
  compare_build<I>(BinaryMatroid<>({1, 2, 3, 0}), BinaryMatroid<>({1, 2, 3, 0}));
  compare_build<I>(LinearMatroid<F>(2, {{1, 0}, {0, 1}, {1, 1}, {0, 0}}),
                   LinearMatroid<F>(2, {{1, 0}, {0, 1}, {1, 1}, {0, 0}}));
  const vc<I> parent{0, 0};
  compare_build<I>(LaminarMatroid({-1, 0}, {2, 1}, {0, 1, 1, 0}),
                   LaminarMatroid(parent, vc<unsigned>{2, 1}, vc<I>{0, 1, 1, 0}));
  const vvc<I> adjacency{{0, 1}, {0}, {1}, {}};
  compare_build<I>(TransversalMatroid(2, {{0, 1}, {0}, {1}, {}}), TransversalMatroid(2, adjacency));
  auto predicate = [](const vc<int> &s) { return s.size() <= 2; };
  compare_build<I>(IndependenceMatroid(4, predicate), IndependenceMatroid(4, predicate));
  assert(matroid_intersection(partition, UniformMatroid(4, 2), vc<I>{0}).size() == 2);

  const vc<typename F::S> points{0, 1, 0, 1};
  assert(partition_linear_representation<F>(group, capacity, points) ==
         PartitionMatroid({0, 0, 1, 1}, {1, 2}).linear_representation<F>(points));
  assert(PartitionMatroid(group, {1, 2}).linear_representation<F>(points) ==
         PartitionMatroid({0, 0, 1, 1}, capacity).linear_representation<F>(points));
  assert(graphic_linear_representation<F>(3, edges) == GraphicMatroid(3, edges).linear_representation<F>());
  auto sample = [] { return F::e1(); };
  assert(bicircular_linear_representation<F>(3, edges, sample) ==
         BicircularMatroid(3, edges).linear_representation<F>(sample));
  CSR<I> compact(adjacency);
  BipartiteMatchingState matching(2, adjacency), matching_csr(2, compact);
  assert(matching.augment(0) && matching_csr.augment(0));
  assert(matching.augment(1) && matching_csr.augment(1));
  assert(!matching.augment(2) && !matching_csr.augment(2));
  assert(transversal_linear_representation<F>(2, adjacency, sample) ==
         transversal_linear_representation<F>(2, compact, sample));
  CSR<I, true> erasable(adjacency);
  erasable.pop_back(0);
  CSR<int> converted(erasable);
  assert(converted.content() == vvc<int>({{0}, {0}, {1}, {}}));
  assert(BipartiteMatchingState(2, erasable).left_size() == 4);
  assert(transversal_linear_representation<F>(2, erasable, sample)[1][0] == F::e0());
  CSR<int> rows(vc<I>{100, 100});
  assert(rows.offset(2) == 200);
  CSR<int, true> mutable_rows(vc<I>{100, 100});
  mutable_rows.pop_back(0);
  assert(mutable_rows[0].size() == 99 && mutable_rows[1].size() == 100);

  GraphUndirected<> g(4, vc<pair<int, int>>{{0, 2}, {1, 3}, {0, 3}});
  const vc<I> color{0, 0, 1, 1};
  const vc<pair<I, u128>> pairs{{0, 2}, {1, 3}};
  assert(bipartite_vertex_cover(g, color, pairs).size() == 2);
  assert(bipartite_independent_set(g, color, pairs).size() == 2);
  assert(minimum_edge_cover(g, pairs)->size() == 2);
  assert(dm_decomposition(g, color, pairs).ids == dm_decomposition(g, color).ids);
  assert(general_matching(g, pairs).size() == 2);
  assert(general_matching(g, {}).size() == 2);
  LinearMatroid<F> identity(4, {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
  assert(linear_matroid_parity_size(identity, pairs, sample) == 2);
  assert(linear_matroid_parity_restore(identity, pairs, sample).size() == 2);
  ProjectSelectionProblem<ll> project(4);
  project.add_all1(vc<I>{0, 1}, -7, 0);
  const auto selected = project.solve();
  assert(selected && selected->first == -7);
  K_ProjectSelectionProblem<ll> labels(2, vc<I>{1, 3});
  labels.add1(1, {3, 0, -2});
  assert((labels.solve() == pair{-2LL, vc<int>{0, 2}}));

  assert(add_virtual_root(2, vc<I>{0, 0}).size() == 3);
  GroupIndex<I, false> indexed(vc<I>{1, 0, 1});
  assert(indexed.idxs(I(1)).size() == 2);
  FlatDvec<int> flat(array<I, 2>{2, 3}, 7);
  assert(flat.at(I(1), I(2)) == 7);
  using Power = MonoidMultivariatePowerSum<ll, 1, 2>;
  static_assert(Power::index(array<I, 2>{1, 2}) == 5);
  typename Power::S value(array<ll, 2>{2, 3});
  assert(value.at(array<I, 2>{1, 2}) == 18);
  assert(as_const(value).at(array<I, 2>{1, 2}) == 18);
  assert(value.at({1, 2}) == 18);
  SubsetSumFromFrequency sums(vc<I>{0, 2, 1}, 1000);
  assert(sums.exists(4) && !sums.exists(5));
}

int main()
{
  CSR deduced(vvc<int>{{1, 2}, {3}});
  static_assert(is_same_v<decltype(deduced), CSR<int>>);
  assert(deduced.content() == vvc<int>({{1, 2}, {3}}));
  const size_t large = size_t(-1) / 4;
  FlatDvec<int> zero_shape(array<u128, 2>{large, 0});
  assert(zero_shape.empty() && zero_shape.shape()[0] == large);
  integer_cases<char>();
  integer_cases<signed char>();
  integer_cases<unsigned char>();
  integer_cases<short>();
  integer_cases<unsigned short>();
  integer_cases<int>();
  integer_cases<unsigned>();
  integer_cases<long>();
  integer_cases<unsigned long>();
  integer_cases<ll>();
  integer_cases<ull>();
  integer_cases<i128>();
  integer_cases<u128>();
  integer_cases<wchar_t>();
  integer_cases<char16_t>();
  integer_cases<char32_t>();
#ifdef __cpp_char8_t
  integer_cases<char8_t>();
#endif
  const u128 huge = u128(1) << 100;
  PartitionMatroid partition(vc<i128>{0, 0, 1}, vc<u128>{huge, 0});
  partition.build(vc<unsigned char>{0, 1});
  assert(partition.circuit(2)->empty());
  using F = FieldAddSubMulDiv<static_modint32<998244353>>;
  assert(partition.linear_representation<F>({0, 1, 0}).shape().first == 2);
  assert(partition_linear_representation<F>(vc<u128>{0, 0}, vc<u128>{huge}, {0, 1}).shape().first == 2);
  LaminarMatroid laminar(vc<unsigned>{0}, vc<u128>{huge}, vc<unsigned char>{0, 0});
  laminar.build(vc<i128>{0, 1});
  auto predicate = [](const vc<int> &s) { return s.size() <= 2; };
  IndependenceMatroid oracle(2, predicate);
  oracle.build(vc<bool>{false, true});
  PartitionMatroid boolean(vc<bool>{false, true}, vc<bool>{true, false});
  boolean.build(vc<bool>{false});
  assert(boolean.circuit(1)->empty());
  CSR<int> bool_rows(vc<bool>{true, false, true});
  assert(bool_rows.offset(3) == 2);
  SubsetSumFromFrequency bool_sums(vc<bool>{false, true, true}, 1000);
  assert(bool_sums.exists(3) && !bool_sums.exists(4));
  int const_calls = 0, mutable_calls = 0;
  IndependenceMatroid qualified(2, QualifiedPredicate{&const_calls, &mutable_calls});
  qualified.build(vc<ll>{0});
  assert(const_calls == 1 && mutable_calls == 0);
  cout << "Hello World\n";
}
