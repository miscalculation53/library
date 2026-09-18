#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/dirichlet_convolution.hpp"

template <class R>
vc<typename R::S> direct_product(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  vc<typename R::S> res(a.size(), R::e0());
  for (int n = 1; n < int(a.size()); n++)
    for (int d = 1; d <= n; d++)
      if (n % d == 0) res[n] = R::add(res[n], R::mul(a[d], b[n / d]));
  return res;
}

template <class R>
vc<typename R::S> direct_compose(const vc<typename R::S> &a, const vc<typename R::S> &c)
{
  vc<typename R::S> res(a.size(), R::e0()), power(res);
  if (a.size() <= 1) return res;
  power[1] = R::e1();
  for (const auto &coefficient : c)
  {
    for (int i = 1; i < int(a.size()); i++)
      res[i] = R::add(res[i], R::mul(coefficient, power[i]));
    power = direct_product<R>(power, a);
  }
  return res;
}

int direct_omega(int n)
{
  int res = 0;
  for (int p = 2; p <= n; p++)
    while (n % p == 0) res++, n /= p;
  return res;
}

template <class R>
void check_general(int n, mt19937 &rng)
{
  using S = typename R::S;
  using D = DirichletSeries<R>;
  const auto integer = [](int v) { return internal::multiplicative_from_integer<R>(v); };
  vc<S> a(n + 1, R::e0()), b(a), c(12, R::e0());
  for (int i = 2; i <= n; i++) a[i] = integer(int(rng() % 17) - 8), b[i] = integer(rng() % 7);
  for (auto &x : c) x = integer(rng() % 13);
  const D A(a), B(b), one = D::unit(n);
  assert(A.compose_fps(c).to_vector() == direct_compose<R>(a, c));
  assert(A.compose_fps({}).to_vector() == D(n).to_vector());
  assert(A.compose_fps({R::e0(), R::e1()}).to_vector() == a);

  const auto derivative = A.diff();
  for (int i = 1; i <= n; i++)
    assert(derivative.f(i) == R::mul(integer(direct_omega(i)), a[i]));
  assert(derivative.integ().to_vector() == a);
  assert(derivative.integ(integer(3)).to_vector() == (A + one * integer(3)).to_vector());
  assert((A * B).diff().to_vector() == (A.diff() * B + A * B.diff()).to_vector());

  const int l = n == 0 ? 0 : int(msb_pos(n));
  vc<S> log_coeff(l + 1, R::e0()), exp_coeff(l + 1, R::e1());
  for (int j = 1; j <= l; j++)
  {
    const auto inverse = R::inv(integer(j));
    log_coeff[j] = j % 2 ? inverse : R::minus(inverse);
    exp_coeff[j] = R::mul(exp_coeff[j - 1], inverse);
  }
  assert((one + A).log().to_vector() == direct_compose<R>(a, log_coeff));
  assert(A.exp().to_vector() == direct_compose<R>(a, exp_coeff));
  assert(A.exp().log().to_vector() == a);
  assert((one + A).log().exp().to_vector() == (one + A).to_vector());
  assert((A + B).exp().to_vector() == (A.exp() * B.exp()).to_vector());
  assert((one + A).pow(integer(5)).to_vector() == direct_compose<R>(a,
    {integer(1), integer(5), integer(10), integer(10), integer(5), integer(1)}));
  const auto root = (one + A).pow(R::inv(integer(2)));
  assert((root * root).to_vector() == (one + A).to_vector());
  assert((one + A).pow(integer(-1)).to_vector() == (one + A).inv().to_vector());
  assert((one + A).pow(5LL).to_vector() == (one + A).pow(integer(5)).to_vector());

  for (ll d : {1LL, 2LL, 3LL, 4LL, 31LL, LLONG_MAX})
  {
    const auto moved = (one + A).substitute_power(d);
    vc<S> expected(n + 1, R::e0());
    if (n > 0) expected[1] = R::e1();
    for (int i = 2; i <= n; i++)
    {
      i128 power = 1;
      for (ll j = 0; j < d && power <= n; j++) power *= i;
      if (power <= n) expected[int(power)] = a[i];
    }
    assert(moved.to_vector() == expected);
    assert(((one + A) * (one + B)).substitute_power(d).to_vector() ==
      (moved * (one + B).substitute_power(d)).to_vector());
    if (d <= 31)
      assert(moved.diff().to_vector() == ((one + A).diff().substitute_power(d) * integer(d)).to_vector());
  }
}

template <class mint>
void check_multiplicative()
{
  using R = FieldAddSubMulDiv<mint>;
  using D = DirichletSeries<R>;
  const int n = 400;
  mt19937 rng(94003);
  for (int round = 0; round < 8; round++)
  {
    const D f(n, [&](const auto &) { return mint(rng() % 23); });
    const D general(f.to_vector());
    assert(f.log().to_vector() == general.log().to_vector());
    assert(!f.log().is_multiplicative());
    assert(f.log().exp().to_vector() == f.to_vector());
    assert(f.log().exp().is_multiplicative());
    assert(f.pow(mint(3)).is_multiplicative());
    assert(f.pow(mint(3)).to_vector() == (f * f * f).to_vector());
    assert(f.substitute_power(3).is_multiplicative());
    for (int i = 2; i <= n; i++)
      if (LinearSieve::lpf_[i].pe != i) assert(f.log().f(i) == mint(0));
  }
  const D zeta(n, zeta_primepower);
  const auto l = zeta.log();
  for (int i = 2; i <= n; i++)
  {
    const auto q = LinearSieve::lpf_[i];
    assert(l.f(i) == (q.pe == i ? mint(q.e).inv() : mint(0)));
  }
  assert(zeta.pow(1'000'000'000'000'000'000LL).to_vector() ==
    zeta.pow(mint(1'000'000'000'000'000'000LL)).to_vector());
}

// 算術演算子を持たず、整数構築・既定構築が環への埋め込みでない体。
struct EncodedField
{
  struct S
  {
    int code = 42;
    explicit S(int code = 42) : code(code) {}
    friend bool operator==(S a, S b) { return a.code == b.code; }
    friend bool operator!=(S a, S b) { return !(a == b); }
  };
  inline static int inversions = 0;
  static S encode(ll x) { return S(int((x % 101 + 101) % 101) + 11); }
  static int decode(S x) { return x.code - 11; }
  static S e0() { return encode(0); }
  static S e1() { return encode(1); }
  static S add(S a, S b) { return encode(decode(a) + decode(b)); }
  static S minus(S a) { return encode(-decode(a)); }
  static S mul(S a, S b) { return encode(decode(a) * decode(b)); }
  static S inv(S a)
  {
    inversions++;
    for (int i = 1; i < 101; i++) if (decode(a) * i % 101 == 1) return encode(i);
    assert(false);
    return e0();
  }
};

// S が modint でも、環の演算が異なる場合は通常 modint 向け最適化を使わない。
struct ShiftedField
{
  using S = modint998244353;
  static S e0() { return 17; }
  static S e1() { return 18; }
  static S add(S a, S b) { return a + b - 17; }
  static S minus(S a) { return 34 - a; }
  static S mul(S a, S b) { return (a - 17) * (b - 17) + 17; }
  static S inv(S a) { return (a - 17).inv() + 17; }
};

struct DynamicCountingField : FieldAddSubMulDiv<dynamic_modint32<92>>
{
  inline static int inversions = 0;
  static S inv(const S &a) { inversions++; return a.inv(); }
};

template <class mint>
void check_inverse_table()
{
  using R = FieldAddSubMulDiv<mint>;
  for (int n : {0, 1, 2, 10, 30, 3})
  {
    const auto inverse = internal::dirichlet_integer_inverses<R>(n);
    assert(inverse.size() >= size_t(n + 1));
    for (int i = 1; i <= n; i++) assert(inverse[i] * mint(i) == mint(1));
  }
}

void check_counting()
{
  using R = RingAddSubMul<ll>;
  using D = DirichletSeries<R>;
  const int n = 100;
  D leaves(n);
  leaves.set_f(2, 1);
  leaves.set_f(3, 1);
  // 葉 k 個の順序付き充満二分木: Catalan(k-1)。外側の FPS は z+z^2+2z^3+...。
  vc<ll> catalan{0, 1, 1, 2, 5, 14, 42};
  const auto trees = leaves.compose_fps(catalan);
  vc<ll> dp(n + 1);
  dp[2] = dp[3] = 1;
  for (int x = 2; x <= n; x++)
    for (int d = 2; d < x; d++)
      if (x % d == 0) dp[x] += dp[d] * dp[x / d];
  assert(trees.to_vector() == dp);
  assert(trees.f(6) == 2 && trees.f(12) == 6);
  // 積が x の列の個数。長さ k の選び方に重み c[k] を付ける。
  const vc<ll> c{3, 2, 5, 7};
  assert(leaves.compose_fps(c).to_vector() == direct_compose<R>(leaves.to_vector(), c));
}

void check_small_characteristic()
{
  using mint = static_modint32<3>;
  using R = FieldAddSubMulDiv<mint>;
  using D = DirichletSeries<R>;
  D a = D::unit(100);
  a.set_f(2, 1);
  a.set_f(3, 2);
  const auto cube = a.pow(3); // log は定義できなくても整数乗は有効。
  assert(cube.to_vector() == (a * a * a).to_vector());
  assert(cube.f(8) == mint(1) && cube.f(27) == mint(2));
  using C = FieldAddSubMulDiv<static_modint32<12>>;
  auto b = DirichletSeries<C>::unit(80);
  b.set_f(2, 3);
  assert(b.pow(5).to_vector() == (b * b * b * b * b).to_vector());
}

int main()
{
  for (int n = 1; n <= 2000; n++) assert(LinearSieve::Omega(n) == direct_omega(n));
  for (int n : {1, 4001, 7, 8000, 2}) assert(LinearSieve::Omega(n) == direct_omega(n));
  check_inverse_table<modint998244353>();
  check_inverse_table<modint61>();
  for (int mod : {101, 1009, 101})
  {
    dynamic_modint32<91>::set_mod(mod);
    dynamic_modint64_odd<91>::set_mod(mod);
    dynamic_modint64<91>::set_mod(mod);
    check_inverse_table<dynamic_modint32<91>>();
    check_inverse_table<dynamic_modint64_odd<91>>();
    check_inverse_table<dynamic_modint64<91>>();
  }
  for (int mod : {101, 1009, 101})
  {
    using R = DynamicCountingField;
    R::S::set_mod(mod);
    const int before = R::inversions;
    const auto &table = internal::dirichlet_integer_inverses<R>(8);
    assert(R::inversions == before + 1);
    for (int i = 1; i <= 8; i++) assert(table[i] * R::S(i) == 1);
    internal::dirichlet_integer_inverses<R>(8);
    internal::dirichlet_integer_inverses<R>(3);
    assert(R::inversions == before + 1);
  }
  EncodedField::inversions = 0;
  internal::dirichlet_integer_inverses<EncodedField>(0);
  internal::dirichlet_integer_inverses<EncodedField>(1);
  assert(EncodedField::inversions == 0);
  internal::dirichlet_integer_inverses<EncodedField>(2);
  assert(EncodedField::inversions == 1);
  internal::dirichlet_integer_inverses<EncodedField>(2);
  assert(EncodedField::inversions == 1);
  internal::dirichlet_integer_inverses<EncodedField>(5);
  assert(EncodedField::inversions == 2);
  internal::dirichlet_integer_inverses<EncodedField>(3);
  assert(EncodedField::inversions == 2);
  const auto inverse = internal::dirichlet_integer_inverses<EncodedField>(30);
  assert(EncodedField::inversions == 3);
  for (int i = 1; i <= 30; i++)
    assert(EncodedField::mul(inverse[i], EncodedField::encode(i)) == EncodedField::e1());
  {
    using R = EncodedField;
    using D = DirichletSeries<R>;
    D a(64);
    a.set_f(2, R::encode(3));
    a.set_f(6, R::encode(5));
    const auto f = D::unit(64) + a;
    assert(f.log().exp().to_vector() == f.to_vector());
    assert(f.diff().integ(R::e1()).to_vector() == f.to_vector());
    assert(f.pow(R::encode(2)).to_vector() == (f * f).to_vector());
    assert(EncodedField::inversions == 3); // 演算を重ねても既存の逆元列を使い回す。
    const vc<R::S> values{R::encode(3), R::encode(7), R::encode(9)};
    const auto inverses = inv_many<R>(values);
    assert(EncodedField::inversions == 4);
    for (int i = 0; i < 3; i++) assert(R::mul(values[i], inverses[i]) == R::e1());
    assert(inv_many<R>(vc<R::S>{}).empty() && EncodedField::inversions == 4);
    const vc<modint998244353> mints{2, 3, 5};
    using M = FieldAddSubMulDiv<modint998244353>;
    const auto inverses_mod = inv_many<M>(mints);
    for (int i = 0; i < 3; i++) assert(inverses_mod[i] * mints[i] == 1);
    assert(inv_many<M>(vc<modint998244353>{}).empty());
  }
  mt19937 rng(804917);
  for (int n : {0, 1, 2, 3, 4, 7, 8, 9, 31, 32, 33, 80, 257})
    for (int round = 0; round < 3; round++)
    {
      check_general<FieldAddSubMulDiv<modint998244353>>(n, rng);
      check_general<EncodedField>(n, rng);
      check_general<ShiftedField>(n, rng);
    }
  // N > p でも、p > floor(log2 N) なら全演算を使える。
  check_general<FieldAddSubMulDiv<static_modint32<11>>>(257, rng);
  check_general<FieldAddSubMulDiv<static_modint32<77>>>(64, rng);
  for (int mod : {101, 1009, 101})
  {
    dynamic_modint64_odd<91>::set_mod(mod);
    check_general<FieldAddSubMulDiv<dynamic_modint64_odd<91>>>(80, rng);
  }
  check_multiplicative<modint998244353>();
  check_multiplicative<modint61>();
  check_counting();
  check_small_characteristic();
  const DirichletSeries<RingAddSubMul<ll>> huge(vc<ll>{0, LLONG_MAX});
  assert(huge.pow(1).f(1) == LLONG_MAX);
  PRINT("Hello World");
}
