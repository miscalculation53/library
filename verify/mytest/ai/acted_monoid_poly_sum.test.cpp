#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/acted_monoid/polyadd_sum.hpp"
#include "algebra/acted_monoid/polyupd_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"
#include "math/rational.hpp"

template <class S>
void assert_same(const S &a, const S &b)
{
  assert(a.val == b.val);
  assert(a.power_sum == b.power_sum);
}

template <class T, size_t N>
T evaluate(const array<T, N> &c, int i)
{
  T res{};
  for (int k = int(N) - 1; k >= 0; k--) res = res * T(i) + c[k];
  return res;
}

// 添字を含む集約値について、モノイド則と作用の分配・合成を確認する。
template <class AM, class T, int D>
void test_laws()
{
  using S = typename AM::S;
  using F = typename AM::F;
  const S a(0, T(3)), b(5, T(-2)), c(13, T(7)), e = AM::e();
  array<T, D + 1> f_coeff{}, g_coeff{};
  repi(k, D + 1) f_coeff[k] = T(k + 1), g_coeff[k] = T(2 - k);
  const F f(f_coeff), g(g_coeff), zero(array<T, D + 1>{});
  const S x = AM::op(a, b);
  assert_same(AM::op(AM::op(a, b), c), AM::op(a, AM::op(b, c)));
  assert_same(AM::op(e, x), x);
  assert_same(AM::op(x, e), x);
  assert_same(AM::mapping(AM::id(), x), x);
  assert_same(AM::mapping(f, e), e);
  assert_same(AM::mapping(f, x), AM::op(AM::mapping(f, a), AM::mapping(f, b)));
  assert_same(AM::mapping(AM::composition(f, g), x), AM::mapping(f, AM::mapping(g, x)));
  assert_same(AM::mapping(AM::composition(f, AM::id()), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::id(), f), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::composition(f, g), zero), x),
              AM::mapping(AM::composition(f, AM::composition(g, zero)), x));
}

// 重なる更新・点更新・空区間を混ぜ、各位置での Horner 法による素朴解と照合する。
template <class AM, class T, int D, bool assign>
void test_tree()
{
  using S = typename AM::S;
  using F = typename AM::F;
  mt19937 rng(173921 + D);
  for (int n : {0, 1, 2, 6, 17, 49})
  {
    vc<T> a(n);
    vc<S> init;
    repi(i, n)
    {
      a[i] = T(int(rng() % 21) - 10);
      init.emplace_back(i, a[i]);
    }
    LazySegmentTree<AM> seg(init);
    auto check = [&](int l, int r)
    {
      const S got = seg.prod(l, r);
      T sum{};
      array<T, D + 1> powers{};
      repi(i, l, r)
      {
        sum = sum + a[i];
        repi(k, D + 1) powers[k] = powers[k] + T(ipow<i128>(i, k));
      }
      assert(got.val == sum && got.power_sum == powers);
    };
    repi(iter, 800)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      if (iter % 11 == 0) l = 0, r = n;
      array<T, D + 1> coeff{};
      if (iter % 13 != 0)
        repi(k, D + 1) coeff[k] = T(int(rng() % 9) - 4);
      const F f(coeff);
      if (n && iter % 7 == 0)
      {
        const int p = rng() % n;
        a[p] = T(int(rng() % 21) - 10);
        seg.set(p, S(p, a[p]));
      }
      else if (iter % 9 == 0)
        seg.apply(l, r, AM::id());
      else
      {
        if (n && iter % 5 == 0)
        {
          l = rng() % n, r = l + 1;
          seg.apply(l, f);
        }
        else
          seg.apply(l, r, f);
        repi(i, l, r)
        {
          const T value = evaluate(coeff, i);
          if constexpr (assign) a[i] = value;
          else a[i] = a[i] + value;
        }
      }
      check(l, r);
      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      check(l, r);
      assert_same(seg.all_prod(), seg.prod(0, n));
    }
    const auto result = seg.content();
    repi(i, n) assert_same(result[i], S(i, a[i]));
    repi(l, n + 1) repi(r, l, n + 1) check(l, r);
  }
}

template <class T, int D>
void test_type()
{
  using Add = ActedMonoidPolyAddSum<T, D>;
  using Upd = ActedMonoidPolyUpdSum<T, D>;
  test_laws<Add, T, D>();
  test_laws<Upd, T, D>();
  test_tree<Add, T, D, false>();
  test_tree<Upd, T, D, true>();
}

// 最終結果が収まる積和の桁あふれと、使用する最高次数を超える不要な累乗を確認する。
void test_wide_intermediates()
{
  using Add = ActedMonoidPolyAddSum<ll, 1>;
  using Upd = ActedMonoidPolyUpdSum<ll, 1>;
  const array<ll, 2> f{-8'000'000'000'000'000'000LL, 4'000'000'000'000'000'000LL};
  assert(Add::mapping(f, Add::S(3, -1)).val == 3'999'999'999'999'999'999LL);
  assert(Upd::mapping(Upd::F(f), Upd::S(3, -1)).val == 4'000'000'000'000'000'000LL);
  using Small = ActedMonoidPolyAddSum<int, 1>;
  assert(Small::mapping({0, 1'000'000'000}, Small::S(3, -2'000'000'000)).val == 1'000'000'000);

  using Powers = MonoidIndexPowerSum<ll, 2>;
  const Powers::S s(1'000'000'000, 0);
  assert((s.power_sum == array<ll, 3>{1, 1'000'000'000, 1'000'000'000'000'000'000LL}));

  // h(i-2)(i-3) は i=3,4 で 0,2h。途中の積は i128 の範囲を超える。
  using WideAdd = ActedMonoidPolyAddSum<i128, 2>;
  using WideUpd = ActedMonoidPolyUpdSum<i128, 2>;
  const i128 h = i128(1) << 124;
  const array<i128, 3> wide{6 * h, -5 * h, h};
  assert(WideUpd::mapping(WideUpd::F(wide), WideUpd::S(3, 1)).val == 0);
  assert(WideAdd::mapping(wide, WideAdd::S(4, -h)).val == h);
  assert(WideUpd::mapping(WideUpd::F({-6 * h, 5 * h, -h}), WideUpd::S(4, 1)).val == -2 * h);
}

// 最小値を含む負数の復元と、小さい整数型の整数昇格を確認する。
template <class T>
void test_integer_boundaries()
{
  using AM = ActedMonoidPolyUpdSum<T, 0>;
  for (T value : {numeric_limits<T>::lowest(), numeric_limits<T>::max(), T(0), T(1)})
    assert(AM::mapping(typename AM::F({value}), typename AM::S(0, T{})).val == value);
}

void test_rational()
{
  using R = Rational<ll>;
  using AM = ActedMonoidPolyAddSum<R, 2>;
  const auto x = AM::mapping({R(1, 2), R(1, 3), R(1, 4)}, AM::S(3, R(1, 2)));
  assert(x.val == R(17, 4));
}

int main()
{
  test_type<ll, 0>();
  test_type<ll, 1>();
  test_type<ll, 2>();
  test_type<ll, 5>();
  test_type<int, 2>();
  test_type<i128, 3>();
  test_type<double, 2>();
  test_type<modint998244353, 4>();
  test_type<static_modint32<6>, 3>();
  test_wide_intermediates();
  test_integer_boundaries<signed char>();
  test_integer_boundaries<unsigned char>();
  test_integer_boundaries<short>();
  test_integer_boundaries<unsigned short>();
  test_integer_boundaries<int>();
  test_integer_boundaries<ll>();
  test_integer_boundaries<i128>();
  test_integer_boundaries<u128>();
  test_rational();
  cout << "Hello World" << endl;
}
