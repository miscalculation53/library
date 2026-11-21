#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/project_selection.hpp"

struct Term
{
  int kind, u, v;
  array<ll, 4> cost{};
  vc<int> group;
};

auto fixed_value(int value)
{
  return [value](int x) -> optional<int>
  {
    if (x == value) return 0;
    return nullopt;
  };
}

auto threshold_implication(int a, int b)
{
  return [=](int x, int y) -> optional<int>
  {
    if (x >= a && y < b) return nullopt;
    return 0;
  };
}

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
      cost += t.cost[all ? 0 : 1];
    }
    if (t.kind == 5) cost += t.cost[0];
  }
  return cost;
}

void test_binary()
{
  mt19937 rng(20261016);
  for (int trial = 0; trial < 1800; trial++)
  {
    const int n = 1 + rng() % 8, m = rng() % 25;
    ProjectSelectionProblem<ll> p(n);
    vc<Term> terms;
    for (int i = 0; i < m; i++)
    {
      int kind = rng() % 6, u = rng() % n, v = rng() % n;
      Term t{kind, u, v, {}, {}};
      for (auto &c : t.cost) c = int(rng() % 31) - 15;
      if (kind == 0) p.add1(u, [&](int x) { return t.cost[x]; });
      if (kind == 1)
      {
        t.cost[1] += max(0LL, t.cost[0] + t.cost[3] - t.cost[1] - t.cost[2]);
        p.add2(u, v, [&](int x, int y) { return t.cost[2 * x + y]; });
      }
      if (kind == 2) p.add2(u, v, 0, 0, nullopt, 0);
      if (kind == 3) t.v &= 1, p.add1(u, fixed_value(t.v));
      if (kind == 4)
      {
        t.v &= 1;
        if (t.cost[0] > t.cost[1]) swap(t.cost[0], t.cost[1]);
        const int count = rng() % (n + 2);
        for (int j = 0; j < count; j++) t.group.eb(rng() % n);
        if (t.v == 0) p.add_all0(t.group, t.cost[0], t.cost[1]);
        else p.add_all1(t.group, t.cost[0], t.cost[1]);
      }
      if (kind == 5) p.add_constant(t.cost[0]);
      terms.eb(t);
    }
    optional<ll> best;
    for (unsigned mask = 0; mask < 1U << n; mask++)
    {
      const auto value = evaluate(mask, terms);
      if (value && (!best || *value < *best)) best = value;
    }
    const auto solution = p.solve();
    assert(solution.has_value() == best.has_value());
    if (solution)
    {
      const auto &[cost, assignment] = *solution;
      assert(cost == *best && assignment.size() == size_t(n));
      unsigned mask = 0;
      for (int v = 0; v < n; v++) if (assignment[v]) mask |= 1U << v;
      assert(evaluate(mask, terms) == best);
    }
    assert(p.solve() == solution);
  }
  {
    ProjectSelectionProblem<ll> p(4);
    for (int v = 0; v < 4; v++)
      p.add1(v, LLONG_MIN, LLONG_MAX), p.add1(v, fixed_value(v % 2));
    assert((p.solve() == pair{-2LL, vc<int>{0, 1, 0, 1}}));
  }
  {
    ProjectSelectionProblem<signed char> p(4);
    for (int v = 0; v < 4; v++) p.add1(v, -128, 127), p.add1(v, fixed_value(v % 2));
    const auto solution = p.solve();
    assert(solution && solution->first == -2);
  }
  {
    // 劣モジュラ余裕が符号なし型の幅を越える場合。
    ProjectSelectionProblem<ll> p(2);
    p.add2(0, 1, LLONG_MIN, LLONG_MAX, LLONG_MAX, LLONG_MIN + 1);
    p.add1(0, nullopt, 0), p.add1(1, 0, nullopt);
    assert((p.solve() == pair{LLONG_MAX, vc<int>{1, 0}}));
  }
  {
    ProjectSelectionProblem<i128> p(3);
    const i128 big = i128(1) << 90;
    p.add1(0, 0, -big);
    p.add1(1, 0, big / 2);
    p.add2(0, 1, 0, 0, nullopt, 0);
    p.add_all1({1, 2}, -big, 0);
    assert((p.solve() == pair{-3 * big / 2, vc<int>{1, 1, 1}}));
  }
  {
    ProjectSelectionProblem<ll> p(2);
    p.add2(0, 1, 0, 7, 7, 0);
    p.add_constant(-2);
    p.add2(0, 1, 0, 0, 3, 0);
    p.add1(0, nullopt, 0);
    auto copy = p;
    p.add1(1, 0, nullopt);
    assert((p.solve() == pair{8LL, vc<int>{1, 0}}));
    assert((copy.solve() == pair{-2LL, vc<int>{1, 1}}));
    copy.add1(0, 0, 4);
    assert((copy.solve() == pair{2LL, vc<int>{1, 1}}));
    copy.add1(0, 0, nullopt);
    assert(!copy.solve());
  }
  ProjectSelectionProblem<int> empty;
  assert((empty.solve() == pair{0, vc<int>{}}));
  empty.add_all0({}, 2, 4);
  empty.add_all1({}, -3, -2);
  assert((empty.solve() == pair{-1, vc<int>{}}));
}

void test_forbidden_pairs()
{
  for (int allowed = 0; allowed < 16; allowed++)
  {
    if ((allowed & 6) == 6 && allowed != 15) continue;
    const array<ll, 4> cost{7, -4, 18, 3};
    auto f = [&](int x, int y) -> optional<ll>
    {
      const int index = 2 * x + y;
      if (allowed >> index & 1) return cost[index];
      return nullopt;
    };
    ProjectSelectionProblem<ll> p(2);
    if (allowed % 2 == 0) p.add2(0, 1, f(0, 0), f(0, 1), f(1, 0), f(1, 1));
    else p.add2(0, 1, f);
    for (int x = 0; x <= 1; x++) for (int y = 0; y <= 1; y++)
    {
      auto fixed = p;
      fixed.add1(0, fixed_value(x));
      fixed.add1(1, fixed_value(y));
      const auto value = f(x, y);
      const auto solution = fixed.solve();
      if (value) assert((solution == pair{*value, vc<int>{x, y}}));
      else assert(!solution);
    }
  }
  ProjectSelectionProblem<int> same(1);
  same.add2(0, 0, nullopt, -100, -100, 5);
  assert((same.solve() == pair{5, vc<int>{1}}));
  same.add2(0, 0, 0, 0, 0, nullopt);
  assert(!same.solve());
  ProjectSelectionProblem<int> forbidden(1);
  forbidden.add1(0, nullopt, nullopt);
  assert(!forbidden.solve());
}

void test_k_valued()
{
  mt19937 rng(20261121);
  for (int trial = 0; trial < 900; trial++)
  {
    const int n = 1 + rng() % 5;
    vc<int> ks(n);
    for (int &k : ks) k = 1 + rng() % 4;
    K_ProjectSelectionProblem<ll, 1000> p(n, [&](int i) { return ks[i]; });
    vvc<ll> unary(n);
    vc<tuple<int, int, vvc<ll>>> pairwise;
    for (int i = 0; i < n; i++)
    {
      unary[i].resize(ks[i]);
      for (ll &value : unary[i]) value = int(rng() % 21) - 10;
      p.add1(i, unary[i]);
    }
    const int m = rng() % 12;
    for (int e = 0; e < m; e++)
    {
      const int i = rng() % n, j = rng() % n;
      vvc<ll> f(ks[i], vc<ll>(ks[j]));
      for (int x = 0; x < ks[i]; x++)
        for (int y = 0; y < ks[j]; y++)
        {
          const int d = rng() % 9;
          if (x == 0 || y == 0 || i == j) f[x][y] = d - 4;
          else f[x][y] = f[x - 1][y] + f[x][y - 1] - f[x - 1][y - 1] - d;
        }
      if (e % 2 == 0) p.add2(i, j, f);
      else p.add2(i, j, [&](int x, int y) { return f[x][y]; });
      pairwise.emplace_back(i, j, move(f));
    }
    vc<array<int, 4>> implications;
    vc<pair<int, int>> fixed;
    const int constraints = rng() % 7;
    for (int c = 0; c < constraints; c++)
    {
      const int i = rng() % n;
      if (rng() % 3 == 0)
      {
        const int a = rng() % ks[i];
        p.add1(i, fixed_value(a));
        fixed.emplace_back(i, a);
      }
      else
      {
        const int j = rng() % n;
        const int a = rng() % (ks[i] + 1), b = rng() % (ks[j] + 1);
        p.add2(i, j, threshold_implication(a, b));
        implications.push_back({i, a, j, b});
      }
    }
    auto evaluate = [&](const vc<int> &x) -> optional<ll>
    {
      for (auto [i, a] : fixed) if (x[i] != a) return nullopt;
      for (auto [i, a, j, b] : implications) if (x[i] >= a && x[j] < b) return nullopt;
      ll value = 0;
      for (int i = 0; i < n; i++) value += unary[i][x[i]];
      for (const auto &[i, j, f] : pairwise) value += f[x[i]][x[j]];
      return value;
    };
    optional<ll> best;
    vc<int> x(n);
    auto enumerate = [&](auto &self, int i) -> void
    {
      if (i == n)
      {
        const auto value = evaluate(x);
        if (value && (!best || *value < *best)) best = value;
        return;
      }
      for (x[i] = 0; x[i] < ks[i]; x[i]++) self(self, i + 1);
    };
    enumerate(enumerate, 0);
    const auto solution = as_const(p).solve();
    assert(solution.has_value() == best.has_value());
    if (solution)
    {
      const auto &[value, assignment] = *solution;
      assert(value == *best && assignment.size() == size_t(n));
      for (int i = 0; i < n; i++) assert(0 <= assignment[i] && assignment[i] < ks[i]);
      assert(evaluate(assignment) == best);
    }
  }
  K_ProjectSelectionProblem<ll> empty;
  assert((empty.solve() == pair{0LL, vc<int>{}}));
  {
    K_ProjectSelectionProblem<int> p(3, {1, 3, 4});
    p.add2(0, 1, threshold_implication(0, 2));
    p.add2(1, 2, threshold_implication(2, 1));
    p.add2(2, 0, threshold_implication(3, 1));
    p.add1(2, {3, 2, 1, 0});
    assert((p.solve() == pair{1, vc<int>{0, 2, 2}}));
    p.add1(2, {0, nullopt, nullopt, nullopt});
    assert(!p.solve());
    K_ProjectSelectionProblem<int> contradiction(1, {1});
    contradiction.add2(0, 0, threshold_implication(0, 1));
    assert(!contradiction.solve());
  }
  {
    K_ProjectSelectionProblem<i128> p(2, {1, 3});
    const i128 big = i128(1) << 100;
    p.add1(1, [&](int x) { return x * big; });
    p.add2(0, 1, {{0, -2 * big, -4 * big}});
    assert((p.solve() == pair{-2 * big, vc<int>{0, 2}}));
    auto copy = p;
    p.add1(1, {0, 0, 4 * big});
    assert((p.solve() == pair{-big, vc<int>{0, 1}}));
    auto moved = move(copy);
    assert((moved.solve() == pair{-2 * big, vc<int>{0, 2}}));
  }
  {
    K_ProjectSelectionProblem<signed char> p(2, {2, 2});
    p.add2(0, 1, {{-120, 120}, {120, -120}});
    const auto solution = p.solve();
    assert(solution && solution->first == -120);
    const auto &x = solution->second;
    assert(x[0] == x[1]);
  }
  {
    const i128 h = i128(1) << 126;
    ProjectSelectionProblem<i128> binary(2);
    binary.add2(0, 1, h - 2, h + 1, h + 1, h);
    assert((binary.solve() == pair{h - 2, vc<int>{0, 0}}));
    K_ProjectSelectionProblem<i128> labeled(2, {2, 2});
    labeled.add2(0, 1, {{h - 2, h + 1}, {h + 1, h}});
    assert((labeled.solve() == pair{h - 2, vc<int>{0, 0}}));
  }
}

void test_partial_monge()
{
  constexpr int k = 3;
  const vvc<ll> cost{{7, -5, 6}, {10, -4, 3}, {2, -15, -9}};
  for (int mask = 0; mask < (1 << (k * k)); mask++)
  {
    vvc<optional<ll>> f(k, vc<optional<ll>>(k));
    for (int x = 0; x < k; x++) for (int y = 0; y < k; y++)
      if (mask >> (k * x + y) & 1) f[x][y] = cost[x][y];

    bool closed = true;
    for (int a = 0; a < k; a++) for (int b = 0; b < k; b++)
      for (int c = a + 1; c < k; c++) for (int d = b + 1; d < k; d++)
        if (f[a][d] && f[c][b] && (!f[a][b] || !f[c][d])) closed = false;
    if (!closed) continue;

    K_ProjectSelectionProblem<ll> p(2, {k, k});
    if (mask % 2 == 0) p.add2(0, 1, f);
    else p.add2(0, 1, [&](int a, int b) { return f[a][b]; });
    for (int a = 0; a < k; a++) for (int b = 0; b < k; b++)
    {
      auto copy = p;
      copy.add1(0, fixed_value(a));
      copy.add1(1, fixed_value(b));
      const auto solution = copy.solve();
      if (f[a][b]) assert((solution == pair{*f[a][b], vc<int>{a, b}}));
      else assert(!solution);
    }
  }
  K_ProjectSelectionProblem<ll> p(2, {2, 2});
  p.add2(0, 1, {{3, 5}, {nullopt, 2}});
  assert((p.solve() == pair{2LL, vc<int>{1, 1}}));
  p.add1(1, {nullopt, nullopt});
  assert(!p.solve());
}

int main()
{
  test_binary();
  test_forbidden_pairs();
  test_k_valued();
  test_partial_monge();
  cout << "Hello World\n";
}
