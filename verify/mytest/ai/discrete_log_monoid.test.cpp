#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/discrete_log_monoid.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "algebra/bit.hpp"
#include "math/modint/modint.hpp"

struct FunctionMonoid
{
  using S = vc<int>;
  static inline int degree = 1;
  static S op(const S &f, const S &g)
  {
    S h(degree);
    for (int i = 0; i < degree; i++) h[i] = f[g[i]];
    return h;
  }
  static S e()
  {
    S id(degree);
    iota(id.begin(), id.end(), 0);
    return id;
  }
};

struct ConstantHash
{
  template <class T>
  size_t operator()(const T &) const { return 0; }
};

optional<ll> walk(const vc<int> &f, int s, int t, ll l, ll r)
{
  for (ll k = 0; k < r; k++, s = f[s])
    if (k >= l && s == t) return k;
  return nullopt;
}

void exhaustive_functions()
{
  auto act = [](const vc<int> &f, int s) { return f[s]; };
  for (int n = 1; n <= 4; n++)
  {
    FunctionMonoid::degree = n;
    const int count = ipow<int>(n, n);
    vvc<int> functions;
    for (int code = 0; code < count; code++)
    {
      vc<int> f(n);
      int z = code;
      for (auto &x : f) x = z % n, z /= n;
      functions.push_back(f);
      for (int s = 0; s < n; s++)
        for (int t = 0; t < n; t++)
          for (ll r = 0; r <= 3 * n + 2; r++)
            for (ll l : {0LL, r / 2, r})
            {
              const auto expected = walk(f, s, t, l, r);
              assert(discrete_log_action<FunctionMonoid>(f, s, t, l, r, act) == expected);
              assert(discrete_log_action<FunctionMonoid>(f, s, t, l, r, act, ConstantHash{}) == expected);
            }
    }
    if (n <= 3)
      for (const auto &f : functions)
        for (const auto &g : functions)
          for (ll r = 0; r <= 2 * count; r++)
            for (ll l : {0LL, r / 3})
            {
              auto power = FunctionMonoid::e();
              optional<ll> expected;
              for (ll k = 0; k < r; k++, power = FunctionMonoid::op(f, power))
                if (k >= l && power == g) { expected = k; break; }
              assert(discrete_log_monoid<FunctionMonoid>(f, g, l, r) == expected);
            }
  }
}

void modular_powers()
{
  using mint = dynamic_modint32<718>;
  for (int modulus = 1; modulus <= 50; modulus++)
  {
    mint::set_mod(modulus);
    for (int a = 0; a < modulus; a++)
      for (int b = 0; b < modulus; b++)
        for (ll r : {0LL, 1LL, ll(modulus / 2), ll(modulus), ll(2 * modulus + 1)})
          for (ll l : {0LL, r / 2})
          {
            optional<ll> expected;
            int x = 1 % modulus;
            for (ll k = 0; k < r; k++, x = x * a % modulus)
              if (k >= l && x == b) { expected = k; break; }
            assert(discrete_log_monoid<MonoidMul<mint>>(mint(a), mint(b), l, r) == expected);
          }
  }
}

void monoid_orders()
{
  using mint = dynamic_modint32<718>;
  using M = MonoidMul<mint>;
  for (int modulus = 1; modulus <= 64; ++modulus)
  {
    mint::set_mod(modulus);
    const int identity = 1 % modulus;
    for (int a = 0; a < modulus; ++a)
    {
      optional<ll> expected;
      int x = identity;
      for (ll k = 1; k <= modulus; ++k)
      {
        x = x * a % modulus;
        if (x == identity)
        {
          expected = k;
          break;
        }
      }
      assert(discrete_log_monoid<M>(mint(a), M::e(), 1, 1) == nullopt);
      assert(discrete_log_monoid<M>(mint(a), M::e(), 1, modulus + 1) == expected);
      if (expected)
      {
        assert(discrete_log_monoid<M>(mint(a), M::e(), 1, *expected + 1) == expected);
        assert(discrete_log_monoid<M>(mint(a), M::e(), 1, *expected) == nullopt);
      }
    }
  }
}

void random_functions()
{
  mt19937_64 rng(20261028);
  for (int trial = 0; trial < 3000; trial++)
  {
    const int n = 1 + rng() % 80;
    FunctionMonoid::degree = n;
    vc<int> f(n);
    for (auto &v : f) v = rng() % n;
    const int s = rng() % n, t = rng() % n;
    const ll l = rng() % (4 * n + 1), r = l + rng() % (4 * n + 1);
    assert(discrete_log_action<FunctionMonoid>(f, s, t, l, r,
        [](const vc<int> &g, int v) { return g[v]; }) == walk(f, s, t, l, r));
  }
}

// 0,...,tail-1 の道の後、tail,...,tail+period-1 の周期に入る。
struct OrbitAction
{
  ll tail, period, &calls;
  ll operator()(ll step, ll x)
  {
    calls++;
    if (x < tail)
    {
      if (step < tail - x) return x + step;
      step -= tail - x;
      x = tail;
    }
    return tail + (x - tail + step) % period;
  }
};

void orbit_boundaries()
{
  auto check = [](ll tail, ll period, ll s, ll t, ll l, ll r, optional<ll> expected)
  {
    ll calls = 0;
    OrbitAction action{tail, period, calls};
    assert(discrete_log_action<MonoidAdd<ll>>(1, s, t, l, r, action) == expected);
    const ll width = r - l, block = iroot<ll>(max(1LL, width) - 1, 2) + 1;
    assert(calls <= (l != 0) + 3 * block + (width + block - 1) / block);
    if (l == r) assert(calls == 0);
  };
  // 初回の一致が道の合流で空振りし、後の一致で解が得られる。
  check(100, 1000, 0, 1099, 0, 10000, 1099);
  // t が道にあり、合流後の短い周期で一致が繰り返される。
  check(2, 1, 1, 0, 0, 100000000, nullopt);
  check(1000, 7000, 0, 999, 0, 999, nullopt);
  check(1000, 7000, 0, 999, 0, 1000, 999);
  check(0, 10000019, 0, 9999999, 0, 10000000, 9999999);
  check(0, 10000019, 0, 10000000, 0, 10000000, nullopt);
  check(0, 1, 0, 0, 0, 0, nullopt);
  check(0, 1, 0, 0, 0, 1, 0);
  const ll l = 1000000000000000000LL;
  check(0, 7, 0, 1, l, l, nullopt);
  check(0, 7, 0, 1, l, l + 1, l);
  check(0, 7, 0, 3, l, l + 2, nullopt);
  check(0, 7, 0, 3, l, l + 3, l + 2);
}

struct State
{
  ll value;
  State() = delete;
  explicit State(ll value) : value(value) {}
};
struct StateHash
{
  size_t salt;
  StateHash() = delete;
  explicit StateHash(size_t salt) : salt(salt) {}
  size_t operator()(const State &s) const { return size_t(s.value) ^ salt; }
};
struct StateEqual
{
  StateEqual() = delete;
  explicit StateEqual(int) {}
  bool operator()(const State &s, const State &t) const { return s.value == t.value; }
};
struct MoveOnlyAction
{
  unique_ptr<ll> calls = make_unique<ll>(0);
  State operator()(const ll &step, const State &s)
  {
    ++*calls;
    return State(s.value + step);
  }
  State operator()(ll &&, const State &) = delete;
};

struct ResidueHash
{
  ll modulus;
  explicit ResidueHash(ll modulus) : modulus(modulus) {}
  size_t operator()(ll x) const { return safe_hash::splitmix64(x % modulus); }
};
struct ResidueEqual
{
  ll modulus;
  explicit ResidueEqual(ll modulus) : modulus(modulus) {}
  bool operator()(ll x, ll y) const { return x % modulus == y % modulus; }
};

struct MoveAssignedState
{
  ll value = 0;
  MoveAssignedState() = default;
  explicit MoveAssignedState(ll value) : value(value) {}
  MoveAssignedState(const MoveAssignedState &) = default;
  MoveAssignedState(MoveAssignedState &&) = default;
  MoveAssignedState &operator=(const MoveAssignedState &) = delete;
  MoveAssignedState &operator=(MoveAssignedState &&) = default;
  friend bool operator==(const MoveAssignedState &a, const MoveAssignedState &b) { return a.value == b.value; }
  friend ll safe_hash_key(const MoveAssignedState &s) { return s.value; }
};

void type_contracts()
{
  assert(discrete_log_monoid<MonoidAnd<bool>>(false, false, 0, 10) == 1);
  assert(discrete_log_monoid<MonoidAnd<bool>>(true, false, 0, 10) == nullopt);
  assert(discrete_log_monoid<MonoidAnd<bool>>(false, true, 1, 11) == nullopt);
  assert(discrete_log_monoid<MonoidAnd<bool>>(true, true, 1, 2) == 1);
  assert(discrete_log_action<MonoidAdd<ll>>(3, MoveAssignedState(4), MoveAssignedState(37), 5, 30,
      [](ll step, const MoveAssignedState &s) { return MoveAssignedState(s.value + step); }) == 11);
  // デフォルト構築できる状態でも、指定したハッシュ・等値関係を使う。
  assert(discrete_log_action<MonoidAdd<ll>>(3, 4LL, 138LL, 0, 30,
      [](ll step, ll x) { return (x + step) % 101; }, ResidueHash(101), ResidueEqual(101)) == 11);
  assert(discrete_log_monoid<MonoidAdd<ll>>(3, 0, 1, 102, ResidueHash(101), ResidueEqual(101)) == 101);
  assert(discrete_log_monoid<MonoidAdd<ll>>(3, 0, 1, 101, ResidueHash(101), ResidueEqual(101)) == nullopt);
  MoveOnlyAction act;
  const ll a = 3;
  const State s(7), t(40);
  static_assert(is_same_v<decltype(discrete_log_action<MonoidAdd<ll>>(
      a, s, t, 8, 20, act, StateHash(17), StateEqual(0))), optional<ll>>);
  assert(discrete_log_action<MonoidAdd<ll>>(a, s, t, 8, 20, act, StateHash(17), StateEqual(0)) == 11);
  assert(*act.calls > 0);
  assert(s.value == 7 && t.value == 40 && a == 3);
  assert(discrete_log_action<MonoidAdd<ll>>(a, s, s, 0, 1, MoveOnlyAction{}, StateHash(17), StateEqual(0)) == 0);
  // a, s, t に同じオブジェクトを渡しても入力の値を維持する。
  const ll shared = 9;
  assert(discrete_log_action<MonoidAdd<ll>>(shared, shared, shared, 0, 10,
      [](ll g, ll x) { return g + x; }) == 0);
  assert(discrete_log_action<MonoidAdd<ll>>(shared, shared, shared, 1, 10,
      [](ll g, ll x) { return g + x; }) == nullopt);
  assert(shared == 9);
  const i128 large = i128(1) << 80;
  assert(discrete_log_monoid<MonoidAdd<i128>>(large, large * 511, 500, 1000) == 511);
  const u128 unsigned_large = u128(1) << 90;
  assert(discrete_log_monoid<MonoidAdd<u128>>(unsigned_large, unsigned_large * 731, 700, 1000) == 731);
  assert(discrete_log_monoid<MonoidAdd<short>>(short(2), short(30), 12, 100) == 15);
  assert(discrete_log_monoid<MonoidAdd<unsigned>>(7, 0, 0, 100) == 0);
  assert(discrete_log_monoid<MonoidAdd<ll>>(0, 1, 0, 100) == nullopt);
}

int main()
{
  exhaustive_functions();
  modular_powers();
  monoid_orders();
  random_functions();
  orbit_boundaries();
  type_contracts();
  cout << "Hello World\n";
}
