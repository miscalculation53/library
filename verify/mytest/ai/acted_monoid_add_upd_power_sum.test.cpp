#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/acted_monoid/add_power_sum.hpp"
#include "algebra/acted_monoid/upd_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

template <class S>
void assert_same(const S &a, const S &b)
{
  assert(a.power_sum == b.power_sum);
}

template <class T, int D, bool assign>
void test()
{
  using AM = conditional_t<assign, ActedMonoidUpdPowerSum<T, D>, ActedMonoidAddPowerSum<T, D>>;
  using Affine = ActedMonoidAffinePowerSum<T, D>;
  using S = typename AM::S;
  using F = typename AM::F;
  const S x = AM::op(S(T(-2)), S(T(3))), e = AM::e();
  const F f(T(3)), g(T(-2)), zero(T(0));
  assert_same(AM::mapping(AM::id(), x), x);
  assert_same(AM::mapping(f, e), e);
  assert_same(AM::mapping(f, x), AM::op(AM::mapping(f, S(T(-2))), AM::mapping(f, S(T(3)))));
  assert_same(AM::mapping(AM::composition(f, g), x), AM::mapping(f, AM::mapping(g, x)));
  assert_same(AM::mapping(AM::composition(f, AM::id()), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::id(), f), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::composition(f, g), zero), x),
              AM::mapping(AM::composition(f, AM::composition(g, zero)), x));
  assert_same(AM::mapping(f, x), Affine::mapping({T(assign ? 0 : 1), T(3)}, x));
  if constexpr (assign)
  {
    assert_same(AM::mapping(F{}, x), x);
    assert_same(AM::mapping(F(true, T(123)), x), x);
    assert_same(AM::mapping(zero, x), AM::op(S(T(0)), S(T(0))));
    assert(!zero.is_id && AM::id().is_id);
    assert(AM::composition(f, g).val == T(3));
  }
  else
  {
    assert(AM::composition(f, g) == T(1));
    assert(AM::id() == T(0));
  }

  mt19937 rng(470132 + D + assign);
  for (int n : {0, 1, 7, 24})
  {
    vc<T> a(n);
    repi(i, n) a[i] = T(int(rng() % 21) - 10);
    LazySegmentTree<AM> seg(a);
    auto expected = [&](int l, int r)
    {
      S res;
      repi(i, l, r)
      {
        T p(1);
        repi(k, D + 1)
        {
          res.power_sum[k] = res.power_sum[k] + p;
          if (k < D) p = p * a[i];
        }
      }
      return res;
    };
    repi(iter, 700)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      if (iter % 11 == 0) l = 0, r = n;
      if (n && iter % 7 == 0)
      {
        const int p = rng() % n;
        a[p] = T(int(rng() % 21) - 10);
        seg.set(p, S(a[p]));
      }
      else if (iter % 9 == 0)
        seg.apply(l, r, AM::id());
      else
      {
        const T value = iter % 13 == 0 ? T(0) : T(int(rng() % 11) - 5);
        if (n && iter % 5 == 0)
        {
          l = rng() % n, r = l + 1;
          seg.apply(l, F(value));
        }
        else
          seg.apply(l, r, F(value));
        repi(i, l, r)
        {
          if constexpr (assign) a[i] = value;
          else a[i] = a[i] + value;
        }
      }
      assert_same(seg.prod(l, r), expected(l, r));
      assert_same(seg.all_prod(), expected(0, n));
      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      assert_same(seg.prod(l, r), expected(l, r));
      if (n)
      {
        const int p = rng() % n;
        assert_same(seg.get(p), expected(p, p + 1));
      }
    }
  }
}

template <class T, int D>
void test_type()
{
  test<T, D, false>();
  test<T, D, true>();
}

template <class T>
void test_integer_boundaries()
{
  using Upd = ActedMonoidUpdPowerSum<T, 1>;
  for (T value : {numeric_limits<T>::lowest(), numeric_limits<T>::max(), T(0)})
  {
    assert_same(Upd::mapping(typename Upd::F(value), typename Upd::S(T(0))), typename Upd::S(value));
    assert_same(Upd::mapping(typename Upd::F(value), Upd::e()), Upd::e());
  }
}

int main()
{
  test_type<ll, 0>();
  test_type<ll, 4>();
  test_type<int, 2>();
  test_type<modint998244353, 4>();
  test_type<static_modint32<6>, 7>();
  test_type<static_modint32<6>, 70>();
  using Add = ActedMonoidAddPowerSum<ll, 2>;
  assert_same(Add::mapping(-4'000'000'000LL, Add::S(2'000'000'000LL)), Add::S(-2'000'000'000LL));
  using Wide = ActedMonoidAddPowerSum<i128, 2>;
  const i128 value = i128(1) << 63;
  assert_same(Wide::mapping(-2 * value, Wide::S(value)), Wide::S(-value));
  test_integer_boundaries<signed char>();
  test_integer_boundaries<short>();
  test_integer_boundaries<int>();
  test_integer_boundaries<ll>();
  test_integer_boundaries<i128>();
  test_integer_boundaries<u128>();
  cout << "Hello World" << endl;
}
