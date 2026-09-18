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
  assert(self.num == 16 && self.den == 16);
  self = x;
  self -= self;
  assert(self.num == 0 && self.den == 16);
  self = x;
  self *= self;
  assert(self.num == 4 && self.den == 16);
  self = x;
  self /= self;
  assert(self.num == 8 && self.den == 8);
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
  test_modint();
  test_infinity();
  test_bigint();
  test_converting_constructor();
  cout << "Hello World" << endl;
}
