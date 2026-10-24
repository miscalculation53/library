#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "ds/segtree/sparse_lazy_segtree.hpp"
#include "algebra/acted_monoid/add_sum.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"

struct ActedMonoidStringAssign
{
  using S = string;
  static S op(const S &a, const S &b) { return a + b; }
  static S e() { return {}; }
  using F = char;
  static S mapping(F f, const S &x) { return f == 0 ? x : string(x.size(), f); }
  static F composition(F f, F g) { return f == 0 ? g : f; }
  static F id() { return 0; }
};

// Test focus: set/modify/range apply and both boundary searches agree with a dense array.
void test_basic_and_binary_search()
{
  using AM = ActedMonoidAddSum<ll>;
  constexpr int n = 73;
  mt19937 rng(123456789);
  vc<ll> a(n);
  SparseLazySegmentTree<AM> seg(n, AM::S(0));
  seg.reserve(1 << 15);

  repi(iter, 5000)
  {
    int type = rng() % 3;
    if (type == 0)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      ll add = rng() % 6;
      seg.apply(l, r, add);
      repi(i, l, r) a[i] += add;
    }
    else
    {
      int p = rng() % n;
      ll x = rng() % 20;
      if (type == 1)
        seg.set(p, AM::S(x)), a[p] = x;
      else
      {
        seg.modify(p, [&](AM::S &s) { s.val += x; });
        a[p] += x;
      }
    }

    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    ll expected = accumulate(a.begin() + l, a.begin() + r, 0LL);
    auto got = seg.prod(l, r);
    assert(got.len == r - l && got.val == expected);
    int p = rng() % n;
    assert(seg.get(p).len == 1 && seg.get(p).val == a[p]);
    assert(seg.all_prod().len == n);
    assert(seg.all_prod().val == accumulate(ALL(a), 0LL));

    l = rng() % (n + 1);
    ll limit = rng() % 500;
    auto pred = [&](const AM::S &s) { return s.val <= limit; };
    r = l;
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

// Test focus: nontrivial affine actions compose correctly on lazily created nodes.
void test_affine()
{
  using mint = modint998244353;
  using AM = ActedMonoidAffineSum<mint>;
  constexpr int n = 61;
  mt19937 rng(987654321);
  vc<mint> a(n);
  SparseLazySegmentTree<AM> seg(n, AM::S(0));
  seg.reserve(1 << 15);

  repi(iter, 5000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    if (rng() % 3 != 0)
    {
      mint b = rng() % 10, c = rng() % 10;
      seg.apply(l, r, {b, c});
      repi(i, l, r) a[i] = b * a[i] + c;
    }
    else
    {
      mint expected = 0;
      repi(i, l, r) expected += a[i];
      auto got = seg.prod(l, r);
      assert(got.len == r - l && got.val == expected);
    }
    if (iter % 17 == 0)
    {
      auto runs = as_const(seg).content();
      int pos = 0;
      for (auto [left, right, x] : runs)
      {
        assert(left == pos && left < right && x.len == 1);
        repi(i, left, right) assert(x.val == a[i]);
        pos = right;
      }
      assert(pos == n);
    }
  }
}

// Test focus: products and boundary searches preserve order for string concatenation.
void test_noncommutative()
{
  constexpr int n = 47;
  vc<string> a(n, "a");
  SparseLazySegmentTree<ActedMonoidStringAssign> seg(n, "a");
  seg.reserve(1 << 14);
  mt19937 rng(314159265);
  repi(iter, 1000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    char c = 'a' + rng() % 5;
    seg.apply(l, r, c);
    repi(i, l, r) a[i] = string(1, c);

    l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    string target;
    repi(i, l, r) target += a[i];
    assert(seg.prod(l, r) == target);

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

// Test focus: 40-bit indices, mostly implicit ranges, point operations, and empty input.
void test_large_array()
{
  using AM = ActedMonoidAddSum<ll>;
  constexpr ll n = 1LL << 40;
  SparseLazySegmentTree<AM> seg(n, AM::S(0));
  seg.reserve(1 << 15);
  seg.apply(10, n - 10, 2);
  seg.apply(100, 200, 3);
  seg.apply(123, 4);
  seg.modify(124, [](AM::S &x) { x.val += 7; });
  assert(seg.get(0).val == 0);
  assert(seg.get(10).val == 2);
  assert(seg.get(100).val == 5);
  assert(seg.get(123).val == 9);
  assert(seg.get(124).val == 12);
  assert(seg.get(199).val == 5);
  assert(seg.get(200).val == 2);
  assert(seg.get(n - 10).val == 0);
  assert(seg.prod(50, 250).val == 711);
  assert(seg.all_prod().val == 2 * (n - 20) + 311);

  SparseLazySegmentTree<AM> search_seg(n, AM::S(0));
  search_seg.reserve(1 << 12);
  search_seg.apply(10, n - 10, 2);
  auto pred = [](const AM::S &x) { return x.val <= 4; };
  assert(search_seg.max_right_ok(0, pred) == 12);
  assert(search_seg.min_left_ok(n, pred) == n - 12);

  SparseLazySegmentTree<AM> empty(0, AM::S(0));
  assert(empty.prod(0, 0).len == 0);
  assert(empty.all_prod().len == 0);
  assert(as_const(empty).content().empty());
  assert(empty.max_right_ok(0, pred) == 0);
  assert(empty.min_left_ok(0, pred) == 0);
}

// Test focus: an explicit e() initial value represents absent positions, not unit leaves.
void test_explicit_identity_initial_value()
{
  using AM = ActedMonoidAddSum<ll>;
  static_assert(!is_constructible_v<SparseLazySegmentTree<AM>, ll>);
  SparseLazySegmentTree<AM> seg(100, AM::e());
  assert(seg.all_prod().len == 0 && seg.all_prod().val == 0);
  seg.apply(0, 100, 5);
  assert(seg.all_prod().len == 0 && seg.all_prod().val == 0);
  seg.set(42, AM::S(3));
  assert(seg.get(42).len == 1 && seg.get(42).val == 3);
}

struct ActedMonoidTrackedFlip
{
  struct S
  {
    inline static int live = 0, peak = 0;
    ll len;
    ull val;
    S(ull val) : S(1, val) {}
    S(ll len, ull val) : len(len), val(val) { peak = max(peak, ++live); }
    S(const S &x) : S(x.len, x.val) {}
    S &operator=(const S &) = default;
    ~S() { --live; }
  };
  using F = bool;
  static S op(const S &a, const S &b) { return {a.len + b.len, a.val + b.val}; }
  static S e() { return {0, 0}; }
  static S mapping(F f, const S &x) { return {x.len, f ? ull(x.len) - x.val : x.val}; }
  static F composition(F f, F g) { return f != g; }
  static F id() { return false; }
};

// Test focus: online splitting uses O(q) aggregate objects; const queries add no nodes.
void test_storage_growth_and_const_queries()
{
  using AM = ActedMonoidTrackedFlip;
  constexpr ll n = 1'000'000'000'000'000'000LL;
  constexpr int q = 2000;
  assert(AM::S::live == 0);
  AM::S::peak = 0;
  {
    SparseLazySegmentTree<AM> seg(n, AM::S(0));
    mt19937_64 rng(20261014);
    repi(_, q)
    {
      ll l = rng() % n, r = rng() % n;
      if (l > r) swap(l, r);
      seg.apply(l, r + 1, true);
    }
    assert(AM::S::peak <= 20 * q + 200);
    int live = AM::S::live;
    const auto &cseg = seg;
    {
      auto runs = cseg.content();
      assert(runs.size() <= 2 * q + 1);
      ll pos = 0;
      ull sum = 0;
      for (const auto &[l, r, x] : runs)
      {
        assert(l == pos && l < r && x.len == 1 && x.val <= 1);
        assert(cseg.get(l).val == x.val && cseg.get(r - 1).val == x.val);
        sum += ull(r - l) * x.val;
        pos = r;
      }
      assert(pos == n && sum == cseg.all_prod().val);
    }
    assert(AM::S::live == live);
    repi(_, 1000)
    {
      ll l = rng() % n, r = rng() % n;
      if (l > r) swap(l, r);
      auto x = cseg.prod(l, r + 1);
      assert(x.len == r + 1 - l && x.val <= ull(x.len));
      assert(cseg.get(l).len == 1 && cseg.get(l).val <= 1);
      auto pred = [](const AM::S &s) { return s.len <= 7; };
      assert(cseg.max_right_ok(l, pred) == min(n, l + 7));
      assert(cseg.min_left_ok(r, pred) == max(0LL, r - 7));
    }
    assert(AM::S::live == live);
  }
  assert(AM::S::live == 0);
}

int main()
{
  test_basic_and_binary_search();
  test_affine();
  test_noncommutative();
  test_large_array();
  test_explicit_identity_initial_value();
  test_storage_growth_and_const_queries();
  cout << "Hello World" << endl;
}
