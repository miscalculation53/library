#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/modint/modint64.hpp"

// static
template <const ll MOD>
void test1()
{
  using mint = static_modint64<MOD>;
  repi(t, 1000)
  {
    i128 a = i128(-10) * MOD + mt() % (i128(21) * MOD);
    i128 b = i128(-10) * MOD + mt() % (i128(21) * MOD);
    i128 ma = safemod<i128>(a, MOD);
    i128 mb = safemod<i128>(b, MOD);
    assert((mint(a) + mint(b)).val() == safemod<i128>(ma + mb, MOD));
    assert((mint(a) - mint(b)).val() == safemod<i128>(ma - mb, MOD));
    assert((mint(a) * mint(b)).val() == safemod<i128>(ma * mb, MOD));
    if (gcd((ll)mb, MOD) == 1)
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
    if (gcd((ll)mb, MOD) == 1)
    {
      z = x / y;
      x /= y;
      assert(x == z);
    }
  }
}

// dynamic odd
void test2(ll MOD)
{
  using mint = dynamic_modint64_odd<-1>;
  mint::set_mod(MOD);

  repi(t, 1000)
  {
    i128 a = i128(-10) * MOD + mt() % (i128(21) * MOD);
    i128 b = i128(-10) * MOD + mt() % (i128(21) * MOD);
    i128 ma = safemod<i128>(a, MOD);
    i128 mb = safemod<i128>(b, MOD);
    assert((mint(a) + mint(b)).val() == safemod<i128>(ma + mb, MOD));
    assert((mint(a) - mint(b)).val() == safemod<i128>(ma - mb, MOD));
    assert((mint(a) * mint(b)).val() == safemod<i128>(ma * mb, MOD));
    if (gcd((ll)mb, MOD) == 1)
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
    if (gcd((ll)mb, MOD) == 1)
    {
      z = x / y;
      x /= y;
      assert(x == z);
    }
  }
}

// dynamic
void test3(ll MOD)
{
  using mint = dynamic_modint64<-1>;
  mint::set_mod(MOD);
  dump(MOD);

  repi(t, 1000)
  {
    i128 a = i128(-10) * MOD + mt() % (i128(21) * MOD);
    i128 b = i128(-10) * MOD + mt() % (i128(21) * MOD);
    i128 ma = safemod<i128>(a, MOD);
    i128 mb = safemod<i128>(b, MOD);
    assert((mint(a) + mint(b)).val() == safemod<i128>(ma + mb, MOD));
    assert((mint(a) - mint(b)).val() == safemod<i128>(ma - mb, MOD));
    assert((mint(a) * mint(b)).val() == safemod<i128>(ma * mb, MOD));
    if (gcd((ll)mb, MOD) == 1)
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
    if (gcd((ll)mb, MOD) == 1)
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
  test1<(1LL << 61) - 1>();
  test1<1LL << 61>();
  test1<(1LL << 62) - 1>();
  test1<1LL << 62>();
  test1<LONG_MAX - 1>();
  test1<LONG_MAX>();

  fec(MOD : vc<ll>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 998244353, 1'000'000'000, 1'000'000'007, 2'000'000'011, 2'000'000'100, INT_MAX, (1LL << 61) - 1, 1LL << 61, (1LL << 62) - 1, 1LL << 62, LONG_MAX - 1, LONG_MAX})
  {
    if (MOD % 2 == 1)
      test2(MOD);
  }
  fec(MOD : vc<ll>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 998244353, 1'000'000'000, 1'000'000'007, 2'000'000'011, 2'000'000'100, INT_MAX, (1LL << 61) - 1, 1LL << 61, (1LL << 62) - 1, 1LL << 62, LONG_MAX - 1, LONG_MAX})
  {
    test3(MOD);
  }

  PRINT("Hello World");
}