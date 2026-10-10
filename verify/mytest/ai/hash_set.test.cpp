#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/hash_set.hpp"

struct IdentityHash
{
  size_t operator()(int x) const { return x; }
};

struct CollisionHash
{
  size_t operator()(int x) const { return x % 8; }
};

template <class Set>
void check(const Set &st, const set<typename Set::iterator::value_type> &expected)
{
  set<typename Set::iterator::value_type> actual;
  for (const auto &key : st)
    assert(actual.insert(key).second);
  assert(actual == expected && st.size() == expected.size());
  assert(st.empty() == expected.empty());
  assert(size_t(distance(st.cbegin(), st.cend())) == st.size());
  for (const auto &key : expected)
    assert(st.contains(key) && *st.find_ptr(key) == key);
}

// Test focus: key-only iteration is const and works with standard forward-iterator algorithms.
void test_iteration()
{
  using Set = HashSet<int, IdentityHash>;
  using Iter = Set::iterator;
  static_assert(is_same_v<Iter, Set::const_iterator>);
  static_assert(is_same_v<decltype(*declval<Iter>()), const int &>);
  static_assert(is_same_v<decltype(declval<Set &>().find_ptr(0)), const int *>);
  static_assert(is_same_v<decltype(declval<Set &>().insert(0)), pair<const int *, bool>>);
  static_assert(is_nothrow_move_constructible_v<Set> && is_nothrow_move_assignable_v<Set>);
  static_assert(is_nothrow_swappable_v<Set>);
#if __cplusplus >= 202002L
  static_assert(forward_iterator<Iter>);
#endif

  Set st;
  const Set &cst = st;
  assert(st.begin() == st.end() && cst.begin() == cst.end());
  assert(Iter{} == Iter{});
  st.reserve(100);
  check(st, {});

  for (int key : {0, 4, 6, 14, 22, 255})
  {
    auto [p, inserted] = st.insert(key);
    assert(inserted && *p == key);
    auto [q, duplicated] = st.insert(key);
    assert(!duplicated && q == p);
  }
  check(st, {0, 4, 6, 14, 22, 255});
  auto it = st.begin(), saved = it;
  assert(it++ == saved && saved == st.begin());
  assert(it != saved);
  assert(++saved == it);
  assert(st.begin().operator->() == st.find_ptr(*st.begin()));
  vector<int> keys(st.begin(), st.end());
  assert((set<int>(ALL(keys)) == set<int>{0, 4, 6, 14, 22, 255}));

  st.clear();
  assert(st.begin() == st.end());
  check(st, {});
  st.insert(255);
  auto last = st.begin();
  assert(*last == 255 && ++last == st.end());
}

// Test focus: wraparound clusters and randomized deletion preserve all remaining keys.
void test_collisions()
{
  HashSet<int, IdentityHash> boundary;
  for (int key = 0; key < 5; key++) boundary.insert(key);
  const int *p = boundary.find_ptr(0);
  auto [q, duplicated] = boundary.insert(0);
  assert(!duplicated && p == q);
  boundary.insert(5);
  check(boundary, {0, 1, 2, 3, 4, 5});

  HashSet<int, IdentityHash> wrap;
  for (int key : {6, 14, 22, 1}) wrap.insert(key);
  assert(wrap.erase(6));
  check(wrap, {14, 22, 1});
  assert(wrap.erase(14));
  check(wrap, {22, 1});
  assert(!wrap.erase(6));

  HashSet<int, CollisionHash> st;
  set<int> expected;
  mt19937 rng(20261009);
  repi(iter, 20000)
  {
    int key = rng() % 256;
    switch (rng() % 5)
    {
      case 0:
        assert(st.erase(key) == (expected.erase(key) != 0));
        break;
      case 1:
        assert(st.contains(key) == (expected.count(key) != 0));
        break;
      default:
      {
        auto [p, inserted] = st.insert(key);
        assert(*p == key && inserted == expected.insert(key).second);
      }
    }
    if (iter % 5000 == 4999) st.reserve(iter / 10);
    if (iter % 31 == 0) check(st, expected);
  }
  check(st, expected);
}

// Test focus: pair keys, rehashing, clearing, and reuse match std::unordered_set.
void test_random()
{
  HashSet<pair<ll, ll>> st;
  unordered_set<pair<ll, ll>, safe_hash> expected;
  mt19937_64 rng(3756);
  repi(iter, 200000)
  {
    pair<ll, ll> key = {ll(rng() % 128), ll(rng() % 128)};
    int type = rng() % 4;
    if (type == 0)
      assert(st.erase(key) == (expected.erase(key) != 0));
    else if (type == 1)
    {
      auto p = st.find_ptr(key);
      assert((p != nullptr) == (expected.count(key) != 0));
      if (p) assert(*p == key);
    }
    else
    {
      auto [p, inserted] = st.insert(key);
      assert(*p == key && inserted == expected.insert(key).second);
    }
    assert(st.size() == expected.size());
    if (iter == 99999)
    {
      st.clear(), expected.clear();
      assert(st.empty());
    }
  }
  check(st, set<pair<ll, ll>>(ALL(expected)));
  st.reserve(50000);
  check(st, set<pair<ll, ll>>(ALL(expected)));
}

struct ModHash
{
  size_t operator()(int x) const { return x % 16; }
};

struct ModEqual
{
  bool operator()(int x, int y) const { return x % 16 == y % 16; }
};

struct StatefulHash
{
  inline static ull next_seed = 0;
  ull seed = next_seed++;
  size_t operator()(int x) const { return safe_hash::splitmix64(ull(x) ^ seed); }
};

// Test focus: custom equality preserves the stored representative for equivalent keys.
void test_equal()
{
  HashSet<int, ModHash, ModEqual> st;
  auto [p, inserted] = st.insert(3);
  auto [q, duplicated] = st.insert(19);
  assert(inserted && !duplicated && p == q && *q == 3);
  assert(st.size() == 1 && *st.find_ptr(35) == 3);
  st.reserve(100);
  assert(*st.find_ptr(51) == 3);
  assert(st.erase(67) && st.empty());
}

// Test focus: copies, moves, member/ADL swap, and small-to-large union keep independent contents.
void test_transfer()
{
  HashSet<string> st;
  st.insert("first"), st.insert("second");
  HashSet<string> copied = st;
  copied.erase("first");
  check(st, {"first", "second"});
  check(copied, {"second"});

  HashSet<string> assigned;
  assigned = st;
  check(assigned, {"first", "second"});
  HashSet<string> moved = std::move(st);
  check(moved, {"first", "second"});
  check(st, {});
  st.insert("reuse");
  copied = std::move(moved);
  check(copied, {"first", "second"});
  check(moved, {});
  auto &alias = copied;
  copied = std::move(alias);
  check(copied, {"first", "second"});
  st.swap(copied);
  check(st, {"first", "second"});
  check(copied, {"reuse"});
  swap(st, copied);
  check(st, {"reuse"});
  check(copied, {"first", "second"});

  HashSet<int> a, b;
  for (int key : {1, 2, 3}) a.insert(key);
  for (int key : {3, 4, 5, 6}) b.insert(key);
  if (a.size() < b.size()) a.swap(b);
  for (const auto &key : b) a.insert(key);
  b.clear();
  check(a, {1, 2, 3, 4, 5, 6});
  check(b, {});

  HashSet<int, StatefulHash> x, y;
  x.insert(10), y.insert(20);
  x.swap(y);
  check(x, {20});
  check(y, {10});
  HashSet<int, StatefulHash> z = x;
  check(z, {20});
  y = std::move(z);
  check(y, {20});
  z.insert(30);
  check(z, {30});
}

int main()
{
  test_iteration();
  test_collisions();
  test_random();
  test_equal();
  test_transfer();
  cout << "Hello World" << endl;
}
