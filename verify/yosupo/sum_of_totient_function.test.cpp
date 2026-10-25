#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_totient_function"

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "math/modint/modint.hpp"

int main()
{
  using mint = modint998244353;
  using R = FieldAddSubMulDiv<mint>;
  LL(N);
  DirichletPrefixSum<R> one(N, zeta_prefix_sum, true);
  DirichletPrefixSum<R> id(N, id_prefix_sum, true);
  PRINT((id / one).F(N));
}
