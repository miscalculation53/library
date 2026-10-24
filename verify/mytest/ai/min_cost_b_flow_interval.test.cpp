#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/min_cost_b_flow.hpp"

using BF = MinCostBFlow<ll, ll>;
using A = BF::Algorithm;
using H = BF::Heap;
using Bounds = vc<pair<ll, ll>>;
using InputEdge = tuple<int, int, ll, ll, ll>;

optional<ll> brute(const Bounds &bounds, const vc<InputEdge> &edges)
{
  optional<ll> best;
  vc<ll> balance(bounds.size());
  auto dfs = [&](auto &self, int i, ll cost) -> void
  {
    if (i == int(edges.size()))
    {
      for (int v = 0; v < int(bounds.size()); v++)
        if (balance[v] < bounds[v].first || bounds[v].second < balance[v]) return;
      if (!best || cost < *best) best = cost;
      return;
    }
    auto [u, v, lower, upper, cost_per_unit] = edges[i];
    for (ll flow = lower; flow <= upper; flow++)
    {
      balance[u] += flow, balance[v] -= flow;
      self(self, i + 1, cost + flow * cost_per_unit);
      balance[u] -= flow, balance[v] += flow;
    }
  };
  dfs(dfs, 0, 0);
  return best;
}

template <class Cap, class Cost, class Total>
void check_state(const MinCostBFlow<Cap, Cost, Total> &g, const vc<pair<Cap, Cap>> &bounds, Total expected)
{
  const int n = bounds.size();
  assert(g.size() == n);
  auto p = g.potential();
  assert(int(p.size()) == n && p == g.potential());
  auto edges = g.edges();
  assert(int(edges.size()) == g.num_of_edges());
  vc<Cap> balance(n);
  Total cost = 0;
  for (int i = 0; i < int(edges.size()); i++)
  {
    auto e = g.get_edge(i);
    assert(0 <= e.from && e.from < n && 0 <= e.to && e.to < n);
    assert(e.from == edges[i].from && e.to == edges[i].to && e.flow == edges[i].flow);
    assert(e.lower <= e.flow && e.flow <= e.upper);
    balance[e.from] += e.flow, balance[e.to] -= e.flow;
    cost += Total(e.flow) * Total(e.cost);
    const i128 reduced = i128(e.cost) + p[e.from] - p[e.to];
    if (e.flow < e.upper) assert(reduced >= 0);
    if (e.flow > e.lower) assert(reduced <= 0);
  }
  for (int v = 0; v < n; v++)
  {
    assert(balance[v] == g.get_supply(v));
    assert(bounds[v].first <= balance[v] && balance[v] <= bounds[v].second);
    // 補助頂点を0に正規化した、供給区間の相補性条件。
    if (balance[v] < bounds[v].second) assert(p[v] <= 0);
    if (balance[v] > bounds[v].first) assert(p[v] >= 0);
  }
  assert(cost == expected);
}

void check(const Bounds &bounds, const vc<InputEdge> &edges)
{
  const auto expected = brute(bounds, edges);
  for (A algorithm : {A::automatic, A::ssp, A::primal_dual, A::capacity_scaling, A::network_simplex})
    for (H heap : {H::automatic, H::binary, H::fibonacci, H::dense})
    {
      BF g(bounds.size());
      for (int v = 0; v < int(bounds.size()); v++) g.set_supply(v, bounds[v].first, bounds[v].second);
      for (auto [u, v, lower, upper, cost] : edges)
        assert(g.add_edge(u, v, lower, upper, cost) + 1 == g.num_of_edges());
      assert(g.num_of_edges() == int(edges.size()));
      const auto cost = g.flow(algorithm, heap);
      assert(cost.has_value() == expected.has_value());
      if (cost)
      {
        assert(*cost == *expected);
        check_state(g, bounds, *cost);
      }
    }
}

void small_random()
{
  mt19937_64 rng(291058);
  for (int trial = 0; trial < 900; trial++)
  {
    const int n = 1 + rng() % 6, m = rng() % 8;
    vc<InputEdge> edges;
    vc<ll> balance(n);
    for (int i = 0; i < m; i++)
    {
      const int u = rng() % n, v = rng() % n;
      const ll lower = ll(rng() % 5) - 2, upper = lower + rng() % 4, cost = ll(rng() % 11) - 5;
      edges.eb(u, v, lower, upper, cost);
      const ll f = lower + rng() % (upper - lower + 1);
      balance[u] += f, balance[v] -= f;
    }
    Bounds bounds(n);
    for (int v = 0; v < n; v++)
      bounds[v] = {balance[v] - ll(rng() % 3), balance[v] + ll(rng() % 3)};
    if (trial % 3 == 0)
    {
      const int v = rng() % n;
      const ll lower = ll(rng() % 9) - 4;
      bounds[v] = {lower, lower + ll(rng() % 5)};
    }
    if (trial % 5 == 0) for (int v = 0; v < n; v++) bounds[v] = {balance[v], balance[v]};
    check(bounds, edges);
  }
}

void mutations()
{
  for (A algorithm : {A::automatic, A::ssp, A::primal_dual, A::capacity_scaling, A::network_simplex})
  {
    BF g(vc<ll>{2, 0, -2});
    g.add_edge(0, 1, 8, 1), g.add_edge(1, 2, 8, 1);
    g.set_supply(0, 1, 4), g.set_supply(2, -4, -1);
    assert(g.flow(algorithm) == (2LL));
    check_state(g, Bounds{{1, 4}, {0, 0}, {-4, -1}}, 2LL);
    g.add_supply(0, 2), g.add_supply(2, -2);
    assert(g.flow(algorithm) == (6LL));
    check_state(g, Bounds{{3, 6}, {0, 0}, {-6, -3}}, 6LL);
    g.add_supply(0, -1, 2), g.add_supply(2, -2, 1);
    assert(g.flow(algorithm) == (4LL));
    check_state(g, Bounds{{2, 8}, {0, 0}, {-8, -2}}, 4LL);
    g.set_supply(0, 4, 4), g.set_supply(2, -4, -4);
    assert(g.flow(algorithm) == (8LL));
    check_state(g, Bounds{{4, 4}, {0, 0}, {-4, -4}}, 8LL);
    auto copy = g;
    copy.set_supply(0, 0, 6), copy.set_supply(2, -6, 0);
    assert(copy.flow(algorithm) == (0LL));
    copy.add_edge(0, 2, 2, -10);
    assert(copy.flow(algorithm) == (-20LL));
    check_state(copy, Bounds{{0, 6}, {0, 0}, {-6, 0}}, -20LL);
    assert(g.num_of_edges() == 2 && copy.num_of_edges() == 3);
    check_state(g, Bounds{{4, 4}, {0, 0}, {-4, -4}}, 8LL);
    copy.set_supply(0, 1, 2), copy.set_supply(2, 1, 2);
    assert(!copy.flow(algorithm).has_value());
    copy.set_supply(0, 0, 6), copy.set_supply(2, -6, 0);
    assert(copy.flow(algorithm) == (-20LL));
  }
}

void interval_slope()
{
  mt19937_64 rng(817426);
  for (int trial = 0; trial < 180; trial++)
  {
    const int n = 2 + rng() % 4, m = rng() % 7;
    BF base(n);
    vc<InputEdge> edges;
    vc<ll> balance(n);
    for (int i = 0; i < m; i++)
    {
      const int u = rng() % n, v = rng() % n;
      const ll lower = ll(rng() % 3) - 1, upper = lower + rng() % 4, cost = ll(rng() % 9) - 4;
      edges.eb(u, v, lower, upper, cost), base.add_edge(u, v, lower, upper, cost);
      const ll f = lower + rng() % (upper - lower + 1);
      balance[u] += f, balance[v] -= f;
    }
    Bounds bounds(n);
    for (int v = 0; v < n; v++)
    {
      bounds[v] = {balance[v] - ll(rng() % 3), balance[v] + ll(rng() % 3)};
      base.add_supply(v, bounds[v].first, bounds[v].second);
    }
    const ll initial = *brute(bounds, edges);
    const A algorithm = A(trial % 5);
    assert(base.flow(algorithm) == (initial));
    for (H heap : {H::binary, H::fibonacci, H::dense})
    {
      auto g = base;
      assert(g.slope(0, n - 1, 0, heap) == (vc<pair<ll, ll>>{{0, 0}}));
      const auto curve = g.slope(0, n - 1, 8, heap);
      assert(curve.front() == make_pair(0LL, 0LL));
      for (ll x = 0; x <= 8; x++)
      {
        auto target = bounds;
        target[0].first += x, target[0].second += x;
        target[n - 1].first -= x, target[n - 1].second -= x;
        const auto expected = brute(target, edges);
        if (x > curve.back().first) { assert(!expected); continue; }
        assert(expected);
        if (x == 0) { assert(*expected == initial); continue; }
        for (int j = 1; j < int(curve.size()); j++) if (curve[j - 1].first < x && x <= curve[j].first)
        {
          auto [a, ca] = curve[j - 1];
          auto [b, cb] = curve[j];
          assert(initial + ca + (cb - ca) / (b - a) * (x - a) == *expected);
          break;
        }
      }
      auto target = bounds;
      const auto [sent, cost] = curve.back();
      target[0].first += sent, target[0].second += sent;
      target[n - 1].first -= sent, target[n - 1].second -= sent;
      check_state(g, target, initial + cost);
      assert(g.flow(n - 1, 0, sent, heap) == make_pair(sent, -cost));
      check_state(g, bounds, initial);
      assert(g.flow(algorithm, heap) == (initial));
      assert(g.slope(0, n - 1, 8, heap) == curve);
    }
  }
}

void explicit_i128()
{
  using G = MinCostBFlow<i128, i128>;
  const i128 amount = i128(1) << 80, unit_cost = i128(1) << 40;
  for (auto algorithm : {G::Algorithm::automatic, G::Algorithm::ssp, G::Algorithm::primal_dual, G::Algorithm::capacity_scaling, G::Algorithm::network_simplex})
    for (auto heap : {G::Heap::binary, G::Heap::fibonacci, G::Heap::dense})
    {
      G g(2);
      g.set_supply(0, amount, 2 * amount), g.add_supply(1, -2 * amount, -amount);
      g.add_edge(0, 1, 3 * amount, -unit_cost);
      const i128 expected = -2 * amount * unit_cost;
      assert(g.flow(algorithm, heap) == (expected));
      check_state(g, vc<pair<i128, i128>>{{amount, 2 * amount}, {-2 * amount, -amount}}, expected);
    }
}

void portfolio()
{
  constexpr int n = 400, paths = 80, t = paths + 1;
  const ll flow = 1000000000 + paths;
  BF base(n);
  base.set_supply(0, flow, flow + 1), base.set_supply(t, -flow - 1, -flow);
  for (int i = 0; i < paths; i++)
    base.add_edge(0, i + 1, 1, i + 1), base.add_edge(i + 1, t, 1, 0);
  base.add_edge(0, t, 1000000000, 1000000);
  base.add_edge(0, t + 1, numeric_limits<ll>::max(), 0);
  mt19937_64 rng(413582);
  for (int i = 0; i < 12000; i++)
  {
    const int u = t + 1 + rng() % (n - t - 1), v = t + 1 + rng() % (n - t - 1);
    base.add_edge(u, v, numeric_limits<ll>::max(), 0);
  }
  const ll expected = 1000000000LL * 1000000 + paths * (paths + 1) / 2;
  Bounds bounds(n);
  bounds[0] = {flow, flow + 1}, bounds[t] = {-flow - 1, -flow};
  for (A algorithm : {A::automatic, A::ssp, A::capacity_scaling})
    for (H heap : {H::automatic, H::binary, H::fibonacci})
    {
      auto g = base;
      assert(g.flow(algorithm, heap) == (expected));
      check_state(g, bounds, expected);
    }
}

int main()
{
  check({}, {});
  check({{1, 2}}, {});
  check({{-2, -1}}, {});
  check({{-1, 3}}, {});
  check({{1, 3}, {-3, -1}}, {{0, 1, 0, 5, 2}});
  check({{1, 3}, {-3, -1}}, {{0, 1, 0, 5, -2}});
  check({{-3, -1}, {1, 3}}, {{0, 1, -4, 2, 3}});
  check({{-1, 2}}, {{0, 0, -2, 3, -4}});
  check({{1, 2}, {-2, -1}}, {});
  small_random();
  mutations();
  interval_slope();
  explicit_i128();
  portfolio();
  cout << "Hello World\n";
}
