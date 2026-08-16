#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "ds/segtree/lazy_segtree.hpp"
#include "ds/segtree/segtree_beats.hpp"
#include "algebra/acted_monoid/chmin_chmax_add_min_max_sum.hpp"
#include "algebra/acted_monoid/clamp_min_max.hpp"

constexpr ll TEST_INF = (1LL << 60);

// Test focus: clamp/add functions are closed under composition, including actions whose ranges do not intersect.
void test_clamp_composition()
{
  using M = MonoidClampAdd<ll, TEST_INF>;
  using F = M::S;
  mt19937 rng(123456789);
  repi(iter, 10000)
  {
    auto make_action = [&]()
    {
      ll x = ll(rng() % 101) - 50;
      int t = rng() % 5;
      if (t == 0)
        return F::chmin(x);
      if (t == 1)
        return F::chmax(x);
      if (t == 2)
        return F::add(x);
      if (t == 3)
        return F::set(x);
      ll y = ll(rng() % 101) - 50;
      if (x > y)
        swap(x, y);
      return F::clamp(x, y);
    };
    F f = make_action(), g = make_action();
    F fg = M::op(f, g);
    repi(_, 10)
    {
      ll x = ll(rng() % 401) - 200;
      assert(fg(x) == f(g(x)));
    }
  }
}

// Test focus: chmin/chmax/add with min/max queries needs no beats when sum is not requested.
void test_lazy_clamp_min_max()
{
  using AM = ActedMonoidClampMinMax<ll, TEST_INF>;
  using F = AM::F;
  constexpr int n = 53;
  mt19937 rng(987654321);
  vc<ll> a(n);
  repi(i, n) a[i] = ll(rng() % 101) - 50;
  LazySegmentTree<AM> seg(a);
  LazySegmentTree<ActedMonoidClampMin<ll, TEST_INF>> seg_mn(a);
  LazySegmentTree<ActedMonoidClampMax<ll, TEST_INF>> seg_mx(a);
  repi(iter, 5000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r)
      swap(l, r);
    ll x = ll(rng() % 101) - 50;
    int t = rng() % 3;
    F f = t == 0 ? F::chmin(x) : t == 1 ? F::chmax(x)
                                             : F::add(x);
    seg.apply(l, r, f);
    seg_mn.apply(l, r, f);
    seg_mx.apply(l, r, f);
    repi(i, l, r) a[i] = f(a[i]);

    l = rng() % n, r = l + 1 + rng() % (n - l);
    auto got = seg.prod(l, r);
    ll mn = *min_element(a.begin() + l, a.begin() + r);
    ll mx = *max_element(a.begin() + l, a.begin() + r);
    assert(got.mn == mn && seg_mn.prod(l, r) == mn);
    assert(got.mx == mx && seg_mx.prod(l, r) == mx);
  }
}

// Test focus: beats updates preserve range minimum, maximum, and sum through forced mapping failures.
void test_segtree_beats()
{
  using AM = ActedMonoidChminChmaxAddMinMaxSum<ll, TEST_INF>;
  using F = AM::F;
  constexpr int n = 73;
  mt19937 rng(314159265);
  vc<ll> a(n);
  repi(i, n) a[i] = ll(rng() % 201) - 100;
  SegmentTreeBeats<AM> seg(a);
  repi(iter, 10000)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r)
      swap(l, r);
    ll x = ll(rng() % 201) - 100;
    int t = rng() % 3;
    F f = t == 0 ? F::chmin(x) : t == 1 ? F::chmax(x)
                                             : F::add(x);
    seg.apply(l, r, f);
    repi(i, l, r) a[i] = f(a[i]);

    l = rng() % n, r = l + 1 + rng() % (n - l);
    auto got = seg.prod(l, r);
    ll mn = a[l], mx = a[l], sum = 0;
    repi(i, l, r) chmin(mn, a[i]), chmax(mx, a[i]), sum += a[i];
    assert(got.mn == mn);
    assert(got.mx == mx);
    assert(got.sum == sum);
  }
}

int main()
{
  test_clamp_composition();
  test_lazy_clamp_min_max();
  test_segtree_beats();
  cout << "Hello World" << endl;
}
