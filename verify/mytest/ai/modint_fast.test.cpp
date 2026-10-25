#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/modint/modint_fast.hpp"
#include "math/convolution/convolution.hpp"
#include "math/modint/binomial.hpp"

template <int p>
void test_small()
{
  using mint = fast_modint<p>;
  using ref = static_modint32<p>;
  static_assert(is_modint_v<mint> && is_static_modint_v<mint>);
  static_assert(!is_dynamic_modint_v<mint> && sizeof(mint) == sizeof(uint));
  for (int a = 0; a < p; ++a)
  {
    for (int e = 0; e <= 2 * p; ++e)
      assert(mint(a).pow(e).val() == ref(a).pow(e).val());
    if (a) assert(mint(a).inv().val() == ref(a).inv().val());
  }
}

template <int p>
void test_large()
{
  using mint = fast_modint<p>;
  using ref = static_modint32<p>;
  mint::precompute();
  mint::precompute();
  mt19937_64 rng(p);
  for (int i = 0; i < 30000; ++i)
  {
    const uint a = rng() % p, b = 1 + rng() % (p - 1);
    const ull e = rng();
    const mint x = a, y = b;
    assert(x.pow(e).val() == ref(a).pow(e).val());
    assert(y.inv().val() == ref(b).inv().val());
    assert((x / y).val() == (ref(a) / ref(b)).val());
    assert((x + y).val() == (ref(a) + ref(b)).val());
    assert((x - y).val() == (ref(a) - ref(b)).val());
    assert((x * y).val() == (ref(a) * ref(b)).val());
    assert(mint(-ll(a)).val() == ref(-ll(a)).val());
    mint z = x;
    assert((z += y) == x + y);
    assert((z -= y) == x);
    assert((z *= y) == x * y);
    assert((z /= y) == x);
    assert(z++ == x && z == x + 1);
    assert(z-- == x + 1 && z == x);
    assert(++z == x + 1 && --z == x);
    assert(-x + x == 0 && +x == x);
  }

  // すべての区間の両端で分数表・逆元表・離散対数表を検証する。
  int width = 1;
  while (ll(width) * width * width < p) width *= 2;
  for (ll l = 0; l < p; l += width)
    for (ll a : {l, min<ll>(p - 1, l + width - 1)})
    {
      const mint x = a;
      assert(x.pow(1) == x);
      assert(x.pow(2) == x * x);
      if (a) assert(x * x.inv() == 1);
    }

  const vc<ull> exponents = {0, 1, 2, ull(p - 2), ull(p - 1), ull(p), ull(LLONG_MAX), ULLONG_MAX};
  for (int a : {0, 1, 2, p / 2, p - 2, p - 1})
  {
    for (ull e : exponents)
      assert(mint(a).pow(e).val() == ref(a).pow(e).val());
    const u128 e = (u128(1) << 127) + (u128(1) << 64) + 123;
    assert(mint(a).pow(e).val() == ref(a).pow(e).val());
    assert(mint(a).pow(i128(e >> 1)).val() == ref(a).pow(i128(e >> 1)).val());
    assert(mint(a).pow(uint8_t(255)).val() == ref(a).pow(255).val());
  }
  assert(mint(0).pow(0) == 1);
  assert(mint(0).pow(p - 1) == 0);
  assert(mint(1).inv() == 1 && mint(p - 1).inv() == p - 1);
  assert(mint::raw(p - 1) == -1);
  assert(mint(~u128(0)).val() == ref(~u128(0)).val());
  assert(mint(i128(1) << 100).val() == ref(i128(1) << 100).val());
  assert(Binomial<mint>::C(10, 3) == 120);

  stringstream ss;
  ss << mint(-1) << ' ' << mint(0) << ' ' << mint(42);
  mint a, b, c;
  ss >> a >> b >> c;
  assert(a == -1 && b == 0 && c == 42);
}

template <int p>
void test_convolution()
{
  using mint = fast_modint<p>;
  vc<mint> a(128), b(129);
  for (int i = 0; i < int(a.size()); ++i) a[i] = i * i - 300;
  for (int i = 0; i < int(b.size()); ++i) b[i] = 2 * i - 42;
  vc<mint> expected(a.size() + b.size() - 1);
  for (int i = 0; i < int(a.size()); ++i)
    for (int j = 0; j < int(b.size()); ++j)
      expected[i + j] += a[i] * b[j];
  assert(convolution(a, b) == expected);
  for (int i = 0; i < int(expected.size()); ++i)
    assert(convolution_point_get(a, b, i) == expected[i]);
}

int main()
{
  test_small<2>();
  test_small<3>();
  test_small<5>();
  test_small<7>();
  test_small<11>();
  test_small<17>();
  test_small<31>();
  test_small<61>();
  test_small<67>();
  test_small<127>();
  test_small<257>();
  test_small<1009>();
  test_large<65537>();
  test_large<998244353>();
  test_large<1000000007>();
  test_large<2147483647>();
  test_convolution<998244353>();
  test_convolution<1000000007>();
  PRINT("Hello World");
}
