#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
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

int main()
{
  test_integer();
  test_modint();
  cout << "Hello World" << endl;
}
