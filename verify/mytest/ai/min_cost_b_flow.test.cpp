#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/min_cost_b_flow.hpp"
#include <atcoder/mincostflow>

using BF = MinCostBFlow<ll, ll>;
using A = BF::Algorithm;
using H = BF::Heap;
using InputEdge = tuple<int, int, ll, ll, ll>;

optional<i128> brute(const vc<ll> &b, const vc<InputEdge> &es)
{
  optional<i128> best;
  vc<ll> balance(b.size());
  auto dfs = [&](auto &self, int i, i128 cost) -> void
  {
    if (i == int(es.size()))
    {
      if (balance == b && (!best || cost < *best)) best = cost;
      return;
    }
    const auto [u, v, l, r, c] = es[i];
    for (ll f = l; f <= r; f++)
    {
      balance[u] += f, balance[v] -= f;
      self(self, i + 1, cost + i128(c) * f);
      balance[u] -= f, balance[v] += f;
    }
  };
  dfs(dfs, 0, 0);
  return best;
}

template <class Cap, class Cost, class Total, class Expected>
void check_state(const MinCostBFlow<Cap, Cost, Total> &g, const vc<i128> &b, Expected expected)
{
  const auto potential = g.potential();
  assert(potential == g.potential());
  vc<i128> balance(b.size());
  Total value = 0;
  auto es = g.edges();
  for (int i = 0; i < int(es.size()); i++)
  {
    const auto e = g.get_edge(i);
    assert(e.from == es[i].from && e.to == es[i].to && e.flow == es[i].flow);
    assert(e.lower <= e.flow && e.flow <= e.upper);
    balance[e.from] += i128(e.flow), balance[e.to] -= i128(e.flow);
    const i128 reduced = i128(e.cost) + potential[e.from] - potential[e.to];
    if (e.flow < e.upper) assert(reduced >= 0);
    if (e.flow > e.lower) assert(reduced <= 0);
    value += Total(e.flow) * Total(e.cost);
  }
  assert(balance == b && value == expected);
}

void check(const vc<ll> &b, const vc<InputEdge> &es, optional<i128> expected)
{
  for (A algorithm : {A::automatic, A::ssp, A::primal_dual, A::capacity_scaling, A::network_simplex})
    for (H heap : {H::automatic, H::binary, H::fibonacci, H::dense})
    {
      BF g(b);
      for (const auto &[u, v, l, r, c] : es)
      {
        const int id = g.add_edge(u, v, l, r, c);
        assert(id + 1 == g.num_of_edges());
      }
      const auto cost = g.flow(algorithm, heap);
      assert(cost.has_value() == expected.has_value());
      if (cost)
      {
        assert(*cost == *expected);
        check_state(g, vc<i128>(ALL(b)), *cost);
      }
    }
}

void small_random()
{
  mt19937_64 rng(20171015);
  for (int trial = 0; trial < 1800; trial++)
  {
    const int n = 1 + rng() % 6, m = rng() % 8;
    vc<InputEdge> es;
    vc<ll> b(n);
    for (int i = 0; i < m; i++)
    {
      int u = rng() % n, v = rng() % n;
      ll l = ll(rng() % 5) - 2, r = l + rng() % 4, c = ll(rng() % 11) - 5;
      es.eb(u, v, l, r, c);
      ll flow = l + rng() % (r - l + 1);
      b[u] += flow, b[v] -= flow;
    }
    if (trial % 3 == 0) b[rng() % n]++;
    if (trial % 3 == 1 && n > 1) b[0] += 3, b[n - 1] -= 3;
    check(b, es, brute(b, es));
  }
}

void st_slope()
{
  mt19937_64 rng(413910);
  for (int trial = 0; trial < 250; trial++)
  {
    const int n = 2 + rng() % 40, m = rng() % 250;
    vc<InputEdge> es;
    for (int i = 0; i < m; i++) es.eb(rng() % n, rng() % n, 0, rng() % 10, rng() % 31);
    for (H heap : {H::binary, H::fibonacci, H::dense})
    {
      BF g(n);
      atcoder::mcf_graph<ll, ll> reference(n);
      for (auto [u, v, l, r, c] : es) g.add_edge(u, v, l, r, c), reference.add_edge(u, v, r, c);
      assert(g.flow(trial % 2 == 0 ? A::network_simplex : A::automatic).has_value());
      const ll limit = trial % 2 == 0 ? ll(rng() % 50) : numeric_limits<ll>::max();
      const auto expected = reference.slope(0, n - 1, limit);
      const auto result = g.slope(0, n - 1, limit, heap);
      assert(result.size() == expected.size());
      for (int i = 0; i < int(result.size()); i++)
        assert(result[i].first == expected[i].first && result[i].second == expected[i].second);
      vc<i128> b(n);
      b[0] = result.back().first, b[n - 1] = -b[0];
      check_state(g, b, result.back().second);
      auto backward = g.flow(n - 1, 0, result.back().first, heap);
      assert(backward.first == result.back().first && backward.second == -result.back().second);
      check_state(g, vc<i128>(n), i128(0));
      const auto second = g.slope(0, n - 1, limit, heap);
      assert(second == result);
    }
  }
}

void general_slope()
{
  // 循環と上下限を含む基準解から供給を変え、各整数流量で全探索と比較する。
  mt19937_64 rng(318509);
  for (int trial = 0; trial < 200; trial++)
  {
    const int n = 2 + rng() % 4, m = 1 + rng() % 6;
    vc<InputEdge> es;
    vc<ll> b(n);
    BF base(n);
    for (int i = 0; i < m; i++)
    {
      const int u = rng() % n, v = rng() % n;
      const ll l = ll(rng() % 3) - 1, r = l + rng() % 4, c = ll(rng() % 9) - 4;
      es.eb(u, v, l, r, c), base.add_edge(u, v, l, r, c);
      ll f = l + rng() % (r - l + 1);
      b[u] += f, b[v] -= f;
    }
    for (int v = 0; v < n; v++) base.add_supply(v, b[v]);
    const auto initial = brute(b, es);
    assert(initial);
    assert(base.flow(trial % 2 == 0 ? A::network_simplex : A::automatic).value() == *initial);
    for (H heap : {H::binary, H::fibonacci, H::dense})
    {
      auto g = base;
      const auto slope = g.slope(0, n - 1, 10, heap);
      assert(slope.front() == make_pair(0LL, 0LL));
      for (ll x = 0; x <= 10; x++)
      {
        auto target = b;
        target[0] += x, target[n - 1] -= x;
        const auto optimum = brute(target, es);
        if (x > slope.back().first) { assert(!optimum); continue; }
        assert(optimum);
        for (int j = 1; j < int(slope.size()); j++) if (slope[j - 1].first <= x && x <= slope[j].first)
        {
          auto [a, ca] = slope[j - 1];
          auto [z, cz] = slope[j];
          assert(*initial + ca + (cz - ca) / (z - a) * (x - a) == *optimum);
          break;
        }
      }
    }
  }
}

template <class Cap, class Cost, class StorageCap, class StorageCost>
void integer_limits()
{
  using G = MinCostBFlow<StorageCap, StorageCost, i128>;
  const Cap lo = numeric_limits<Cap>::min(), hi = numeric_limits<Cap>::max();
  const Cost low_cost = numeric_limits<Cost>::min(), high_cost = numeric_limits<Cost>::max();
  for (auto algorithm : {G::Algorithm::automatic, G::Algorithm::ssp, G::Algorithm::primal_dual, G::Algorithm::capacity_scaling, G::Algorithm::network_simplex})
    for (auto heap : {G::Heap::binary, G::Heap::fibonacci, G::Heap::dense})
    {
      G g(2);
      g.add_edge(0, 1, lo, hi, low_cost);
      g.add_edge(1, 0, lo, hi, Cost(0));
      g.add_edge(0, 0, lo, hi, high_cost);
      const auto cost = g.flow(algorithm, heap);
      const i128 expected = i128(hi) * low_cost + i128(lo) * high_cost;
      assert(cost && *cost == expected);
      check_state(g, vc<i128>(2), *cost);
      // 入力型の端点を使い、容量幅・距離を保持できる型を明示する。
      assert(g.get_edge(0).flow == hi && g.get_edge(1).flow == hi && g.get_edge(2).flow == lo);
    }
}

void cancellation_overflow()
{
  const ll hi = numeric_limits<ll>::max(), q = hi / 2;
  BF g(1);
  for (int i = 0; i < 6; i++) g.add_edge(0, 0, hi, hi, i < 3 ? hi : -hi);
  assert(g.flow().value() == 0);
  MinCostBFlow<int, int> narrow(1);
  const int narrow_hi = numeric_limits<int>::max();
  for (int i = 0; i < 6; i++) narrow.add_edge(0, 0, narrow_hi, narrow_hi, i < 3 ? narrow_hi : -narrow_hi);
  assert(narrow.flow().value() == 0);
  using Wide = MinCostBFlow<ll, i128>;
  for (auto heap : {Wide::Heap::binary, Wide::Heap::fibonacci, Wide::Heap::dense})
  {
    Wide h(8);
    h.add_edge(0, 1, q, -hi), h.add_edge(1, 2, q, -hi), h.add_edge(2, 7, q, -hi);
    h.add_edge(0, 3, q, hi), h.add_edge(3, 4, q, hi), h.add_edge(4, 5, q, hi);
    h.add_edge(5, 6, q, hi), h.add_edge(6, 7, q, hi);
    assert(h.flow().value() == 0);
    const auto result = h.slope(0, 7, 2 * q, heap);
    assert(result.size() == 3);
    assert(result[1] == make_pair(q, -i128(q) * hi * 3));
    assert(result[2] == make_pair(2 * q, i128(q) * hi * 2));
  }
}

template <class Integer>
void declared_types()
{
  using G = MinCostBFlow<Integer, Integer>;
  static_assert(is_same_v<typename G::Balance, Integer>);
  static_assert(is_same_v<typename G::Potential, Integer>);
  static_assert(is_same_v<decltype(declval<G>().flow()), optional<Integer>>);
  for (auto algorithm : {G::Algorithm::automatic, G::Algorithm::ssp, G::Algorithm::primal_dual, G::Algorithm::capacity_scaling, G::Algorithm::network_simplex})
    for (auto heap : {G::Heap::binary, G::Heap::fibonacci, G::Heap::dense})
    {
      G g(vc<Integer>{3, 0, -3});
      g.add_edge(0, 1, 4, 2), g.add_edge(1, 2, 4, 3);
      assert(g.flow(algorithm, heap) == (Integer(15)));
      assert(g.flow(0, 2, 5, heap) == make_pair(Integer(1), Integer(5)));
      assert(g.flow(2, 0, 4, heap) == make_pair(Integer(4), Integer(-20)));
      check_state(g, vc<i128>(3), Integer(0));
    }
}

void explicit_i128()
{
  using G = MinCostBFlow<i128, i128>;
  for (auto [capacity_bits, cost_bits] : {pair{80, 40}, pair{40, 80}})
  {
    const i128 amount = i128(1) << capacity_bits, unit = i128(1) << cost_bits;
    for (auto algorithm : {G::Algorithm::automatic, G::Algorithm::ssp, G::Algorithm::primal_dual, G::Algorithm::capacity_scaling, G::Algorithm::network_simplex})
      for (auto heap : {G::Heap::binary, G::Heap::fibonacci, G::Heap::dense})
      {
        G g(vc<i128>{amount, 0, 0, -amount});
        g.add_edge(0, 1, -amount, amount, -unit);
        g.add_edge(1, 3, -amount, amount, 3 * unit);
        g.add_edge(0, 2, amount, 4 * unit), g.add_edge(2, 3, amount, 0);
        const i128 cost = 2 * unit * amount;
        assert(g.flow(algorithm, heap) == (cost));
        check_state(g, vc<i128>{amount, 0, 0, -amount}, cost);
        assert(g.flow(3, 0, amount, heap) == make_pair(amount, -cost));
        check_state(g, vc<i128>(4), i128(0));
      }
  }
}

void portfolio_paths()
{
  constexpr int paths = 50, n = 1000, t = paths + 1;
  vc<ll> b(n);
  b[0] = 1000000000 + paths, b[t] = -b[0];
  BF base(b);
  for (int i = 0; i < paths; i++)
    base.add_edge(0, i + 1, 1, i + 1), base.add_edge(i + 1, t, 1, 0);
  base.add_edge(0, t, 1000000000, 1000000);
  base.add_edge(0, t + 1, numeric_limits<ll>::max(), 0);
  for (int v = t + 1; v < n; v++) base.add_edge(v, v + 1 == n ? t + 1 : v + 1, numeric_limits<ll>::max(), 0);
  mt19937_64 rng(918427);
  for (int i = 0; i < 20000; i++)
  {
    const int u = t + 1 + rng() % (n - t - 1), v = t + 1 + rng() % (n - t - 1);
    base.add_edge(u, v, numeric_limits<ll>::max(), 0);
  }
  const ll expected = 1000000000LL * 1000000 + paths * (paths + 1) / 2;
  for (H heap : {H::automatic, H::binary, H::fibonacci}) for (A algorithm : {A::automatic, A::ssp, A::capacity_scaling})
  {
    auto g = base;
    assert(g.flow(algorithm, heap) == (expected));
    check_state(g, vc<i128>(ALL(b)), expected);
  }
}

int main()
{
  check({}, {}, i128(0));
  check({1}, {}, nullopt);
  check({0}, {{0, 0, -3, 7, -5}}, i128(-35));
  check({0, 0}, {{0, 1, 0, 5, -3}, {1, 0, 0, 4, 1}}, i128(-8));
  small_random();
  st_slope();
  general_slope();
  integer_limits<signed char, signed char, int, int>();
  integer_limits<short, short, int, int>();
  integer_limits<int, int, ll, ll>();
  integer_limits<ll, ll, i128, i128>();
  integer_limits<ll, int, i128, ll>();
  cancellation_overflow();
  declared_types<signed char>();
  declared_types<short>();
  declared_types<int>();
  declared_types<ll>();
  declared_types<i128>();
  explicit_i128();
  portfolio_paths();
  cout << "Hello World\n";
}
