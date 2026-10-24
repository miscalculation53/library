#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/segtree/sparse_segtree.hpp"
#include "algebra/min_max_sum.hpp"
#include "algebra/affine_function.hpp"
#include "math/modint/modint.hpp"

struct StringConcatMonoid
{
  using S = string;
  static S op(const S &a, const S &b) { return a + b; }
  static S e() { return {}; }
};

// Test focus: modify sees the current value exactly once at sparse 40-bit positions.
void test_sparse_segtree_modify()
{
  constexpr ll n = 1LL << 40;
  SparseSegmentTree<MonoidAdd<ll>> seg(n, 0);
  seg.reserve(1 << 12);
  map<ll, ll> a;
  mt19937_64 rng(123456789);

  repi(iter, 3000)
  {
    ll p = rng() % n;
    ll old = a[p];
    ll add = ll(rng() % 2001) - 1000;
    int called = 0;
    seg.modify(p, [&](ll &value)
    {
      called++;
      assert(value == old);
      value += add;
    });
    assert(called == 1);
    a[p] += add;
    assert(seg.get(p) == a[p]);

    if (iter % 7 == 0)
    {
      ll l = rng() % n, r = rng() % n;
      if (l > r) swap(l, r);
      r = min(n, r + 1);
      ll expected = 0;
      for (auto it = a.lower_bound(l); it != a.end() && it->first < r; ++it)
        expected += it->second;
      assert(seg.prod(l, r) == expected);
    }
  }

  ll expected = 0;
  for (auto [p, value] : a) expected += value;
  assert(seg.all_prod() == expected);

  constexpr ll p = 123456789;
  seg.set(p, 42);
  assert(seg.get(p) == 42);
  seg.modify(p, [](ll &value) { value *= -3; });
  assert(seg.get(p) == -126);
}

// Test focus: repeated modifications of existing nodes keep ancestors consistent.
void test_sparse_segtree_repeated_modify()
{
  constexpr int n = 257;
  SparseSegmentTree<MonoidAdd<ll>> seg(n, 0);
  seg.reserve(n + 1);
  array<ll, n> a{};
  mt19937_64 rng(314159265);

  repi(iter, 10000)
  {
    int p = rng() % n;
    ll old = a[p];
    seg.modify(p, [&](ll &value)
    {
      assert(value == old);
      value = value * 2 + ll(rng() % 11) - 5;
      a[p] = value;
    });
    assert(seg.get(p) == a[p]);

    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    ll expected = accumulate(a.begin() + l, a.begin() + r, 0LL);
    assert(seg.prod(l, r) == expected);
  }
}

// Test focus: both ACL-style boundary searches agree with a dense reference array.
void test_sparse_segtree_binary_search()
{
  constexpr int n = 257;
  SparseSegmentTree<MonoidAdd<ll>> seg(n, 0);
  seg.reserve(n + 1);
  array<ll, n> a{};
  mt19937 rng(161803398);

  repi(iter, 5000)
  {
    int p = rng() % n;
    a[p] = rng() % 6;
    seg.set(p, a[p]);

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

// Test focus: boundary searches return exact coordinates in a 40-bit sparse domain.
void test_sparse_segtree_binary_search_large_index()
{
  constexpr ll n = 1LL << 40;
  SparseSegmentTree<MonoidAdd<ll>> seg(n, 0);
  seg.reserve(1 << 10);
  map<ll, ll> a;
  mt19937_64 rng(141421356);
  repi(_, 500)
  {
    ll p = rng() % n;
    ll value = rng() % 10 + 1;
    a[p] = value;
    seg.set(p, value);
  }

  repi(_, 3000)
  {
    ll l = rng() % (n + 1), limit = rng() % 100;
    auto pred = [&](ll sum) { return sum <= limit; };
    ll sum = 0, ng = -1;
    for (auto it = a.lower_bound(l); it != a.end(); ++it)
    {
      sum += it->second;
      if (sum > limit)
      {
        ng = it->first;
        break;
      }
    }
    ll expected_r = ng == -1 ? n : ng;
    assert(seg.max_right_ok(l, pred) == expected_r);

    ll r = rng() % (n + 1);
    sum = 0, ng = -1;
    auto it = a.lower_bound(r);
    while (it != a.begin())
    {
      --it;
      sum += it->second;
      if (sum > limit)
      {
        ng = it->first;
        break;
      }
    }
    ll expected_l = ng == -1 ? 0 : ng + 1;
    assert(seg.min_left_ok(r, pred) == expected_l);
  }
}

// Test focus: products and boundary searches preserve noncommutative operand order.
void test_sparse_segtree_noncommutative()
{
  constexpr int n = 64;
  SparseSegmentTree<StringConcatMonoid> seg(n, "");
  seg.reserve(n + 1);
  array<string, n> a;
  vc<int> order(n);
  iota(order.begin(), order.end(), 0);
  mt19937 rng(271828182);
  shuffle(order.begin(), order.end(), rng);

  for (int p : order)
  {
    string value = "[" + to_string(p) + "]";
    seg.modify(p, [&](string &current) { current += value; });
    a[p] += value;
  }
  repi(iter, 1000)
  {
    int p = rng() % n;
    seg.modify(p, [](string &value) { value += "!"; });
    a[p] += "!";
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    string expected;
    repi(i, l, r) expected += a[i];
    assert(seg.prod(l, r) == expected);
  }

  repi(l, n + 1) repi(r, l, n + 1)
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

// Test focus: both boundary searches return zero on an empty domain.
void test_sparse_segtree_empty()
{
  SparseSegmentTree<MonoidAdd<ll>> seg(0, 7);
  auto pred = [](ll sum) { return sum == 0; };
  assert(seg.prod(0, 0) == 0 && seg.all_prod() == 0 && seg.content().empty());
  assert(seg.max_right_ok(0, pred) == 0);
  assert(seg.min_left_ok(0, pred) == 0);
}

// Test focus: untouched points contribute the initial value, including search boundaries inside gaps.
void test_sparse_segtree_initial_value()
{
  static_assert(!is_default_constructible_v<SparseSegmentTree<MonoidAdd<ll>>>);
  static_assert(!is_constructible_v<SparseSegmentTree<MonoidAdd<ll>>, ll>);
  mt19937 rng(20261014);
  for (int n : {1, 2, 3, 17, 64, 257})
  {
    SparseSegmentTree<MonoidAdd<ll>> seg(n, 3);
    vc<ll> a(n, 3);
    map<ll, ll> registered;
    assert(seg.content().empty() && seg.all_prod() == 3LL * n);
    repi(iter, 1000)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      assert(seg.prod(l, r) == accumulate(a.begin() + l, a.begin() + r, 0LL));
      ll limit = rng() % (3 * n + 1), sum = 0;
      auto pred = [&](ll x) { return x <= limit; };
      int expected_r = l;
      while (expected_r < n && sum + a[expected_r] <= limit)
        sum += a[expected_r++];
      assert(seg.max_right_ok(l, pred) == expected_r);
      int expected_l = r;
      sum = 0;
      while (expected_l > 0 && sum + a[expected_l - 1] <= limit)
        sum += a[--expected_l];
      assert(seg.min_left_ok(r, pred) == expected_l);

      int p = rng() % n, called = 0;
      ll value = rng() % 8;
      seg.modify(p, [&](ll &x)
      {
        called++;
        assert(x == a[p]);
        x = value;
      });
      a[p] = registered[p] = value;
      assert(called == 1 && seg.get(p) == a[p]);
      assert(seg.all_prod() == accumulate(ALL(a), 0LL));
      assert(seg.content() == registered);
    }
  }

  SparseSegmentTree<MonoidMin<ll>> seg(5, 0);
  assert(seg.all_prod() == 0 && seg.get(3) == 0);
  seg.set(2, 7);
  assert(seg.prod(2, 3) == 7 && seg.prod(1, 4) == 0);
  auto positive = [](ll x) { return x > 0; };
  assert(seg.max_right_ok(2, positive) == 3 && seg.min_left_ok(3, positive) == 2);
}

// Test focus: noncommutative products preserve the order of initial runs and updated points.
void test_sparse_segtree_initial_noncommutative()
{
  constexpr int n = 31;
  SparseSegmentTree<StringConcatMonoid> seg(n, "ab");
  array<string, n> a;
  a.fill("ab");
  mt19937 rng(173205080);
  repi(iter, 300)
  {
    repi(l, n + 1) repi(r, l, n + 1)
    {
      string expected;
      repi(p, l, r) expected += a[p];
      assert(seg.prod(l, r) == expected);
      auto prefix = [&](const string &x)
      {
        return x.size() <= expected.size() && equal(ALL(x), expected.begin());
      };
      auto suffix = [&](const string &x)
      {
        return x.size() <= expected.size() && equal(ALL(x), expected.end() - x.size());
      };
      assert(seg.max_right_ok(l, prefix) == r);
      assert(seg.min_left_ok(r, suffix) == l);
    }
    int p = rng() % n;
    a[p] = "[" + to_string(p) + "]";
    seg.set(p, a[p]);
  }
}

// Test focus: full-width implicit lengths remain valid with and without a specialized pow.
void test_sparse_segtree_initial_large_index()
{
  using M = MonoidLenSum<ll>;
  struct WithoutPow
  {
    using S = M::S;
    static S e() { return M::e(); }
    static S op(const S &a, const S &b) { return M::op(a, b); }
  };
  auto test = [](auto monoid)
  {
    using A = decltype(monoid);
    for (ll n : {1LL << 40, LLONG_MAX})
    {
      SparseSegmentTree<A> seg(n, typename A::S(0));
      auto pred = [](const typename A::S &x) { return x.len <= 10; };
      assert(seg.all_prod().len == n && seg.all_prod().val == 0);
      assert(seg.max_right_ok(n / 2, pred) == n / 2 + 10);
      assert(seg.min_left_ok(n / 2, pred) == n / 2 - 10);
      assert(seg.max_right_ok(n - 5, pred) == n && seg.min_left_ok(5, pred) == 0);
      seg.set(n - 1, typename A::S(7));
      seg.modify(n / 2, [](typename A::S &x) { assert(x.len == 1 && x.val == 0); x.val = 2; });
      seg.set(0, typename A::S(3));
      auto got = seg.prod(1, n);
      assert(got.len == n - 1 && got.val == 9);
      assert(seg.all_prod().len == n && seg.all_prod().val == 12);
      assert(seg.get(n / 3).len == 1 && seg.get(n / 3).val == 0);
      assert(seg.max_right_ok(n / 2, pred) == n / 2 + 10);
      assert(seg.min_left_ok(n / 2, pred) == n / 2 - 10);
      assert(seg.prod(0, 0).len == 0 && seg.prod(n, n).len == 0);
    }
  };
  test(M{}), test(WithoutPow{});

  using A = MonoidAffineFunction<modint998244353>;
  constexpr ll n = LLONG_MAX;
  const A::S initial{2, 3};
  SparseSegmentTree<A> seg(n, initial);
  auto same = [](const A::S &x, const A::S &y) { return x.b == y.b && x.c == y.c; };
  assert(same(seg.all_prod(), pow_monoid<A>(initial, n)));
  seg.set(0, {5, 7});
  seg.set(n - 1, {11, 13});
  auto middle = pow_monoid<A>(initial, n - 2);
  assert(same(seg.prod(1, n - 1), middle));
  assert(same(seg.all_prod(), A::op(A::op({5, 7}, middle), {11, 13})));
}

int main()
{
  test_sparse_segtree_modify();
  test_sparse_segtree_repeated_modify();
  test_sparse_segtree_binary_search();
  test_sparse_segtree_binary_search_large_index();
  test_sparse_segtree_noncommutative();
  test_sparse_segtree_empty();
  test_sparse_segtree_initial_value();
  test_sparse_segtree_initial_noncommutative();
  test_sparse_segtree_initial_large_index();
  cout << "Hello World" << endl;
}
