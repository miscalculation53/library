#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/rational_sum.hpp"

template <class Mint>
FormalPowerSeries<Mint> naive_mul
(
  const FormalPowerSeries<Mint> &f,
  const FormalPowerSeries<Mint> &g
)
{
  if (f.empty() || g.empty())
    return {};
  FormalPowerSeries<Mint> res(f.size() + g.size() - 1);
  repi(i, f.size()) repi(j, g.size()) res[i + j] += f[i] * g[j];
  return res;
}

// Test focus: the dense branch works for both NTT-friendly and arbitrary moduli.
template <class Mint>
void test_dense_branch()
{
  using F = FormalPowerSeries<Mint>;
  F p(61), q(100), r(100), s(61);
  repi(i, p.size()) p[i] = Mint((ll)(i + 1) * (i + 4) + 1);
  repi(i, q.size()) q[i] = Mint((ll)(i + 2) * (i + 5) + 1);
  repi(i, r.size()) r[i] = Mint((ll)(i + 3) * (i + 6) + 1);
  repi(i, s.size()) s[i] = Mint((ll)(i + 4) * (i + 7) + 1);

  const auto actual = rational_plus(pair{p, q}, pair{r, s});
  const pair<F, F> expected =
  {
    naive_mul(p, s) + naive_mul(q, r),
    naive_mul(q, s)
  };
  assert(actual == expected);
}

int main()
{
  test_dense_branch<modint998244353>();
  test_dense_branch<modint1000000007>();
  PRINT("Hello World");
}
