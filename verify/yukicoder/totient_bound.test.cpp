#define PROBLEM "https://yukicoder.me/problems/no/1322"

#include "math/prime/sieve/euler_product_prefix_sum.hpp"
#include "math/prime/large/primality_test.hpp"

int main()
{
  using R = RingAddSubMul<ll>;
  LL(N);
  const int K = int(iroot(N, 2));
  vvc<pair<ll, ll>> factors;
  for (int p : LinearSieve::primes(K + 1))
  {
    factors.push_back({{p - 1, 1}, {p, -1}});
  }
  // p>K+1 の因子は二つ選べない。和は境界の素数判定だけになる。
  DirichletPrefixSum<R> tail(N, [&](ll x) { return 1LL + (x > K && is_prime(x + 1)); });
  auto product = euler_product_prefix_sum(tail, factors);
  DirichletPrefixSum<R> zeta(N, zeta_prefix_sum);
  PRINT(dirichlet_convolution_sum(product, zeta));
}
