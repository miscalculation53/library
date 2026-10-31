#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/meldable_integer_set.hpp"

void check(const MeldableIntegerSet &st, const set<ll> &expected)
{
  assert(st.size() == ll(expected.size()));
  assert(st.empty() == expected.empty());
  assert(st.content() == vc<ll>(ALL(expected)));
  assert(vc<ll>(st.begin(), st.end()) == vc<ll>(ALL(expected)));
  assert(vc<ll>(st.cbegin(), st.cend()) == vc<ll>(ALL(expected)));
  ll n = st.universe_size(), k = 0;
  for (ll key : expected) assert(st.contains(key) && st.kth(k++) == key);
  for (ll key : {numeric_limits<ll>::min(), -1LL, 0LL, 1LL, 63LL, 64LL, 65LL, n - 1, n, numeric_limits<ll>::max()})
  {
    auto lo = expected.lower_bound(key), hi = expected.upper_bound(key);
    ll below = distance(expected.begin(), lo), through = distance(expected.begin(), hi);
    assert(st.lt_cnt(key) == below && st.leq_cnt(key) == through);
    assert(st.geq_cnt(key) == ll(expected.size()) - below);
    assert(st.gt_cnt(key) == ll(expected.size()) - through);
    assert(st.geq_min(key) == (lo == expected.end() ? n : *lo));
    assert(st.leq_max(key) == (hi == expected.begin() ? -1 : *prev(hi)));
    assert(st.gt_min(key) == (hi == expected.end() ? n : *hi));
    assert(st.lt_max(key) == (lo == expected.begin() ? -1 : *prev(lo)));
    if (0 <= key && key < n) assert(st.contains(key) == (expected.count(key) != 0));
  }
  assert(st.min_element() == (expected.empty() ? n : *expected.begin()));
  assert(st.max_element() == (expected.empty() ? -1 : *expected.rbegin()));
  assert(st.count(0, n) == st.size());
}

// Test focus: randomized independent forests, destructive union, cloning, and pruning match std::set.
void test_random()
{
  mt19937_64 rng(375664);
  for (ll n : {0LL, 1LL, 63LL, 64LL, 65LL, 127LL, 128LL, 129LL, 4095LL, 4096LL, 4097LL, 200000LL})
  {
    MeldableIntegerSetPool sets(n);
    sets.reserve(100);
    constexpr int groups = 12;
    vc<MeldableIntegerSet> handles;
    handles.reserve(groups);
    repi(i, groups) handles.eb(sets.make_set());
    array<set<ll>, groups> expected;
    check(handles[0], expected[0]);
    if (n == 0) continue;
    repi(iter, 10000)
    {
      int a = rng() % groups, b = rng() % groups, op = rng() % 16;
      ll key = rng() % ull(n);
      if (op < 6)
        assert(handles[a].insert(key) == expected[a].insert(key).second);
      else if (op < 9)
        assert(handles[a].erase(key) == (expected[a].erase(key) != 0));
      else if (op < 13)
      {
        size_t before = sets.node_count();
        handles[a].merge(handles[b]);
        assert(sets.node_count() == before);
        if (a != b)
        {
          expected[a].insert(ALL(expected[b]));
          expected[b].clear();
          assert(handles[b].empty());
        }
      }
      else if (op == 13)
      {
        handles[b] = handles[a].clone();
        expected[b] = expected[a];
      }
      else if (op == 14)
      {
        size_t before = sets.node_count();
        handles[a].clear();
        expected[a].clear();
        assert(sets.node_count() == before);
      }
      else
        assert(handles[a].contains(key) == (expected[a].count(key) != 0));

      ll l = rng() % (ull(n) + 1), r = rng() % (ull(n) + 1);
      if (l > r) swap(l, r);
      vc<ll> actual;
      handles[a].enumerate(l, r, [&](ll x) { actual.eb(x); });
      vc<ll> wanted(expected[a].lower_bound(l), expected[a].lower_bound(r));
      assert(actual == wanted && handles[a].count(l, r) == ll(wanted.size()));
      if (iter % 97 == 0)
        repi(i, groups) check(handles[i], expected[i]);
      if (iter % 2000 == 1999)
      {
        sets.clear();
        assert(sets.node_count() == 0);
        for (auto &st : handles) st = sets.make_set();
        for (auto &st : expected) st.clear();
      }
    }
    repi(i, groups) check(handles[i], expected[i]);
  }
}

// Test focus: every leaf position and word-select rank, including duplicate keys and empty handles.
void test_words()
{
  mt19937_64 rng(123456789);
  MeldableIntegerSetPool sets(129, 1);
  repi(iter, 200)
  {
    auto a = sets.make_set(), b = sets.make_set();
    set<ll> expected;
    ull mask = rng();
    repi(bit, 64) if ((mask >> bit) & 1)
    {
      a.insert(bit);
      expected.insert(bit);
    }
    for (ll key : {0LL, 63LL, 64LL, 127LL, 128LL})
    {
      b.insert(key);
      expected.insert(key);
    }
    a.merge(b);
    check(a, expected);
    auto copy = a.clone();
    for (ll key : expected) assert(copy.erase(key));
    check(copy, {});
    check(a, expected);
    auto it = a.begin(), saved = it;
    assert(it++ == saved && *saved.operator->() == *expected.begin());
    assert(++saved == it);
    sets.clear();
  }
  using Iter = MeldableIntegerSet::iterator;
  static_assert(is_same_v<decltype(*declval<Iter>()), ll>);
#if __cplusplus >= 202002L
  static_assert(input_iterator<Iter>);
#endif
  assert(Iter{} == Iter{});
}

// Test focus: large sparse universes and signed-integer extrema keep shifts and boundaries valid.
void test_wide()
{
  ll limit = numeric_limits<ll>::max();
  MeldableIntegerSetPool sets(limit);
  auto a = sets.make_set();
  set<ll> expected = {0, 1, 63, 64, 65, (1LL << 40) - 1, 1LL << 40,
                      (1LL << 62) - 1, 1LL << 62, limit - 1};
  for (ll key : expected) a.insert(key);
  check(a, expected);
  auto b = sets.make_set(limit - 1);
  size_t before = sets.node_count();
  a.merge(b);
  assert(sets.node_count() == before && b.empty());
  check(a, expected);
  vc<ll> tail;
  a.enumerate(limit - 64, limit, [&](ll key) { tail.eb(key); });
  assert(tail == vc<ll>{limit - 1});
  assert(a.count(limit - 64, limit) == 1);
  auto copy = a.clone();
  assert(copy.erase(limit - 1));
  set<ll> copied = expected;
  copied.erase(limit - 1);
  check(copy, copied);
  check(a, expected);
}

// Test focus: move-only handles own independent sets across relocation, clone, merge, and pool resets.
void test_handles()
{
  static_assert(is_same_v<MeldableIntegerSet, MeldableIntegerSetPool::Set>);
  static_assert(!is_copy_constructible_v<MeldableIntegerSet> && !is_copy_assignable_v<MeldableIntegerSet>);
  static_assert(is_nothrow_move_constructible_v<MeldableIntegerSet> && is_nothrow_move_assignable_v<MeldableIntegerSet>);
  mt19937_64 rng(2026101001);
  for (ll n : {0LL, 1LL, 65LL, 4097LL, numeric_limits<ll>::max()})
  {
    constexpr int groups = 8;
    MeldableIntegerSetPool pool(n, groups * 700);
    vc<MeldableIntegerSet> handles;
    array<set<ll>, groups> model;
    repi(i, groups)
    {
      if (n > 0 && i % 2)
      {
        ll key = rng() % ull(n);
        handles.eb(pool.make_set(key));
        model[i].insert(key);
      }
      else handles.eb(pool.make_set());
    }
    repi(i, groups) check(handles[i], model[i]);
    if (n == 0) continue;
    repi(iter, 3000)
    {
      int a = rng() % groups, b = rng() % groups, op = rng() % 12;
      ll key = rng() % ull(n);
      if (iter % 7 == 0) key = n - 1;
      if (op < 4)
        assert(handles[a].insert(key) == model[a].insert(key).second);
      else if (op < 6)
        assert(handles[a].erase(key) == (model[a].erase(key) != 0));
      else if (op < 8)
      {
        size_t before = pool.node_count();
        handles[a].merge(handles[b]);
        assert(pool.node_count() == before);
        if (a != b)
        {
          model[a].insert(ALL(model[b]));
          model[b].clear();
          assert(handles[b].empty());
        }
      }
      else if (op == 8)
      {
        handles[b] = handles[a].clone();
        model[b] = model[a];
      }
      else if (op == 9)
      {
        size_t before = pool.node_count();
        handles[a].clear();
        model[a].clear();
        assert(pool.node_count() == before);
      }
      else if (op == 10)
      {
        size_t before = pool.node_count();
        handles[b] = std::move(handles[a]);
        if (a != b)
        {
          model[b] = model[a];
          model[a].clear();
          assert(handles[a].empty());
        }
        assert(pool.node_count() == before);
      }
      else
      {
        swap(handles[a], handles[b]);
        swap(model[a], model[b]);
      }
      ll l = rng() % (ull(n) + 1), r = rng() % (ull(n) + 1);
      if (l > r) swap(l, r);
      vc<ll> actual;
      handles[a].enumerate(l, r, [&](ll x) { actual.eb(x); });
      vc<ll> expected(model[a].lower_bound(l), model[a].lower_bound(r));
      assert(actual == expected && handles[a].count(l, r) == ll(expected.size()));
      if (iter % 113 == 0) repi(i, groups) check(handles[i], model[i]);
      if (iter % 700 == 699)
      {
        pool.clear();
        for (auto &st : handles) st = pool.make_set();
        for (auto &values : model) values.clear();
        assert(pool.node_count() == 0);
      }
    }
    repi(i, groups) check(handles[i], model[i]);
  }
}

// Test focus: moving and swapping handles also transfer their pool binding; destruction never reads old handles.
void test_handle_transfers()
{
  MeldableIntegerSetPool p(65), q(1000);
  auto a = p.make_set(64), b = q.make_set(999);
  size_t p_before = p.node_count(), q_before = q.node_count();
  swap(a, b);
  assert(a.universe_size() == 1000 && b.universe_size() == 65);
  check(a, {999});
  check(b, {64});
  a = std::move(b);
  assert(a.universe_size() == 65 && b.universe_size() == 65);
  check(a, {64});
  check(b, {});
  auto moved = std::move(a);
  check(moved, {64});
  check(a, {});
  auto &alias = moved;
  moved = std::move(alias);
  check(moved, {64});
  assert(p.node_count() == p_before && q.node_count() == q_before);
  auto clone = moved.clone();
  clone.erase(64);
  clone.insert(0);
  check(clone, {0});
  check(moved, {64});
  p.clear();
  q.clear();
}

int main()
{
  test_random();
  test_words();
  test_wide();
  test_handles();
  test_handle_transfers();
  cout << "Hello World" << endl;
}
