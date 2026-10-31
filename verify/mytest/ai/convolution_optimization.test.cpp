#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/convolution/convolution.hpp"
#include "math/convolution/convolution64.hpp"
#include "math/convolution/convolution_ll.hpp"

template <class mint>
vc<mint> naive_product(const vc<mint> &a, const vc<mint> &b)
{
  if (a.empty() || b.empty()) return {};
  vc<mint> result(a.size() + b.size() - 1);
  repi(i, a.size()) if (a[i] != 0)
    repi(j, b.size()) result[i + j] += a[i] * b[j];
  return result;
}

template <class mint> void check_product()
{
  mt19937_64 rng(20261031);
  for (int length : {128, 256, 512})
    for (int excess : {1, 2, 7, 31, 32, 33})
    {
      const int n = length / 2 + 1, m = length + excess - n + 1;
      vc<mint> a(n), b(m);
      for (auto &x : a) x = mint(rng()) - mint(rng());
      for (auto &x : b) x = mint(rng()) - mint(rng());
      const auto original = a, other = b;
      assert(convolution(a, b) == naive_product(a, b));
      assert(a == original && b == other);
      assert(convolution(a, a) == naive_product(a, a));
      assert(convolution(a, {}) == vc<mint>{});
      assert(convolution(vc<mint>{}, b) == vc<mint>{});
    }

  // Test focus: identical coefficient spans at different offsets/lengths,
  // restoring both leading/trailing zeros, and the full-size zero product.
  for (int placement : {0, 1, 2})
  {
    vc<mint> a(700), b(600), active(65);
    for (auto &x : active) x = mint(rng()) - mint(rng());
    int al = placement == 0 ? 0 : placement == 1 ? 635 : 123;
    int bl = placement == 0 ? 0 : placement == 1 ? 535 : 217;
    repi(i, active.size()) a[al + i] = b[bl + i] = active[i];
    const auto expected = naive_product(a, b);
    assert(convolution(a, b) == expected);
    if constexpr (is_static_modint_v<mint>)
      if (ntt_ok<mint>(int(a.size() + b.size() - 1)))
        assert(internal::convolution_ntt(a, b) == expected);
    assert(convolution(vc<mint>(a.size()), b) == vc<mint>(expected.size()));
  }

  // Test focus: a cyclic product may be zero while its linear product is nonzero.
  for (int length : {16, 128, 512})
  {
    vc<mint> a(length / 2 + 1), b(a.size()), expected(length + 1);
    a.front() = -1; b.front() = 1;
    a.back() = b.back() = 1;
    expected.front() = -1; expected.back() = 1;
    assert(convolution(a, b) == expected);
    if constexpr (is_static_modint_v<mint>)
      if (ntt_ok<mint>(length + 1)) assert(internal::convolution_ntt(a, b) == expected);
  }

  // Test focus: one or both operands use the zero-skipping direct path.
  for (int n : {61, 129, 1025})
  {
    vc<mint> a(n), b(2049);
    repi(i, n) if (n == 61 || i % 5 == 0) a[i] = mint(rng());
    repi(i, b.size()) if (i % 7 == 0) b[i] = mint(rng());
    assert(convolution(a, b) == naive_product(a, b));
  }
  {
    vc<mint> a(8193), b(8193);
    repi(i, 80) a[97 * i] = mint(rng());
    repi(j, 90) b[89 * j] = mint(rng());
    assert(convolution(a, b) == naive_product(a, b));
    a.assign(8193, 0), b.assign(8193, 0);
    repi(i, 316) a[23 * i] = mint(rng());
    repi(j, 317) b[25 * j] = mint(rng());
    assert(convolution(a, b) == naive_product(a, b));
    a.resize(61);
    for (auto &x : a) x = mint(rng());
    assert(convolution(a, b) == naive_product(a, b));
  }
}

void check_integer_products()
{
  mt19937_64 rng(20261031);
  for (int n : {513, 2049})
  {
    vc<ull> a(n), b(n), expected(2 * n - 1);
    const int first = n == 513 ? 0 : n - 401;
    repi(i, first, n) a[i] = rng(), b[i] = rng();
    repi(i, n) if (a[i]) repi(j, n) expected[i + j] += a[i] * b[j];
    assert(convolution64(a, b) == expected);
  }
  vc<ll> a(4097), b(4097), expected(8193);
  for (auto &x : a) x = 1 + rng() % 100;
  for (auto &x : b) x = 1 + rng() % 100;
  repi(i, a.size()) repi(j, b.size()) expected[i + j] += a[i] * b[j];
  assert(convolution_4e18(a, b) == expected);
}

int main()
{
  check_product<static_modint32<97>>();
  check_product<modint998244353>();
  check_product<static_modint32<2013265921>>();
  check_product<modint1000000007>();
  check_product<static_modint64<998244353LL>>();
  check_product<static_modint64<3221225473LL>>();
  check_product<static_modint64<9223372006790004737LL>>();
  dynamic_modint32<971>::set_mod(1000000007);
  check_product<dynamic_modint32<971>>();

  // Test focus: CRT conversion can turn an entire operand into zero in one prime.
  vc<modint1000000007> a(321, 469762049), b(337);
  repi(i, b.size()) b[i] = i + 7;
  assert(convolution(a, b) == naive_product(a, b));

  vc<ll> integers(129), expected(257);
  repi(i, integers.size()) integers[i] = i % 7 - 3;
  repi(i, integers.size()) repi(j, integers.size()) expected[i + j] += integers[i] * integers[j];
  for (auto &x : expected) x = (x % 998244353 + 998244353) % 998244353;
  assert(convolution(integers, integers) == expected);
  const vc<unsigned> ones(129, 1);
  vc<unsigned> triangular(257);
  repi(i, ones.size()) repi(j, ones.size()) ++triangular[i + j];
  assert(convolution(ones, ones) == triangular);
  check_integer_products();
  cout << "Hello World" << endl;
}
