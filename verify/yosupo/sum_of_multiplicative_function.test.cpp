#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_multiplicative_function"

#include "math/prime/sieve/multiplicative_prefix_sum.hpp"
#include "math/modint/modint.hpp"

int main()
{
  using mint = static_modint32<469762049>;
  using R = FieldAddSubMulDiv<mint>;
  INT(T);
  while (T--)
  {
    LL(N, a, b);
    DirichletPrefixSum<R> one(N, zeta_prefix_sum), id(N, id_prefix_sum);
    const auto prime_sum = a * prime_prefix_sum(one) + b * prime_prefix_sum(id);
    auto result = multiplicative_prefix_sum(prime_sum,
      [&](auto &q) { return a * q.e + b * q.p; });
    PRINT(result.F(N));
  }
}
