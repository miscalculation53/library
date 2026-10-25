#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/bipartite_coloring.hpp"

bool brute_force(int n, const vc<pair<int, int>> &es)
{
  repi(mask, 1 << n)
  {
    bool ok = true;
    for (auto [u, v] : es)
      if (((mask >> u) & 1) == ((mask >> v) & 1)) ok = false;
    if (ok) return true;
  }
  return false;
}

template <class Cost, class I>
void check_result(int n, const vc<pair<int, int>> &es, const vc<ll> &weights,
                  const tuple<bool, vc<I>, vc<Edge<Cost>>> &result, bool expected)
{
  const auto &[ok, color, cycle] = result;
  assert(ok == expected);
  if (ok)
  {
    assert(int(color.size()) == n && cycle.empty());
    for (auto c : color) assert(c == 0 || c == 1);
    for (auto [u, v] : es) assert(color[u] != color[v]);
    return;
  }
  assert(color.empty() && cycle.size() % 2 == 1);
  vc<bool> vertices(n), edges(es.size());
  int cur = cycle.front().from;
  for (const auto &e : cycle)
  {
    assert(e.from == cur && 0 <= cur && cur < n && !vertices[cur]);
    assert(0 <= e.index && e.index < int(es.size()) && !edges[e.index]);
    vertices[cur] = edges[e.index] = true;
    auto [u, v] = es[e.index];
    assert((e.from == u && e.to == v) || (e.from == v && e.to == u));
    if constexpr (!is_void_v<Cost>) assert(e.cost == weights[e.index]);
    cur = e.to;
  }
  assert(cur == cycle.front().from);
}

void check(int n, const vc<pair<int, int>> &es, bool expected)
{
  vc<ll> weights;
  vc<tuple<int, int, ll>> weighted;
  repi(i, es.size())
  {
    ll w = (ll(i) - 3) * 1000000007;
    weights.eb(w);
    weighted.eb(es[i].first, es[i].second, w);
  }
  const GraphUndirected<> g(n, es);
  const GraphUndirected<ll> wg(n, weighted);
  const auto result = bipartite_coloring(g);
  static_assert(is_same_v<decay_t<decltype(result)>, tuple<bool, vl, vc<Edge<void>>>>);
  check_result(n, es, weights, result, expected);
  check_result(n, es, weights, bipartite_coloring(wg), expected);
  const auto int_result = bipartite_coloring<void, int>(g);
  static_assert(is_same_v<decay_t<decltype(int_result)>, tuple<bool, vc<int>, vc<Edge<void>>>>);
  check_result(n, es, weights, int_result, expected);
}

int main()
{
  check(0, {}, true);
  check(5, {}, true);
  check(1, {{0, 0}}, false);
  check(2, {{0, 1}, {1, 0}, {0, 1}}, true);
  check(5, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}, true);
  check(5, {{0, 1}, {2, 3}, {3, 4}, {4, 2}}, false);
  check(4, {{0, 1}, {2, 3}, {3, 3}}, false);
  // 共通祖先が根以外で、両側の復元経路が複数辺となる奇閉路。
  check(7, {{1, 0}, {2, 1}, {3, 2}, {4, 3}, {5, 2}, {6, 5}, {6, 4}}, false);
  const GraphUndirected<> forest(6, vc<pair<int, int>>{{2, 3}, {4, 5}});
  assert((get<1>(bipartite_coloring(forest)) == vl{0, 0, 0, 1, 0, 1}));

  // n <= 4 は自己ループ込み、n = 5 は単純グラフを全列挙し全彩色と比較。
  repi(n, 1, 6)
  {
    vc<pair<int, int>> possible;
    repi(u, n) repi(v, u + (n == 5), n) possible.eb(u, v);
    repi(mask, 1 << possible.size())
    {
      vc<pair<int, int>> es;
      repi(i, possible.size()) if (mask >> i & 1) es.eb(possible[i]);
      check(n, es, brute_force(n, es));
    }
  }

  mt19937 rng(20260916);
  repi(tc, 2000)
  {
    int n = 1 + rng() % 8, m = rng() % 20;
    vc<int> color(n);
    for (auto &c : color) c = rng() % 2;
    vc<pair<int, int>> es;
    repi(i, m)
    {
      int u = rng() % n, v = rng() % n;
      if (tc % 2 == 0 && color[u] == color[v]) continue;
      es.eb(u, v);
    }
    check(n, es, brute_force(n, es));
  }

  // 長い木と奇閉路でも反復処理で復元できる。
  const int n = 200001;
  vc<pair<int, int>> es;
  repi(v, 1, n) es.eb(v - 1, v);
  check(n, es, true);
  es.eb(n - 1, 0);
  check(n, es, false);
  PRINT("Hello World");
}
