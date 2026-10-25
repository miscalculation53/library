#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/factorial.hpp"
#include "math/mod_of_linear/split_linear_mod.hpp"

template <class mint>
void small_prime()
{
  const ll p = mint::mod();
  for (int k : {-1, 0, 1, 3, 20})
  {
    const Factorial<mint> fac(k);
    for (ll a = -2; a <= p; ++a) for (ll b = -2; b <= p; ++b)
    {
      mint ans = 1;
      for (ll n = 0; n <= p + 2; ++n)
      {
        assert(fac.arithmetic_product(n, a, b) == ans);
        ans *= mint(a) * mint(n) + mint(b);
      }
    }
  }
}

template <class mint>
mint solve(const Factorial<mint> &fac, ll n, ll a, ll b, ll m)
{
  a %= m;
  b %= m;
  const ll g = gcd(a, m), period = m / g;
  mint ans = 1;
  if (n >= period)
    ans = fac.arithmetic_product(period, g, b % g).pow(n / period);
  for (auto p : split_linear_mod(n % period, m, a, b))
    ans *= fac.arithmetic_product(p.n, p.a, p.b);
  return ans;
}

template <class mint>
void large_prime()
{
  const Factorial<mint> fac;
  const ll block = ntt_ok<mint>(1 << 22) ? 512 : 2048;
  // 区切りの両側、直接計算への切り替え、p-1 付近の端点を確認する。
  for (ll n : {1LL, 31LL, 32LL, 33LL, block - 1, block, block + 1, 2 * block + 1})
    for (ll c : {1LL, block / 2, block - 1, block, block + 1,
                 ll(mint::mod()) - n - 1, ll(mint::mod()) - n, ll(mint::mod()) - n + 1})
      for (mint a : {mint(1), mint(-1), mint(1234567)})
      {
        const mint b = a * mint(c);
        mint expected = 1, x = b;
        for (ll i = 0; i < n; ++i, x += a) expected *= x;
        assert(fac.arithmetic_product(n, a, b) == expected);
      }
  mt19937 rng(20260923);
  for (int it = 0; it < 1000; ++it)
  {
    ll n = rng() % 100, a = ll(rng()) - ll(rng()), b = ll(rng()) - ll(rng());
    mint ans = 1;
    for (ll i = 0; i < n; ++i) ans *= mint(a) * mint(i) + mint(b);
    assert(fac.arithmetic_product(n, a, b) == ans);
  }
  const ll hi = numeric_limits<ll>::max(), p = mint::mod();
  assert(fac.arithmetic_product(0, 0, 0) == 1);
  assert(fac.arithmetic_product(hi, p, 2) == mint(2).pow(hi));
  assert(fac.arithmetic_product(hi, 1, 1) == 0);
  assert(fac.arithmetic_product(p - 1, 1, 1) == -1);
  assert(fac.arithmetic_product(p - 1, -1, -1) == -1);
  assert(fac.arithmetic_product(2, 1, -1) == 0);
  assert(fac.factorial(p) == 0);
  if constexpr (mint::mod() == 998244353)
  {
    assert(solve(fac, 4, 4, 3, 9) == 252);
    assert(solve(fac, 46, 8724, 294, 10007) == 744202079);
    assert(solve(fac, 998244352, 1, 1, 998244353) == 998244352);
    assert(solve(fac, 600000000, 998244353, 206, 924844033) == 180124642);
    assert(solve(fac, 1000000000, 444444444, 314159265, 897932384) == 422453182);
    for (int it = 0; it < 1000; ++it)
    {
      ll n = rng() % 100, m = 1 + rng() % 100;
      ll a = rng() % 100, b = rng() % 100;
      mint ans = 1;
      for (ll i = 0; i < n; ++i) ans *= (a * i + b) % m;
      assert(solve(fac, n, a, b, m) == ans);
    }
  }
}

int main()
{
  small_prime<static_modint32<2>>();
  small_prime<static_modint32<3>>();
  small_prime<static_modint32<17>>();
  small_prime<static_modint32<101>>();
  large_prime<modint998244353>();
  large_prime<modint1000000007>();
  // ブロック内の評価点が法を超える k も確認する。
  const Factorial<modint998244353> wide_block(15);
  assert(wide_block.factorial(998244352) == -1);
  modint998244353 fact = 1;
  for (ll n = 0; n <= 100000; ++n)
  {
    if (n > 0) fact *= n;
    if (n % 10000 == 0) assert(wide_block.factorial(n) == fact);
  }
  cout << "Hello World\n";
}
