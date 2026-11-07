#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/general_matching.hpp"
#include "flow/bipartite_matching.hpp"

int brute(int n, const vc<pair<int, int>> &es)
{
  vc<int> neighbor(n), dp(1 << n, -1);
  for (auto [u, v] : es) if (u != v) neighbor[u] |= 1 << v, neighbor[v] |= 1 << u;
  dp[0] = 0;
  auto solve = [&](auto self, int mask) -> int
  {
    if (dp[mask] != -1) return dp[mask];
    const int u = __builtin_ctz(unsigned(mask)), rest = mask ^ (1 << u);
    int answer = self(self, rest);
    for (int v = 0; v < n; v++) if ((rest >> v & 1) && (neighbor[u] >> v & 1))
      answer = max(answer, 1 + self(self, rest ^ (1 << v)));
    return dp[mask] = answer;
  };
  return solve(solve, (1 << n) - 1);
}

template <class Cost, bool erasable>
void check(const GraphUndirected<Cost, erasable> &g, int expected,
           const vc<pair<int, int>> &initial = {})
{
  const auto matching = general_matching(g, initial);
  static_assert(is_same_v<decay_t<decltype(matching)>, vc<pair<int, int>>>);
  assert(int(matching.size()) == expected);
  vc<bool> used(g.template size<int>());
  for (auto [u, v] : matching)
  {
    assert(u < v && !used[u] && !used[v]);
    used[u] = used[v] = true;
    bool exists = false;
    for (auto e : g.out_edges(u)) if (e.to == v) exists = true;
    assert(exists);
  }
}

void exhaustive()
{
  for (int n = 0; n <= 6; n++)
  {
    vc<pair<int, int>> all;
    for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++) all.eb(u, v);
    for (int mask = 0; mask < (1 << all.size()); mask++)
    {
      vc<pair<int, int>> es;
      for (int i = 0; i < int(all.size()); i++) if (mask >> i & 1) es.eb(all[i]);
      check(GraphUndirected<>(n, es), brute(n, es));
    }
  }
}

void random_cases()
{
  mt19937_64 rng(20261101);
  for (int trial = 0; trial < 10000; trial++)
  {
    const int n = 1 + rng() % 17, density = rng() % 101;
    vc<pair<int, int>> es, initial;
    vc<bool> used(n);
    for (int u = 0; u < n; u++) for (int v = u; v < n; v++)
      if (int(rng() % 100) < density)
      {
        es.eb(u, v);
        if (rng() % 5 == 0) es.eb(v, u);
      }
    shuffle(ALL(es), rng);
    for (auto [u, v] : es) if (u != v && !used[u] && !used[v] && rng() % 2)
      used[u] = used[v] = true, initial.eb(v, u);
    const int expected = brute(n, es);
    check(GraphUndirected<>(n, es), expected);
    check(GraphUndirected<>(n, es), expected, initial);
    if (trial % 10 == 0)
    {
      vc<tuple<int, int, i128>> weighted;
      for (auto [u, v] : es) weighted.eb(u, v, i128(rng()) * rng());
      GraphUndirected<i128, true> g(n, weighted);
      for (int i = 0; i < int(es.size()); i++) if (rng() % 2) g.erase_edge(i);
      vc<pair<int, int>> remaining;
      for (auto e : g.edges()) remaining.eb(e.from, e.to);
      check(g, brute(n, remaining));
    }
  }
  // 大きい二部グラフでは既存の Hopcroft–Karp 法とも照合する。
  for (int trial = 0; trial < 150; trial++)
  {
    const int n = 20 + rng() % 700;
    vc<bool> color(n);
    for (int v = 0; v < n; v++) color[v] = rng() % 2;
    vc<pair<int, int>> es;
    for (int i = rng() % 10000; i > 0; i--)
    {
      int u = rng() % n, v = rng() % n;
      if (color[u] != color[v]) es.eb(u, v);
    }
    const GraphUndirected<> g(n, es);
    check(g, bipartite_matching(g, color).size());
  }
}

void long_paths_and_petals()
{
  constexpr int k = 100000;
  vc<pair<int, int>> es, initial;
  for (int v = 0; v + 1 < 2 * k; v++) es.eb(v, v + 1);
  for (int v = 1; v + 1 < 2 * k; v += 2) initial.eb(v, v + 1);
  check(GraphUndirected<>(2 * k, es), k, initial);
  // 基点を共有しながら深く入れ子になる奇閉路。
  es.clear(), initial.clear();
  for (int i = 0; i < k; i++)
  {
    es.eb(2 * i, 2 * i + 1), es.eb(2 * i + 1, 2 * i + 2), es.eb(2 * i + 2, 0);
    initial.eb(2 * i + 1, 2 * i + 2);
  }
  check(GraphUndirected<>(2 * k + 1, es), k, initial);
  es.eb(2 * k, 2 * k + 1);
  check(GraphUndirected<>(2 * k + 2, es), k + 1, initial);
}

// Tutte 行列のランダム評価による、非二部グラフの独立な照合。
void tutte_cases()
{
  constexpr ll p = 1000000007;
  mt19937_64 rng(48220261101ULL);
  for (int trial = 0; trial < 300; trial++)
  {
    const int n = 30 + rng() % 100, density = 1 + rng() % 30;
    vc<pair<int, int>> es;
    vvc<ll> a(n, vc<ll>(n));
    for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++)
      if (int(rng() % 100) < density)
      {
        es.eb(u, v);
        a[u][v] = 1 + rng() % (p - 1), a[v][u] = p - a[u][v];
      }
    int rank = 0;
    for (int col = 0; col < n; col++)
    {
      int pivot = rank;
      while (pivot < n && a[pivot][col] == 0) pivot++;
      if (pivot == n) continue;
      swap(a[rank], a[pivot]);
      ll inverse = 1, x = a[rank][col];
      for (ll e = p - 2; e; e >>= 1, x = x * x % p) if (e & 1) inverse = inverse * x % p;
      for (int row = rank + 1; row < n; row++) if (a[row][col])
      {
        const ll factor = a[row][col] * inverse % p;
        for (int j = col; j < n; j++)
          a[row][j] = (a[row][j] - factor * a[rank][j] % p + p) % p;
      }
      rank++;
    }
    assert(rank % 2 == 0);
    check(GraphUndirected<>(n, es), rank / 2);
  }
}

int main()
{
  check(GraphUndirected<>(0, vc<pair<int, int>>{}), 0);
  check(GraphUndirected<>(5, vc<pair<int, int>>{{0, 0}, {2, 2}}), 0);
  exhaustive();
  random_cases();
  tutte_cases();
  long_paths_and_petals();
  cout << "Hello World\n";
}
