#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "algebra/acted_monoid/upd_min_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"

constexpr ll TEST_INF = (1LL << 60);

// Test focus: range assignment preserves both the extremum and sum, including identity actions.
void test()
{
  using AMn = ActedMonoidUpdMinSum<ll, TEST_INF>;
  using AMx = ActedMonoidUpdMaxSum<ll, TEST_INF>;
  using AMnx = ActedMonoidUpdMinMaxSum<ll, TEST_INF>;
  mt19937 rng(987654321);
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
    seg_mn.apply(l, r, AMn::F(x));
    seg_mx.apply(l, r, AMx::F(x));
    seg_mn_mx.apply(l, r, AMnx::F(x));
    repi(i, l, r) a[i] = x;

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

  assert(AMn::mapping(AMn::id(), AMn::S(3)).mn == 3);
  assert(AMx::mapping(AMx::id(), AMx::S(3)).mx == 3);
  assert(AMnx::mapping(AMnx::id(), AMnx::S(3)).mn == 3);
}

int main()
{
  test();
  cout << "Hello World" << endl;
}
