#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/general_weighted_matching.hpp"

template <class Cost, bool erasable>
u128 matching_weight(const GraphUndirected<Cost, erasable> &g, const vc<int> &selected)
{
  vc<Edge<Cost>> by_index;
  for (auto e : g.edges())
  {
    if (int(by_index.size()) <= e.index) by_index.resize(e.index + 1);
    by_index[e.index] = e;
  }
  vc<unsigned char> used(g.size());
  u128 result = 0;
  for (int id : selected)
  {
    assert(0 <= id && id < int(by_index.size()));
    const auto &e = by_index[id];
    assert(0 <= e.from && e.from < e.to && e.cost > 0);
    assert(!used[e.from] && !used[e.to]);
    used[e.from] = used[e.to] = true;
    result += u128(e.cost);
  }
  return result;
}

template <class Cost, bool erasable>
u128 brute(const GraphUndirected<Cost, erasable> &g)
{
  const int n = g.size();
  vvc<u128> weights(n, vc<u128>(n));
  for (auto e : g.edges()) if (e.from != e.to && e.cost > 0)
    weights[e.from][e.to] = max(weights[e.from][e.to], u128(e.cost));
  vc<u128> dp(1U << n);
  for (unsigned mask = 1; mask < dp.size(); mask++)
  {
    const int v = __builtin_ctz(mask);
    const unsigned rest = mask ^ (1U << v);
    dp[mask] = dp[rest];
    for (unsigned left = rest; left; left &= left - 1)
    {
      const int u = __builtin_ctz(left);
      if (weights[v][u]) dp[mask] = max(dp[mask], dp[rest ^ (1U << u)] + weights[v][u]);
    }
  }
  return dp.back();
}

template <class Cost>
void check_both(int n, const vc<Edge<Cost>> &edges)
{
  const GraphUndirected<Cost> g(n, edges);
  const u128 expected = brute(g);
  vc<internal::WeightedMatchingEdge<Cost>> input;
  u128 maximum = 0;
  for (const auto &e : edges)
  {
    input.push_back({e.from, e.to, e.cost});
    maximum = max(maximum, u128(e.cost));
  }
  auto run = [&](auto weight)
  {
    using Weight = decltype(weight);
    vc<int> dense, sparse;
    internal::GeneralWeightedMatching<Weight, Cost, false>(n, input.data(), input.size(), weight).solve(dense);
    internal::GeneralWeightedMatching<Weight, Cost, true>(n, input.data(), input.size(), weight).solve(sparse);
    assert(matching_weight(g, dense) == expected);
    assert(matching_weight(g, sparse) == expected);
  };
  if constexpr (sizeof(Cost) <= 4) run(ull(maximum));
  else run(maximum);
  assert(matching_weight(g, general_weighted_matching(g)) == expected);
}

void exhaustive()
{
  for (int n = 0; n <= 5; n++)
  {
    vc<pair<int, int>> pairs;
    int cases = 1;
    for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++)
    {
      pairs.eb(u, v);
      cases *= 3;
    }
    for (int i = 0; i < cases; i++)
    {
      vc<Edge<int>> edges;
      int value = i;
      for (auto [u, v] : pairs)
      {
        const int weight = value % 3;
        if (weight) edges.eb(u, v, weight, edges.size());
        value /= 3;
      }
      check_both(n, edges);
    }
  }
}

void random_cases()
{
  mt19937_64 random(20261112);
  for (int trial = 0; trial < 5000; trial++)
  {
    const int n = trial % 53 == 0 ? 17 : int(random() % 15);
    const int density = random() % 101;
    const int weight_limit = trial % 3 == 0 ? 4 : 1'000'000'000;
    vc<Edge<int>> simple;
    vc<tuple<int, int, ll>> edges;
    for (int u = 0; u < n; u++) for (int v = u; v < n; v++)
    {
      if (int(random() % 100) >= density) continue;
      const int weight = 1 + random() % weight_limit;
      if (u != v) simple.eb(u, v, weight, simple.size());
      edges.eb(u, v, weight);
      if (random() % 3 == 0) edges.eb(v, u, ll(random() % 17) - 8);
    }
    shuffle(ALL(simple), random);
    for (int i = 0; i < int(simple.size()); i++)
    {
      if (random() % 2) swap(simple[i].from, simple[i].to);
      simple[i].index = i;
    }
    check_both(n, simple);

    shuffle(ALL(edges), random);
    GraphUndirected<ll, true> g(n, edges);
    for (int i = 0; i < int(edges.size()); i++) if (random() % 4 == 0) g.erase_edge(i);
    assert(matching_weight(g, general_weighted_matching(g)) == brute(g));
  }
}

void special_cases()
{
  const GraphUndirected<int, true> empty(4, vc<tuple<int, int, int>>{
      {0, 0, 100}, {0, 1, 0}, {1, 2, -9}});
  assert(general_weighted_matching(empty).empty());
  GraphUndirected<int, true> multi(8, vc<tuple<int, int, int>>{
      {0, 1, 2}, {1, 0, 5}, {1, 1, 100}, {1, 0, 8}, {3, 4, 4}});
  multi.erase_edge(3);
  auto ids = general_weighted_matching(multi);
  sort(ALL(ids));
  assert((ids == vc<int>{1, 4}));

  const GraphUndirected<> unweighted(4, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}, {2, 3}});
  assert(matching_weight(unweighted, general_weighted_matching(unweighted)) == 2);
  const GraphUndirected<bool> booleans(3, vc<tuple<int, int, bool>>{{0, 1, false}, {1, 2, true}});
  assert((general_weighted_matching(booleans) == vc<int>{1}));

  const ull high64 = numeric_limits<ull>::max();
  const GraphUndirected<ull> wide64(4, vc<tuple<int, int, ull>>{
      {0, 1, high64}, {1, 2, high64}, {0, 3, high64 - 1}});
  assert(matching_weight(wide64, general_weighted_matching(wide64)) == u128(high64) * 2 - 1);
  const u128 high128 = numeric_limits<u128>::max() / 4;
  check_both(5, vc<Edge<u128>>{
      {0, 1, high128, 0}, {1, 2, high128 - 1, 1}, {2, 0, high128 - 3, 2},
      {1, 3, high128 - 7, 3}, {2, 4, 9, 4}, {3, 4, high128 - 4, 5}});
  const GraphUndirected<i128> signed128(4, vc<tuple<i128, i128, i128>>{
      {0, 1, numeric_limits<i128>::min()}, {1, 2, i128(1) << 123}, {2, 3, 1}});
  assert(matching_weight(signed128, general_weighted_matching(signed128)) == (u128(1) << 123));

  const int dense_n = 50, sparse_n = 400;
  vc<tuple<int, int, i128>> mixed;
  const i128 heavy = i128(1) << 116;
  for (int u = 0; u < dense_n; u++) for (int v = u + 1; v < dense_n; v++)
    mixed.eb(u, v, u / 2 == v / 2 ? 100 : 1);
  for (int v = 0; v + 1 < sparse_n; v++) mixed.eb(dense_n + v, dense_n + v + 1, v % 2 ? 1 : heavy);
  mixed.eb(0, dense_n, -10);
  const GraphUndirected<i128> disconnected(dense_n + sparse_n + 100, mixed);
  assert(matching_weight(disconnected, general_weighted_matching(disconnected)) ==
         u128(dense_n / 2) * 100 + u128(sparse_n / 2) * u128(heavy));
}

int main()
{
  special_cases();
  exhaustive();
  random_cases();
  cout << "Hello World\n";
}
