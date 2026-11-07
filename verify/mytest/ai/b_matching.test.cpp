#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/b_matching.hpp"

int brute(const vc<pair<int, int>> &es, vc<int> b)
{
  int answer = 0;
  auto dfs = [&](auto self, int i, int count) -> void
  {
    if (i == int(es.size())) { answer = max(answer, count); return; }
    if (count + int(es.size()) - i <= answer) return;
    const auto [u, v] = es[i];
    if (b[u] && b[v])
    {
      b[u]--, b[v]--;
      self(self, i + 1, count + 1);
      b[u]++, b[v]++;
    }
    self(self, i + 1, count);
  };
  dfs(dfs, 0, 0);
  return answer;
}

template <class Cost, bool erasable, class Capacity>
void check(const GraphUndirected<Cost, erasable> &g, const vc<Capacity> &b, int expected)
{
  const auto result = b_matching(g, b);
  static_assert(is_same_v<decay_t<decltype(result)>, vc<pair<int, Capacity>>>);
  assert(int(result.size()) == expected);
  set<int> used;
  vc<int> degree(g.template size<int>());
  const auto es = g.edges();
  for (const auto &entry : result)
  {
    const int index = entry.first;
    assert(used.insert(index).second && entry.second == 1);
    const auto it = find_if(ALL(es), [&](const auto &e) { return e.index == index; });
    assert(it != es.end());
    degree[it->from]++, degree[it->to]++;
  }
  for (int v = 0; v < int(b.size()); v++) assert(u128(degree[v]) <= u128(b[v]));
  if (all_of(ALL(b), [](auto x) { return u128(x) <= 1; }))
  {
    assert(int(b_matching(g, b, nullopt).size()) == expected);
    int length = 0;
    for (const auto &e : es) length = max(length, e.index + 1);
    assert(int(b_matching(g, b, vc<Capacity>(length, Capacity(1))).size()) == expected);
  }
}

int main()
{
  check(GraphUndirected<>(0, vc<pair<int, int>>{}), vc<int>{}, 0);
  check(GraphUndirected<>(1, vc<pair<int, int>>{{0, 0}}), vc<int>{1}, 0);
  check(GraphUndirected<>(1, vc<pair<int, int>>{{0, 0}}), vc<int>{2}, 1);
  check(GraphUndirected<>(1, vc<pair<int, int>>{{0, 0}}), vc<int>{4}, 1);
  check(GraphUndirected<>(2, vc<pair<int, int>>{{0, 0}, {0, 1}}), vc<int>{2, 1}, 1);
  check(GraphUndirected<>(2, vc<pair<int, int>>{{0, 1}}), vc<int>{2, 2}, 1);
  check(GraphUndirected<>(2, vc<pair<int, int>>{{0, 1}, {0, 1}, {1, 0}}), vc<int>{2, 2}, 2);
  check(GraphUndirected<>(3, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}}), vc<int>{1, 1, 1}, 1);
  check(GraphUndirected<>(2, vc<pair<int, int>>{{0, 1}}), vc<u128>(2, u128(1) << 100), 1);
  mt19937_64 rng(20261101);
  // 小さい全単純グラフと、0..2 の全容量。
  for (int mask = 0; mask < 64; mask++)
  {
    vc<pair<int, int>> es;
    int bit = 0;
    for (int u = 0; u < 4; u++) for (int v = u + 1; v < 4; v++, bit++)
      if (mask >> bit & 1) es.eb(u, v);
    for (int code = 0; code < 81; code++)
    {
      vc<int> b(4);
      for (int v = 0, x = code; v < 4; v++, x /= 3) b[v] = x % 3;
      check(GraphUndirected<>(4, es), b, brute(es, b));
    }
  }
  for (int trial = 0; trial < 10000; trial++)
  {
    const int n = 1 + rng() % 9;
    vc<int> b(n);
    for (int &x : b) x = rng() % 6;
    vc<pair<int, int>> es;
    for (int i = rng() % 20; i > 0; i--)
    {
      int u = rng() % n, v = rng() % n;
      if (u != v) es.eb(u, v);
    }
    const int expected = brute(es, b);
    check(GraphUndirected<>(n, es), b, expected);
    if (trial % 10 == 0)
    {
      check(GraphUndirected<>(n, es), vc<u128>(ALL(b)), expected);
      check(GraphUndirected<>(n, es), vc<bool>(n, true), brute(es, vc<int>(n, 1)));
      vc<tuple<int, int, ll>> weighted;
      for (auto [u, v] : es) weighted.eb(u, v, ll(rng()));
      GraphUndirected<ll, true> g(n, weighted);
      for (int i = 0; i < int(es.size()); i++) if (rng() % 2) g.erase_edge(i);
      vc<pair<int, int>> remaining;
      for (auto e : g.edges()) remaining.eb(e.from, e.to);
      check(g, vc<i128>(ALL(b)), brute(remaining, b));
    }
  }
  constexpr int n = 250;
  vc<pair<int, int>> es;
  for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++) es.eb(u, v);
  assert(b_matching(GraphUndirected<>(n, es), vc<int>(n, 2)).size() == n);
  cout << "Hello World\n";
}
