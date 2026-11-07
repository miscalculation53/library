#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/dm_decomposition.hpp"

namespace
{
struct AllMatchings
{
  int cardinality = -1;
  vc<vc<pair<int, int>>> maximum;
};

template<class Cost, bool Erasable, class Color>
AllMatchings enumerate(const GraphUndirected<Cost, Erasable> &g, const vc<Color> &color)
{
  const int n = g.template size<int>();
  vc<int> left;
  vvc<int> adj(n);
  for (int v = 0; v < n; v++) if (color[v] == 0)
  {
    left.push_back(v);
    for (const auto &e : g.out_edges(v)) adj[v].push_back(e.to);
    sort(ALL(adj[v]));
    adj[v].erase(unique(ALL(adj[v])), adj[v].end());
  }
  AllMatchings result;
  vc<bool> used(n);
  vc<pair<int, int>> matching;
  auto visit = [&](auto &&self, int i) -> void
  {
    if (i == int(left.size()))
    {
      if (int(matching.size()) > result.cardinality)
        result.cardinality = matching.size(), result.maximum.clear();
      if (int(matching.size()) == result.cardinality) result.maximum.push_back(matching);
      return;
    }
    self(self, i + 1);
    for (int v : adj[left[i]]) if (!used[v])
    {
      used[v] = true;
      matching.emplace_back(left[i], v);
      self(self, i + 1);
      matching.pop_back();
      used[v] = false;
    }
  };
  visit(visit, 0);
  return result;
}

template<class Cost, bool Erasable, class Color>
void check(const GraphUndirected<Cost, Erasable> &g, const vc<Color> &color)
{
  const int n = g.template size<int>();
  const auto all = enumerate(g, color);
  vc<bool> exposed(n), left_region(n), right_region(n);
  vvc<bool> allowed(n, vc<bool>(n)), connected(n, vc<bool>(n));
  for (const auto &matching : all.maximum)
  {
    vc<bool> used(n);
    for (auto [u, v] : matching) used[u] = used[v] = true, allowed[u][v] = allowed[v][u] = true;
    for (int v = 0; v < n; v++) if (!used[v]) exposed[v] = true;
  }
  for (int v = 0; v < n; v++)
  {
    connected[v][v] = true;
    if (exposed[v])
    {
      auto &region = color[v] == 0 ? left_region : right_region;
      region[v] = true;
      for (const auto &e : g.out_edges(v)) region[e.to] = true;
    }
    for (int u = 0; u < n; u++) if (allowed[u][v]) connected[u][v] = true;
  }
  for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
    connected[i][j] = connected[i][j] || (connected[i][k] && connected[k][j]);

  auto verify = [&](const DMDecomposition &dm)
  {
    assert(dm.size() >= 2 && int(dm.ids.size()) == n);
    assert(dm.right.template size<int>() == dm.size());
    assert(dm.dag.template size<int>() == dm.size());
    assert(int(dm.matching.size()) == all.cardinality);
    vc<int> used(n), listed(n);
    for (auto [u, v] : dm.matching)
    {
      assert(color[u] == 0 && color[v] == 1 && !used[u] && !used[v]);
      used[u] = used[v] = true;
      assert(allowed[u][v] && dm.ids[u] == dm.ids[v]);
    }
    int matching_sum = 0;
    for (int c = 0; c < dm.size(); c++)
    {
      const auto left = dm.left[c], right = dm.right[c];
      assert(is_sorted(ALL(left)) && is_sorted(ALL(right)));
      for (int v : left) assert(color[v] == 0 && dm.ids[v] == c), listed[v]++;
      for (int v : right) assert(color[v] == 1 && dm.ids[v] == c), listed[v]++;
      if (c == 0) assert((left.empty() && right.empty()) || left.size() > right.size());
      else if (c + 1 == dm.size()) assert((left.empty() && right.empty()) || left.size() < right.size());
      else assert(!left.empty() && left.size() == right.size());
      matching_sum += min(left.size(), right.size());
    }
    assert(matching_sum == all.cardinality);
    for (int v = 0; v < n; v++)
    {
      assert(listed[v] == 1);
      assert(0 <= dm.ids[v] && dm.ids[v] < dm.size());
      assert((dm.ids[v] == 0) == left_region[v]);
      assert((dm.ids[v] + 1 == dm.size()) == right_region[v]);
      assert(!(left_region[v] && right_region[v]));
      for (int u = 0; u < n; u++)
      {
        bool same = (left_region[u] && left_region[v]) || (right_region[u] && right_region[v]) ||
          (!left_region[u] && !right_region[u] && !left_region[v] && !right_region[v] && connected[u][v]);
        assert((dm.ids[u] == dm.ids[v]) == same);
      }
    }
    set<pair<int, int>> expected, actual;
    for (const auto &e : g.edges())
    {
      int u = e.from, v = e.to;
      if (color[u] == 1) swap(u, v);
      assert((dm.ids[u] == dm.ids[v]) == allowed[u][v]);
      assert(dm.ids[u] >= dm.ids[v]);
      if (dm.ids[u] != dm.ids[v]) expected.emplace(dm.ids[v], dm.ids[u]);
    }
    for (const auto &e : dm.dag.edges())
    {
      assert(e.from < e.to);
      assert(actual.emplace(e.from, e.to).second);
    }
    assert(actual == expected);
  };
  const auto automatic = dm_decomposition(g, color);
  verify(automatic);
  auto copied = automatic;
  auto moved = std::move(copied);
  verify(moved);
  for (auto matching : all.maximum)
  {
    const auto saved = matching;
    auto dm = dm_decomposition(g, color, matching);
    assert(matching == saved && dm.matching == saved);
    verify(dm);
    reverse(ALL(matching));
    auto reversed = dm_decomposition(g, color, std::move(matching));
    verify(reversed);
  }
}

struct LiteralColor
{
  int value;
  friend bool operator==(const LiteralColor &color, int value) { return color.value == value; }
};

void exhaustive()
{
  for (int l = 0; l <= 3; l++) for (int r = 0; r <= 3; r++)
    for (int mask = 0; mask < (1 << (l * r)); mask++)
  {
    vc<pair<int, int>> edges;
    vc<int> color(l + r, 1);
    for (int u = 0; u < l; u++)
    {
      color[u] = 0;
      for (int v = 0; v < r; v++) if (mask >> (u * r + v) & 1) edges.emplace_back(u, l + v);
    }
    check(GraphUndirected<>(l + r, edges), color);
  }
}

void random_cases()
{
  mt19937 rng(2026110601);
  for (int trial = 0; trial < 250; trial++)
  {
    const int l = rng() % 5, r = rng() % 5, n = l + r;
    vc<int> order(n); iota(ALL(order), 0); shuffle(ALL(order), rng);
    vc<bool> color(n, true); vc<ll> wide(n, 1); vc<LiteralColor> literal(n, {1});
    for (int i = 0; i < l; i++) color[order[i]] = false, wide[order[i]] = 0, literal[order[i]].value = 0;
    vc<pair<int, int>> edges; vc<tuple<int, int, ll>> weighted;
    for (int i = 0; i < l; i++) for (int j = 0; j < r; j++) if (rng() % 3)
    {
      int u = order[i], v = order[l + j];
      if (rng() & 1) swap(u, v);
      edges.emplace_back(u, v), weighted.emplace_back(u, v, ll(rng() % 31) - 15);
      if (rng() % 3 == 0) edges.emplace_back(u, v), weighted.emplace_back(u, v, 0);
    }
    check(GraphUndirected<>(n, edges), color);
    if (trial < 30)
    {
      auto inverted = color;
      for (int v = 0; v < n; v++) inverted[v] = !inverted[v];
      check(GraphUndirected<>(n, edges), inverted);
    }
    check(GraphUndirected<ll>(n, weighted), wide);
    if (trial < 100) check(GraphUndirected<>(n, edges), literal);
    GraphUndirected<ll, true> erasable(n, weighted);
    for (int e = 0; e < int(weighted.size()); e++) if (rng() % 3 == 0) erasable.erase_edge(e);
    check(erasable, color);
  }
}

void large_cases()
{
  constexpr int k = 20000;
  vc<pair<int, int>> edges, matching;
  vc<int> color(2 * k);
  for (int i = 0; i < k; i++)
  {
    color[2 * i + 1] = 1;
    matching.emplace_back(2 * i, 2 * i + 1);
    edges.emplace_back(2 * i, 2 * i + 1);
    if (i) edges.emplace_back(2 * i, 2 * i - 1);
  }
  const auto perfect = dm_decomposition(GraphUndirected<>(2 * k, edges), color);
  assert(perfect.size() == k + 2 && perfect.left[0].empty() && perfect.right[k + 1].empty());
  assert(perfect.dag.num_of_edges() == k - 1);
  for (int i = 0; i < k; i++) assert(perfect.ids[2 * i] == i + 1 && perfect.ids[2 * i + 1] == i + 1);
  edges.emplace_back(2 * k, 2 * k - 1);
  color.push_back(0);
  const auto deficient = dm_decomposition(GraphUndirected<>(2 * k + 1, edges), color, matching);
  assert(deficient.size() == 2 && deficient.dag.num_of_edges() == 0);
  assert(all_of(ALL(deficient.ids), [](int id) { return id == 0; }));
  GraphUndirected<short, true> source(2, vc<pair<int, int>>{{0, 1}});
  const auto kept = dm_decomposition(source, vc<int>{0, 1});
  source.erase_edge(0);
  assert(kept.size() == 3 && kept.matching.size() == 1 && kept.ids[0] == kept.ids[1]);
}
}

int main()
{
  exhaustive();
  random_cases();
  large_cases();
  PRINT("Hello World");
}
