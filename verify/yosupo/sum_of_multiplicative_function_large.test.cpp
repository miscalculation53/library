#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large"

#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/modint/modint.hpp"

int main()
{
  using mint = static_modint32<469762049>;
  using R = RingAddSubMul<mint>;
  INT(T);
  while (T--)
  {
    LL(N, a, b);
    DirichletPrefixSum<R> one(N, zeta_prefix_sum), id(N, id_prefix_sum);
    const auto prime_sum = prime_prefix_sum(one) * mint(a) + prime_prefix_sum(id) * mint(b);
    const auto result = multiplicative_prefix_sum(prime_sum,
      [&](const auto &q) { return mint(a) * q.e + mint(b) * q.p; });
    PRINT(result.F(N));
  }
}
