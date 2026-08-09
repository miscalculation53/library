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

int main()
{
  test_binary_search();
  test_binary_search_noncommutative();
  test_empty();
  cout << "Hello World" << endl;
}
