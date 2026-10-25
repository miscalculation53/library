#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/euler_product_prefix_sum.hpp"
#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/prime/large/primality_test.hpp"
#include "math/modint/modint.hpp"
#include "algebra/bit.hpp"
#include "algebra/polynomial_ring.hpp"

template <class R>
vc<typename R::S> naive_product(vc<typename R::S> f, const vvc<pair<ll, typename R::S>> &factors)
{
  const int n = int(f.size()) - 1;
  for (const auto &factor : factors)
  {
    auto next = f;
    for (const auto &[d, a] : factor)
      for (int v = 1; v <= n / d; v++) next[v * d] = R::add(next[v * d], R::mul(f[v], a));
    f = move(next);
  }
  for (int i = 1; i <= n; i++) f[i] = R::add(f[i - 1], f[i]);
  return f;
}

template <class R>
void random_test()
{
  using S = typename R::S;
  mt19937 rng(419825);
  auto value = [&] { return internal::multiplicative_from_integer<R>(int(rng() % 9) - 4); };
  for (int n : {0, 1, 2, 3, 4, 8, 9, 15, 16, 63, 64, 99, 128, 257})
    for (int repeat = 0; repeat < 12; repeat++)
  {
    vc<S> seed(n + 1, R::e0());
    for (int i = 1; i <= n; i++) if (rng() % 3 == 0) seed[i] = value();
    if (n > 0) seed[1] = value();
    vvc<pair<ll, typename R::S>> factors(rng() % 8);
    for (auto &factor : factors)
      for (int j = rng() % 9; j > 0; j--)
        factor.emplace_back(rng() % (n + 10) + 1, value());
    // 同じコストの重複、負の重み、コスト 1 を同じ局所因子に含める。
    factors.push_back({{1, R::minus(R::e1())}, {2, R::e1()}, {2, R::e1()}, {3, R::minus(R::e1())}});
    const auto expected = naive_product<R>(seed, factors);
    auto prefix = seed;
    for (int i = 1; i <= n; i++) prefix[i] = R::add(prefix[i - 1], seed[i]);
    {
      const DirichletPrefixSum<R> initial(n, [&](ll x) { return prefix[x]; });
      const auto result = euler_product_prefix_sum(initial, factors);
      assert(result.n() == n && result.size() == initial.size() && !result.is_multiplicative());
      for (int x = 0; x <= n; x++) if (result.contains(x)) assert(result.F(x) == expected[x]);
      auto unit = vc<S>(n + 1, R::e0());
      if (n) unit[1] = R::e1();
      const auto expected_unit = naive_product<R>(unit, factors);
      const auto product = euler_product_prefix_sum<R>(n, factors);
      for (int x = 0; x <= n; x++) if (product.contains(x)) assert(product.F(x) == expected_unit[x]);
    }
  }
}

// 既定構築は零でなく、要素に通常の算術演算子もない。
struct ShiftRing
{
  struct S
  {
    int x = -456;
    friend bool operator==(S a, S b) { return a.x == b.x; }
    friend bool operator!=(S a, S b) { return a.x != b.x; }
  };
  static S make(ll x) { return {int((x % 101 + 101) % 101) + 7}; }
  static S e0() { return make(0); }
  static S e1() { return make(1); }
  static S add(S a, S b) { return make(a.x + b.x - 14); }
  static S minus(S a) { return make(7 - a.x); }
  static S mul(S a, S b) { return make((a.x - 7) * (b.x - 7)); }
};

// φ(x)>=sqrt(x/2) より、上限 N に対して x<=2N^2 を篩えば全解を含む。
void totient_test()
{
  using R = RingAddSubMul<ll>;
  const int limit = 400;
  vc<int> phi(2 * limit * limit + 1);
  vc<int> omega(phi.size(), 0);
  iota(phi.begin(), phi.end(), 0);
  for (int p = 2; p < int(phi.size()); p++) if (phi[p] == p)
    for (int j = p; j < int(phi.size()); j += p) phi[j] -= phi[j] / p, omega[j]++;
  vc<ll> count(limit + 1, 0), weighted(count);
  using Poly = PolynomialRingArray<R, 3>;
  vc<Poly::S> by_omega(limit + 1, Poly::e0());
  for (int i = 1; i < int(phi.size()); i++) if (phi[i] <= limit) count[phi[i]]++, weighted[phi[i]] += i;
  for (int i = 1; i < int(phi.size()); i++) if (phi[i] <= limit && omega[i] < 3) by_omega[phi[i]][omega[i]]++;
  for (int i = 1; i <= limit; i++) count[i] += count[i - 1], weighted[i] += weighted[i - 1];
  for (int i = 1; i <= limit; i++) by_omega[i] = Poly::add(by_omega[i - 1], by_omega[i]);
  for (int n : {1, 2, 3, 4, 9, 15, 16, 24, 25, 26, 48, 49, 63, 64, 120, 121, 334, 400})
  {
    const int root = int(iroot(n, 2));
    vvc<pair<ll, typename R::S>> factors, weights, transformed;
    for (int p : LinearSieve::primes(n + 1))
    {
      vc<pair<ll, typename R::S>> factor, weight;
      for (ll d = p - 1, pe = p; d <= n; d *= p, pe *= p)
        factor.emplace_back(d, 1), weight.emplace_back(d, pe);
      factors.push_back(factor); weights.push_back(weight);
      if (p <= root + 1) transformed.push_back({{p - 1, 1}, {p, -1}});
    }
    const auto result = euler_product_prefix_sum<R>(n, factors);
    const auto sum = euler_product_prefix_sum<R>(n, weights);
    // 大素数側を集約しても同じ全累積和になる。重みは w(x)=x も試す。
    const int small = int(LinearSieve::primes(root + 1).size());
    factors.resize(small); weights.resize(small);
    auto tail_value = [&](ll x, bool weight)
    {
      ll value = 1;
      for (int p : LinearSieve::primes(int(x + 1)))
      {
        if (p > root + 1) value += weight ? p : 1;
      }
      return value;
    };
    const auto partitioned = euler_product_prefix_sum(DirichletPrefixSum<R>(n,
      [&](ll x) { return tail_value(x, false); }), factors);
    const auto weighted_partitioned = euler_product_prefix_sum(DirichletPrefixSum<R>(n,
      [&](ll x) { return tail_value(x, true); }), weights);
    vvc<pair<ll, Poly::S>> marked;
    for (const auto &factor : factors)
    {
      vc<pair<ll, Poly::S>> local;
      for (const auto &[d, a] : factor) { (void)a; local.push_back({d, {0, 1, 0}}); }
      marked.push_back(move(local));
    }
    const auto marked_sum = euler_product_prefix_sum(DirichletPrefixSum<Poly>(n,
      [&](ll x) { return Poly::S{1, tail_value(x, false) - 1, 0}; }), marked);
    const DirichletPrefixSum<R> tail(n, [&](ll x) { return 1LL + (x > root && is_prime(x + 1)); });
    const auto transformed_sum = euler_product_prefix_sum(tail, transformed);
    const auto zeta = DirichletPrefixSum<R>(n, zeta_prefix_sum);
    assert(dirichlet_convolution_sum(transformed_sum, zeta) == count[n]);
    for (int x = 0; x <= n; x++) if (result.contains(x))
    {
      assert(result.F(x) == count[x] && sum.F(x) == weighted[x]);
      assert(partitioned.F(x) == count[x] && weighted_partitioned.F(x) == weighted[x]);
      assert(marked_sum.F(x) == by_omega[x]);
    }
  }
}

// ブロックへの更新を使わず、最小素因数で分ける DFS と最大制約でも照合する。
void large_totient_test()
{
  using R = RingAddSubMul<ll>;
  const ll n = 10'000'000'000LL;
  const int k = int(iroot(n, 2));
  const auto pi = prime_prefix_sum(DirichletPrefixSum<R>(n, zeta_prefix_sum));
  const DirichletPrefixSum<R> shifted(n, [&](ll x) { return pi.F(x) + is_prime(x + 1); });
  const auto primes = LinearSieve::primes(k + 1);
  auto dfs = [&](auto &&self, ll t, int begin) -> ll
  {
    const ll count = shifted.F(t) - begin;
    if (count <= 0) return 1;
    ll value = 1 + count;
    for (int i = begin; i < int(primes.size()); i++)
    {
      const ll p = primes[i];
      if (p - 1 > t / (p - 1)) break;
      value += self(self, t / (p - 1), i + 1) - 1;
      for (ll cost = p - 1; cost <= t / p;)
      {
        cost *= p;
        value += self(self, t / cost, i + 1);
      }
    }
    return value;
  };
  vvc<pair<ll, typename R::S>> factors, transformed;
  for (int p : primes)
  {
    transformed.push_back({{p - 1, 1}, {p, -1}});
    vc<pair<ll, typename R::S>> factor;
    for (ll cost = p - 1; cost <= n;)
    {
      factor.emplace_back(cost, 1);
      if (cost > n / p) break;
      cost *= p;
    }
    factors.push_back(move(factor));
  }
  const DirichletPrefixSum<R> tail(n, [&](ll x)
  { return x <= k ? 1LL : 1 + shifted.F(x) - ll(factors.size()); });
  const auto count = euler_product_prefix_sum(tail, factors);
  const auto product = euler_product_prefix_sum(DirichletPrefixSum<R>(n,
    [&](ll x) { return 1LL + (x > k && is_prime(x + 1)); }), transformed);
  assert(count.F(n) == dirichlet_convolution_sum(product, DirichletPrefixSum<R>(n, zeta_prefix_sum)));
  for (ll j : {1LL, 2LL, 7LL, 31LL, 1000LL, 100000LL}) assert(count.F(n / j) == dfs(dfs, n / j, 0));
}

int main()
{
  random_test<RingAddSubMul<ll>>();
  random_test<RingAddSubMul<static_modint32<6>>>();
  random_test<RingXorAnd<ull>>();
  random_test<ShiftRing>();
  random_test<PolynomialRingArray<RingAddSubMul<ll>, 3>>();
  totient_test();
  large_totient_test();
  cout << "Hello World\n";
}
