#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/enumerate_multiplicative.hpp"
#include "math/modint/modint.hpp"

// 通常の整数環と、逆元を持つ modint の構造を明示する。
template <class T>
using TestRing = conditional_t<is_integral_ext<T>, RingAddSubMul<T>, FieldAddSubMulDiv<T>>;

// 線形篩から独立した試し割りで期待値を求める。
vc<PrimePower<int>> trial_factorize(int n)
{
  vc<PrimePower<int>> res;
  for (int p = 2; p <= n / p; p++)
  {
    if (n % p != 0)
      continue;
    int e = 0, pe = 1;
    while (n % p == 0)
      n /= p, e++, pe *= p;
    res.emplace_back(p, e, pe);
  }
  if (n > 1)
    res.emplace_back(n, 1, n);
  return res;
}

// 小さい境界値、篩の拡張後・縮小した範囲での再実行、呼び出し回数。
void test_boundaries()
{
  for (int n : {0, 1, 2, 3, 4, 8, 9, 25, 16, 100, 63, 257, 0, 1, 2})
  {
    vc<int> calls(n + 1), prime_calls(n + 1);
    int last = 1, last_prime = 1;
    auto f = enumerate_multiplicative<TestRing<ll>>(n, [&](const PrimePower<ll> &q) -> ll
    {
      assert(2 <= q.pe && q.pe <= n && last < q.pe);
      assert(q.e >= 1 && ipow<ll>(q.p, q.e) == q.pe);
      auto fac = trial_factorize(q.pe);
      assert(fac.size() == 1 && fac[0] == q);
      calls[q.pe]++;
      last = q.pe;
      return q.pe;
    });
    auto g = enumerate_completely_multiplicative<TestRing<ll>>(n, [&](const PrimePower<int> &q)
    {
      assert(2 <= q.p && q.p <= n && last_prime < q.p);
      assert(q.e == 1 && q.pe == q.p);
      prime_calls[q.p]++;
      last_prime = q.p;
      return q.p;
    });
    assert(f.size() == size_t(n + 1) && g == f && f[0] == 0);
    for (int d = 1; d <= n; d++)
    {
      assert(f[d] == d);
      const auto fac = trial_factorize(d);
      const bool is_primepower = fac.size() == 1;
      const bool is_prime = is_primepower && fac[0].e == 1;
      assert(calls[d] == int(is_primepower));
      assert(prime_calls[d] == int(is_prime));
    }
  }
}

// φ は gcd による定義、τ・σ・μ は約数から検証する。
template <class T>
void test_standard_functions()
{
  const int n = 512;
  auto phi = enumerate_multiplicative<TestRing<T>>(n, totient_primepower);
  auto mu = enumerate_multiplicative<TestRing<T>>(n, mobius_primepower);
  auto tau = enumerate_multiplicative<TestRing<T>>(n, divisor_count_primepower);
  auto sigma = enumerate_multiplicative<TestRing<T>>(n, divisor_sum_primepower);
  auto unit = enumerate_completely_multiplicative<TestRing<T>>(n, e_primepower);
  auto zeta = enumerate_completely_multiplicative<TestRing<T>>(n, zeta_primepower);
  auto id = enumerate_completely_multiplicative<TestRing<T>>(n, id_primepower);
  static_assert(is_same_v<decltype(phi), vc<T>>);
  static_assert(is_same_v<decltype(id), vc<T>>);
  assert(unit == enumerate_multiplicative<TestRing<T>>(n, e_primepower));
  assert(zeta == enumerate_multiplicative<TestRing<T>>(n, zeta_primepower));
  assert(id == enumerate_multiplicative<TestRing<T>>(n, id_primepower));
  for (int d = 1; d <= n; d++)
  {
    ll expected_phi = 0, expected_tau = 0, expected_sigma = 0;
    T sum_mu = 0;
    for (int k = 1; k <= d; k++)
    {
      expected_phi += gcd(k, d) == 1;
      if (d % k == 0)
        expected_tau++, expected_sigma += k, sum_mu += mu[k];
    }
    assert(phi[d] == T(expected_phi));
    assert(tau[d] == T(expected_tau));
    assert(sigma[d] == T(expected_sigma));
    assert(sum_mu == T(d == 1));
    assert(unit[d] == T(d == 1));
    assert(zeta[d] == T(1));
    assert(id[d] == T(d));
  }
}

// 素べきに任意の値を割り当てる。0・負数・合成数 mod の非可逆元も含む。
template <class T>
void test_arbitrary_values()
{
  const int n = 4096;
  mt19937 rng(20260906);
  for (int t = 0; t < 8; t++)
  {
    vc<T> values(n + 1);
    for (int d = 2; d <= n; d++)
      values[d] = T(int(rng() % 9) - 4);
    auto f = enumerate_multiplicative<TestRing<T>>(n, [&](const auto &q)
    { return values[q.pe]; });
    auto g = enumerate_completely_multiplicative<TestRing<T>>(n, [&](const auto &q)
    { return values[q.p]; });
    assert(f[0] == T(0) && g[0] == T(0));
    for (int d = 1; d <= n; d++)
    {
      T expected_f = 1, expected_g = 1;
      for (const auto &q : trial_factorize(d))
      {
        expected_f = expected_f * values[q.pe];
        for (int e = 0; e < q.e; e++)
          expected_g = expected_g * values[q.p];
      }
      assert(f[d] == expected_f);
      assert(g[d] == expected_g);
    }
  }
}

void test_completely_multiplicative()
{
  using mint = modint998244353;
  for (ll k : {0, 1, 2, 17})
  {
    auto f = enumerate_completely_multiplicative<TestRing<mint>>(4096, pow_primepower(k));
    for (int d = 1; d <= 4096; d++)
      assert(f[d] == mint(d).pow(k));
  }
  auto unit = enumerate_completely_multiplicative<TestRing<int>>(4096, [](const auto &)
  { return 0; });
  assert(unit[0] == 0 && unit[1] == 1);
  for (int d = 2; d <= 4096; d++)
    assert(unit[d] == 0);
}

// 引数付きラムダを関数から返して保存し、後から使う。
template <class T>
void test_parameterized_callbacks()
{
  auto make_callbacks = [](ll k)
  {
    return make_pair(pow_primepower(k), divisor_k_primepower(k));
  };
  auto [square, sigma2] = make_callbacks(2);
  auto [cube, sigma3] = make_callbacks(3);
  for (int k : {0, 1, 2, 3})
  {
    const int n = 128;
    auto power = k == 2 ? square : k == 3 ? cube : pow_primepower(k);
    auto divisor_power = k == 2 ? sigma2 : k == 3 ? sigma3 : divisor_k_primepower(k);
    auto f = enumerate_multiplicative<TestRing<T>>(n, power);
    auto sigma = enumerate_multiplicative<TestRing<T>>(n, divisor_power);
    assert(f == enumerate_completely_multiplicative<TestRing<T>>(n, power));
    for (int d = 1; d <= n; d++)
    {
      ll expected_sum = 0;
      for (int a = 1; a <= d; a++)
        if (d % a == 0)
          expected_sum += ipow<ll>(a, k);
      assert(f[d] == T(ipow<ll>(d, k)));
      assert(sigma[d] == T(expected_sum));
    }
  }
}

// 小さい法では逆元を全探索して比較する。非可逆元と指数 0 も検証する。
template <class T>
void test_inverse_powers()
{
  auto make_callback = [](ll k) { return pow_inv_primepower(k); };
  auto first_power = make_callback(1);
  auto sixth_power = make_callback(6);
  for (int k : {0, 1, 2, 6, 16})
  {
    auto callback = k == 1 ? first_power : k == 6 ? sixth_power : make_callback(k);
    auto f = enumerate_completely_multiplicative<TestRing<T>>(128, callback);
    assert(f == enumerate_multiplicative<TestRing<T>>(128, callback));
    for (int d = 1; d <= 128; d++)
    {
      T expected = 0;
      if (k == 0)
        expected = 1;
      else
        for (int r = 0; r < T::mod(); r++)
          if ((d * r) % T::mod() == 1 % T::mod())
          {
            expected = 1;
            for (int e = 0; e < k; e++)
              expected *= T(r);
            break;
          }
      assert(f[d] == expected);
    }
  }
}

// 直接 p^0 に適用しても f(1) = 1。素べきの型は引数から推論する。
template <class T>
void test_callback_identity()
{
  const PrimePower<int> q(2, 0, 1);
  assert(e_primepower(q, TestRing<T>{}) == T(1));
  assert(zeta_primepower(q, TestRing<T>{}) == T(1));
  assert(id_primepower(q, TestRing<T>{}) == T(1));
  assert(mobius_primepower(q, TestRing<T>{}) == T(1));
  assert(divisor_count_primepower(q, TestRing<T>{}) == T(1));
  assert(divisor_sum_primepower(q, TestRing<T>{}) == T(1));
  assert(totient_primepower(q, TestRing<T>{}) == T(1));
  assert(pow_primepower(0)(q, TestRing<T>{}) == T(1));
  assert(pow_inv_primepower(2)(q, TestRing<T>{}) == T(1));
  assert(divisor_k_primepower(3)(q, TestRing<T>{}) == T(1));
}

void test_divisor_sum_overflow()
{
  assert(divisor_sum_primepower(PrimePower<int>(INT_MAX), RingAddSubMul<ll>{}) == ll(INT_MAX) + 1);
  const PrimePower<ll> q(2, 62, 1LL << 62);
  assert(divisor_sum_primepower(q, RingAddSubMul<ll>{}) == LLONG_MAX);
  assert(divisor_sum_primepower(q, TestRing<modint998244353>{}) == modint998244353(LLONG_MAX));
  assert(divisor_sum_primepower(q, TestRing<static_modint32<12>>{}) == static_modint32<12>(LLONG_MAX));
}

void test_type_deduction()
{
  const ll value = 1;
  int calls = 0;
  auto callback = [&](const auto &) -> const ll &
  {
    calls++;
    return value;
  };
  // 推論のためにコールバックを実行せず、参照・const は要素型から除く。
  for (int n : {0, 1})
  {
    auto f = enumerate_multiplicative(n, callback);
    auto g = enumerate_completely_multiplicative(n, callback);
    static_assert(is_same_v<decltype(f), vc<ll>>);
    static_assert(is_same_v<decltype(g), vc<ll>>);
    assert(f == (n == 0 ? vc<ll>{0} : vc<ll>{0, 1}));
    assert(f == g && calls == 0);
    assert(f == enumerate_multiplicative<TestRing<ll>>(n, zeta_primepower));
    assert(f == enumerate_completely_multiplicative<TestRing<ll>>(n, zeta_primepower));
  }
  // 明示指定した場合は、コールバックとは異なる型でも結果を受け取れる。
  auto f = enumerate_multiplicative<TestRing<ll>>(12, [](const auto &q) { return q.e + 1; });
  auto g = enumerate_completely_multiplicative<TestRing<ll>>(12, [](const auto &q) { return q.p; });
  static_assert(is_same_v<decltype(f), vc<ll>>);
  static_assert(is_same_v<decltype(g), vc<ll>>);
  assert(f[12] == 6 && g[12] == 12);
}

void test_reference_callbacks()
{
  auto get = [](auto &q) { return ll(q.pe); };
  auto get_with_ring = [](auto &q, auto) { return ll(q.pe); };
  for (int n : {0, 1, 2, 16, 100})
  {
    vc<ll> expected(n + 1);
    iota(expected.begin(), expected.end(), 0LL);
    assert(enumerate_multiplicative(n, get) == expected);
    assert(enumerate_completely_multiplicative(n, get) == expected);
    assert(enumerate_multiplicative<TestRing<ll>>(n, get) == expected);
    assert(enumerate_completely_multiplicative<TestRing<ll>>(n, get) == expected);
    assert(enumerate_multiplicative<TestRing<ll>>(n, get_with_ring) == expected);
    assert(enumerate_completely_multiplicative<TestRing<ll>>(n, get_with_ring) == expected);
  }
}

int main()
{
  test_boundaries();
  test_standard_functions<int>();
  test_standard_functions<ll>();
  test_standard_functions<modint998244353>();
  test_standard_functions<static_modint32<12>>();
  test_arbitrary_values<ll>();
  test_arbitrary_values<modint998244353>();
  test_arbitrary_values<static_modint32<12>>();
  test_completely_multiplicative();
  test_parameterized_callbacks<modint998244353>();
  test_parameterized_callbacks<static_modint32<12>>();
  test_inverse_powers<static_modint32<7>>();
  test_inverse_powers<static_modint32<12>>();
  for (int m : {17, 12, 1})
  {
    modint::set_mod(m);
    test_inverse_powers<modint>();
    test_parameterized_callbacks<modint>();
  }
  test_callback_identity<modint998244353>();
  test_callback_identity<static_modint32<12>>();
  test_divisor_sum_overflow();
  test_type_deduction();
  test_reference_callbacks();
  PRINT("Hello World");
}
