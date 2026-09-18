#define PROBLEM "https://judge.yosupo.jp/problem/counting_squarefrees"

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"

int main()
{
  LL(N);
  using R = RingAddSubMul<ll>;
  const auto mobius = DirichletPrefixSum<R, 2>(N, zeta_prefix_sum, true).inv();
  ll answer = 0;
  const ll root = iroot(N, 2);
  for (ll l = 1; l <= root; )
  {
    const ll q = N / l / l;
    const ll r = iroot(N / q, 2);
    answer += q * (mobius.F(r) - mobius.F(l - 1));
    l = r + 1;
  }
  PRINT(answer);
}
