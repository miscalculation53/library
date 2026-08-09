#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/fps/power_projection.hpp"

template <class mint>
FormalPowerSeries<mint> naive_mul
(
  const FormalPowerSeries<mint> &f,
  const FormalPowerSeries<mint> &g,
  int d
)
{
  FormalPowerSeries<mint> res(d);
  repi(i, f.size()) repi(j, g.size())
  {
    if (i + j >= d)
      break;
    res[i + j] += f[i] * g[j];
  }
  return res;
}

// Test focus: the optimized result matches repeated naive multiplication for both mod paths.
template <class mint>
void test()
{
  using F = FormalPowerSeries<mint>;
  constexpr int n = 50;
  constexpr int m = 20;

  F f(n + 1), g(n + 1);
  repi(i, n + 1)
  {
    f[i] = mint((ll)(i + 2) * (i + 5) + 1);
    g[i] = mint((ll)(i + 3) * (i + 7) + 2);
  }

  F expected(m), product = g;
  repi(i, m)
  {
    expected[i] = product.get(n);
    product = naive_mul(product, f, n + 1);
  }
  assert(power_projection(n, m, f, g) == expected);
}

int main()
{
  test<modint998244353>();
  test<modint1000000007>();

  cout << "Hello World" << endl;
}
