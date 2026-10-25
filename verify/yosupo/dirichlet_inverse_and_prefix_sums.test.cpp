#define PROBLEM "https://judge.yosupo.jp/problem/dirichlet_inverse_and_prefix_sums"
#define FAST_IO

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "math/modint/modint.hpp"

int main()
{
  using mint = modint998244353;
  // f(1) は任意の非零値なので、環に逆元演算を提供する。
  using R = FieldAddSubMulDiv<mint>;
  INT(T);
  while (T--)
  {
    LL(N);
    DirichletPrefixSum<R> f(N);
    for (int i = 0; i < f.size(); i++)
    {
      IN(mint, x);
      f.setF(f.value(i), x);
    }
    const auto g = f.inv();
    vc<mint> answer(g.size());
    for (int i = 0; i < g.size(); i++) answer[i] = g.F(g.value(i));
    PRINT(answer);
  }
}
