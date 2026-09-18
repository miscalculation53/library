#define PROBLEM "https://judge.yosupo.jp/problem/dirichlet_convolution_and_prefix_sums"
#define FAST_IO

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "math/modint/modint.hpp"

int main()
{
  using mint = modint998244353;
  using R = RingAddSubMul<mint>;
  INT(T);
  while (T--)
  {
    LL(N);
    DirichletPrefixSum<R> f(N), g(N);
    for (int i = 0; i < f.size(); i++) { IN(mint, x); f.setF(f.value(i), x); }
    for (int i = 0; i < g.size(); i++) { IN(mint, x); g.setF(g.value(i), x); }
    const auto h = f * g;
    vc<mint> answer;
    answer.reserve(h.size());
    for (int i = 0; i < h.size(); i++) answer.push_back(h.F(h.value(i)));
    PRINT(answer);
  }
}
