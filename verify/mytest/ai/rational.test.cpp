#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/bigint.hpp"
#include "math/modint/modint.hpp"
#include "math/rational.hpp"

template <class T, class = void>
struct has_less : false_type
{};

template <class T>
struct has_less<T, void_t<decltype(declval<T>() < declval<T>())>> : true_type
{};

template <class T, class = void>
struct has_ordered_helpers : false_type
{};

template <class T>
struct has_ordered_helpers<T, void_t<decltype(abs(declval<T>())), decltype(sgn(declval<T>()))>> : true_type
{};

template <class T, class = void>
struct has_rounding_helpers : false_type
{};

template <class T>
struct has_rounding_helpers<T, void_t<decltype(floor(declval<T>())), decltype(ceil(declval<T>()))>> : true_type
{};

// Test focus: unreduced storage, overflow-safe comparison, arithmetic, reduction, and printing.
void test_integer()
{
  Rational<ll> x(2, 4), y(-3, -6);
  assert(x.num == 2 && x.den == 4);
  assert(y.num == 3 && y.den == 6);
  assert(x == y);
  assert(!(x != y));
  assert(Rational<ll>(1, 3) < x);
  assert(x > Rational<ll>(1, 3));
  assert(x <= y && x >= y);
  assert(Rational<ll>(4'000'000'000LL, 4'000'000'001LL) <
         Rational<ll>(4'000'000'001LL, 4'000'000'002LL));
  assert(Rational<ll>(4'000'000'000LL, 8'000'000'000LL) ==
         Rational<ll>(2'000'000'000LL, 4'000'000'000LL));

  auto sum = x + Rational<ll>(1, 3);
  assert(sum.num == 10 && sum.den == 12);
  auto dif = x - Rational<ll>(1, 3);
  assert(dif.num == 2 && dif.den == 12);
  auto prod = x * Rational<ll>(3, 5);
  assert(prod.num == 6 && prod.den == 20);
  auto quot = x / Rational<ll>(3, 5);
  assert(quot.num == 10 && quot.den == 12);
  auto self = x;
  self += self;
  assert(self.num == 4 && self.den == 4);
  self = x;
  self -= self;
  assert(self.num == 0 && self.den == 4);
  self = x;
  self *= self;
  assert(self.num == 4 && self.den == 16);
  self = x;
  self /= self;
  assert(self.num == 1 && self.den == 1);
  assert(-x == Rational<ll>(-1, 2));
  assert(x + 1 == Rational<ll>(3, 2));
  assert(1 + x == Rational<ll>(3, 2));

  auto [num, den] = sum.reduced();
  assert(num == 5 && den == 6);
  ostringstream oss;
  oss << Rational<ll>(-18, 24);
  assert(oss.str() == "-3/4");
  assert(numeric_limits<Rational<ll>>::max() == Rational<ll>(numeric_limits<ll>::max()));
  static_assert(has_less<Rational<ll>>::value);
}

// 大きな共通因子を掛ける前に処理し、gcd を使わずに表現の肥大化を抑える。
void test_gcd_free_arithmetic()
{
  using R = Rational<ll>;
  const ll v = numeric_limits<ll>::max();
  auto check = [](const R &x, ll num, ll den) { assert(x.num == num && x.den == den); };
  R x(1, v), y(2, v), zero(0, v - 1);
  check(x + y, 3, v);
  check(y - x, 1, v);
  check(x - y, -1, v);
  check(x + R(-2, -v), 3, v);
  R sum;
  repi(i, 1000) sum += x;
  check(sum, 1000, v);
  sum = x;
  sum += sum;
  check(sum, 2, v);
  sum -= sum;
  check(sum, 0, v);

  check(x + zero, 1, v);
  check(zero + x, 1, v);
  check(x - zero, 1, v);
  check(zero - x, -1, v);
  check(x * zero, 0, 1);
  check(zero * x, 0, 1);
  check(zero / x, 0, 1);
  check(zero * zero, 0, 1);

  check(R(v, 3) * R(5, v), 5, 3);
  check(R(5, v) * R(v, 3), 5, 3);
  check(R(2, v) * R(v, 2), 1, 1);
  check(R(2, v) / R(3, v), 2, 3);
  check(R(2, v) / R(-3, v), -2, 3);
  check(R(v, 3) / R(v, 5), 5, 3);
  check(R(-v, 3) / R(-v, 5), 5, 3);
  check(R(1LL << 62) / R(-1, 2), numeric_limits<ll>::lowest(), 1);
  R self(v, v);
  self *= self;
  check(self, 1, 1);
  self = R(v, v);
  self /= self;
  check(self, 1, 1);
  self = R(numeric_limits<ll>::lowest(), v);
  self /= self;
  check(self, 1, 1);

  // 交差積は ll を超えるが、差と保存する分母は ll に収まる。
  check(R(v, 2) + R(-v, 3), v, 6);
  check(R(v, 2) - R(v, 3), v, 6);
  const uint uv = numeric_limits<uint>::max();
  auto udiff = Rational<uint>(uv, 2) - Rational<uint>(uv, 3);
  assert(udiff.num == uv && udiff.den == 6);

  // 128 ビット型でも共通分母の比較は乗算を要しない。
  using W = Rational<i128>;
  const i128 wv = numeric_limits<i128>::max();
  auto wsum = W(wv, 2) + W(-wv, 3);
  auto wdiff = W(wv, 2) - W(wv, 3);
  assert(wsum.num == wv && wsum.den == 6);
  assert(wdiff.num == wv && wdiff.den == 6);
  assert(W(wv - 1, wv) < W(wv, wv));
  assert(W(wv, wv) == W(wv, wv));
  assert(W(wv - 1, wv) != W(wv, wv));
  assert(W(-wv, wv) < W(-wv + 1, wv));
  const u128 uwv = numeric_limits<u128>::max();
  assert(Rational<u128>(uwv - 1, uwv) < Rational<u128>(uwv, uwv));
}

// 小さな値は整数の交差積を参照にして、すべての四則演算を検証する。
void test_small_arithmetic()
{
  using R = Rational<ll>;
  auto check = [](const R &x, ll num, ll den) {
    assert(x.den > 0);
    assert(x.num * den == num * x.den);
  };
  for (ll a = -6; a <= 6; ++a)
    for (ll b = 1; b <= 6; ++b)
      for (ll c = -6; c <= 6; ++c)
        for (ll d = 1; d <= 6; ++d)
        {
          R x(a, b), y(c, d);
          check(x + y, a * d + c * b, b * d);
          check(x - y, a * d - c * b, b * d);
          check(x * y, a * c, b * d);
          if (c != 0) check(x / y, a * d, b * c);
          assert((x == y) == (a * d == c * b));
          assert((x < y) == (a * d < c * b));
        }
}

void test_helpers()
{
  using R = Rational<ll>;
  static_assert(has_ordered_helpers<R>::value && has_rounding_helpers<R>::value);
  static_assert(!has_rounding_helpers<Rational<double>>::value);
  for (ll num = -30; num <= 30; ++num)
    for (ll den = 1; den <= 20; ++den)
    {
      R x(num, den);
      R a = abs(x);
      assert(a.num == std::abs(num) && a.den == den);
      assert(x.abs() == a);
      assert(sgn(x) == (num > 0) - (num < 0) && x.sgn() == sgn(x));
      ll f = floor(x), c = ceil(x);
      assert(f * den <= num && num < (f + 1) * den);
      assert((c - 1) * den < num && num <= c * den);
      assert(x.floor() == f && x.ceil() == c);
      if (num != 0)
      {
        R y = inv(x);
        assert(y.num == (num < 0 ? -den : den) && y.den == std::abs(num));
        assert(x.inv() == y && x * y == R(1));
      }
      assert(x.num == num && x.den == den);
    }
  const ll lo = numeric_limits<ll>::lowest(), hi = numeric_limits<ll>::max();
  assert(floor(R(lo)) == lo && ceil(R(lo)) == lo);
  assert(floor(R(hi)) == hi && ceil(R(hi)) == hi);
  assert(floor(R(lo, 3)) == lo / 3 - 1 && ceil(R(lo, 3)) == lo / 3);
  assert(floor(R(hi, 3)) == hi / 3 && ceil(R(hi, 3)) == hi / 3 + 1);
  assert(sgn(R(lo, hi)) == -1 && sgn(R(hi, hi)) == 1);
  assert((R(lo, 2).reduced() == pair<ll, ll>{lo / 2, 1}));
  const ull uhi = numeric_limits<ull>::max();
  assert(floor(Rational<ull>(uhi, 2)) == uhi / 2);
  assert(ceil(Rational<ull>(uhi, 2)) == uhi / 2 + 1);
  const i128 wlo = numeric_limits<i128>::lowest();
  assert(floor(Rational<i128>(wlo, 3)) == wlo / 3 - 1);
  assert(ceil(Rational<i128>(wlo, 3)) == wlo / 3);
}

// Test focus: Rational<mint> supports field arithmetic without ordering or integer reduction.
void test_modint()
{
  using mint = modint998244353;
  Rational<mint> x(mint(2), mint(4)), y(mint(1), mint(2));
  assert(x == y);
  assert(x + y == Rational<mint>(mint(1)));
  assert(x - y == Rational<mint>(mint(0)));
  assert(x * Rational<mint>(mint(6), mint(5)) == Rational<mint>(mint(3), mint(5)));
  assert(x / Rational<mint>(mint(3), mint(7)) == Rational<mint>(mint(7), mint(6)));
  assert(!has_less<Rational<mint>>::value);
  static_assert(!has_ordered_helpers<Rational<mint>>::value);
  static_assert(!has_rounding_helpers<Rational<mint>>::value);
  assert(x.inv() == Rational<mint>(mint(2)) && inv(x) == x.inv());
  auto common_sum = x + Rational<mint>(mint(3), mint(4));
  assert(common_sum.num == mint(5) && common_sum.den == mint(4));
  auto common_quot = x / Rational<mint>(mint(3), mint(4));
  assert(common_quot.num == mint(2) && common_quot.den == mint(3));
  auto canceled = x * Rational<mint>(mint(4), mint(2));
  assert(canceled.num == mint(1) && canceled.den == mint(1));

  auto [num, den] = x.reduced();
  assert(num == mint(2) && den == mint(4));
  ostringstream oss;
  oss << x;
  assert(oss.str() == "2/4");
}

// Test focus: ordered rationals represent signed infinity canonically and obey extended-real arithmetic.
void test_infinity()
{
  using R = Rational<ll>;
  R inf(123, 0), ninf(-456, 0);
  assert(inf == R(1, 0) && ninf == R(-1, 0));
  assert(inf.is_infinite() && !inf.is_finite());
  assert(R(10).is_finite() && !R(10).is_infinite());
  assert(ninf < R(-1'000'000) && R(1'000'000) < inf);
  assert(-inf == ninf);
  assert(inf + R(10) == inf);
  assert(ninf - R(10) == ninf);
  assert(inf - ninf == inf);
  assert(inf * R(-2) == ninf);
  assert(ninf / R(-2) == inf);
  assert(R(123) / inf == R(0));
  assert(numeric_limits<R>::has_infinity);
  assert(R::infty() == inf && -R::infty() == ninf);
  assert(numeric_limits<R>::infinity() == inf);
  assert(abs(inf) == inf && abs(ninf) == inf);
  assert(sgn(inf) == 1 && sgn(ninf) == -1);
  assert(inv(inf) == R(0) && inv(ninf) == R(0));
}

// Test focus: BigInteger numerators support ordering, reduction, infinity, and integral conversion.
void test_bigint()
{
  using BI = BigInteger<>;
  using R = Rational<BI>;
  R x(BI(2), BI(4)), y(BI(1), BI(2));
  R inf(BI(1), BI(0)), ninf(BI(-1), BI(0));
  assert(R::infty() == inf && -R::infty() == ninf);
  assert(x == y);
  assert((x.reduced() == pair<BI, BI>(BI(1), BI(2))));
  assert(ninf < R(-100) && R(100) < inf);
  assert(inf + R(1) == inf);
  assert(R(1) / ninf == R(0));
  assert(-inf == ninf);
  static_assert(has_ordered_helpers<R>::value && has_rounding_helpers<R>::value);
  BI large("10000000000000000000000000000000000000001");
  R z(-large, BI(3));
  assert(abs(z).num == large && abs(z).den == BI(3));
  assert(sgn(z) == -1);
  assert(floor(z) == -large / BI(3) - BI(1));
  assert(ceil(z) == -large / BI(3));
  assert(inv(z).num == BI(-3) && inv(z).den == large);
  assert(abs(ninf) == inf && sgn(inf) == 1 && inv(ninf) == R(0));
  auto common_sum = R(BI(2), large) + R(BI(3), large);
  assert(common_sum.num == BI(5) && common_sum.den == large);
  ostringstream oss;
  oss << inf;
  assert(oss.str() == "1/0");
}

void test_converting_constructor()
{
  using R = Rational<ll>;
  using W = Rational<i128>;
  static_assert(is_constructible_v<W, R>);
  static_assert(!is_convertible_v<R, W>);
  W x(R(-18, 24));
  assert(x.num == -18 && x.den == 24);
  W y(R(18, -24));
  assert(y.num == -18 && y.den == 24);
  W inf(numeric_limits<R>::infinity());
  W ninf(-numeric_limits<R>::infinity());
  assert(inf.num == 1 && inf.den == 0);
  assert(ninf.num == -1 && ninf.den == 0);

  constexpr ll v = 4'000'000'000LL;
  W product = W(R(v, v - 1)) * W(R(v, v - 1));
  assert(product.num == i128(v) * v);
  assert(product.den == i128(v - 1) * (v - 1));
  ostringstream oss;
  oss << x << ' ' << product << ' ' << inf;
  assert(oss.str() == "-3/4 16000000000000000000/15999999992000000001 1/0");
  W lowest(numeric_limits<i128>::lowest());
  assert((lowest.reduced() == pair<i128, i128>{numeric_limits<i128>::lowest(), 1}));
  Rational<u128> unsigned_wide(u128(1) << 127, u128(1) << 126);
  assert((unsigned_wide.reduced() == pair<u128, u128>{2, 1}));
  Rational<BigInteger<>> big(R(-18, 24));
  assert(big.num == BigInteger<>(-18) && big.den == BigInteger<>(24));
}

int main()
{
  test_integer();
  test_gcd_free_arithmetic();
  test_small_arithmetic();
  test_helpers();
  test_modint();
  test_infinity();
  test_bigint();
  test_converting_constructor();
  cout << "Hello World" << endl;
}
