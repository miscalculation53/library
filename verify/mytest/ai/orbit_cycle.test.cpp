#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/orbit_cycle.hpp"

struct Concatenate
{
  using S = string;
  static S e() { return {}; }
  static S op(S a, const S &b) { return a += b; }
};

struct WideSum
{
  using S = u128;
  static constexpr S e() { return 0; }
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S pow(S x, ll k) { return x * k; }
};

struct BoolXor
{
  using S = bool;
  static S e() { return false; }
  static S op(S a, S b) { return a != b; }
};

constexpr auto two_cycle = [](int x) { return 1 - x; };
constexpr auto fixed_point = [](int x) { return x; };
constexpr u128 wide = (u128(1) << 110) + 123;
static_assert(orbit_cycle(0, two_cycle).preperiod == 0);
static_assert(orbit_cycle(0, two_cycle).period == 2);
static_assert(orbit_cycle(0, two_cycle).reduce_index(i128(wide)) == 1);
static_assert(orbit_at(0, wide, two_cycle) == 1);
static_assert(orbit_prefix_prod<WideSum>(3, wide, fixed_point, [](int x) { return u128(x); })
              == 3 * wide);

void check(const vc<int> &f, int initial, int limit)
{
  vc<int> first(f.size(), -1), visited;
  int x = initial;
  while (first[x] < 0)
  {
    first[x] = visited.size();
    visited.push_back(x);
    x = f[x];
  }
  const ull preperiod = first[x], period = visited.size() - preperiod;
  auto next = [&](int state) { return f[state]; };
  const auto found = orbit_cycle(initial, next);
  assert(found.preperiod == preperiod && found.period == period);
  const Orbit orbit(initial, next);
  assert(orbit.cycle().preperiod == preperiod && orbit.cycle().period == period);
  assert(orbit.states() == visited);

  const int reduced = preperiod + (wide - preperiod) % period;
  assert(orbit_at(initial, wide, next) == visited[reduced]);
  assert(found.reduce_index(wide) == u128(reduced));
  assert(orbit[wide] == visited[reduced]);

  auto project = [](int state) { return to_string(state) + ','; };
  const OrbitPrefixProd<Concatenate> products(orbit, project);
  string expected;
  x = initial;
  for (int k = 0; k <= limit; ++k)
  {
    assert(orbit_at(initial, k, next) == x);
    assert(orbit[k] == x);
    assert(orbit_prefix_prod<Concatenate>(initial, k, next, project) == expected);
    assert(products.prod(k) == expected);
    expected += project(x);
    x = f[x];
  }
}

int main()
{
  for (int n = 1; n <= 5; ++n)
  {
    const int count = ipow<int>(n, n);
    for (int encoding = 0; encoding < count; ++encoding)
    {
      vc<int> f(n);
      int digits = encoding;
      for (int &x : f) { x = digits % n; digits /= n; }
      for (int s = 0; s < n; ++s) check(f, s, 3 * n + 1);
    }
  }
  mt19937 rng(613);
  for (int trial = 0; trial < 1000; ++trial)
  {
    const int n = 1 + rng() % 128;
    vc<int> f(n);
    for (int &x : f) x = rng() % n;
    check(f, rng() % n, rng() % 130);
  }

  ull calls = 0;
  auto increasing = [&](uint x) { ++calls; return x + 1; };
  assert(orbit_at(7u, 0, increasing) == 7 && calls == 0);
  ull comparisons = 0;
  auto compare = [&](uint a, uint b) { ++comparisons; return a == b; };
  assert(orbit_at(7u, 13, increasing, compare) == 20 && calls == 13 && comparisons == 12);
  calls = 0;
  auto to_wide = [](uint x) { return u128(x); };
  assert(orbit_prefix_prod<WideSum>(7u, 0, increasing, to_wide) == 0 && calls == 0);
  assert(orbit_prefix_prod<WideSum>(7u, 1, increasing, to_wide) == 7 && calls == 0);
  assert(orbit_prefix_prod<WideSum>(7u, 13, increasing, to_wide) == 169 && calls == 12);

  struct State
  {
    int value, steps;
    State(int value, int steps) : value(value), steps(steps) {}
  };
  auto next_state = [mod = make_unique<int>(7), &calls](const State &state) {
    ++calls;
    return State((state.value + 1) % *mod, state.steps + 1);
  };
  auto equal_state = [key = make_unique<bool>(true)](const State &a, const State &b) {
    return *key && a.value == b.value;
  };
  const State initial(3, 0);
  const auto cycle = orbit_cycle(initial, next_state, equal_state);
  assert(cycle.preperiod == 0 && cycle.period == 7);
  assert(orbit_at(initial, wide, next_state, equal_state).value == int((3 + wide) % 7));
  assert(orbit_prefix_prod<WideSum>(initial, 15, next_state,
      [](const State &state) { return u128(state.value); }, equal_state) == 45);

  const Orbit states(initial, next_state, equal_state);
  int projections = 0;
  auto project_state = [one = make_unique<int>(1), &projections](const State &state) {
    ++projections;
    return u128(state.value) * *one;
  };
  const OrbitPrefixProd<WideSum> direct(initial, next_state, project_state, equal_state);
  const auto saved_calls = calls;
  const OrbitPrefixProd<WideSum> reused(states, project_state);
  assert(projections == 14 && calls == saved_calls);
  assert(direct.cycle().preperiod == 0 && direct.cycle().period == 7);
  u128 expected_wide = (wide / 7) * 21;
  for (int i = 0; i < int(wide % 7); ++i) expected_wide += (3 + i) % 7;
  assert(reused.prod(wide) == expected_wide && direct.prod(i128(wide)) == expected_wide);
  auto copied = states;
  const auto moved = std::move(copied);
  assert(moved[wide].value == int((3 + wide) % 7));
  assert(projections == 14 && calls == saved_calls);

  auto negate_state = [](bool x) { return !x; };
  const Orbit alternating(false, negate_state);
  static_assert(is_same_v<decltype(alternating[0]), bool>);
  assert(!alternating[0] && alternating[wide]);
  const OrbitPrefixProd<BoolXor> bits(alternating);
  assert(!bits.prod(0) && bits.prod(3) && !bits.prod(4));

  cout << "Hello World\n";
}
