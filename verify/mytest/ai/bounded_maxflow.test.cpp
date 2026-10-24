#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/bounded_maxflow.hpp"

struct Input { int from, to, lower, upper; };

pair<bool, optional<pair<int, int>>> brute(int n, int s, int t, const vc<Input> &es)
{
  vc<int> balance(n);
  bool circulation = false;
  optional<pair<int, int>> range;
  auto dfs = [&](auto &self, int id) -> void
  {
    if (id == int(es.size()))
    {
      bool all_zero = true, interior_zero = true;
      for (int v = 0; v < n; v++)
      {
        all_zero &= balance[v] == 0;
        if (v != s && v != t) interior_zero &= balance[v] == 0;
      }
      circulation |= all_zero;
      if (interior_zero)
      {
        const int value = balance[s];
        if (!range) range = pair{value, value};
        range->first = min(range->first, value), range->second = max(range->second, value);
      }
      return;
    }
    const auto e = es[id];
    for (int f = e.lower; f <= e.upper; f++)
    {
      balance[e.from] += f, balance[e.to] -= f;
      self(self, id + 1);
      balance[e.from] -= f, balance[e.to] += f;
    }
  };
  dfs(dfs, 0);
  return {circulation, range};
}

template <class Cap>
void check_state(const BoundedMaxFlow<Cap> &g, int s, int t, i128 value)
{
  vc<i128> balance(g.template size<int>());
  auto edges = g.edges();
  for (int i = 0; i < int(edges.size()); i++)
  {
    const auto e = g.get_edge(i);
    assert(e.flow == edges[i].flow && e.lower <= e.flow && e.flow <= e.upper);
    balance[e.from] += i128(e.flow), balance[e.to] -= i128(e.flow);
  }
  for (int v = 0; v < int(balance.size()); v++)
    assert(balance[v] == (v == s ? value : v == t ? -value : 0));
}

template <class Cap>
void check(int n, int s, int t, const vc<Input> &es, bool circulation,
           optional<pair<int, int>> expected)
{
  BoundedMaxFlow<Cap> g(n);
  for (auto e : es) g.add_edge(e.from, e.to, e.lower, e.upper);
  assert(g.feasible() == circulation);
  if (circulation) check_state(g, -1, -1, 0);
  const auto maximum = g.flow(s, t);
  assert(maximum.has_value() == expected.has_value());
  if (maximum)
  {
    assert(*maximum == expected->second);
    check_state(g, s, t, *maximum);
    const auto cut = g.min_cut(s);
    assert(cut[s] && !cut[t]);
    i128 bound = 0;
    for (auto e : es)
    {
      if (cut[e.from] && !cut[e.to]) bound += e.upper;
      if (!cut[e.from] && cut[e.to]) bound -= e.lower;
    }
    assert(bound == *maximum);
  }
  const auto minimum = g.min_flow(s, t);
  assert(minimum.has_value() == expected.has_value());
  if (minimum) assert(*minimum == expected->first), check_state(g, s, t, *minimum);
  const auto range = g.flow_range(s, t);
  assert(bool(range) == bool(expected));
  if (range)
  {
    assert(range->first == expected->first && range->second == expected->second);
    check_state(g, s, t, range->second);
  }
}

int main()
{
  mt19937 rng(20261016);
  for (int trial = 0; trial < 1600; trial++)
  {
    int n = 2 + rng() % 4, m = rng() % 8, s = rng() % n, t = (s + 1 + rng() % (n - 1)) % n;
    vc<Input> es;
    for (int i = 0; i < m; i++)
    {
      int lower = int(rng() % 5) - 2;
      es.push_back({int(rng() % n), int(rng() % n), lower, lower + int(rng() % 3)});
    }
    const auto [circulation, expected] = brute(n, s, t, es);
    check<int>(n, s, t, es, circulation, expected);
    check<ll>(n, s, t, es, circulation, expected);
    if (trial < 100) check<i128>(n, s, t, es, circulation, expected);
  }
  {
    BoundedMaxFlow<ll> g(2);
    g.add_edge(0, 1, LLONG_MIN, LLONG_MAX);
    const auto range = g.flow_range(0, 1);
    assert(range && range->first == LLONG_MIN && range->second == LLONG_MAX);
    check_state(g, 0, 1, LLONG_MAX);
    g.change_edge(0, -3, -2);
    assert(g.flow(0, 1) == (-2LL));
    auto copy = g;
    copy.add_edge(0, 1, 7, 9);
    assert(copy.min_flow(0, 1) == (4LL));
    assert(g.min_flow(0, 1) == (-3LL));
  }
  {
    BoundedMaxFlow<signed char> g(2);
    g.add_edge(0, 1, -128, 127);
    const auto range = g.flow_range(0, 1);
    assert(range && range->first == -128 && range->second == 127);
  }
  {
    // 需給点の不足量も、需給の総和も同じ幅の符号なし型を越える。
    BoundedMaxFlow<ll> g(2);
    for (int i = 0; i < 4; i++) g.add_edge(0, 1, LLONG_MAX, LLONG_MAX);
    for (int i = 0; i < 4; i++) g.add_edge(1, 0, 0, LLONG_MAX);
    assert(g.feasible());
    check_state(g, -1, -1, 0);
  }
  {
    BoundedMaxFlow<i128> g(3);
    const i128 big = i128(1) << 90;
    g.add_edge(0, 1, big, 3 * big);
    g.add_edge(1, 2, 2 * big, 4 * big);
    const auto range = g.flow_range(0, 2);
    assert(range && range->first == 2 * big && range->second == 3 * big);
    check_state(g, 0, 2, 3 * big);
  }
  assert(BoundedMaxFlow<int>(0).feasible());
  cout << "Hello World\n";
}
