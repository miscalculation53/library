#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/weighted_matroid_intersection.hpp"
#include "matroid/partition_matroid.hpp"
#include "matroid/graphic_matroid.hpp"
#include "matroid/binary_matroid.hpp"

namespace
{
template <class M>
bool independent(M matroid, const vc<int> &ids)
{
  vc<int> selected;
  for (int e : ids)
  {
    matroid.build(selected);
    if (matroid.circuit(e)) return false;
    selected.push_back(e);
  }
  return true;
}

template <class M1, class M2, class W, class Solve>
vc<pair<W, vc<int>>> run(M1 first, M2 second, const vc<W> &weights, Solve solve)
{
  vc<pair<W, vc<int>>> result;
  internal::weighted_matroid_intersection_run(first, second, weights, first.size(),
      [&](const W &total, const vc<int> &ids) { result.emplace_back(total, ids); }, solve);
  return result;
}

template <class M1, class M2, class W>
void check(M1 first, M2 second, const vc<W> &weights)
{
  auto automatic = weighted_matroid_intersection_slope(first, second, weights);
  auto heap = run(first, second, weights, [](auto &solver, int s, int t) { return solver.dijkstra(s, t); });
  auto dense = run(first, second, weights, [](auto &solver, int s, int t) { return solver.dijkstra_dense(s, t); });
  auto fibonacci = run(first, second, weights, [](auto &solver, int s, int t) { return solver.dijkstra_fibonacci(s, t); });
  auto bf = run(first, second, weights, [](auto &solver, int s, [[maybe_unused]] int t) { return solver.bellman_ford(s); });
  vc<optional<W>> optimum(weights.size() + 1);
  for (int mask = 0; mask < (1 << weights.size()); mask++)
  {
    vc<int> selected;
    W total{};
    for (int e = 0; e < int(weights.size()); e++) if (mask >> e & 1)
      selected.push_back(e), total += weights[e];
    if (independent(first, selected) && independent(second, selected))
    {
      int k = selected.size();
      if (!optimum[k] || *optimum[k] < total) optimum[k] = total;
    }
  }
  for (const auto *result : {&automatic, &heap, &dense, &fibonacci, &bf})
  {
    assert(result->size() == automatic.size());
    for (int k = 0; k < int(result->size()); k++)
    {
      const auto &[total, selected] = (*result)[k];
      assert(int(selected.size()) == k && optimum[k] && !(*optimum[k] < total) && !(total < *optimum[k]));
      assert(independent(first, selected) && independent(second, selected));
    }
  }
}

// W の無限大・数値上限・整数からの構築・等値比較を要求しない。
struct ExactWeight
{
  i128 value;
  ExactWeight() : value(0) {}
  static ExactWeight from(i128 value) { ExactWeight result; result.value = value; return result; }
  friend ExactWeight operator+(const ExactWeight &a, const ExactWeight &b) { return from(a.value + b.value); }
  friend ExactWeight operator-(const ExactWeight &a) { return from(-a.value); }
  ExactWeight &operator+=(const ExactWeight &b) { value += b.value; return *this; }
  friend bool operator<(const ExactWeight &a, const ExactWeight &b) { return a.value < b.value; }
};

template <class M>
struct CountingOracle
{
  M matroid;
  int *builds;
  int size() const { return matroid.size(); }
  void build(const vc<int> &ids) { ++*builds; matroid.build(ids); }
  optional<vc<int>> circuit(int e) const { return matroid.circuit(e); }
};

void cardinality_stops()
{
  PartitionMatroid left({0, 0, 1}, {1, 1}), right({0, 1, 0}, {1, 1});
  vc<ll> weights{10, 4, 3};
  int first_builds = 0, second_builds = 0;
  CountingOracle<PartitionMatroid> first{left, &first_builds}, second{right, &second_builds};
  auto zero = weighted_matroid_intersection(first, second, weights, 0);
  assert(zero && zero->first == 0 && zero->second.empty());
  assert(first_builds == 0 && second_builds == 0);
  auto one = weighted_matroid_intersection(first, second, weights, 1);
  assert(one && one->first == 10 && one->second == vc<int>{0});
  assert(first_builds == 1 && second_builds == 1);
  auto two = weighted_matroid_intersection(first, second, weights, 2);
  assert(two && two->first == 7 && two->second == vc<int>({1, 2}));
  assert(!weighted_matroid_intersection(first, second, weights, 3));
}
} // namespace

int main()
{
  mt19937_64 rng(2026110210);
  for (int trial = 0; trial < 120; trial++)
  {
    int n = rng() % 10, groups = 1 + rng() % 4, vertices = 1 + rng() % 5;
    vc<int> group1(n), group2(n), cap1(groups), cap2(groups);
    vc<ll> weights(n);
    vc<i128> wide(n);
    vc<ExactWeight> custom(n);
    vc<pair<int, int>> edges;
    vc<ull> vectors(n);
    for (int &c : cap1) c = rng() % 4;
    for (int &c : cap2) c = rng() % 4;
    for (int e = 0; e < n; e++)
    {
      group1[e] = rng() % groups, group2[e] = rng() % groups;
      weights[e] = trial % 4 == 0 ? 0 : ll(rng() % 13) - 6;
      wide[e] = i128(weights[e]) * (i128(1) << 90);
      custom[e] = ExactWeight::from(wide[e]);
      vectors[e] = rng() % 32;
      edges.emplace_back(rng() % vertices, rng() % vertices);
    }
    PartitionMatroid first(group1, cap1), second(group2, cap2);
    check(first, second, weights);
    check(first, GraphicMatroid(vertices, edges), wide);
    check(first, BinaryMatroid<>(vectors), custom);
  }
  cardinality_stops();
  PRINT("Hello World");
}
