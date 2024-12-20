#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../math/modint/modint.hpp"

mt19937 mt;

// static
template <const int MOD>
void test1()
{
  using mint = static_modint<MOD>;
  repi(t, 1000)
  {
    ll a = -10LL * MOD + mt() % (21LL * MOD);
    ll b = -10LL * MOD + mt() % (21LL * MOD);
    assert((mint(a) + mint(b)).val() == safemod(a + b, MOD));
    assert((mint(a) - mint(b)).val() == safemod(a - b, MOD));
    assert((mint(a) * mint(b)).val() == safemod((a % MOD) * (b % MOD), MOD));
    if (gcd(b, MOD) == 1)
    {
      mint c = mint(a) / mint(b);
      assert(mint(b) * c == mint(a));
    }
    ll k = mt() % 100;
    mint pw = 1;
    rep(_, k) pw *= a;
    assert(mint(a).pow(k) == pw);

    mint x(a), y(b), z;
    z = x + y;
    x += y;
    assert(x == z);
    z = x - y;
    x -= y;
    assert(x == z);
    z = x * y;
    x *= y;
    assert(x == z);
    if (gcd(b, MOD) == 1)
    {
      z = x / y;
      x /= y;
      assert(x == z);
    }
  }
}

// dynamic
void test2(int MOD)
{
  using mint = modint;
  mint::set_mod(MOD);

  repi(t, 1000)
  {
    ll a = -10LL * MOD + mt() % (21LL * MOD);
    ll b = -10LL * MOD + mt() % (21LL * MOD);
    assert((mint(a) + mint(b)).val() == safemod(a + b, MOD));
    assert((mint(a) - mint(b)).val() == safemod(a - b, MOD));
    assert((mint(a) * mint(b)).val() == safemod((a % MOD) * (b % MOD), MOD));
    if (gcd(b, MOD) == 1)
    {
      mint c = mint(a) / mint(b);
      assert(b * c == a);
    }
    ll k = mt() % 100;
    mint pw = 1;
    rep(_, k) pw *= a;
    assert(mint(a).pow(k) == pw);

    mint x(a), y(b), z;
    z = x + y;
    x += y;
    assert(x == z);
    z = x - y;
    x -= y;
    assert(x == z);
    z = x * y;
    x *= y;
    assert(x == z);
    if (gcd(b, MOD) == 1)
    {
      z = x / y;
      x /= y;
      assert(x == z);
    }
  }
}

int main()
{
  test1<1>();
  test1<2>();
  test1<3>();
  test1<4>();
  test1<5>();
  test1<6>();
  test1<7>();
  test1<8>();
  test1<9>();
  test1<10>();
  test1<998244353>();
  test1<1'000'000'000>();
  test1<1'000'000'007>();
  test1<2'000'000'011>();
  test1<2'000'001'000>();
  test1<INT_MAX>();

  fec(MOD : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 998244353, 1'000'000'000, 1'000'000'007, 2'000'000'011, 2'000'000'100, INT_MAX})
  {
    test2(MOD);
  }

  PRINT("Hello World");
}