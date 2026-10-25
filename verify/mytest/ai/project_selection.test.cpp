#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/project_selection.hpp"

struct Term
{
  int kind, u, v;
  array<ll, 4> cost{};
  vc<int> group;
};

optional<ll> evaluate(unsigned mask, const vc<Term> &terms)
{
  ll cost = 0;
  for (const auto &t : terms)
  {
    const int x = mask >> t.u & 1, y = mask >> t.v & 1;
    if (t.kind == 0) cost += t.cost[x];
    if (t.kind == 1) cost += t.cost[2 * x + y];
    if (t.kind == 2 && x && !y) return nullopt;
    if (t.kind == 3 && x != t.v) return nullopt;
    if (t.kind == 4)
    {
      bool all = true;
      for (int v : t.group) all &= int(mask >> v & 1) == t.v;
      if (all) cost -= t.cost[0];
    }
    if (t.kind == 5) cost -= t.cost[x];
  }
  return cost;
}

int main()
{
  mt19937 rng(20261016);
  for (int trial = 0; trial < 1800; trial++)
  {
    const int n = 1 + rng() % 8, m = rng() % 25;
    ProjectSelection<ll> p(n);
    vc<Term> terms;
    for (int i = 0; i < m; i++)
    {
      int kind = rng() % 6, u = rng() % n, v = rng() % n;
      Term t{kind, u, v, {}, {}};
      for (auto &c : t.cost) c = int(rng() % 31) - 15;
      if (kind == 0) p.add_cost(u, t.cost[0], t.cost[1]);
      if (kind == 1)
      {
        t.cost[1] += max(0LL, t.cost[0] + t.cost[3] - t.cost[1] - t.cost[2]);
        p.add_pairwise_cost(u, v, t.cost[0], t.cost[1], t.cost[2], t.cost[3]);
      }
      if (kind == 2) p.imply(u, v);
      if (kind == 3) t.v &= 1, p.fix(u, t.v);
      if (kind == 4)
      {
        t.v &= 1, t.cost[0] = rng() % 21;
        const int count = rng() % (n + 2);
        for (int j = 0; j < count; j++) t.group.eb(rng() % n);
        p.add_all_profit(t.group, t.v, t.cost[0]);
      }
      if (kind == 5) p.add_profit(u, t.cost[0], t.cost[1]);
      terms.eb(t);
    }
    optional<ll> best;
    for (unsigned mask = 0; mask < 1U << n; mask++)
    {
      const auto value = evaluate(mask, terms);
      if (value && (!best || *value < *best)) best = value;
    }
    const auto cost = p.solve();
    assert(cost == best);
    if (cost)
    {
      const auto assignment = p.assignment();
      unsigned mask = 0;
      for (int v = 0; v < n; v++) if (assignment[v]) mask |= 1U << v;
      assert(evaluate(mask, terms) == best);
    }
    assert(p.solve() == cost);
  }
  {
    ProjectSelection<ll> p(4);
    for (int v = 0; v < 4; v++)
      p.add_cost(v, LLONG_MIN, LLONG_MAX), p.fix(v, v % 2);
    assert(p.solve() == (-2LL));
    assert((p.assignment() == vc<bool>{0, 1, 0, 1}));
  }
  {
    ProjectSelection<signed char> p(4);
    for (int v = 0; v < 4; v++) p.add_cost(v, -128, 127), p.fix(v, v % 2);
    const auto cost = p.solve();
    assert(cost && *cost == -2);
  }
  {
    // 劣モジュラ余裕が符号なし型の幅を越える場合。
    ProjectSelection<ll> p(2);
    p.add_pairwise_cost(0, 1, LLONG_MIN, LLONG_MAX, LLONG_MAX, LLONG_MIN);
    p.fix(0, 1), p.fix(1, 0);
    assert(p.solve() == (LLONG_MAX));
  }
  {
    ProjectSelection<i128> p(3);
    const i128 big = i128(1) << 90;
    p.add_profit(0, 0, big);
    p.add_cost(1, 0, big / 2);
    p.imply(0, 1);
    p.add_all_profit({1, 2}, true, big);
    assert(p.solve() == (-3 * big / 2));
    assert((p.assignment() == vc<bool>{1, 1, 1}));
  }
  {
    ProjectSelection<ll> p(2);
    p.add_disagreement_cost(0, 1, 7);
    p.add_constant(-2);
    p.add_penalty(0, 1, 3);
    p.fix(0, true);
    auto copy = p;
    p.fix(1, false);
    assert(p.solve() == (8LL));
    assert(copy.solve() == (-2LL));
    copy.add_cost(0, 0, 4);
    assert(copy.solve() == (2LL));
  }
  assert(ProjectSelection<int>(0).solve() == (0));
  cout << "Hello World\n";
}
