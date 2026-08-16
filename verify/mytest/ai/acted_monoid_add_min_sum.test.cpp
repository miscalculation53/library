#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "algebra/acted_monoid/add_min_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

constexpr ll TEST_INF = (1LL << 60);

// Test focus: range addition preserves both the extremum and sum, including empty ranges.
void test()
{
  using AMn = ActedMonoidAddMinSum<ll, TEST_INF>;
  using AMx = ActedMonoidAddMaxSum<ll, TEST_INF>;
  using AMnx = ActedMonoidAddMinMaxSum<ll, TEST_INF>;
  mt19937 rng(123456789);
  constexpr int n = 57;
  vc<ll> a(n);
  repi(i, n) a[i] = ll(rng() % 101) - 50;
  LazySegmentTree<AMn> seg_mn(a);
  LazySegmentTree<AMx> seg_mx(a);
  LazySegmentTree<AMnx> seg_mn_mx(a);

  repi(iter, 5000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r)
      swap(l, r);
    ll x = ll(rng() % 101) - 50;
    seg_mn.apply(l, r, x), seg_mx.apply(l, r, x), seg_mn_mx.apply(l, r, x);
    repi(i, l, r) a[i] += x;

    l = rng() % n, r = l + 1 + rng() % (n - l);
    auto got_mn = seg_mn.prod(l, r);
    auto got_mx = seg_mx.prod(l, r);
    auto got_mn_mx = seg_mn_mx.prod(l, r);
    ll mn = a[l], mx = a[l], sum = 0;
    repi(i, l, r) chmin(mn, a[i]), chmax(mx, a[i]), sum += a[i];
    assert(got_mn.mn == mn && got_mn.sum == sum && got_mn.len == r - l);
    assert(got_mx.mx == mx && got_mx.sum == sum && got_mx.len == r - l);
    assert(got_mn_mx.mn == mn && got_mn_mx.mx == mx && got_mn_mx.sum == sum && got_mn_mx.len == r - l);
  }

  assert(AMn::mapping(5, AMn::e()).len == 0);
  assert(AMx::mapping(5, AMx::e()).len == 0);
  assert(AMnx::mapping(5, AMnx::e()).len == 0);
}

int main()
{
  test();
  cout << "Hello World" << endl;
}
