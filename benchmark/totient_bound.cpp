#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/prime/sieve/euler_product_prefix_sum.hpp"
#include "math/prime/large/primality_test.hpp"

// 同じ Euler 積エンジンで、直接の φ(p^e) と ζ をくくり出した形を比較する。
int main(int argc, char **argv)
{
  using R = RingAddSubMul<ll>;
  const string method = argc >= 2 ? argv[1] : "transformed";
  assert(argc <= 2 && (method == "direct" || method == "transformed"));
  LL(N);
  assert(N >= 1);
  const int k = int(iroot(N, 2));
  vvc<pair<ll, ll>> factors;
  for (int p : LinearSieve::primes(k + 1))
  {
    if (method == "transformed")
      factors.push_back({{p - 1, 1}, {p, -1}});
    else
    {
      vc<pair<ll, ll>> factor;
      for (ll cost = p - 1; cost <= N;)
      {
        factor.emplace_back(cost, 1);
        if (cost > N / p) break;
        cost *= p;
      }
      factors.push_back(move(factor));
    }
  }
  if (method == "direct")
  {
    const auto pi = prime_prefix_sum(DirichletPrefixSum<R>(N, zeta_prefix_sum));
    const DirichletPrefixSum<R> tail(N, [&](ll x)
    { return x <= k ? 1LL : 1 + pi.F(x) + is_prime(x + 1) - ll(factors.size()); });
    PRINT(euler_product_prefix_sum(tail, factors).F(N));
  }
  else
  {
    const DirichletPrefixSum<R> tail(N, [&](ll x)
    { return 1LL + (x > k && is_prime(x + 1)); });
    const auto product = euler_product_prefix_sum(tail, factors);
    PRINT(dirichlet_convolution_sum(product, DirichletPrefixSum<R>(N, zeta_prefix_sum)));
  }
}
