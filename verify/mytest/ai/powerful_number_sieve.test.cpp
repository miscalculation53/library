#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/powerful_number_sieve.hpp"
#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "algebra/polynomial_ring.hpp"
#include "algebra/bit.hpp"

template <class R, class Get>
typename R::S trial_value(int n, const Get &get)
{
  auto value = R::e1();
  for (int p = 2; p <= n / p; p++) if (n % p == 0)
  {
    int e = 0, pe = 1;
    while (n % p == 0) n /= p, pe *= p, ++e;
    value = R::mul(value, internal::eval_primepower<R>(get, PrimePower<ll>(p, e, pe)));
  }
  if (n > 1) value = R::mul(value, internal::eval_primepower<R>(get, PrimePower<ll>(n, 1, n)));
  return value;
}

template <class R>
void test_ring()
{
  auto integer = [](ll n) { return internal::multiplicative_from_integer<R>(n); };
  for (int mode = 0; mode < 3; mode++)
  {
    auto h = [&](auto &pp)
    {
      if (pp.e == 1 || (mode == 1 && pp.e == 2) || (mode == 2 && pp.e % 2 == 1)) return R::e0();
      return integer((pp.p * 7 + pp.e * 3) % 9 - 4);
    };
    for (int n : {0, 1, 2, 3, 4, 7, 8, 9, 15, 16, 25, 26, 27, 63, 64, 65, 99, 128, 729, 1000})
    {
      // g は任意の数列でもよい。負の値と g(1)!=1 も含める。
      vc<typename R::S> prefix(n + 1, R::e0());
      for (int i = 1; i <= n; i++) prefix[i] = R::add(prefix[i - 1], integer((i * 13) % 11 - 5));
      auto expected = R::e0();
      for (int d = 1; d <= n; d++) expected = R::add(expected, R::mul(trial_value<R>(d, h), prefix[n / d]));
      map<ll, int> calls;
      auto checked_h = [&](auto &pp)
      {
        assert(pp.e >= 2 && pp.pe <= n && pp.pe == ipow(pp.p, pp.e));
        assert(++calls[pp.pe] == 1);
        return h(pp);
      };
      auto getG = [&](ll x)
      {
        assert(1 <= x && x <= n && x == n / (n / x));
        return prefix[x];
      };
      assert(powerful_number_sieve<R>(n, getG, checked_h) == expected);
      assert(LinearSieve::n >= iroot(n, 2));
      for (int d = 2; d <= n; d++)
      {
        int p = 2;
        while (d % p != 0) ++p;
        int rest = d, e = 0;
        while (rest % p == 0) rest /= p, ++e;
        assert(calls.count(d) == size_t(rest == 1 && e >= 2));
      }
      auto getG_ring = [&](ll x, auto) { return getG(x); };
      auto h_ring = [&](auto &pp, auto) { return h(pp); };
      assert(powerful_number_sieve<R>(n, getG_ring, h_ring) == expected);
      const DirichletPrefixSum<R> table(n, getG);
      assert(powerful_number_sieve<R>(n, [&](ll x) { return table.F(x); }, h) == expected);
    }
  }
}

struct EncodedRing
{
  struct S
  {
    int value = -123;
    friend bool operator==(S a, S b) { return a.value == b.value; }
    friend bool operator!=(S a, S b) { return !(a == b); }
  };
  static S make(int x) { return {((x % 6 + 6) % 6) + 10}; }
  static S e0() { return make(0); }
  static S e1() { return make(1); }
  static S add(S a, S b) { return make(a.value + b.value - 20); }
  static S minus(S a) { return make(10 - a.value); }
  static S mul(S a, S b) { return make((a.value - 10) * (b.value - 10)); }
};

void test_boundaries()
{
  using R = RingAddSubMul<ll>;
  // p^e の積を ll で直接作るため、境界直前でも不要な次の冪を作らない。
  for (ll n : {1LL << 30, (1LL << 30) - 1})
  {
    auto only_two = [](auto &pp) { return pp.p == 2 ? 1LL : 0LL; };
    ll expected = 1;
    for (ll pe = 4; pe <= n; pe *= 2) ++expected;
    assert(powerful_number_sieve<R>(n, [](ll) { return 1LL; }, only_two) == expected);
  }
}

void test_shared_sieve()
{
  using R = RingAddSubMul<ll>;
  const int n = 1000;
  LinearSieve::reserve(100);
  auto h = [](auto &pp) { return ll(pp.e >= 2); };
  ll expected = 0;
  for (int d = 1; d <= n; d++) expected += trial_value<R>(d, h);
  auto checked_h = [&](auto &pp)
  {
    assert(pp.e >= 2 && pp.pe <= n);
    LinearSieve::reserve(300);
    return h(pp);
  };
  auto getG = [](ll)
  {
    LinearSieve::reserve(700);
    return 1LL;
  };
  // 必要な範囲より大きい共有表と、コールバック内での共有表の拡張。
  assert(powerful_number_sieve<R>(n, getG, checked_h) == expected);
  const int sieve_size = LinearSieve::n;
  assert(powerful_number_sieve<R>(n, getG, checked_h) == expected);
  assert(LinearSieve::n == sieve_size);
}

int main()
{
  test_shared_sieve();
  test_ring<RingAddSubMul<ll>>();
  test_ring<RingAddSubMul<static_modint32<6>>>();
  test_ring<RingAddSubMul<modint998244353>>();
  test_ring<RingXorAnd<ull>>();
  test_ring<PolynomialRingArray<RingAddSubMul<ll>, 3>>();
  test_ring<EncodedRing>();
  test_boundaries();
  PRINT("Hello World");
}
