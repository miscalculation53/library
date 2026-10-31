#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/prefix_min_max.hpp"
#include "algebra/algebra_base.hpp"
#include "ds/segtree/segtree.hpp"

template <class T, bool include_empty>
void check(const vc<T> &a, int l, int r,
           const typename MonoidPrefixMin<T, include_empty>::S &mn,
           const typename MonoidPrefixMax<T, include_empty>::S &mx,
           const typename MonoidPrefixMinMax<T, include_empty>::S &both)
{
  T sum{}, lo{}, hi{};
  if constexpr (!include_empty) lo = resolved_infty<T>(), hi = -resolved_infty<T>();
  repi(i, l, r)
  {
    sum += a[i];
    lo = min(lo, sum), hi = max(hi, sum);
  }
  assert(mn.sum == sum && mn.mn == lo);
  assert(mx.sum == sum && mx.mx == hi);
  assert(both.sum == sum && both.mn == lo && both.mx == hi);
}

template <class T, bool include_empty>
void test()
{
  using Mn = MonoidPrefixMin<T, include_empty>;
  using Mx = MonoidPrefixMax<T, include_empty>;
  using M = MonoidPrefixMinMax<T, include_empty>;
  constexpr auto x = M::op(typename M::S(T(3)), typename M::S(T(-5)));
  static_assert(x.sum == T(-2) && x.mn == T(-2) && x.mx == T(3));
  constexpr T lo = [] { if constexpr (include_empty) return T{}; else return T(resolved_infty<T>()); }();
  constexpr T hi = [] { if constexpr (include_empty) return T{}; else return T(-resolved_infty<T>()); }();
  static_assert(Mn::e().sum == T{} && Mn::e().mn == lo);
  static_assert(Mx::e().sum == T{} && Mx::e().mx == hi);
  static_assert(M::e().sum == T{} && M::e().mn == lo && M::e().mx == hi);
  static_assert(Mn::op(typename Mn::S(T(2)), typename Mn::S(T(-3))).mn == T(-1));
  static_assert(Mx::op(typename Mx::S(T(-3)), typename Mx::S(T(2))).mx == T(include_empty ? 0 : -1));
  static_assert((typename Mn::S(T(3))).mn == T(include_empty ? 0 : 3));
  static_assert((typename Mx::S(T(-3))).mx == T(include_empty ? 0 : -3));
  static_assert(M::pow(M::e(), u128(1) << 80).mn == lo);
  static_assert(M::pow(M::e(), u128(1) << 80).mx == hi);
  static_assert(Mn::pow(Mn::e(), 17).mn == lo);
  static_assert(Mx::pow(Mx::e(), 17).mx == hi);
  static_assert(is_trivially_copyable_v<typename M::S>);
  static_assert(sizeof(typename Mn::S) == 2 * sizeof(T));
  static_assert(sizeof(typename Mx::S) == 2 * sizeof(T));
  static_assert(sizeof(typename M::S) == 3 * sizeof(T));

  typename Mn::S mn_default;
  typename Mx::S mx_default;
  typename M::S both_default;
  check<T, include_empty>({}, 0, 0, mn_default, mx_default, both_default);

  mt19937 rng(20261027);
  for (int n : {0, 1, 2, 3, 8, 19, 64})
  {
    vc<T> a(n);
    repi(i, n) a[i] = T(int(rng() % 21) - 10);
    SegmentTree<Mn> seg_mn(a);
    SegmentTree<Mx> seg_mx(a);
    SegmentTree<M> seg(a);
    repi(iter, 700)
    {
      if (n && iter % 2)
      {
        int p = rng() % n;
        a[p] = T(int(rng() % 21) - 10);
        seg_mn.set(p, a[p]), seg_mx.set(p, a[p]), seg.set(p, a[p]);
      }
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      check<T, include_empty>(a, l, r, seg_mn.prod(l, r), seg_mx.prod(l, r), seg.prod(l, r));
      check<T, include_empty>(a, 0, n, seg_mn.all_prod(), seg_mx.all_prod(), seg.all_prod());

      int p = l + rng() % (r - l + 1), q = p + rng() % (r - p + 1);
      auto u = seg.prod(l, p), v = seg.prod(p, q), w = seg.prod(q, r);
      auto lhs = M::op(M::op(u, v), w), rhs = M::op(u, M::op(v, w));
      assert(lhs.sum == rhs.sum && lhs.mn == rhs.mn && lhs.mx == rhs.mx);
      auto id_left = M::op(M::e(), lhs), id_right = M::op(lhs, M::e());
      assert(lhs.sum == id_left.sum && lhs.mn == id_left.mn && lhs.mx == id_left.mx);
      assert(lhs.sum == id_right.sum && lhs.mn == id_right.mn && lhs.mx == id_right.mx);
      auto doubled = M::op(lhs, lhs);
      assert(doubled.sum == lhs.sum + lhs.sum);
      vc<T> twice;
      repi(k, 2) repi(i, l, r) twice.push_back(a[i]);
      check<T, include_empty>(twice, 0, twice.size(), Mn::op(seg_mn.prod(l, r), seg_mn.prod(l, r)),
                             Mx::op(seg_mx.prod(l, r), seg_mx.prod(l, r)), doubled);
      int k = rng() % 7;
      vc<T> repeated;
      repi(j, k) repi(i, l, r) repeated.push_back(a[i]);
      check<T, include_empty>(repeated, 0, repeated.size(), Mn::pow(seg_mn.prod(l, r), k),
                             Mx::pow(seg_mx.prod(l, r), k), M::pow(lhs, k));
      auto wrapped = pow_monoid<OppositeMonoid<M>>(lhs, k);
      auto expected = M::pow(lhs, k);
      assert(tie(wrapped.sum, wrapped.mn, wrapped.mx) == tie(expected.sum, expected.mn, expected.mx));
      auto copied = seg;
      auto moved = std::move(copied);
      assert(moved.prod(l, r).mn == lhs.mn);

      // OppositeMonoid reads the same interval from right to left.
      auto rev = OppositeMonoid<M>::e();
      repi(i, l, r) rev = OppositeMonoid<M>::op(rev, typename M::S(a[i]));
      T rev_sum{}, rev_lo = lo, rev_hi = hi;
      for (int i = r; i-- > l;)
      {
        rev_sum += a[i];
        rev_lo = min(rev_lo, rev_sum), rev_hi = max(rev_hi, rev_sum);
      }
      assert(rev.sum == rev_sum && rev.mn == rev_lo && rev.mx == rev_hi);

      if constexpr (!include_empty)
      {
        int first_nonnegative = n, first_nonpositive = n;
        T prefix{};
        repi(i, l, n)
        {
          prefix += a[i];
          if (first_nonnegative == n && prefix >= T{}) first_nonnegative = i;
          if (first_nonpositive == n && prefix <= T{}) first_nonpositive = i;
        }
        assert(seg_mx.max_right_ok(l, [](const auto &s) { return s.mx < T{}; }) == first_nonnegative);
        assert(seg_mn.max_right_ok(l, [](const auto &s) { return s.mn > T{}; }) == first_nonpositive);
        assert(seg.max_right_ok(l, [](const auto &s) { return s.mx < T{}; }) == first_nonnegative);
        assert(seg.max_right_ok(l, [](const auto &s) { return s.mn > T{}; }) == first_nonpositive);
      }
    }
  }

  for (const vc<T> &a : vc<vc<T>>{{T(0), T(0)}, {T(1), T(2)}, {T(-1), T(-2)}, {T(3), T(-5), T(4)}})
  {
    SegmentTree<Mn> mn(a);
    SegmentTree<Mx> mx(a);
    SegmentTree<M> both(a);
    repi(l, a.size() + 1) repi(r, l, a.size() + 1)
      check<T, include_empty>(a, l, r, mn.prod(l, r), mx.prod(l, r), both.prod(l, r));
  }
}

struct OrderedAddOnly
{
  ll value{};
  constexpr OrderedAddOnly() = default;
  constexpr explicit OrderedAddOnly(ll x) : value(x) {}
  friend constexpr OrderedAddOnly operator+(const OrderedAddOnly &a, const OrderedAddOnly &b)
  { return OrderedAddOnly(a.value + b.value); }
  friend constexpr OrderedAddOnly operator-(const OrderedAddOnly &a) { return OrderedAddOnly(-a.value); }
  friend constexpr bool operator<(const OrderedAddOnly &a, const OrderedAddOnly &b) { return a.value < b.value; }
  friend constexpr bool operator==(const OrderedAddOnly &a, const OrderedAddOnly &b) { return a.value == b.value; }
};

ll custom_infty() { return 123456789; }

void test_infty_and_types()
{
  using SmallMin = MonoidPrefixMin<signed char, false, 127>;
  using SmallMax = MonoidPrefixMax<signed char, false, 127>;
  static_assert(SmallMin::op(SmallMin::S(100), SmallMin::e()).mn == 100);
  static_assert(SmallMax::op(SmallMax::S(-100), SmallMax::e()).mx == -100);
  using Large = MonoidPrefixMinMax<ll, false, numeric_limits<ll>::max()>;
  constexpr auto large_positive = Large::op(Large::S(100), Large::e());
  constexpr auto large_negative = Large::op(Large::S(-100), Large::e());
  static_assert(large_positive.sum == 100 && large_positive.mn == 100 && large_positive.mx == 100);
  static_assert(large_negative.sum == -100 && large_negative.mn == -100 && large_negative.mx == -100);
  using M = MonoidPrefixMinMax<ll, false, 123456789>;
  constexpr auto empty = M::e();
  static_assert(empty.sum == 0 && empty.mn == 123456789 && empty.mx == -123456789);
  constexpr auto empty_power = M::pow(empty, 123);
  static_assert(empty_power.mn == empty.mn && empty_power.mx == empty.mx);
  using F = MonoidPrefixMinMax<ll, false, custom_infty>;
  auto f = F::pow(F::e(), 123);
  assert(f.sum == 0 && f.mn == empty.mn && f.mx == empty.mx);
  auto value = F::op(F::S(2), F::S(-5));
  assert(value.sum == -3 && value.mn == -3 && value.mx == 2);
  auto id = F::op(value, F::e());
  assert(id.sum == value.sum && id.mn == value.mn && id.mx == value.mx);
  using C = MonoidPrefixMinMax<OrderedAddOnly, false, 1000000>;
  constexpr auto c = C::pow(C::S(OrderedAddOnly(-2)), 10);
  static_assert(c.sum.value == -20 && c.mn.value == -20 && c.mx.value == -2);
  constexpr auto ce = C::op(C::e(), C::pow(C::e(), 10));
  static_assert(ce.mn.value == 1000000 && ce.mx.value == -1000000);
  using U = MonoidPrefixMin<u128, false>;
  constexpr auto u = U::pow(U::S(u128(3)), u128(1) << 80);
  static_assert(u.sum == (u128(3) << 80) && u.mn == 3);
  static_assert(U::op(u, U::e()).mn == 3);
  using UE = MonoidPrefixMinMax<u128, true>;
  constexpr auto ue = UE::pow(UE::S(u128(3)), u128(1) << 80);
  static_assert(ue.sum == u.sum && ue.mn == 0 && ue.mx == u.sum);
  using W = MonoidPrefixMinMax<i128, false>;
  constexpr auto positive = W::pow(W::S(3), u128(1) << 80);
  constexpr auto negative = W::pow(W::S(-3), i128(1) << 80);
  static_assert(positive.sum == (i128(3) << 80) && positive.mn == 3 && positive.mx == positive.sum);
  static_assert(negative.sum == -(i128(3) << 80) && negative.mn == negative.sum && negative.mx == -3);
}

int main()
{
  test<short, true>(), test<short, false>();
  test<int, true>(), test<int, false>();
  test<ll, true>(), test<ll, false>();
  test<i128, true>(), test<i128, false>();
  test<double, true>(), test<double, false>();
  test_infty_and_types();
  using M = MonoidPrefixMinMax<i128, true>;
  constexpr i128 big = i128(1) << 90;
  constexpr auto x = M::op(M::S(big), M::S(-2 * big));
  static_assert(x.sum == -big && x.mn == -big && x.mx == big);
  cout << "Hello World" << endl;
}
