#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "algebra/acted_monoid/add_min.hpp"
#include "ds/segtree/lazy_segtree.hpp"

constexpr ll TEST_INF = (1LL << 60);

// Test focus: the compact add/min/max monoid preserves both extrema and its intended object sizes.
void test()
{
  using AM = ActedMonoidAddMinMax<ll, TEST_INF>;
  static_assert(sizeof(AM::S) == 2 * sizeof(ll));
  static_assert(sizeof(AM::F) == sizeof(ll));

  mt19937 rng(123456789);
  constexpr int n = 57;
  vc<ll> a(n);
  repi(i, n) a[i] = ll(rng() % 101) - 50;
  LazySegmentTree<AM> seg(a);

  repi(iter, 5000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r)
      swap(l, r);
    ll x = ll(rng() % 101) - 50;
    seg.apply(l, r, x);
    repi(i, l, r) a[i] += x;

    l = rng() % n, r = l + 1 + rng() % (n - l);
    auto got = seg.prod(l, r);
    ll mn = a[l], mx = a[l];
    repi(i, l, r) chmin(mn, a[i]), chmax(mx, a[i]);
    assert(got.mn == mn && got.mx == mx);
  }

  auto empty = AM::mapping(5, AM::e());
  assert(empty.mn == TEST_INF && empty.mx == -TEST_INF);
}

int main()
{
  test();
  cout << "Hello World" << endl;
}
