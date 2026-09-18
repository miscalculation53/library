#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "math/modint/modint.hpp"

// デフォルト値が零でなく、算術演算子を持たない環でも動くことを確認する。
struct EncodedRing
{
  struct S
  {
    int encoded = 99;
    bool operator==(const S &b) const { return encoded == b.encoded; }
    bool operator!=(const S &b) const { return !(*this == b); }
  };
  static S make(ll x) { return S{int((x % 101 + 101) % 101) + 7}; }
  static S e0() { return make(0); }
  static S e1() { return make(1); }
  static S add(S a, S b) { return make(a.encoded + b.encoded - 14); }
  static S minus(S a) { return make(7 - a.encoded); }
  static S mul(S a, S b) { return make((a.encoded - 7) * (b.encoded - 7)); }
};

template <class R>
vc<typename R::S> naive_product(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  vc<typename R::S> c(a.size(), R::e0());
  const int x = int(a.size()) - 1;
  for (int i = 1; i <= x; i++)
    for (int j = 1; j <= x / i; j++)
      c[i * j] = R::add(c[i * j], R::mul(a[i], b[j]));
  return c;
}

template <class R>
vc<typename R::S> prefix(const vc<typename R::S> &a)
{
  auto f = a;
  for (int i = 1; i < int(f.size()); i++) f[i] = R::add(f[i - 1], a[i]);
  return f;
}

template <int D>
void check_coordinates(ll n)
{
  using P = DirichletPrefixSum<RingAddSubMul<ll>, D>;
  // 定義から根を列挙する。大きい N は同じ根を取る区間を飛ばす。
  set<ll> expected;
  for (ll i = 1; i <= n; )
  {
    const ll x = iroot(n / i, D);
    expected.insert(x);
    const ll end = n / ipow<ll>(x, D);
    if (end == n) break;
    i = end + 1;
  }
  set<ll> called;
  P f(n, [&](ll x) { assert(called.insert(x).second); return x; });
  assert(called == expected);
  assert(f.n() == n && f.size() == int(expected.size()));
  assert(f.F(0) == 0 && f.contains(0) && !f.contains(-1));
  int index = 0;
  for (ll x : expected)
  {
    assert(f.contains(x) && f.F(x) == x);
    assert(f.index(x) == index && f.value(index) == x);
    assert(f.contains(x - 1) == (x == 1 || expected.count(x - 1)));
    if (x < LLONG_MAX) assert(f.contains(x + 1) == bool(expected.count(x + 1)));
    // Dirichlet 積・商で必要な床除算に関して閉じている。
    for (ll a : {1, 2, 3, 7, 31}) assert(f.contains(x / a));
    index++;
  }
  assert(!f.contains(iroot(n, D) + 1));
  const int k = int(iroot(n, min<ll>(ll(D) + 1, 63)));
  assert(f.to_series().n() == k);
  const auto series = f.to_series();
  for (int i = 1; i <= k; i++) assert(series.f(i) == 1);
  if (n)
  {
    auto copy = f;
    const ll x = f.value(f.size() - 1);
    copy.setF(x, -3);
    assert(copy.F(x) == -3 && f.F(x) == x);
  }
}

template <class R, int D>
void check_arithmetic(ll n, bool nonunit = false)
{
  using P = DirichletPrefixSum<R, D>;
  using S = typename R::S;
  const int x = int(iroot(n, D));
  mt19937 rng(712341U + unsigned(D));
  vc<S> a(x + 1, R::e0()), b(x + 1, R::e0());
  for (int i = 1; i <= x; i++)
  {
    a[i] = internal::multiplicative_from_integer<R>(int(rng() % 7) - 3);
    b[i] = internal::multiplicative_from_integer<R>(int(rng() % 7) - 3);
  }
  if (x) b[1] = nonunit ? internal::multiplicative_from_integer<R>(3) : R::e1();
  const auto ap = prefix<R>(a), bp = prefix<R>(b);
  const auto c = naive_product<R>(a, b), cp = prefix<R>(c);
  P f(n, [&](ll v) { return ap[v]; }), g(n, [&](ll v) { return bp[v]; });
  auto check = [&](const P &p, const vc<S> &expected)
  {
    assert(p.F(0) == R::e0());
    for (int j = 0; j < p.size(); j++) assert(p.F(p.value(j)) == expected[p.value(j)]);
  };
  const P product = f * g;
  check(product, cp);
  check(f.convolve_sparse(g), cp);
  check(product / g, ap);
  // 積の実装を使わずに作った入力からも復元する。
  P h(n, [&](ll v) { return cp[v]; });
  check(h / g, ap);
  assert(dirichlet_convolution_sum(f, g) == cp.back());
  assert(!product.is_multiplicative());
  if (x > 1000) return;
  const auto unit = P::unit(n);
  check(f * unit, ap);
  check(f / unit, ap);
  check(g * g.inv(), prefix<R>([&] { auto e = vc<S>(x + 1, R::e0()); if (x) e[1] = R::e1(); return e; }()));
  check(f.pow(3), prefix<R>(naive_product<R>(naive_product<R>(a, a), a)));
  check((f + g) - g, ap);
  check(-(-f), ap);
  check(f * R::minus(R::e1()) / R::minus(R::e1()), ap);
  auto mutate = f;
  mutate *= g;
  mutate /= g;
  check(mutate, ap);
  assert(unit.is_multiplicative() && unit.pow(0).is_multiplicative());
  assert((unit * unit).is_multiplicative() && (unit / unit).is_multiplicative());
  assert(unit.inv().to_series().is_multiplicative());
  assert(!(unit + unit).is_multiplicative());
}

template <int D>
void check_degree()
{
  using R = FieldAddSubMulDiv<modint998244353>;
  static_assert(is_default_constructible_v<DirichletPrefixSum<R, D>>);
  for (int n = 0; n <= 300; n++)
  {
    check_coordinates<D>(n);
    check_arithmetic<R, D>(n, true);
  }
  for (ll n : {511, 512, 513, 999, 1000, 1001, 10007, 99991})
  {
    check_coordinates<D>(n);
    check_arithmetic<RingAddSubMul<ll>, D>(n);
    check_arithmetic<EncodedRing, D>(n);
  }
}

int main()
{
  check_degree<1>();
  check_degree<2>();
  check_degree<3>();
  check_degree<4>();
  check_degree<5>();
  // 区間集約を通るサイズで、全係数の通常の Dirichlet 積と比較する。
  using R = FieldAddSubMulDiv<modint998244353>;
  for (ll n : {9999999999LL, 10000000000LL, 10000000001LL}) check_arithmetic<R, 2>(n, true);
  check_arithmetic<R, 3>(1000000000000000LL, true);
  check_arithmetic<R, 4>(1000000000000000000LL, true);
  check_arithmetic<RingAddSubMul<ll>, 2>(100000000LL, true); // f(1)=3 による割り切れる商
  check_coordinates<2>(1000000000000000000LL);
  {
    DirichletPrefixSum<RingAddSubMul<ll>, 2> f(LLONG_MAX, zeta_prefix_sum);
    const ll root = iroot(LLONG_MAX, 2);
    assert(f.F(root) == root && !f.contains(root + 1));
    for (int j = 1; j <= 1000; j++)
    {
      const ll x = iroot(LLONG_MAX / j, 2);
      assert(f.value(f.index(x)) == x && f.F(x) == x);
    }
  }
  check_coordinates<3>(LLONG_MAX);
  check_coordinates<62>(LLONG_MAX);
  check_coordinates<63>(LLONG_MAX);
  check_coordinates<100>(LLONG_MAX);
  check_coordinates<INT_MAX>(LLONG_MAX);
  check_arithmetic<R, 62>(LLONG_MAX, true);
  check_arithmetic<R, 63>(LLONG_MAX, true);
  check_arithmetic<R, INT_MAX>(LLONG_MAX, true);
  PRINT("Hello World");
}
