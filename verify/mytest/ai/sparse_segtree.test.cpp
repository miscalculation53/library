#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/segtree/sparse_segtree.hpp"

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
  SparseSegmentTree<MonoidAdd<ll>> seg(n);
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
  SparseSegmentTree<MonoidAdd<ll>> seg(n);
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
  SparseSegmentTree<MonoidAdd<ll>> seg(n);
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
  SparseSegmentTree<MonoidAdd<ll>> seg(n);
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
  SparseSegmentTree<StringConcatMonoid> seg(n);
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
  SparseSegmentTree<MonoidAdd<ll>> seg(0);
  auto pred = [](ll sum) { return sum == 0; };
  assert(seg.max_right_ok(0, pred) == 0);
  assert(seg.min_left_ok(0, pred) == 0);
}

int main()
{
  test_sparse_segtree_modify();
  test_sparse_segtree_repeated_modify();
  test_sparse_segtree_binary_search();
  test_sparse_segtree_binary_search_large_index();
  test_sparse_segtree_noncommutative();
  test_sparse_segtree_empty();
  cout << "Hello World" << endl;
}
