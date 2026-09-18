#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/modint/modint.hpp"
#include "algebra/polynomial_ring.hpp"
#include "algebra/bit.hpp"

// 線形篩を使わず、試し割りで素因数分解して全係数を計算する。
template <class R, class GetPrimePower>
vc<typename R::S> naive_values(int n, const GetPrimePower &get)
{
  vc<typename R::S> a(n + 1, R::e0());
  for (int i = 1; i <= n; i++)
  {
    auto value = R::e1();
    int x = i;
    for (int p = 2; p <= x / p; p++)
      if (x % p == 0)
      {
        int e = 0, q = 1;
        while (x % p == 0) x /= p, q *= p, ++e;
        value = R::mul(value, internal::eval_primepower<R>(get, PrimePower<ll>(p, e, q)));
      }
    if (x > 1) value = R::mul(value, internal::eval_primepower<R>(get, PrimePower<ll>(x, 1, x)));
    a[i] = value;
  }
  return a;
}

bool trial_prime(int n)
{
  if (n < 2) return false;
  for (int d = 2; d <= n / d; d++) if (n % d == 0) return false;
  return true;
}

template <class R>
vc<typename R::S> sums(const vc<typename R::S> &a)
{
  auto b = a;
  for (int i = 1; i < int(a.size()); i++) b[i] = R::add(b[i - 1], a[i]);
  return b;
}

template <class R, class Table>
void check(const Table &a, const vc<typename R::S> &expected)
{
  for (int x = 0; x < int(expected.size()); x++)
    if (a.contains(x)) assert(a.F(x) == expected[x]);
}

template <class R, class GetPrimePower>
void check_sum(int n, const GetPrimePower &get)
{
  const auto a = naive_values<R>(n, get), expected = sums<R>(a);
  vc<typename R::S> p(n + 1, R::e0());
  for (int i = 2; i <= n; i++) if (trial_prime(i)) p[i] = a[i];
  p = sums<R>(p);
  DirichletPrefixSum<R> prime(n, [&](ll x) { return p[x]; });
  const auto slow = internal::multiplicative_prefix_sum_sieve(prime, get);
  check<R>(slow, expected);
  const auto automatic = multiplicative_prefix_sum(prime, get);
  check<R>(automatic, expected);
  assert(automatic.size() == prime.size() && automatic.is_multiplicative());
  check<R>(internal::multiplicative_prefix_sum_sieve_2_3(prime, get), expected);
  assert(slow.is_multiplicative());
}

// S の既定構築は零ではなく、算術演算子もない。
struct EncodedRing
{
  struct S
  {
    int value = 54321;
    explicit S(int v = 54321) : value(v) {}
    friend bool operator==(S a, S b) { return a.value == b.value; }
    friend bool operator!=(S a, S b) { return !(a == b); }
  };
  static S make(ll x) { return S(int((x % 101 + 101) % 101) + 19); }
  static S e0() { return make(0); }
  static S e1() { return make(1); }
  static S add(S a, S b) { return make(a.value + b.value - 38); }
  static S minus(S a) { return make(19 - a.value); }
  static S mul(S a, S b) { return make((a.value - 19) * (b.value - 19)); }
  static S inv(S a)
  {
    for (int b = 1; b < 101; b++) if ((a.value - 19) * b % 101 == 1) return make(b);
    assert(false); return e0();
  }
};

template <class R>
void check_primes(int n)
{
  vc<typename R::S> count(n + 1, R::e0()), sum(count);
  for (int i = 2; i <= n; i++) if (trial_prime(i))
    count[i] = R::e1(), sum[i] = internal::multiplicative_from_integer<R>(i);
  count = sums<R>(count); sum = sums<R>(sum);
  DirichletPrefixSum<R> one(n, zeta_prefix_sum, true), id(n, id_prefix_sum, true);
  check<R>(prime_prefix_sum(one), count);
  check<R>(prime_prefix_sum(id), sum);
  check<R>(internal::prime_prefix_sum_sieve(one), count);
  check<R>(internal::prime_prefix_sum_sieve(id), sum);
  check<R>(internal::prime_prefix_sum_sieve_2_3(one), count);
  check<R>(internal::prime_prefix_sum_sieve_2_3(id), sum);
  // mod 3 の指標: 完全乗法的だが、素数で零や負の値を取る。
  DirichletPrefixSum<R> chi(n, [](ll x) { return x % 3 == 1 ? R::e1() : R::e0(); });
  vc<typename R::S> weighted(n + 1, R::e0());
  for (int p = 2; p <= n; p++) if (trial_prime(p))
    weighted[p] = p % 3 == 0 ? R::e0() : p % 3 == 1 ? R::e1() : R::minus(R::e1());
  weighted = sums<R>(weighted);
  check<R>(prime_prefix_sum(chi), weighted);
  check<R>(internal::prime_prefix_sum_sieve(chi), weighted);
  check<R>(internal::prime_prefix_sum_sieve_2_3(chi), weighted);
}

// 整数での線形結合・戻り値と auto & のラムダをそのまま使える。
void check_reference_callbacks()
{
  using mint = static_modint32<469762049>;
  using R = FieldAddSubMulDiv<mint>;
  const ll a = 123456789, b = 345678901;
  auto get = [&](auto &pp) { return a * pp.e + b * pp.p; };
  auto get_with_ring = [&](auto &pp, auto) { return a * pp.e + b * pp.p; };
  for (int n : {0, 1, 2, 64, 257, 1000})
  {
    DirichletPrefixSum<R> zeta(n, zeta_prefix_sum), id(n, id_prefix_sum);
    auto pi0 = prime_prefix_sum(zeta), pi1 = prime_prefix_sum(id);
    const auto expected = sums<R>(naive_values<R>(n,
      [&](const auto &pp) { return mint(a) * pp.e + mint(b) * pp.p; }));
    check<R>(multiplicative_prefix_sum(a * pi0 + b * pi1, get), expected);
    check<R>(internal::multiplicative_prefix_sum_sieve_2_3(a * pi0 + b * pi1, get), expected);
    check<R>(multiplicative_prefix_sum(a * pi0 + b * pi1, get_with_ring), expected);
    check<R>(internal::multiplicative_prefix_sum_sieve_2_3(a * pi0 + b * pi1, get_with_ring), expected);
  }
}

int main()
{
  check_reference_callbacks();
  using R = FieldAddSubMulDiv<modint998244353>;
  for (int n : {0, 1, 2, 3, 4, 7, 8, 9, 24, 25, 26, 63, 64, 65, 99, 128, 257, 728, 729, 730, 4095, 4096, 15624, 15625, 15626})
  {
    check_primes<R>(n);
    check_sum<R>(n, zeta_primepower);
    check_sum<R>(n, totient_primepower);
    check_sum<R>(n, divisor_sum_primepower);
    check_sum<R>(n, mobius_primepower);
    check_sum<R>(n, [](const auto &q) { return R::S((q.p * 3 + q.e * q.e) % 11 - 5); });
    check_sum<RingAddSubMul<ll>>(n, totient_primepower);
  }
  for (int n : {0, 1, 64, 257, 1000})
  {
    check_primes<EncodedRing>(n);
    check_primes<RingXorAnd<ull>>(n);
    check_sum<EncodedRing>(n, totient_primepower);
    check_sum<RingXorAnd<ull>>(n, divisor_count_primepower);
    check_sum<PolynomialRingArray<RingAddSubMul<ll>, 3>>(n,
      [](const auto &q) { return array<ll, 3>{1, q.e, q.p % 3}; });
    check_sum<RingAddSubMul<static_modint32<6>>>(n, totient_primepower);
  }
  // 複数の素数上の和を組み合わせても同じ商集合を返す。
  {
    const int n = 1000;
    DirichletPrefixSum<R> id(n, id_prefix_sum), one(n, zeta_prefix_sum);
    auto p = internal::prime_prefix_sum_sieve(id) - internal::prime_prefix_sum_sieve(one);
    auto phi = internal::multiplicative_prefix_sum_sieve(p, totient_primepower);
    check<R>(phi, sums<R>(naive_values<R>(n, totient_primepower)));
    auto p23 = internal::prime_prefix_sum_sieve_2_3(id) - internal::prime_prefix_sum_sieve_2_3(one);
    auto phi23 = internal::multiplicative_prefix_sum_sieve_2_3(p23, totient_primepower);
    assert(phi23.size() == id.size());
    check<R>(phi23, sums<R>(naive_values<R>(n, totient_primepower)));
    auto automatic = multiplicative_prefix_sum(prime_prefix_sum(id) - prime_prefix_sum(one), totient_primepower);
    assert(automatic.size() == id.size());
    check<R>(automatic, sums<R>(naive_values<R>(n, totient_primepower)));
  }
  // 自動選択の境界前後を独立した Eratosthenes の篩と照合する。
  {
    using Z = RingAddSubMul<ll>;
    const int limit = 1'000'001;
    vc<ll> phi(limit + 1), count(limit + 1, 0), weighted(count);
    iota(phi.begin(), phi.end(), 0LL);
    for (int p = 2; p <= limit; p++) if (phi[p] == p)
    {
      count[p] = 1;
      weighted[p] = p;
      for (int j = p; j <= limit; j += p) phi[j] -= phi[j] / p;
    }
    phi = sums<Z>(phi); count = sums<Z>(count); weighted = sums<Z>(weighted);
    for (int n : {999'999, 1'000'000, 1'000'001})
    {
      DirichletPrefixSum<Z> one(n, zeta_prefix_sum), id(n, id_prefix_sum);
      const auto pi = prime_prefix_sum(one), sum = prime_prefix_sum(id);
      const auto result = multiplicative_prefix_sum(sum - pi, totient_primepower);
      assert(pi.size() == one.size() && sum.size() == one.size()
        && result.size() == one.size() && result.is_multiplicative());
      check<Z>(pi, count); check<Z>(sum, weighted); check<Z>(result, phi);
    }
  }
  // 外部の既知値に頼らず、大きい入力でも閉じた式に戻ることを確認する。
  {
    const ll n = 10'000'000'019LL;
    const int root = int(iroot(n, 2));
    DirichletPrefixSum<R> one(n, zeta_prefix_sum);
    const auto primes = prime_prefix_sum(one);
    const auto restored = multiplicative_prefix_sum(primes, zeta_primepower);
    for (int j = 1; j <= root; j++) assert(restored.F(n / j) == R::S(n / j));
    // 独立した Lucy DP とも照合する。
    const auto lucy = internal::prime_prefix_sum_sieve(one);
    for (int j = 1; j <= root; j++) assert(primes.F(n / j) == lucy.F(n / j));
    DirichletPrefixSum<R> id(n, id_prefix_sum);
    const auto phi_primes = internal::prime_prefix_sum_sieve(id) - lucy;
    const auto phi_slow = internal::multiplicative_prefix_sum_sieve(phi_primes, totient_primepower);
    const auto phi23 = multiplicative_prefix_sum(phi_primes, totient_primepower);
    // p^e が int の範囲を超える評価も含め、全商位置で照合する。
    for (int j = 1; j <= root; j++) assert(phi23.F(n / j) == phi_slow.F(n / j));
  }
  cout << "Hello World\n";
}
