#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/b_matching.hpp"

int brute(const vc<pair<int, int>> &es, const vc<int> &capacity, vc<int> b)
{
  int best = 0;
  auto dfs = [&](auto self, int i, int total) -> void
  {
    if (i == int(es.size())) { best = max(best, total); return; }
    const auto [u, v] = es[i];
    const int limit = min(capacity[i], u == v ? b[u] / 2 : min(b[u], b[v]));
    for (int count = 0; count <= limit; count++)
    {
      b[u] -= count, b[v] -= count;
      self(self, i + 1, total + count);
      b[u] += count, b[v] += count;
    }
  };
  dfs(dfs, 0, 0);
  return best;
}

template <class Cost, bool erasable, class Cap>
void check(const GraphUndirected<Cost, erasable> &g, const vc<Cap> &b,
           const vc<Cap> &capacity, Cap expected, bool unlimited = false)
{
  const bool unit = all_of(ALL(capacity), [](auto x) { return x == 1; });
  for (int pass = 0; pass < (unit && !unlimited ? 2 : 1); pass++)
  {
    const auto result = unlimited ? b_matching(g, b, nullopt)
        : pass == 0 ? b_matching(g, b, capacity) : b_matching(g, b);
    static_assert(is_same_v<decay_t<decltype(result)>, vc<pair<int, Cap>>>);
    map<int, pair<int, int>> edge;
    for (const auto &e : g.edges()) edge[e.index] = {e.from, e.to};
    vc<Cap> degree(b.size());
    Cap total = 0;
    set<int> used;
    for (auto [id, count] : result)
    {
      assert(used.insert(id).second && edge.count(id));
      assert(count > 0 && count <= capacity[id]);
      auto [u, v] = edge[id];
      degree[u] += count, degree[v] += count, total += count;
    }
    assert(total == expected);
    for (int v = 0; v < int(b.size()); v++) assert(degree[v] <= b[v]);
  }
}

int main()
{
  check(GraphUndirected<>(0, vc<pair<int, int>>{}), vc<int>{}, vc<int>{}, 0);
  check(GraphUndirected<>(3, vc<pair<int, int>>{}), vc<int>{0, 3, 0}, vc<int>{}, 0);
  // 2 頂点のループと通常辺について、0..2 の辺容量と 0..4 の頂点容量を全探索。
  const vc<pair<int, int>> two{{0, 0}, {0, 1}, {1, 1}};
  for (int code = 0; code < 27; code++)
  {
    vc<int> capacity(3);
    for (int i = 0, x = code; i < 3; i++, x /= 3) capacity[i] = x % 3;
    for (int a = 0; a <= 4; a++) for (int b = 0; b <= 4; b++)
      check(GraphUndirected<>(2, two), vc<int>{a, b}, capacity, brute(two, capacity, {a, b}));
  }
  const vc<pair<int, int>> three{{0, 0}, {0, 1}, {0, 2}, {1, 1}, {1, 2}, {2, 2}};
  for (int mask = 0; mask < 64; mask++)
  {
    vc<int> capacity(6);
    for (int i = 0; i < 6; i++) capacity[i] = mask >> i & 1;
    for (int code = 0; code < 64; code++)
    {
      vc<int> b(3);
      for (int i = 0, x = code; i < 3; i++, x /= 4) b[i] = x % 4;
      check(GraphUndirected<>(3, three), b, capacity, brute(three, capacity, b));
    }
  }
  mt19937_64 rng(20261101);
  for (int trial = 0; trial < 4000; trial++)
  {
    const int n = 1 + rng() % 9, m = rng() % 12;
    vc<tuple<int, int, ll>> es;
    vc<int> b(n), capacity(m);
    for (int &x : b) x = rng() % 7;
    for (int i = 0; i < m; i++)
    {
      es.eb(rng() % n, rng() % n, ll(rng()));
      capacity[i] = rng() % 4;
    }
    GraphUndirected<ll, true> g(n, es);
    for (int i = 0; i < m; i++) if (rng() % 4 == 0) g.erase_edge(i);
    vc<pair<int, int>> remaining;
    vc<int> active_capacity;
    for (const auto &e : g.edges()) remaining.eb(e.from, e.to), active_capacity.eb(capacity[e.index]);
    const int expected = brute(remaining, active_capacity, b);
    check(g, b, capacity, expected);
    if (trial % 20 == 0)
    {
      check(g, vc<ll>(ALL(b)), vc<ll>(ALL(capacity)), ll(expected));
      check(g, vc<unsigned char>(ALL(b)), vc<unsigned char>(ALL(capacity)), (unsigned char)expected);
      check(g, vc<i128>(ALL(b)), vc<i128>(ALL(capacity)), i128(expected));
      check(g, vc<u128>(ALL(b)), vc<u128>(ALL(capacity)), u128(expected));
      const int bound = *max_element(ALL(b));
      const int unrestricted = brute(remaining, vc<int>(remaining.size(), bound), b);
      check(g, b, vc<int>(m, bound), unrestricted, true);
    }
  }
  const vc<pair<int, int>> joined{{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 5}, {5, 3}, {0, 3}};
  const i128 huge = (i128(1) << 100) + 1;
  check(GraphUndirected<>(6, joined), vc<i128>(6, huge),
        vc<i128>{huge, huge, huge, huge, huge, huge, 1}, 3 * huge);
  check(GraphUndirected<>(1, vc<pair<int, int>>{{0, 0}}), vc<u128>{u128(huge)},
        vc<u128>{u128(huge)}, u128(huge / 2));
  const auto unbounded = b_matching(GraphUndirected<>(6, joined), vc<i128>(6, huge), nullopt);
  i128 total = 0;
  for (auto [id, count] : unbounded) { (void)id; total += count; }
  assert(total == 3 * huge);
  cout << "Hello World\n";
}
