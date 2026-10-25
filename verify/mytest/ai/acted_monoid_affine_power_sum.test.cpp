#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/acted_monoid/affine_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

template <class S>
void assert_same(const S &a, const S &b)
{
  assert(a.power_sum == b.power_sum);
}

template <class T, int D>
void test_laws()
{
  using AM = ActedMonoidAffinePowerSum<T, D>;
  using S = typename AM::S;
  using F = typename AM::F;
  const S a(T(-2)), b(T(0)), c(T(3)), e = AM::e();
  const S x = AM::op(AM::op(a, b), c);
  const F f(T(2), T(3)), g(T(3), T(-1)), h(T(-1), T(4));
  assert_same(AM::op(AM::op(a, b), c), AM::op(a, AM::op(b, c)));
  assert_same(AM::op(e, x), x);
  assert_same(AM::op(x, e), x);
  assert_same(AM::mapping(AM::id(), x), x);
  assert_same(AM::mapping(f, e), e);
  assert_same(AM::mapping(f, AM::op(a, b)), AM::op(AM::mapping(f, a), AM::mapping(f, b)));
  assert_same(AM::mapping(AM::composition(f, g), x), AM::mapping(f, AM::mapping(g, x)));
  const F fg = AM::composition(f, g);
  assert(fg.b == T(6) && fg.c == T(1));
  assert_same(AM::mapping(AM::composition(f, AM::id()), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::id(), f), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::composition(f, g), h), x),
              AM::mapping(AM::composition(f, AM::composition(g, h)), x));
  for (const F &update : {AM::id(), F{T(1), T(5)}, F{T(-2), T(0)}, F{T(0), T(0)}, F{T(0), T(7)}})
  {
    S expected;
    for (T value : {T(-2), T(0), T(3)})
    {
      const T changed = update.b * value + update.c;
      T p(1);
      repi(k, D + 1)
      {
        expected.power_sum[k] = expected.power_sum[k] + p;
        if (k < D) p = p * changed;
      }
    }
    assert_same(AM::mapping(update, x), expected);
  }
}

// 重なる一次関数作用・点更新・空区間を素朴な配列と照合する。
template <class T, int D>
void test_tree()
{
  using AM = ActedMonoidAffinePowerSum<T, D>;
  using S = typename AM::S;
  using F = typename AM::F;
  mt19937 rng(837429 + D);
  for (int n : {0, 1, 2, 7, 24, 67})
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
    repi(iter, 900)
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
      else
      {
        F f(T(int(rng() % 3) - 1), T(int(rng() % 11) - 5));
        if (iter % 13 == 0) f = F(T(0), T(0));
        if (iter % 9 == 0) f = AM::id();
        if (n && iter % 5 == 0)
        {
          l = rng() % n, r = l + 1;
          seg.apply(l, f);
        }
        else
          seg.apply(l, r, f);
        repi(i, l, r) a[i] = f.b * a[i] + f.c;
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

// 更新係数・二項展開の中間値が型の範囲を超え、結果は収まる場合。
template <class T>
void test_wide_intermediates()
{
  using AM = ActedMonoidAffinePowerSum<T, 4>;
  const T h = numeric_limits<T>::max() / 2;
  const typename AM::F f(h, -2 * h);
  assert_same(AM::mapping(f, typename AM::S(T(2))), typename AM::S(T(0)));
  const auto composition = AM::composition(f, typename AM::F(T(1), T(3)));
  assert(composition.b == h && composition.c == h);
  assert_same(AM::mapping(typename AM::F(h, T(0)), AM::e()), AM::e());
  assert_same(AM::mapping(typename AM::F(T(0), h), AM::e()), AM::e());
}

// 加算・乗算・等値比較だけを持つ係数型。
struct Semiring
{
  int val = 0;
  Semiring() {}
  explicit Semiring(int val) : val(val % 6) {}
  friend Semiring operator+(Semiring a, Semiring b) { return Semiring(a.val + b.val); }
  friend Semiring operator*(Semiring a, Semiring b) { return Semiring(a.val * b.val); }
  friend bool operator==(Semiring a, Semiring b) { return a.val == b.val; }
};

void test_semiring()
{
  using T = Semiring;
  using AM = ActedMonoidAffinePowerSum<T, 4>;
  for (const AM::F &f : {AM::F(T(2), T(3)), AM::F(T(0), T(5)), AM::F(T(3), T(0))})
    assert_same(AM::mapping(f, AM::S(T(2))), AM::S(f.b * T(2) + f.c));
}

template <class T>
void test_integer_boundaries()
{
  using AM = ActedMonoidAffinePowerSum<T, 1>;
  const T lo = numeric_limits<T>::lowest(), hi = numeric_limits<T>::max();
  assert_same(AM::mapping(typename AM::F(T(-1), lo), typename AM::S(lo)), typename AM::S(T(0)));
  assert_same(AM::mapping(typename AM::F(T(0), hi), typename AM::S(lo)), typename AM::S(hi));
}

void test_dynamic_modulus()
{
  using T = dynamic_modint32<837429>;
  using AM = ActedMonoidAffinePowerSum<T, 70>;
  for (int mod : {6, 2, 998244353, 15, 1})
  {
    T::set_mod(mod);
    test_laws<T, 70>();
    const AM::F f(T(4), T(7));
    assert_same(AM::mapping(f, AM::S(T(12))), AM::S(T(55)));
  }
}

void test_boundary_search()
{
  using AM = ActedMonoidAffinePowerSum<ll, 2>;
  LazySegmentTree<AM> seg(vc<ll>{1, 2, 3, 4});
  seg.apply(0, 4, AM::F{1, 1});
  assert(seg.max_right_ok(0, [](const auto &s) { return s.power_sum[2] <= 13; }) == 2);
  assert(seg.min_left_ok(4, [](const auto &s) { return s.power_sum[2] <= 41; }) == 2);
  assert_same(seg.prod(1, 1), AM::e());
}

int main()
{
  test_laws<ll, 0>();
  test_laws<ll, 4>();
  test_laws<modint998244353, 4>();
  test_laws<static_modint32<6>, 70>();
  test_tree<ll, 0>();
  test_tree<ll, 4>();
  test_tree<int, 2>();
  test_tree<modint998244353, 4>();
  test_tree<static_modint32<6>, 7>();
  test_wide_intermediates<int>();
  test_wide_intermediates<ll>();
  test_wide_intermediates<i128>();
  test_integer_boundaries<signed char>();
  test_integer_boundaries<short>();
  test_integer_boundaries<int>();
  test_integer_boundaries<ll>();
  test_integer_boundaries<i128>();
  using Narrow = MonoidPowerSum<unsigned short, 2>;
  assert((Narrow::S(65535).power_sum == array<unsigned short, 3>{1, 65535, 1}));
  test_semiring();
  test_dynamic_modulus();
  test_boundary_search();
  cout << "Hello World" << endl;
}
