#define FAST_IO

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "math/modint/modint.hpp"
#include "math/quotients.hpp"

// Library Checker と同じ入力。auto/direct は積、divide/divide-direct は商。
int main(int argc, char **argv)
{
  using mint = modint998244353;
  using R = FieldAddSubMulDiv<mint>;
  const string method = argc >= 2 ? argv[1] : "auto";
  assert(argc <= 2 && (method == "auto" || method == "direct"
    || method == "divide" || method == "divide-direct"));
  INT(T);
  while (T--)
  {
    LL(N);
    vc<ll> q;
    for (const auto &[x, l, r] : quotients(N)) q.push_back(x);
    VEC(mint, int(q.size()), a, b);
    DirichletPrefixSum<R> f(N), g(N);
    for (int i = 0; i < int(q.size()); i++)
      f.setF(q[i], a[i]), g.setF(q[i], b[i]);
    const auto direct = [&]
    {
      const auto af = f.to_series(), bf = g.to_series(), hf = af * bf;
      DirichletPrefixSum<R> h(N);
      mint prefix = 0;
      for (int i = 1; i <= hf.n(); i++) h.setF(i, prefix += hf.f(i));
      for (int j = 1; j <= N / (ll(hf.n()) + 1); j++)
      {
        const ll x = N / j;
        const int m = int(iroot(x, 2));
        auto value = R::minus(R::mul(f.F(m), g.F(m)));
        for (int i = 1; i <= m; i++)
          value = R::add(value, R::add(R::mul(af.f(i), g.F(x / i)), R::mul(bf.f(i), f.F(x / i))));
        h.setF(x, value);
      }
      return h;
    };
    const auto direct_division = [&]
    {
      const auto af = f.to_series(), bf = g.to_series(), hf = af / bf;
      DirichletPrefixSum<R> h(N);
      mint prefix = 0;
      for (int i = 1; i <= hf.n(); i++) h.setF(i, prefix += hf.f(i));
      const mint inverse = bf.f(1).inv();
      for (int j = int(N / (ll(hf.n()) + 1)); j >= 1; j--)
      {
        const ll x = N / j;
        const int m = int(iroot(x, 2));
        mint value = f.F(x);
        for (int i = 1; i <= m; i++) value -= hf.f(i) * (g.F(x / i) - g.F(m));
        for (int i = 2; i <= m; i++) value -= bf.f(i) * h.F(x / i);
        h.setF(x, value * inverse);
      }
      return h;
    };
    const auto h = method == "direct" ? direct() : method == "divide-direct" ? direct_division()
      : method == "divide" ? f / g : f * g;
    vc<mint> answer;
    answer.reserve(q.size());
    for (ll x : q) answer.push_back(h.F(x));
    PRINT(answer);
  }
}
