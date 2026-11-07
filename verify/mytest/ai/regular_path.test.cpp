#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/b_matching.hpp"

int main()
{
  mt19937_64 rng(20261101);
  for (int trial = 0; trial < 20000; trial++)
  {
    const int n = 2 * (1 + rng() % 7), m = rng() % 22;
    vc<pair<int, int>> arc;
    vvc<int> adj(n);
    for (int i = 0; i < m; i++)
    {
      const int u = rng() % n, v = rng() % n;
      adj[u].eb(arc.size()), arc.eb(u, v);
      adj[v ^ 1].eb(arc.size()), arc.eb(v ^ 1, u ^ 1);
    }
    auto dfs = [&](auto self, int v, unsigned seen, ull used) -> bool
    {
      if (v == 1) return true;
      for (int a : adj[v])
        if (!(seen >> arc[a].second & 1) && !(used >> (a ^ 1) & 1))
          if (self(self, arc[a].second, seen | (1U << arc[a].second), used | (1ULL << a)))
            return true;
      return false;
    };
    const bool expected = dfs(dfs, 0, 1, 0);
    const auto result = internal::SkewRegularPath(n, arc).run(0);
    assert(bool(result) == expected);
    if (result)
    {
      unsigned seen = 1;
      ull used = 0;
      int v = 0;
      for (int a : *result)
      {
        assert(arc[a].first == v && !(seen >> arc[a].second & 1) && !(used >> (a ^ 1) & 1));
        v = arc[a].second, seen |= 1U << v, used |= 1ULL << a;
      }
      assert(v == 1);
    }
  }
  // 深く入れ子になった bud を通る経路を、再帰を使わずに復元する。
  constexpr int depth = 30000;
  vc<pair<int, int>> arc;
  auto add = [&](int u, int v) { arc.eb(u, v), arc.eb(v ^ 1, u ^ 1); };
  for (int i = 1; i <= depth; i++) add(2 * i - 2, 2 * i);
  for (int i = 1; i <= depth; i++) add(2 * i - 2, 2 * i);
  add(2 * depth, 2 * depth + 1);
  const auto result = internal::SkewRegularPath(2 * depth + 2, arc).run(0);
  assert(result && result->size() == 2 * depth + 1);
  vc<bool> seen(2 * depth + 2), used(arc.size());
  seen[0] = true;
  int v = 0;
  for (int a : *result)
  {
    assert(arc[a].first == v && !seen[arc[a].second] && !used[a ^ 1]);
    v = arc[a].second, seen[v] = true, used[a] = true;
  }
  assert(v == 1);
  cout << "Hello World\n";
}
