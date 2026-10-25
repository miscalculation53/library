#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/segtree/meldable_sparse_segtree.hpp"

struct StringConcatMonoid
{
  using S = string;
  static S op(const S &a, const S &b) { return a + b; }
  static S e() { return {}; }
};

template <class M>
void check(const MeldableSparseSegmentTreePool<M> &pool,
           const typename MeldableSparseSegmentTreePool<M>::Tree &seg, const map<ll, typename M::S> &expected)
{
  size_t before = pool.node_count();
  assert(seg.empty() == expected.empty());
  assert(seg.content() == expected);
  typename M::S total = M::e();
  for (const auto &[p, value] : expected)
  {
    assert(seg.contains(p));
    assert(seg.get(p) == value);
    total = M::op(total, value);
  }
  assert(seg.all_prod() == total);
  ll n = seg.universe_size();
  assert(seg.prod(0, n) == total);
  assert(seg.prod(0, 0) == M::e() && seg.prod(n, n) == M::e());
  if (n > 0)
    for (ll p : {0LL, n - 1, n / 2})
    {
      auto it = expected.find(p);
      assert(seg.contains(p) == (it != expected.end()));
      assert(seg.get(p) == (it == expected.end() ? M::e() : it->second));
    }
  assert(pool.node_count() == before);
}

void check_sum_search(const MeldableSparseSegmentTreePool<MonoidAdd<ll>> &pool,
                      const MeldableSparseSegmentTree<MonoidAdd<ll>> &seg,
                      const map<ll, ll> &expected, ll l, ll r, ll limit)
{
  ll sum = 0, want_r = seg.universe_size(), want_l = 0;
  for (auto it = expected.lower_bound(l); it != expected.end(); ++it)
  {
    sum += it->second;
    if (sum > limit) { want_r = it->first; break; }
  }
  sum = 0;
  auto it = expected.lower_bound(r);
  while (it != expected.begin())
  {
    --it;
    sum += it->second;
    if (sum > limit) { want_l = it->first + 1; break; }
  }
  auto pred = [&](ll value) { return value <= limit; };
  size_t before = pool.node_count();
  assert(seg.max_right_ok(l, pred) == want_r);
  assert(seg.min_left_ok(r, pred) == want_l);
  assert(pool.node_count() == before);
}

// Test focus: independent trees, point updates, deletion, both merge operations, and searches match maps.
void test_random()
{
  using Pool = MeldableSparseSegmentTreePool<MonoidAdd<ll>>;
  mt19937_64 rng(20261010);
  for (ll n : {0LL, 1LL, 2LL, 3LL, 7LL, 63LL, 64LL, 65LL, 129LL, 1LL << 40, numeric_limits<ll>::max()})
  {
    Pool pool(n);
    pool.reserve(1);
    constexpr int groups = 8;
    vc<Pool::Tree> segs;
    repi(i, groups) segs.eb(pool.make_tree());
    array<map<ll, ll>, groups> model;
    check(pool, segs[0], model[0]);
    check_sum_search(pool, segs[0], model[0], 0, n, 0);
    if (n == 0) continue;
    repi(iter, 3000)
    {
      int a = rng() % groups, b = rng() % groups, op = rng() % 20;
      ll p = rng() % ull(n);
      if (iter % 7 == 0) p = n - 1;
      if (iter % 11 == 0) p = 0;
      if (op < 5)
      {
        size_t before = pool.node_count();
        bool exists = model[a].count(p);
        ll value = rng() % 10;
        segs[a].set(p, value);
        model[a][p] = value;
        assert(pool.node_count() - before <= 64);
        if (exists) assert(pool.node_count() == before);
      }
      else if (op < 9)
      {
        ll old = model[a][p], add = rng() % 10;
        int calls = 0;
        segs[a].modify(p, [&](ll &value)
        {
          assert(value == old);
          calls++;
          value += add;
        });
        assert(calls == 1);
        model[a][p] += add;
      }
      else if (op < 11)
      {
        size_t before = pool.node_count();
        assert(segs[a].erase(p) == (model[a].erase(p) != 0));
        assert(pool.node_count() == before);
      }
      else if (op < 17)
      {
        size_t before = pool.node_count();
        if (op < 15)
        {
          segs[a].merge(segs[b]);
        }
        else
        {
          int calls = 0, overlaps = 0;
          if (a != b) for (auto [key, value] : model[b]) overlaps += model[a].count(key);
          auto combine = [&](ll x, ll y) { calls++; return max(x, y); };
          segs[a].merge(segs[b], combine);
          assert(calls == overlaps);
        }
        assert(pool.node_count() == before);
        if (a != b)
        {
          for (auto [key, value] : model[b])
          {
            if (op < 15) model[a][key] += value;
            else model[a][key] = max(model[a][key], value);
          }
          model[b].clear();
          assert(segs[b].empty());
        }
      }
      else if (op == 17)
      {
        segs[b] = segs[a].clone();
        model[b] = model[a];
      }
      else if (op == 18)
      {
        size_t before = pool.node_count();
        segs[a].clear();
        model[a].clear();
        assert(pool.node_count() == before);
      }
      else
        assert(segs[a].contains(p) == (model[a].count(p) != 0));

      ll l = rng() % (ull(n) + 1), r = rng() % (ull(n) + 1);
      if (l > r) swap(l, r);
      ll sum = 0;
      map<ll, ll> range;
      for (auto it = model[a].lower_bound(l); it != model[a].end() && it->first < r; ++it)
      {
        sum += it->second;
        range.insert(*it);
      }
      size_t before = pool.node_count();
      assert(segs[a].prod(l, r) == sum);
      map<ll, ll> actual;
      segs[a].enumerate(l, r, [&](ll key, ll value) { actual.emplace(key, value); });
      assert(actual == range && pool.node_count() == before);
      check_sum_search(pool, segs[a], model[a], l, r, rng() % 100);
      if (iter % 97 == 0) repi(i, groups) check(pool, segs[i], model[i]);
      if (iter % 1000 == 999)
      {
        pool.clear();
        assert(pool.node_count() == 0);
        for (auto &seg : segs) seg = pool.make_tree();
        for (auto &values : model) values.clear();
      }
    }
  }
}

// Test focus: range order, collision order, and both predicate search orders remain noncommutative.
void test_noncommutative()
{
  using Pool = MeldableSparseSegmentTreePool<StringConcatMonoid>;
  constexpr ll n = 17;
  Pool pool(n);
  auto a = pool.make_tree(), b = pool.make_tree(), c = pool.make_tree();
  map<ll, string> expected;
  rep(p, n)
  {
    if (p % 2 == 0)
    {
      string value = "A" + to_string(p) + ";";
      a.set(p, value);
      expected[p] += value;
    }
    if (p % 2 || p % 3 == 0)
    {
      string value = "B" + to_string(p) + ";";
      b.set(p, value);
      expected[p] += value;
    }
  }
  size_t before = pool.node_count();
  a.merge(b);
  assert(pool.node_count() == before && b.empty());
  check(pool, a, expected);
  rep(p, n) if (p % 3 == 0)
  {
    string value = "C" + to_string(p) + ";";
    c.set(p, value);
    expected[p] = value;
  }
  a.merge(c, [](const string &, const string &y) { return y; });
  check(pool, a, expected);
  rep(l, n + 1) rep(r, l, n + 1)
  {
    string target;
    rep(p, l, r) target += expected.at(p);
    assert(a.prod(l, r) == target);
    auto prefix = [&](const string &value)
    {
      return value.size() <= target.size() && equal(ALL(value), target.begin());
    };
    auto suffix = [&](const string &value)
    {
      return value.size() <= target.size() && equal(ALL(value), target.end() - value.size());
    };
    assert(a.max_right_ok(l, prefix) == r);
    assert(a.min_left_ok(r, suffix) == l);
  }
  auto copied = a.clone();
  for (auto [p, value] : expected) assert(copied.erase(p));
  check(pool, copied, map<ll, string>{});
  check(pool, a, expected);
}

// Test focus: identity-valued registrations, empty trees, self-merge, and signed endpoint limits.
void test_identity_and_extrema()
{
  using Pool = MeldableSparseSegmentTreePool<MonoidAdd<ll>>;
  ll n = numeric_limits<ll>::max();
  Pool pool(n);
  auto a = pool.make_tree(n - 1, 0), b = pool.make_tree();
  check(pool, a, map<ll, ll>{{n - 1, 0}});
  assert(!a.empty());
  assert(a.max_right_ok(0, [](ll x) { return x == 0; }) == n);
  assert(a.min_left_ok(n, [](ll x) { return x == 0; }) == 0);
  assert(a.erase(n - 1) && !a.erase(n - 1) && a.empty());
  a = pool.make_tree(0, 5);
  a.set(1, 7);
  b = pool.make_tree(0, 0);
  a.merge(b, [](ll, ll y) { return y; });
  check(pool, a, map<ll, ll>{{0, 0}, {1, 7}});
  a.clear();
  for (ll p : {0LL, 1LL, (1LL << 62) - 1, 1LL << 62, n - 1}) b.set(p, 1);
  size_t before = pool.node_count();
  a.merge(b);
  assert(pool.node_count() == before && b.empty());
  assert(a.max_right_ok(0, [](ll x) { return x <= 4; }) == n - 1);
  assert(a.min_left_ok(n, [](ll x) { return x == 0; }) == n);
  assert(a.max_right_ok(n, [](ll x) { return x == 0; }) == n);
  assert(a.min_left_ok(0, [](ll x) { return x == 0; }) == 0);
  assert(a.prod(n - 1, n) == 1);
  auto saved = a.content();
  a.merge(a);
  check(pool, a, saved);
  a.merge(b);
  check(pool, a, saved);
  pool.clear();
  a = pool.make_tree(n - 1, 7);
  check(pool, a, map<ll, ll>{{n - 1, 7}});
}

struct ValueWithoutDefaultOrEquality
{
  ll value;
  ValueWithoutDefaultOrEquality() = delete;
  explicit ValueWithoutDefaultOrEquality(ll value) : value(value) {}
};
struct MonoidWithoutDefaultOrEquality
{
  using S = ValueWithoutDefaultOrEquality;
  static S e() { return S(0); }
  static S op(const S &x, const S &y) { return S(x.value + y.value); }
};

// Test focus: sparse storage uses the monoid identity without requiring S() or equality.
void test_value_requirements()
{
  using Pool = MeldableSparseSegmentTreePool<MonoidWithoutDefaultOrEquality>;
  Pool pool(100);
  auto a = pool.make_tree(3, Pool::S(2)), b = pool.make_tree(3, Pool::S(4));
  b.set(70, Pool::S(5));
  a.merge(b);
  assert(a.all_prod().value == 11 && a.get(3).value == 6);
  assert(a.get(4).value == 0);
  auto c = a.clone();
  c.modify(3, [](auto &value) { value.value = 0; });
  assert(c.contains(3) && c.prod(0, 100).value == 5);
  auto values = c.content();
  assert(values.size() == 2 && values.at(3).value == 0);
  assert(a.max_right_ok(0, [](const auto &x) { return x.value <= 6; }) == 70);
  c.erase(3);
  c.erase(70);
  assert(c.empty());
}

// Test focus: moves and swaps transfer ownership, and pool resets allow fresh tree assignment.
void test_transfers()
{
  using Pool = MeldableSparseSegmentTreePool<MonoidAdd<ll>>;
  using Tree = MeldableSparseSegmentTree<MonoidAdd<ll>>;
  static_assert(is_same_v<Tree, Pool::Tree> && is_same_v<Tree::S, ll>);
  static_assert(!is_copy_constructible_v<Tree> && !is_copy_assignable_v<Tree>);
  static_assert(is_nothrow_move_constructible_v<Tree> && is_nothrow_move_assignable_v<Tree>);

  Pool p(5), q(1LL << 40);
  auto a = p.make_tree(2, 7), b = q.make_tree(1LL << 39, 11);
  size_t p_before = p.node_count(), q_before = q.node_count();
  swap(a, b);
  assert(a.universe_size() == q.universe_size() && b.universe_size() == p.universe_size());
  check(q, a, map<ll, ll>{{1LL << 39, 11}});
  check(p, b, map<ll, ll>{{2, 7}});
  a = std::move(b);
  check(p, a, map<ll, ll>{{2, 7}});
  check(p, b, map<ll, ll>{});
  auto moved = std::move(a);
  check(p, moved, map<ll, ll>{{2, 7}});
  check(p, a, map<ll, ll>{});
  auto &alias = moved;
  moved = std::move(alias);
  check(p, moved, map<ll, ll>{{2, 7}});
  assert(p.node_count() == p_before && q.node_count() == q_before);

  a.set(4, 9);
  moved.merge(a);
  check(p, moved, map<ll, ll>{{2, 7}, {4, 9}});
  check(p, a, map<ll, ll>{});
  auto copied = moved.clone();
  copied.erase(2);
  check(p, copied, map<ll, ll>{{4, 9}});
  check(p, moved, map<ll, ll>{{2, 7}, {4, 9}});
  p.clear();
  moved = p.make_tree(0, 3);
  a = p.make_tree();
  check(p, moved, map<ll, ll>{{0, 3}});
  check(p, a, map<ll, ll>{});
}

int main()
{
  test_random();
  test_noncommutative();
  test_identity_and_extrema();
  test_value_requirements();
  test_transfers();
  cout << "Hello World" << endl;
}
