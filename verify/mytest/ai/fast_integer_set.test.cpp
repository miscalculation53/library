#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/fast_integer_set.hpp"

void check(const FastIntegerSet &st, const set<int> &expected)
{
  assert(st.size() == int(expected.size()) && st.empty() == expected.empty());
  assert(st.content() == vc<int>(ALL(expected)));
  assert(vc<int>(st.begin(), st.end()) == vc<int>(ALL(expected)));
  int n = st.universe_size();
  for (int x : {numeric_limits<int>::min(), -1, 0, 1, 63, 64, 65, n - 1, n, numeric_limits<int>::max()})
  {
    auto lower = expected.lower_bound(x), upper = expected.upper_bound(x);
    assert(st.next(x) == (lower == expected.end() ? n : *lower));
    assert(st.geq_min(x) == st.next(x));
    assert(st.gt_min(x) == (upper == expected.end() ? n : *upper));
    assert(st.prev(x) == (upper == expected.begin() ? -1 : *prev(upper)));
    assert(st.leq_max(x) == st.prev(x));
    assert(st.lt_max(x) == (lower == expected.begin() ? -1 : *prev(lower)));
    if (0 <= x && x < n) assert(st[x] == (expected.count(x) != 0));
  }
  assert(st.min_element() == (expected.empty() ? n : *expected.begin()));
  assert(st.max_element() == (expected.empty() ? -1 : *expected.rbegin()));
}

// Test focus: exhaustive small universes and all neighbor/range boundaries match std::set.
void test_exhaustive()
{
  repi(n, 9) repi(mask, 1 << n)
  {
    FastIntegerSet st(n, [&](int key) { return (mask >> key) & 1; });
    set<int> expected;
    repi(key, n) if ((mask >> key) & 1) expected.insert(key);
    check(st, expected);
    repi(x, -2, n + 3)
    {
      auto lo = expected.lower_bound(x), hi = expected.upper_bound(x);
      assert(st.next(x) == (lo == expected.end() ? n : *lo));
      assert(st.prev(x) == (hi == expected.begin() ? -1 : *prev(hi)));
    }
    repi(l, n + 1) repi(r, l, n + 1)
    {
      vc<int> actual;
      st.enumerate(l, r, [&](int key) { actual.eb(key); });
      assert(actual == vc<int>(expected.lower_bound(l), expected.lower_bound(r)));
    }
  }
}

// Test focus: hierarchy propagation across 64, 4096, and 262144, including partial last words.
void test_random()
{
  mt19937 rng(20261009);
  for (int n : {0, 1, 63, 64, 65, 127, 4095, 4096, 4097, 262143, 262144, 262145})
  {
    FastIntegerSet st(n);
    set<int> expected;
    check(st, expected);
    if (n == 0) continue;
    repi(iter, 10000)
    {
      int key = rng() % n;
      switch (rng() % 5)
      {
        case 0:
          assert(st.erase(key) == (expected.erase(key) != 0));
          break;
        case 1:
          assert(st.contains(key) == (expected.count(key) != 0));
          break;
        default:
          assert(st.insert(key) == expected.insert(key).second);
      }
      auto lo = expected.lower_bound(key), hi = expected.upper_bound(key);
      assert(st.next(key) == (lo == expected.end() ? n : *lo));
      assert(st.prev(key) == (hi == expected.begin() ? -1 : *prev(hi)));
      if (iter % 257 == 0) check(st, expected);
      if (iter % 2000 == 1999)
      {
        st.clear(), expected.clear();
        check(st, expected);
      }
    }
    check(st, expected);
  }
}

// Test focus: a dense bottom level and state transfers preserve cardinality and iteration.
void test_dense_and_transfer()
{
  using Iter = FastIntegerSet::iterator;
  static_assert(is_same_v<decltype(*declval<Iter>()), int>);
  static_assert(is_nothrow_move_constructible_v<FastIntegerSet>);
#if __cplusplus >= 202002L
  static_assert(input_iterator<Iter>);
#endif
  assert(Iter{} == Iter{});
  FastIntegerSet st(4097, [](int) { return true; });
  set<int> expected;
  repi(key, st.universe_size()) expected.insert(key);
  check(st, expected);
  repi(key, st.universe_size())
  {
    assert(st.erase(key));
    expected.erase(key);
    if (key % 64 == 0) check(st, expected);
  }
  check(st, {});
  st.insert(4096);
  auto saved = st.begin(), it = saved;
  assert(*it == 4096 && it++ == saved && it == st.end());
  assert(*saved.operator->() == 4096);

  FastIntegerSet copied = st;
  copied.insert(0);
  check(st, {4096});
  check(copied, {0, 4096});
  FastIntegerSet moved = std::move(copied);
  check(moved, {0, 4096});
  check(copied, {});
  assert(copied.universe_size() == 0);
  FastIntegerSet assigned(1);
  assigned = moved;
  check(assigned, {0, 4096});
  assigned = std::move(st);
  check(assigned, {4096});
  check(st, {});
  auto &alias = assigned;
  assigned = std::move(alias);
  check(assigned, {4096});
  swap(assigned, moved);
  check(assigned, {0, 4096});
  check(moved, {4096});
}

int main()
{
  test_exhaustive();
  test_random();
  test_dense_and_transfer();
  cout << "Hello World" << endl;
}
