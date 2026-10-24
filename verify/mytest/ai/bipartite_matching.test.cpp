#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/bipartite_matching.hpp"
#include "graph/bipartite_coloring.hpp"
#include <atcoder/maxflow>

template <class Cost, bool erasable, class Color>
void check_result(const GraphUndirected<Cost, erasable> &g, const vc<Color> &color,
                  const vc<pair<int, int>> &matching, int expected)
{
  const int n = g.template size<int>();
  assert(int(matching.size()) == expected);
  vc<int> mate(n, -1), que(n);
  for (auto [u, v] : matching)
  {
    assert(0 <= u && u < n && 0 <= v && v < n);
    assert(color[u] == 0 && color[v] == 1 && mate[u] == -1 && mate[v] == -1);
    bool found = false;
    for (const auto &e : g.out_edges(u)) if (e.to == v) found = true;
    assert(found);
    mate[u] = v, mate[v] = u;
  }
  // 交互路から頂点被覆を作り、同じ大きさで全辺を覆えることも確認する。
  vc<bool> reached(n), cover(n);
  int front = 0, back = 0;
  for (int v = 0; v < n; v++) if (color[v] == 0 && mate[v] == -1)
    reached[v] = true, que[back++] = v;
  while (front < back)
  {
    const int u = que[front++];
    for (const auto &e : g.out_edges(u)) if (mate[u] != e.to && !reached[e.to])
    {
      const int v = e.to;
      reached[v] = true;
      assert(mate[v] != -1);
      if (!reached[mate[v]]) reached[mate[v]] = true, que[back++] = mate[v];
    }
  }
  int cover_size = 0;
  for (int v = 0; v < n; v++) cover_size += cover[v] = (color[v] == 0 ? !reached[v] : reached[v]);
  assert(cover_size == expected);
  for (int u = 0; u < n; u++) for (const auto &e : g.out_edges(u))
    assert(cover[u] || cover[e.to]);
}

int brute(int l, int r, const vc<pair<int, int>> &es)
{
  vc<int> neighbor(l), dp(1 << r, -1);
  for (auto [u, v] : es) neighbor[u] |= 1 << v;
  dp[0] = 0;
  for (int u = 0; u < l; u++)
  {
    auto next = dp;
    for (int mask = 0; mask < (1 << r); mask++) if (dp[mask] != -1)
      for (int v = 0; v < r; v++) if ((neighbor[u] >> v & 1) && !(mask >> v & 1))
        next[mask | (1 << v)] = max(next[mask | (1 << v)], dp[mask] + 1);
    dp.swap(next);
  }
  return *max_element(ALL(dp));
}

template <class Color>
void check(int n, const vc<pair<int, int>> &es, const vc<Color> &color, int expected)
{
  const GraphUndirected<> g(n, es);
  const auto matching = bipartite_matching(g, color);
  static_assert(is_same_v<decay_t<decltype(matching)>, vc<pair<int, int>>>);
  check_result(g, color, matching, expected);
}

struct ComparableColor
{
  int value;
  bool operator==(int rhs) const { return value == rhs; }
};

void initial_matching_cases()
{
  // 完全マッチングを持つ偶閉路と、両側に未対応頂点が残る連結グラフ。
  const vc<pair<int, int>> cycle{
      {0, 1}, {0, 3}, {2, 3}, {2, 5}, {4, 7}, {4, 5}, {6, 7}, {6, 1}};
  const vc<pair<int, int>> hall{
      {0, 1}, {3, 0}, {2, 1}, {2, 3}, {4, 1}, {3, 4},
      {4, 1}, {6, 5}, {7, 6}, {6, 3}};
  for (int flipped = 0; flipped < 2; flipped++)
  {
    vc<ComparableColor> color(8);
    for (int v = 0; v < 8; v++) color[v] = {(v % 2) ^ flipped};
    check(8, cycle, color, 4);
    check(8, hall, color, 3);
  }
}

void mixed_deficiency_case()
{
  // 完全マッチングを持つ部分と、Hall の不足がある部分を合わせる。
  const vc<pair<int, int>> core{
      {5, 6}, {0, 0}, {9, 9}, {1, 1}, {4, 4}, {2, 9},
      {1, 8}, {9, 2}, {3, 2}, {2, 2}, {7, 7}, {6, 3},
      {6, 2}, {7, 0}, {4, 3}, {5, 5}, {8, 6}, {4, 1},
      {7, 7}, {8, 3}, {1, 4}, {0, 8}, {3, 3}, {8, 8},
      {2, 2}, {9, 3}, {0, 0}, {6, 6}, {5, 5}, {3, 6}};
  constexpr int k = 14;
  vc<pair<int, int>> es;
  for (auto [u, v] : core) es.eb(4 + u, k + 4 + v);
  for (int u = 0; u < 3; u++) for (int v = 0; v < 2; v++) es.eb(u, k + v);
  es.eb(3, k + 2), es.eb(3, k + 3), es.eb(3, k);
  for (int flipped = 0; flipped < 2; flipped++)
  {
    vc<ComparableColor> color(2 * k);
    for (int v = 0; v < 2 * k; v++) color[v] = {int(v >= k) ^ flipped};
    check(2 * k, es, color, 13);
  }
}

void exhaustive()
{
  constexpr int l = 3, r = 4;
  // 色と頂点番号の大小が一致しない配置。
  const vc<int> left{6, 0, 3}, right{1, 5, 2, 4};
  vc<ll> color(l + r, 1);
  for (int u : left) color[u] = 0;
  for (int mask = 0; mask < (1 << (l * r)); mask++)
  {
    vc<pair<int, int>> es, plain;
    for (int u = 0; u < l; u++) for (int v = 0; v < r; v++) if (mask >> (u * r + v) & 1)
    {
      plain.eb(u, v);
      if ((u + v) % 2) es.eb(right[v], left[u]);
      else es.eb(left[u], right[v]);
    }
    check(l + r, es, color, brute(l, r, plain));
    auto flipped = color;
    for (auto &c : flipped) c ^= 1;
    check(l + r, es, flipped, brute(l, r, plain));
  }
}

void random_cases()
{
  mt19937_64 rng(20261015);
  for (int trial = 0; trial < 2000; trial++)
  {
    const int l = 1 + rng() % 7, r = 1 + rng() % 7, n = l + r + rng() % 5;
    vc<int> perm(n), color(n, 1);
    iota(ALL(perm), 0);
    shuffle(ALL(perm), rng);
    for (int i = 0; i < l; i++) color[perm[i]] = 0;
    vc<pair<int, int>> es, plain;
    for (int j = rng() % 100; j > 0; j--)
    {
      int u = rng() % l, v = rng() % r;
      plain.eb(u, v);
      int a = perm[u], b = perm[l + v];
      if (rng() % 2) swap(a, b);
      es.eb(a, b);
    }
    const int expected = brute(l, r, plain);
    check(n, es, color, expected);
    check(n, es, vc<bool>(ALL(color)), expected);
    check(n, es, vc<unsigned char>(ALL(color)), expected);
    if (trial % 10 == 0)
    {
      check(n, es, vc<u128>(ALL(color)), expected);
      check(n, es, vc<double>(ALL(color)), expected);
    }
  }

  for (int trial = 0; trial < 250; trial++)
  {
    const int n = 2 + rng() % 400, m = rng() % 5000;
    vc<short> color(n);
    for (auto &c : color) c = rng() % 2;
    vc<tuple<int, int, ll>> es;
    atcoder::mf_graph<int> reference(n + 2);
    for (int u = 0; u < n; u++)
      if (color[u] == 0) reference.add_edge(n, u, 1);
      else reference.add_edge(u, n + 1, 1);
    for (int i = 0; i < m; i++)
    {
      int u = rng() % n, v = rng() % n;
      if (color[u] == color[v]) continue;
      es.eb(u, v, ll(rng() % 2000000001) - 1000000000);
      if (color[u] != 0) swap(u, v);
      reference.add_edge(u, v, 1);
    }
    GraphUndirected<ll, true> g(n, es);
    vc<int> order(es.size());
    iota(ALL(order), 0);
    shuffle(ALL(order), rng);
    int expected = reference.flow(n, n + 1);
    check_result(g, color, bipartite_matching(g, color), expected);
    for (int j = 0; j < int(order.size()) / 2; j++) g.erase_edge(order[j]);
    reference = atcoder::mf_graph<int>(n + 2);
    for (int u = 0; u < n; u++)
      if (color[u] == 0) reference.add_edge(n, u, 1);
      else reference.add_edge(u, n + 1, 1);
    for (auto e : g.edges())
    {
      if (color[e.from] != 0) swap(e.from, e.to);
      reference.add_edge(e.from, e.to, 1);
    }
    expected = reference.flow(n, n + 1);
    check_result(g, color, bipartite_matching(g, color), expected);
    // 既存の彩色関数の vector<ll> をそのまま渡す。
    GraphUndirected<ll> remaining(n, g.edges());
    const auto [ok, coloring, cycle] = bipartite_coloring(remaining);
    assert(ok && cycle.empty());
    check_result(remaining, coloring, bipartite_matching(remaining, coloring), expected);
  }
}

void deep_alternating_path()
{
  constexpr int k = 200000;
  vc<pair<int, int>> es;
  es.reserve(2 * k - 1);
  for (int u = 0; u + 1 < k; u++) es.eb(u, k + u), es.eb(u, k + u + 1);
  es.eb(k - 1, k);
  const GraphUndirected<> g(2 * k, es);
  vc<bool> color(2 * k);
  for (int v = k; v < 2 * k; v++) color[v] = true;
  check_result(g, color, bipartite_matching(g, color), k);
  color.flip();
  check_result(g, color, bipartite_matching(g, color), k);
}

int main()
{
  check(0, {}, vc<bool>{}, 0);
  check(5, {}, vc<ll>(5, 0), 0);
  check(5, {}, vc<bool>(5, true), 0);
  check(6, {{0, 4}, {0, 5}, {1, 4}, {2, 4}, {3, 4}}, vc<signed char>{0, 0, 0, 0, 1, 1}, 2);
  initial_matching_cases();
  mixed_deficiency_case();
  exhaustive();
  random_cases();
  deep_alternating_path();
  cout << "Hello World\n";
}
