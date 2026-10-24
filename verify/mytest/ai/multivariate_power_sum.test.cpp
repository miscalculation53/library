#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/acted_monoid/add_multivariate_power_sum.hpp"
#include "algebra/acted_monoid/upd_multivariate_power_sum.hpp"
#include "ds/segtree/lazy_segtree.hpp"
#include "math/modint/modint.hpp"

template <class S, class U>
void assert_same(const S &a, const U &b)
{
  assert(a.power_sum == b.power_sum);
}

// 各単項式を各点で直接評価する。
template <class T, int... Ds>
auto naive(const vc<array<T, sizeof...(Ds)>> &a, int l, int r)
{
  using M = MonoidMultivariatePowerSum<T, Ds...>;
  typename M::S res;
  repi(k, M::size)
  {
    array<int, M::dim> exponent{};
    int index = k;
    repi(d, M::dim - 1, -1, -1)
      exponent[d] = index % (M::degree[d] + 1), index /= M::degree[d] + 1;
    assert(M::index(exponent) == k);
    repi(i, l, r)
    {
      T p(1);
      repi(d, M::dim) repi(j, exponent[d]) p = p * a[i][d];
      res.power_sum[k] = res.power_sum[k] + p;
    }
    assert(res.at(exponent) == res.power_sum[k]);
  }
  return res;
}

template <class AM>
void test_laws(const typename AM::S &a, const typename AM::S &b, const typename AM::S &c,
               const typename AM::F &f, const typename AM::F &g, const typename AM::F &h)
{
  const auto x = AM::op(a, b), e = AM::e();
  assert_same(AM::op(AM::op(a, b), c), AM::op(a, AM::op(b, c)));
  assert_same(AM::op(e, x), x);
  assert_same(AM::op(x, e), x);
  assert_same(AM::mapping(AM::id(), x), x);
  assert_same(AM::mapping(f, e), e);
  assert_same(AM::mapping(f, x), AM::op(AM::mapping(f, a), AM::mapping(f, b)));
  assert_same(AM::mapping(AM::composition(f, g), x), AM::mapping(f, AM::mapping(g, x)));
  assert_same(AM::mapping(AM::composition(f, AM::id()), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::id(), f), x), AM::mapping(f, x));
  assert_same(AM::mapping(AM::composition(AM::composition(f, g), h), x),
              AM::mapping(AM::composition(f, AM::composition(g, h)), x));
}

template <class T, int... Ds>
void test()
{
  using Affine = ActedMonoidAffineMultivariatePowerSum<T, Ds...>;
  using Add = ActedMonoidAddMultivariatePowerSum<T, Ds...>;
  using Upd = ActedMonoidUpdMultivariatePowerSum<T, Ds...>;
  using M = typename Affine::M;
  using S = typename M::S;
  using Point = array<T, M::dim>;
  mt19937 rng(567012 + M::size);
  auto point = [&]
  {
    Point x;
    repi(d, M::dim) x[d] = T(int(rng() % 7) - 3);
    return x;
  };
  auto affine = [&]
  {
    typename Affine::F f;
    repi(d, M::dim) f.b[d] = T(int(rng() % 3) - 1), f.c[d] = T(int(rng() % 5) - 2);
    return f;
  };
  auto upd = [&]
  {
    typename Upd::F f;
    repi(d, M::dim)
      if (rng() % 2) f[d] = T(int(rng() % 5) - 2);
    return f;
  };
  repi(iter, 30)
  {
    vc<Point> a{point(), point(), point()};
    const S x(a[0]), y(a[1]), z(a[2]);
    assert_same(x, (naive<T, Ds...>(a, 0, 1)));
    assert_same(M::op(M::op(x, y), z), (naive<T, Ds...>(a, 0, 3)));
    assert_same(S(Point{}), (naive<T, Ds...>(vc<Point>{Point{}}, 0, 1)));
    test_laws<Affine>(x, y, z, affine(), affine(), affine());
    test_laws<Add>(x, y, z, point(), point(), point());
    test_laws<Upd>(x, y, z, upd(), upd(), upd());
    const auto f = affine(), g = affine(), h = affine();
    const auto left = Affine::composition(Affine::composition(f, g), h);
    const auto right = Affine::composition(f, Affine::composition(g, h));
    assert(left.b == right.b && left.c == right.c);
    assert_same(Affine::mapping(typename Affine::F{}, x), x);
  }

  for (int n : {0, 1, 7, 19})
  {
    vc<Point> a(n);
    for (auto &x : a) x = point();
    LazySegmentTree<Affine> seg(a);
    repi(iter, 400)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      if (iter % 11 == 0) l = 0, r = n;
      if (n && iter % 7 == 0)
      {
        const int p = rng() % n;
        a[p] = point();
        seg.set(p, S(a[p]));
      }
      else
      {
        auto f = affine();
        if (iter % 3 == 0)
        {
          // 一つの数列だけを更新する。
          f = Affine::id();
          const int d = rng() % M::dim;
          f.b[d] = T(int(rng() % 3) - 1), f.c[d] = T(int(rng() % 5) - 2);
        }
        if (iter % 13 == 0) f = Affine::id();
        if (n && iter % 5 == 0)
        {
          l = rng() % n, r = l + 1;
          seg.apply(l, f);
        }
        else
          seg.apply(l, r, f);
        repi(i, l, r) repi(d, M::dim) a[i][d] = f.b[d] * a[i][d] + f.c[d];
      }
      assert_same(seg.prod(l, r), (naive<T, Ds...>(a, l, r)));
      assert_same(seg.all_prod(), (naive<T, Ds...>(a, 0, n)));
      if (n)
      {
        const int p = rng() % n;
        assert_same(seg.get(p), (naive<T, Ds...>(a, p, p + 1)));
      }
    }

    LazySegmentTree<Add> add_seg(a);
    LazySegmentTree<Upd> upd_seg(a);
    vc<Point> added = a, assigned = a;
    repi(iter, 150)
    {
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      const auto f = point();
      const auto g = upd();
      add_seg.apply(l, r, f);
      upd_seg.apply(l, r, g);
      repi(i, l, r) repi(d, M::dim)
      {
        added[i][d] = added[i][d] + f[d];
        if (!g[d].is_id) assigned[i][d] = g[d].val;
      }
      assert_same(add_seg.all_prod(), (naive<T, Ds...>(added, 0, n)));
      assert_same(upd_seg.all_prod(), (naive<T, Ds...>(assigned, 0, n)));
      assert_same(add_seg.prod(l, r), (naive<T, Ds...>(added, l, r)));
      assert_same(upd_seg.prod(l, r), (naive<T, Ds...>(assigned, l, r)));
    }
  }
}

void test_univariate()
{
  using T = modint998244353;
  using AM = ActedMonoidAffineMultivariatePowerSum<T, 5>;
  using Old = ActedMonoidAffinePowerSum<T, 5>;
  auto x = AM::op(AM::S({T(-2)}), AM::S({T(3)}));
  auto y = Old::op(Old::S(T(-2)), Old::S(T(3)));
  assert_same(x, y);
  const AM::F f({T(2)}, {T(3)});
  assert_same(AM::mapping(f, x), Old::mapping({T(2), T(3)}, y));
}

template <class T>
void test_wide_intermediates()
{
  using AM = ActedMonoidAffineMultivariatePowerSum<T, 2, 2>;
  const T h = numeric_limits<T>::max() / 2;
  const typename AM::S x(array<T, 2>{T(2), T(2)});
  const typename AM::F f({h, h}, {T(-2 * h), T(-2 * h)});
  assert_same(AM::mapping(f, x), typename AM::S(array<T, 2>{T(0), T(0)}));
  const auto fg = AM::composition(f, typename AM::F({T(1), T(1)}, {T(3), T(3)}));
  assert((fg.b == array<T, 2>{h, h}));
  assert((fg.c == array<T, 2>{h, h}));
  assert_same(AM::mapping(f, AM::e()), AM::e());
  // 最初の変数の更新後だけ、混合項が型の範囲を超える。
  using Mixed = ActedMonoidAffineMultivariatePowerSum<T, 1, 1>;
  assert_same(Mixed::mapping(typename Mixed::F({h, T(0)}, {T(0), T(0)}),
                             typename Mixed::S(array<T, 2>{T(2), T(2)})),
              typename Mixed::S(array<T, 2>{T(2 * h), T(0)}));
}

template <class T>
void test_boundaries()
{
  using AM = ActedMonoidAffineMultivariatePowerSum<T, 1, 0>;
  using Upd = ActedMonoidUpdMultivariatePowerSum<T, 1, 0>;
  const T lo = numeric_limits<T>::lowest(), hi = numeric_limits<T>::max();
  const typename AM::S x(array<T, 2>{lo, T(7)});
  assert_same(AM::mapping(typename AM::F({T(-1), hi}, {lo, hi}), x),
              typename AM::S(array<T, 2>{T(0), T(0)}));
  typename Upd::F f;
  f[0] = hi;
  assert_same(Upd::mapping(f, x), typename AM::S(array<T, 2>{hi, T(7)}));
  f[0] = T(0);
  assert_same(Upd::mapping(f, x), typename AM::S(array<T, 2>{T(0), T(7)}));
}

struct Semiring
{
  int val = 0;
  Semiring() {}
  explicit Semiring(int val) : val((val % 6 + 6) % 6) {}
  friend Semiring operator+(Semiring a, Semiring b) { return Semiring(a.val + b.val); }
  friend Semiring operator*(Semiring a, Semiring b) { return Semiring(a.val * b.val); }
  friend bool operator==(Semiring a, Semiring b) { return a.val == b.val; }
};

int main()
{
  test<ll, 2, 3>();
  test<int, 2, 1>();
  test<ll, 2, 1, 2>();
  test<ll, 0, 0>();
  test<ll, 0, 2, 0>();
  test<modint998244353, 3, 4>();
  test<static_modint32<6>, 2, 3, 1>();
  test<Semiring, 2, 3>();
  test_univariate();
  test_wide_intermediates<int>();
  test_wide_intermediates<ll>();
  test_wide_intermediates<i128>();
  test_boundaries<signed char>();
  test_boundaries<short>();
  test_boundaries<int>();
  test_boundaries<ll>();
  test_boundaries<i128>();
  test_boundaries<u128>();
  using Narrow = MonoidMultivariatePowerSum<unsigned short, 2, 1>;
  assert((Narrow::S({65535, 65535}).power_sum == array<unsigned short, 6>{1, 65535, 65535, 1, 1, 65535}));
  using Dynamic = dynamic_modint32<567012>;
  using AM = ActedMonoidAffineMultivariatePowerSum<Dynamic, 70, 2>;
  for (int mod : {6, 2, 998244353, 15, 1})
  {
    Dynamic::set_mod(mod);
    assert_same(AM::mapping({{4, 3}, {7, 5}}, AM::S({12, 2})), AM::S({55, 11}));
  }
  cout << "Hello World" << endl;
}
