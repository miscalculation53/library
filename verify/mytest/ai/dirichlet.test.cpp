#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "math/modint/modint.hpp"
#include "algebra/polynomial_ring.hpp"
#include "algebra/bit.hpp"

// 通常の整数環と、逆元を持つ modint の構造を明示する。
template <class T>
using TestRing = conditional_t<is_integral_ext<T>, RingAddSubMul<T>, FieldAddSubMulDiv<T>>;

// 商集合だけで構築し、getter は const かどうかによらず読取り専用。
using PrefixLL = DirichletPrefixSum<RingAddSubMul<ll>>;
static_assert(is_default_constructible_v<PrefixLL>);
static_assert(is_constructible_v<PrefixLL, ll>);
static_assert(is_constructible_v<PrefixLL, ll, decltype(zeta_prefix_sum)>);
static_assert(!is_constructible_v<PrefixLL, ll, int>);
static_assert(!is_constructible_v<PrefixLL, ll, int, decltype(zeta_prefix_sum)>);
static_assert(!is_constructible_v<PrefixLL, const DirichletSeries<RingAddSubMul<ll>> &, ll, decltype(zeta_prefix_sum)>);
static_assert(!is_constructible_v<PrefixLL, ll, const DirichletSeries<RingAddSubMul<ll>> &, decltype(zeta_prefix_sum)>);
static_assert(is_invocable_v<decltype(&PrefixLL::unit), ll>);
static_assert(is_same_v<decltype(declval<PrefixLL &>().F(1)), const ll &>);
static_assert(is_same_v<decltype(declval<const PrefixLL &>().F(1)), const ll &>);
static_assert(!is_assignable_v<decltype(declval<PrefixLL &>().n()), ll>);
static_assert(!is_assignable_v<decltype(declval<PrefixLL &>().is_multiplicative()), bool>);
static_assert(is_same_v<decltype(declval<DirichletSeries<RingAddSubMul<ll>> &>().f(1)), const ll &>);
static_assert(is_same_v<decltype(declval<const DirichletSeries<RingAddSubMul<ll>> &>().f(1)), const ll &>);

// 篩や素因数分解を使わず、定義から積を計算する。
template <class T>
vc<T> naive_product(const vc<T> &a, const vc<T> &b)
{
  vc<T> c(a.size(), T(0));
  for (int n = 1; n < int(a.size()); n++)
    for (int d = 1; d <= n; d++)
      if (n % d == 0)
        c[n] += a[d] * b[n / d];
  return c;
}

template <class T>
vc<T> prefix(const vc<T> &a)
{
  vc<T> s(a.size(), T(0));
  for (int i = 1; i < int(a.size()); i++)
    s[i] = s[i - 1] + a[i];
  return s;
}

template <class R>
void check_prefix(const DirichletPrefixSum<R> &a, const vc<typename R::S> &f)
{
  using T = typename R::S;
  vc<T> s(f.size(), R::e0());
  for (int i = 1; i < int(f.size()); i++)
    s[i] = R::add(s[i - 1], f[i]);
  assert(a.n() == ll(f.size()) - 1);
  assert(a.F(0) == R::e0());
  const int root = int(iroot(a.n(), 2));
  vc<bool> valid(f.size(), false);
  for (int i = 0; i <= root; i++)
    valid[i] = true;
  for (ll i = 1; i <= a.n() + 1; i++)
  {
    valid[a.n() / i] = true;
    assert(a.F(a.n() / i) == s[a.n() / i]);
  }
  int count = 0;
  for (int x = 0; x <= a.n(); x++)
  {
    assert(a.contains(x) == valid[x]);
    if (valid[x])
      assert(a.F(x) == s[x]);
    if (x >= 1 && valid[x])
    {
      assert(a.index(x) == count);
      assert(a.value(count) == x);
      count++;
    }
  }
  assert(a.size() == count);
  assert(!a.contains(-1) && !a.contains(a.n() + 1));
  assert(a.F(a.n()) == s.back());
  for (int i = 1; i <= root; i++)
    assert(R::add(a.F(i), R::minus(a.F(i - 1))) == f[i]);
  assert(a.to_series().to_vector() == vc<T>(f.begin(), f.begin() + root + 1));
}

// reserve 前に、配列から乗法的だと指定した商を作る。
void test_first_division()
{
  using R = RingAddSubMul<ll>;
  const int n = 73;
  vc<ll> id(n + 1), zeta(n + 1, 1);
  zeta[0] = 0;
  for (int i = 1; i <= n; i++)
    id[i] = i;
  const auto phi = DirichletSeries<R>(id, true) / DirichletSeries<R>(zeta, true);
  for (int i = 1; i <= n; i++)
  {
    ll expected = 0;
    for (int j = 1; j <= i; j++)
      expected += gcd(i, j) == 1;
    assert(phi.f(i) == expected);
  }
}

template <class T>
void test_products()
{
  using R = TestRing<T>;
  mt19937 rng(712341);
  for (int n : {0, 1, 2, 3, 4, 8, 9, 16, 25, 49, 97, 257, 19, 400, 1})
  {
    vc<DirichletSeries<R>> fs;
    // ゼロを含む素べき値も使い、値による除算に依存しないことを確認する。
    fs.emplace_back(n, [](const auto &q) { return int((q.p + 3 * q.e) % 7) - 3; });
    fs.emplace_back(n, mobius_primepower);
    fs.emplace_back(n, divisor_count_primepower);
    fs.emplace_back(n, zeta_primepower, true);
    vc<T> random(n + 1);
    for (int i = 1; i <= n; i++)
      random[i] = T(int(rng() % 7) - 3);
    fs.emplace_back(random);
    fs.emplace_back(n);
    for (const auto &a : fs)
      for (const auto &b : fs)
      {
        const auto c = naive_product(a.to_vector(), b.to_vector());
        assert((a * b).to_vector() == c);
        const auto sparse = a.convolve_sparse(b);
        assert(sparse.to_vector() == c);
        assert(sparse.is_multiplicative() == (a.is_multiplicative() && b.is_multiplicative()));
        assert((a * b).is_multiplicative() == (a.is_multiplicative() && b.is_multiplicative()));
        assert((DirichletSeries<R>(a.to_vector()) * DirichletSeries<R>(b.to_vector())).to_vector() == c);
        if (b.is_multiplicative())
          assert((DirichletSeries<R>(c, a.is_multiplicative()) / b).to_vector() == a.to_vector());
      }
    const auto a = fs[0];
    assert(a.pow(0).to_vector() == DirichletSeries<R>::unit(n).to_vector());
    assert(a.pow(1).to_vector() == a.to_vector());
    assert(a.pow(3).to_vector() == naive_product(naive_product(a.to_vector(), a.to_vector()), a.to_vector()));
    assert((a * a.inv()).to_vector() == DirichletSeries<R>::unit(n).to_vector());
    auto b = a;
    b *= b;
    assert(b.to_vector() == naive_product(a.to_vector(), a.to_vector()));
    b = a;
    b /= b;
    assert(b.to_vector() == DirichletSeries<R>::unit(n).to_vector());
    assert((-a + a).to_vector() == DirichletSeries<R>(n).to_vector());
    assert((T(3) * a).to_vector() == (a + a + a).to_vector());
    assert(!(a + a).is_multiplicative() && !(-a).is_multiplicative());
  }
}

template <class T>
void test_arbitrary_division(T leading)
{
  using R = TestRing<T>;
  mt19937 rng(99173);
  for (int n : {1, 2, 8, 35, 128, 257})
  {
    vc<T> a(n + 1), b(n + 1);
    for (int i = 1; i <= n; i++)
      a[i] = T(int(rng() % 5) - 2), b[i] = T(int(rng() % 5) - 2);
    a[1] = leading;
    const auto c = naive_product(a, b);
    assert((DirichletSeries<R>(c) / DirichletSeries<R>(a)).to_vector() == b);
    if (leading == T(1) || leading == T(-1) || !is_integral_v<T>)
      assert(naive_product(a, DirichletSeries<R>(a).inv().to_vector()) == DirichletSeries<R>::unit(n).to_vector());
    const auto ap = prefix(a), bp = prefix(b), cp = prefix(c);
    {
      DirichletPrefixSum<R> A(n, [&](ll x) { return ap[x]; });
      DirichletPrefixSum<R> B(n, [&](ll x) { return bp[x]; });
      DirichletPrefixSum<R> C(n, [&](ll x) { return cp[x]; });
      check_prefix(A * B, c);
      check_prefix(A.convolve_sparse(B), c);
      check_prefix(C / A, b);
      assert(dirichlet_convolution_sum(A, B) == cp[n]);
      if (leading == T(1) || leading == T(-1) || !is_integral_v<T>)
        check_prefix(A * A.inv(), DirichletSeries<R>::unit(n).to_vector());
      auto D = A;
      D *= B;
      D /= A;
      check_prefix(D, b);
    }
  }
}

template <class T>
void test_prefix()
{
  using R = TestRing<T>;
  for (int n : {0, 1, 2, 3, 8, 9, 15, 16, 24, 25, 26, 97, 257})
  {
    const auto one = DirichletSeries<R>(n, zeta_primepower);
    const auto id = DirichletSeries<R>(n, id_primepower);
    const auto mu = DirichletSeries<R>(n, mobius_primepower);
    const auto phi = DirichletSeries<R>(n, totient_primepower);
    {
      int calls = 0;
      DirichletPrefixSum<R> A(n, [&](ll x)
      {
        assert(x >= 1);
        calls++;
        return T(x);
      }, true);
      assert(calls == A.size());
      DirichletPrefixSum<R> B(n, id_prefix_sum, true);
      check_prefix(A, one.to_vector());
      check_prefix(B, id.to_vector());
      check_prefix(A.inv(), mu.to_vector());
      check_prefix(B / A, phi.to_vector());
      check_prefix(A * B, naive_product(one.to_vector(), id.to_vector()));
      check_prefix(A.pow(3), naive_product(naive_product(one.to_vector(), one.to_vector()), one.to_vector()));
      check_prefix(A.pow(0), DirichletSeries<R>::unit(n).to_vector());
      check_prefix(A.pow(1), one.to_vector());
      auto C = A;
      C /= C;
      check_prefix(C, DirichletSeries<R>::unit(n).to_vector());
      C = A;
      C -= C;
      check_prefix(C, vc<T>(n + 1, T(0)));
      C = A;
      C *= C;
      check_prefix(C, naive_product(one.to_vector(), one.to_vector()));
      check_prefix(-A + A, vc<T>(n + 1, T(0)));
      check_prefix((T(5) * B) / T(5), id.to_vector());
      if (n > 0)
      {
        C = A;
        C *= C.F(1);
        check_prefix(C, one.to_vector());
        C.setF(n, T(123));
        assert(C.F(n) == T(123) && !C.is_multiplicative());
      }
      DirichletPrefixSum<R> E(n, e_prefix_sum);
      assert(dirichlet_convolution_sum(B, E) == B.F(n));
      DirichletPrefixSum<R> populated(n);
      for (int x = 1; x <= n; x++)
        if (populated.contains(x))
          populated.setF(x, T(x));
      check_prefix(populated, one.to_vector());
    }
  }
}

// 除算を持たない係数環でも積・非負整数乗は利用できる。
struct Dual
{
  ll a, b;
  Dual(ll a = 0, ll b = 0) : a(a), b(b) {}
  Dual &operator+=(const Dual &x) { a += x.a; b += x.b; return *this; }
  Dual &operator-=(const Dual &x) { a -= x.a; b -= x.b; return *this; }
  Dual &operator*=(const Dual &x) { return *this = Dual(a * x.a, a * x.b + b * x.a); }
  Dual operator-() const { return {-a, -b}; }
  friend Dual operator+(Dual x, const Dual &y) { return x += y; }
  friend Dual operator-(Dual x, const Dual &y) { return x -= y; }
  friend Dual operator*(Dual x, const Dual &y) { return x *= y; }
  friend bool operator==(const Dual &x, const Dual &y) { return x.a == y.a && x.b == y.b; }
  friend bool operator!=(const Dual &x, const Dual &y) { return !(x == y); }
};

void test_coefficient_ring()
{
  const int n = 150;
  vc<Dual> a(n + 1);
  for (int i = 1; i <= n; i++)
  {
    int x = i, omega = 0;
    for (int d = 2; d <= x; d++)
      if (x % d == 0)
      {
        omega++;
        while (x % d == 0)
          x /= d;
      }
    a[i] = Dual(1, omega);
  }
  const auto ap = prefix(a);
  DirichletSeries<RingAddSubMul<Dual>> A(n, [](const auto &) { return Dual(1, 1); });
  assert(A.to_vector() == a);
  DirichletPrefixSum<RingAddSubMul<Dual>> B(n, [&](ll x) { return ap[x]; }, true);
  check_prefix(B.pow(2), naive_product(a, a));
  assert(A.pow(2).to_vector() == naive_product(a, a));
  DirichletSeries<RingAddSubMul<Dual>> C(a);
  assert((C * A).to_vector() == naive_product(a, a));
  assert((C * C).to_vector() == naive_product(a, a));
}

void test_updates_and_conversions()
{
  {
    DirichletSeries<RingAddSubMul<ll>> series(36, zeta_primepower);
    const auto &read = series;
    assert(read.f(6) == 1 && series.is_multiplicative());
    auto value = series.f(6);
    const auto constant = series.f(6);
    const auto &reference = series.f(6);
    assert(value == 1 && constant == 1 && reference == 1 && series.f(6) == 1);
    assert(series.is_multiplicative());
    value += 4;
    assert(series.f(6) == 1 && series.is_multiplicative());
    series.set_f(6, value);
    assert(!series.is_multiplicative());
    assert(read.f(6) == 5);
    assert((series * series).to_vector() == naive_product(series.to_vector(), series.to_vector()));
    PrefixLL sums(36, zeta_prefix_sum, true);
    const auto &read_sums = sums;
    assert(read_sums.F(36) == 36 && sums.is_multiplicative());
    auto sum = sums.F(2);
    const auto constant_sum = sums.F(2);
    const auto &reference_sum = sums.F(2);
    assert(sum == 2 && constant_sum == 2 && reference_sum == 2 && sums.F(2) == 2);
    assert(sums.is_multiplicative());
    sum += 5;
    assert(sums.F(2) == 2 && sums.is_multiplicative());
    sums.setF(2, sum);
    assert(!sums.is_multiplicative());
    const auto coefficients = sums.to_series();
    assert(coefficients.f(2) == 6 && coefficients.f(3) == -4);
    assert(read_sums.F(0) == 0 && sums.index(36) == sums.size() - 1);
  }
  vc<PrefixLL> pending(3);
  for (const auto &a : pending) assert(a.n() == 0 && a.size() == 0 && a.F(0) == 0);
  pending[1] = PrefixLL(16, zeta_prefix_sum, true);
  assert(pending[1].F(16) == 16);
  const int n = 36, k = 6;
  vc<ll> values(n + 1, 1);
  values[0] = 0;
  DirichletSeries<RingAddSubMul<ll>> a(n, zeta_primepower);
  a.set_f(2, 6);
  values[2] = 6;
  assert(!a.is_multiplicative());
  assert((a * a).to_vector() == naive_product(values, values));
  auto copied = a.to_vector();
  copied[2] = 123;
  assert(a.f(2) == 6 && a.n() == n);
  const auto moved = move(a).to_vector();
  assert(moved == values);
  assert(a.n() == 0 && a.f(0) == 0);

  DirichletPrefixSum<RingAddSubMul<ll>> b(n, zeta_prefix_sum, true);
  b.setF(2, 7);
  values[3] = -4;
  assert(!b.is_multiplicative());
  check_prefix(b, values);
  check_prefix(b * b, naive_product(values, values));
  auto series = b.to_series();
  assert(series.n() == k && !series.is_multiplicative());
  series.set_f(2, 123);
  assert(b.F(2) - b.F(1) == 6 && b.F(2) == 7);
}

void test_boundaries_and_large_n()
{
  using R = RingAddSubMul<ll>;
  assert((DirichletSeries<R>() * DirichletSeries<R>()).to_vector() == vc<ll>{0});
  assert((DirichletSeries<R>() / DirichletSeries<R>()).to_vector() == vc<ll>{0});
  assert(DirichletSeries<R>().inv().to_vector() == vc<ll>{0});
  assert((DirichletSeries<R>(vc<ll>{42}) * DirichletSeries<R>(vc<ll>{99})).to_vector() == vc<ll>{0});
  assert(DirichletSeries<RingAddSubMul<ll>>(vc<ll>{}).n() == 0);
  auto a = DirichletSeries<RingAddSubMul<ll>>::unit(1);
  a.set_f(1, 3);
  assert(a.f(1) == 3 && !a.is_multiplicative());
  a *= a.f(1);
  assert(a.f(1) == 9);
  // pow(1) で不要な自乗をすると符号付き整数が溢れる。
  const DirichletSeries<RingAddSubMul<ll>> huge(vc<ll>{0, LLONG_MAX});
  assert(huge.pow(1).f(1) == LLONG_MAX);
  const DirichletPrefixSum<RingAddSubMul<ll>> huge_sum(1, [](ll) { return LLONG_MAX; });
  assert(huge_sum.pow(1).F(1) == LLONG_MAX);
  using mint = modint998244353;
  assert(id_prefix_sum(LLONG_MAX, FieldAddSubMulDiv<mint>{}) == mint(i128(LLONG_MAX) * (i128(LLONG_MAX) + 1) / 2));
  assert(e_prefix_sum(0, RingAddSubMul<ll>{}) == 0 && e_prefix_sum(1, RingAddSubMul<ll>{}) == 1);
  const ll n = 10'000'000'019LL;
  DirichletPrefixSum<FieldAddSubMulDiv<mint>> id(n, id_prefix_sum);
  auto unit = DirichletPrefixSum<FieldAddSubMulDiv<mint>>::unit(n);
  assert(dirichlet_convolution_sum(id, unit) == id_prefix_sum(n, FieldAddSubMulDiv<mint>{}));
  assert(id.F(n / (n + 1)) == mint(0));
}

// 既定構築は零でなく、整数の構築も環への埋め込みでない。算術演算子を持たない要素型。
struct EncodedField
{
  struct S
  {
    int code = 93;
    S() = default;
    explicit S(int code) : code(code) {}
    friend bool operator==(S a, S b) { return a.code == b.code; }
    friend bool operator!=(S a, S b) { return !(a == b); }
  };
  static S encode(ll a) { return S(int((a % 101 + 101) % 101) + 17); }
  static int decode(S a) { return a.code - 17; }
  static S e0() { return encode(0); }
  static S e1() { return encode(1); }
  static S add(S a, S b) { return encode(decode(a) + decode(b)); }
  static S minus(S a) { return encode(-decode(a)); }
  static S mul(S a, S b) { return encode(decode(a) * decode(b)); }
  static S inv(S a)
  {
    for (int i = 1; i < 101; i++)
      if (decode(a) * i % 101 == 1)
        return encode(i);
    assert(false);
    return e0();
  }
};

void test_ring_operations()
{
  using R = EncodedField;
  const int n = 81;
  vc<R::S> a(n + 1, R::e0()), b(a), c(a), ap(a), bp(a), cp(a);
  for (int i = 1; i <= n; i++)
    a[i] = R::encode(i % 11 - 5), b[i] = R::encode((i * 3) % 13 - 6);
  b[1] = R::encode(7);
  for (int i = 1; i <= n; i++)
  {
    ll expected = 0;
    for (int d = 1; d <= i; d++)
      if (i % d == 0)
        expected += R::decode(a[d]) * R::decode(b[i / d]);
    c[i] = R::encode(expected);
    ap[i] = R::encode(R::decode(ap[i - 1]) + R::decode(a[i]));
    bp[i] = R::encode(R::decode(bp[i - 1]) + R::decode(b[i]));
    cp[i] = R::encode(R::decode(cp[i - 1]) + R::decode(c[i]));
  }
  assert((DirichletSeries<R>(a) * DirichletSeries<R>(b)).to_vector() == c);
  assert((DirichletSeries<R>(c) / DirichletSeries<R>(b)).to_vector() == a);
  DirichletPrefixSum<R> A(n, [&](ll x) { return ap[x]; });
  DirichletPrefixSum<R> B(n, [&](ll x) { return bp[x]; });
  DirichletPrefixSum<R> C(n, [&](ll x) { return cp[x]; });
  check_prefix(A * B, c);
  check_prefix(C / B, a);
  check_prefix((R::encode(7) * A) / R::encode(7), a);
  DirichletPrefixSum<R> one(n, zeta_prefix_sum, true), id(n, id_prefix_sum, true);
  auto values = enumerate_multiplicative<R>(n, id_primepower);
  for (int i = 1; i <= n; i++)
    assert(values[i] == R::encode(i));
  check_prefix(id, values);
  assert(one.F(n) == R::encode(n));
  check_prefix(DirichletPrefixSum<R>(n), vc<R::S>(n + 1, R::e0()));
}

void test_polynomial_and_bit_rings()
{
  using R = PolynomialRingArray<RingAddSubMul<ll>, 3>;
  const int n = 64;
  DirichletSeries<R> a(n, [](const auto &q) { return R::S{1, q.e, 0}; });
  vc<R::S> expected(n + 1, R::e0()), sums(expected);
  // 係数ごとの積を直接計算して比較する。
  for (int i = 1; i <= n; i++)
  {
    for (int d = 1; d <= i; d++)
      if (i % d == 0)
        for (int u = 0; u < 3; u++)
          for (int v = 0; u + v < 3; v++)
            expected[i][u + v] += a.f(d)[u] * a.f(i / d)[v];
    for (int j = 0; j < 3; j++)
      sums[i][j] = sums[i - 1][j] + a.f(i)[j];
  }
  assert((a * a).to_vector() == expected);
  assert((a * a.inv()).to_vector() == DirichletSeries<R>::unit(n).to_vector());
  DirichletPrefixSum<R> A(n, [&](ll x) { return sums[x]; }, true);
  check_prefix(A * A, expected);
  check_prefix((A * A) / A, a.to_vector());
  check_prefix((-A) / (-DirichletPrefixSum<R>::unit(n)), a.to_vector());
  assert(pow_primepower(1)(PrimePower<ll>(2, 62, 1LL << 62), R{}) == (R::S{1LL << 62, 0, 0}));

  using Z = PolynomialRingArray<RingAddSubMul<ll>, 0>;
  DirichletPrefixSum<Z> zero(n, zeta_prefix_sum, true);
  assert((zero / zero).F(n) == Z::e0()); // 零環では 0 = 1。

  using B = RingXorAnd<ull>;
  DirichletSeries<B> one(n, zeta_primepower);
  const auto tau = one * one;
  const auto id = enumerate_multiplicative<B>(n, id_primepower);
  DirichletPrefixSum<B> prefix_one(n, zeta_prefix_sum, true);
  check_prefix(prefix_one * prefix_one, tau.to_vector());
  for (int i = 1; i <= n; i++)
  {
    int divisors = 0;
    for (int d = 1; d <= i; d++)
      divisors += i % d == 0;
    assert(tau.f(i) == (divisors % 2 ? ULLONG_MAX : 0));
    assert(id[i] == (i % 2 ? ULLONG_MAX : 0));
  }
}

// 非零率を変え、疎な積の自動選択と直接呼び出しを同じ素朴な積と照合する。
template <class R>
void test_sparse_convolution()
{
  using S = typename R::S;
  mt19937 rng(7813451);
  for (int n : {0, 1, 2, 15, 16, 17, 99, 257})
    for (int density : {1, 3, 20})
  {
    vc<S> a(n + 1, R::e0()), b(a), c(a);
    for (int i = 1; i <= n; i++)
    {
      if (rng() % density == 0) a[i] = internal::multiplicative_from_integer<R>(int(rng() % 7) - 3);
      if (rng() % density == 0) b[i] = internal::multiplicative_from_integer<R>(int(rng() % 7) - 3);
      for (int d = 1; d <= i; d++) if (i % d == 0)
        c[i] = R::add(c[i], R::mul(a[d], b[i / d]));
    }
    const DirichletSeries<R> A(a), B(b);
    assert(A.convolve(B).to_vector() == c);
    assert(A.convolve_sparse(B).to_vector() == c);
    auto ap = a, bp = b;
    for (int i = 1; i <= n; i++)
      ap[i] = R::add(ap[i - 1], a[i]), bp[i] = R::add(bp[i - 1], b[i]);
    {
      const DirichletPrefixSum<R> PA(n, [&](ll x) { return ap[x]; });
      const DirichletPrefixSum<R> PB(n, [&](ll x) { return bp[x]; });
      check_prefix(PA.convolve(PB), c);
      check_prefix(PA.convolve_sparse(PB), c);
      const DirichletPrefixSum<R> zero(n);
      check_prefix(PA * zero, vc<S>(n + 1, R::e0()));
      check_prefix(zero.convolve_sparse(PB), vc<S>(n + 1, R::e0()));
    }
  }
}

// 区間をまとめる分岐を通る大きさで、全保持位置を直接の双曲線分割と比較する。
template <class R>
void test_grouped_convolution(ll n)
{
  const int k = int(iroot(n, 2));
  mt19937 rng(418621);
  const auto random_value = [&](ll)
  { return internal::multiplicative_from_integer<R>(int(rng() % 101) - 50); };
  const DirichletPrefixSum<R> a(n, random_value), b(n, random_value);
  const auto result = a * b, expected = a.convolve_sparse(b);
  assert(result.size() == a.size() && !result.is_multiplicative());
  for (int x = 0; x <= k; x++) assert(result.F(x) == expected.F(x));
  for (ll i = 1; i <= n / (ll(k) + 1); i++) assert(result.F(n / i) == expected.F(n / i));
}

// 大きい入力でも、独立した積の実装から元の任意の累積和を復元できる。
template <class R>
void test_grouped_division(ll n, typename R::S leading)
{
  mt19937 rng(814752);
  const auto random_value = [&](ll)
  { return internal::multiplicative_from_integer<R>(int(rng() % 31) - 15); };
  const DirichletPrefixSum<R> a(n, random_value);
  DirichletPrefixSum<R> b(n, random_value);
  b.setF(1, leading);
  const auto result = (a * b) / b;
  for (int i = 0; i < a.size(); i++)
  {
    const ll x = a.value(i);
    assert(result.F(x) == a.F(x));
  }
  // 整数のランダムな逆数は巨大になるため、有限環で全位置を検証する。
  if constexpr (!internal::dirichlet_divisor<R>::exact_integer)
  {
    const auto identity = b * b.inv();
    for (int i = 0; i < b.size(); i++) assert(identity.F(b.value(i)) == R::e1());
  }
}

void test_quotient_indices()
{
  // 除算の等値区間を降順に列挙し、添字の閉じた式とは独立に照合する。
  for (ll n : {0LL, 1LL, 2LL, 99LL, 100LL, 109LL, 110LL, 120LL, 121LL,
                999'999'999'999LL, 1'000'000'000'000LL, 1'000'000'000'001LL})
  {
    const PrefixLL a(n);
    int remaining = a.size();
    for (ll l = 1; l <= n; )
    {
      const ll x = n / l, r = n / x;
      assert(a.value(--remaining) == x && a.index(x) == remaining);
      assert(a.contains(x));
      if (r == n) break;
      l = r + 1;
    }
    assert(remaining == 0);
  }
}

// 小さい N を連続して試し、平方根・立方根・商集合の境界を網羅する。
void test_all_small_quotient_divisions()
{
  using mint = modint998244353;
  using R = FieldAddSubMulDiv<mint>;
  const int limit = 2048;
  mt19937 rng(452188);
  vc<mint> a(limit + 1), b(limit + 1);
  for (int i = 1; i <= limit; i++) a[i] = int(rng() % 11) - 5, b[i] = int(rng() % 13) - 6;
  b[1] = 7;
  const auto ap = prefix(a), bp = prefix(b), cp = prefix(naive_product(a, b));
  for (int n = 0; n <= limit; n++)
  {
    const DirichletPrefixSum<R> B(n, [&](ll x) { return bp[x]; });
    const DirichletPrefixSum<R> C(n, [&](ll x) { return cp[x]; });
    const auto A = C / B;
    assert(A.F(0) == mint(0));
    for (int i = 0; i < A.size(); i++) assert(A.F(A.value(i)) == ap[A.value(i)]);
  }
}

int main()
{
  test_first_division();
  test_products<ll>();
  test_products<modint998244353>();
  test_products<static_modint32<12>>();
  test_arbitrary_division<ll>(1);
  test_arbitrary_division<ll>(-1);
  test_arbitrary_division<ll>(2);
  test_arbitrary_division<modint998244353>(7);
  test_arbitrary_division<static_modint32<12>>(5);
  test_prefix<ll>();
  test_prefix<modint998244353>();
  test_prefix<static_modint32<12>>();
  test_coefficient_ring();
  test_updates_and_conversions();
  test_boundaries_and_large_n();
  test_quotient_indices();
  test_all_small_quotient_divisions();
  test_ring_operations();
  test_polynomial_and_bit_rings();
  test_sparse_convolution<FieldAddSubMulDiv<modint998244353>>();
  test_sparse_convolution<RingAddSubMul<ll>>();
  test_sparse_convolution<EncodedField>();
  test_sparse_convolution<RingXorAnd<ull>>();
  for (ll n : {63'999'999LL, 64'000'000LL, 64'000'001LL})
  {
    test_grouped_convolution<FieldAddSubMulDiv<modint998244353>>(n);
    test_grouped_convolution<EncodedField>(n);
    test_grouped_convolution<RingXorAnd<ull>>(n);
  }
  test_grouped_convolution<RingAddSubMul<ll>>(1'000'000'019LL);
  test_grouped_convolution<RingXorAnd<ull>>(100'000'000'019LL);
  for (ll n : {999'999LL, 1'000'000LL, 1'000'001LL, 100'000'019LL})
  {
    test_grouped_division<FieldAddSubMulDiv<modint998244353>>(n, 7);
    test_grouped_division<FieldAddSubMulDiv<static_modint32<12>>>(n, 5);
    test_grouped_division<EncodedField>(n, EncodedField::encode(7));
    test_grouped_division<RingXorAnd<ull>>(n, ULLONG_MAX);
  }
  test_grouped_division<RingAddSubMul<ll>>(1'000'000'019LL, 1);
  {
    const ll n = 64'000'001;
    const int k = int(iroot(n, 2));
    using R = RingAddSubMul<ll>;
    const DirichletPrefixSum<R> one(n, zeta_prefix_sum, true), id(n, id_prefix_sum, true);
    const auto result = one * id, expected = one.convolve_sparse(id);
    assert(result.is_multiplicative());
    for (int x = 0; x <= k; x++) assert(result.F(x) == expected.F(x));
    for (ll i = 1; i <= n / (ll(k) + 1); i++) assert(result.F(n / i) == expected.F(n / i));
  }
  PRINT("Hello World");
}
