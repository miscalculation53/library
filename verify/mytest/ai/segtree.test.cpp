#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/segtree/segtree.hpp"

struct StringConcatMonoid
{
  using S = string;
  static S op(const S &a, const S &b) { return a + b; }
  static S e() { return {}; }
};

// Test focus: ACL-style boundary searches remain correct after point updates.
void test_binary_search()
{
  constexpr int n = 73;
  mt19937 rng(123456789);
  vc<ll> a(n);
  repi(i, n) a[i] = rng() % 6;
  SegmentTree<MonoidAdd<ll>> seg(a);

  repi(iter, 5000)
  {
    if (rng() % 3 == 0)
    {
      int p = rng() % n;
      a[p] = rng() % 6;
      seg.set(p, a[p]);
    }

    int l = rng() % (n + 1);
    ll limit = rng() % 100;
    auto pred = [&](ll sum) { return sum <= limit; };
    int r = l;
    ll sum = 0;
    while (r < n && sum + a[r] <= limit)
      sum += a[r++];
    assert(seg.max_right_ok(l, pred) == r);

    r = rng() % (n + 1);
    int expected_l = r;
    sum = 0;
    while (expected_l > 0 && a[expected_l - 1] + sum <= limit)
      sum += a[--expected_l];
    assert(seg.min_left_ok(r, pred) == expected_l);
  }
}

// Test focus: max_right/min_left preserve operand order for a noncommutative monoid.
void test_binary_search_noncommutative()
{
  vc<string> a = {"a", "bc", "d", "efg", "h", "ij", "k"};
  SegmentTree<StringConcatMonoid> seg(a);
  repi(l, SZ(a) + 1) repi(r, l, SZ(a) + 1)
  {
    string target;
    repi(i, l, r) target += a[i];
    auto is_prefix = [&](const string &s)
    {
      return s.size() <= target.size() && equal(ALL(s), target.begin());
    };
    auto is_suffix = [&](const string &s)
    {
      return s.size() <= target.size() && equal(ALL(s), target.end() - s.size());
    };
    assert(seg.max_right_ok(l, is_prefix) == r);
    assert(seg.min_left_ok(r, is_suffix) == l);
  }
}

// Test focus: both boundary searches return zero on an empty tree.
void test_empty()
{
  SegmentTree<MonoidAdd<ll>> seg(0);
  auto pred = [](ll sum) { return sum == 0; };
  assert(seg.max_right_ok(0, pred) == 0);
  assert(seg.min_left_ok(0, pred) == 0);
}

// 各有効区間の和が ll に収まる更新は、繰り返しても扱える。
struct CheckedNonnegativeSum
{
  using S = ll;
  static S e() { return 0; }
  static S op(S a, S b)
  {
    assert(0 <= a && 0 <= b && a <= numeric_limits<ll>::max() - b);
    return a + b;
  }
};

void test_repeated_large_values()
{
  const ll large = numeric_limits<ll>::max() - 10;
  for (int n : {1, 2, 3})
  {
    SegmentTree<CheckedNonnegativeSum> seg(vc<ll>(n, 0));
    for (int step = 0; step < 10; ++step)
    {
      seg.set(0, large);
      assert(seg.get(0) == large);
      assert(seg.prod(0, n) == large);
      assert(seg.all_prod() == large);
    }
  }
}

int main()
{
  test_binary_search();
  test_binary_search_noncommutative();
  test_empty();
  test_repeated_large_values();
  cout << "Hello World" << endl;
}
