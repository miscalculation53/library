#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/b_matching.hpp"

bool brute(const vc<pair<int, int>> &es, vc<int> demand)
{
  auto dfs = [&](auto self, int i) -> bool
  {
    if (i == int(es.size())) return all_of(ALL(demand), [](int x) { return x == 0; });
    if (self(self, i + 1)) return true;
    const auto [u, v] = es[i];
    if (!demand[u] || !demand[v]) return false;
    demand[u]--, demand[v]--;
    const bool found = self(self, i + 1);
    demand[u]++, demand[v]++;
    return found;
  };
  return dfs(dfs, 0);
}

template <class Cost, bool erasable, class Demand>
void check(const GraphUndirected<Cost, erasable> &g, const vc<Demand> &demand, bool expected)
{
  const auto selected = f_factor(g, demand);
  static_assert(is_same_v<decay_t<decltype(selected)>, optional<vc<int>>>);
  assert(bool(selected) == expected);
  if (!selected) return;
  map<int, pair<int, int>> edge;
  for (const auto &e : g.edges()) edge[e.index] = {e.from, e.to};
  vc<int> degree(demand.size());
  set<int> used;
  for (int id : *selected)
  {
    assert(edge.count(id) && used.insert(id).second);
    const auto [u, v] = edge[id];
    degree[u]++, degree[v]++;
  }
  for (int v = 0; v < int(demand.size()); v++) assert(u128(degree[v]) == u128(demand[v]));
}

int main()
{
  check(GraphUndirected<>(0, vc<pair<int, int>>{}), vc<int>{}, true);
  check(GraphUndirected<>(3, vc<pair<int, int>>{}), vc<int>{0, 0, 0}, true);
  check(GraphUndirected<>(3, vc<pair<int, int>>{}), vc<int>{0, 1, 0}, false);
  const GraphUndirected<> triangle(3, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}});
  check(triangle, vc<int>{2, 2, 2}, true);
  check(triangle, vc<int>{1, 1, 1}, false);
  check(triangle, vc<int>{2, 1, 1}, true);
  check(GraphUndirected<>(6, vc<pair<int, int>>{
      {0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 5}, {5, 3}}), vc<int>(6, 1), false);
  const GraphUndirected<> parallel(2, vc<pair<int, int>>{{0, 1}, {1, 0}, {0, 1}});
  check(parallel, vc<int>{2, 2}, true);
  check(parallel, vc<int>{1, 3}, false);
  check(parallel, vc<u128>(2, u128(1) << 100), false);
  check(parallel, vc<i128>(2, i128(1) << 100), false);
  // 4 頂点の全単純グラフと、0..3 の全指定次数を辺部分集合と比較する。
  for (int mask = 0; mask < 64; mask++)
  {
    vc<pair<int, int>> es;
    int bit = 0;
    for (int u = 0; u < 4; u++) for (int v = u + 1; v < 4; v++, bit++)
      if (mask >> bit & 1) es.eb(u, v);
    const GraphUndirected<> g(4, es);
    for (int code = 0; code < 256; code++)
    {
      vc<int> demand(4);
      for (int v = 0, x = code; v < 4; v++, x /= 4) demand[v] = x % 4;
      check(g, demand, brute(es, demand));
    }
  }
  mt19937_64 rng(20261101);
  for (int trial = 0; trial < 2000; trial++)
  {
    const int n = 1 + rng() % 9;
    vc<tuple<int, int, ll>> es;
    for (int i = rng() % 17; i > 0; i--)
    {
      const int u = rng() % n, v = rng() % n;
      if (u != v) es.eb(u, v, ll(rng()));
    }
    GraphUndirected<ll, true> g(n, es);
    for (int i = 0; i < int(es.size()); i++) if (rng() % 5 == 0) g.erase_edge(i);
    vc<pair<int, int>> remaining;
    vc<int> demand(n);
    for (const auto &e : g.edges())
    {
      remaining.eb(e.from, e.to);
      if (rng() % 2) demand[e.from]++, demand[e.to]++;
    }
    if (trial % 2) for (int &d : demand) d = rng() % 5;
    const bool expected = brute(remaining, demand);
    check(g, demand, expected);
    if (trial % 10 == 0)
    {
      check(g, vc<ll>(ALL(demand)), expected);
      check(g, vc<i128>(ALL(demand)), expected);
      check(g, vc<u128>(ALL(demand)), expected);
      vc<bool> ones(n, true);
      check(g, ones, brute(remaining, vc<int>(n, 1)));
    }
  }
  cout << "Hello World\n";
}
