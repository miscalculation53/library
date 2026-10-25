#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/bmbm.hpp"

template <class mint>
void test()
{
  using F = FormalPowerSeries<mint>;
  for (const vc<mint> &a : {vc<mint>{}, vc<mint>(8), vc<mint>{1, 0, 0, 0, 0, 0},
                            vc<mint>{0, 1, 1, 1, 1, 1, 1, 1}})
  {
    const auto c = berlekamp_massey<FieldAddSubMulDiv<mint>>(a);
    const auto [p, q] = linear_recurrence_gf(a, c);
    assert(!q.empty() && q.back() != 0);
    for (int k = 0; k < 20; ++k)
    {
      const mint expected = a.empty() ? mint(0) : a[min(k, int(a.size()) - 1)];
      assert(bmbm(a, k, false) == expected);
      assert(bostan_mori(p, q, k) == expected);
    }
    assert(bmbm(a, 1000000000000000000LL, false) == (a.empty() ? mint(0) : a.back()));
  }

  // 高次の係数がゼロでも、初期項に由来する分子の次数を保つ。
  const auto [p, q] = linear_recurrence_gf(vc<mint>{0, 1}, vc<mint>{-1, 1, 0});
  assert(p == F({0, 1}) && q == F({1, -1}));

  mt19937_64 rng(20261025);
  for (int d = 1; d <= 12; ++d) for (int zeros = 0; zeros <= d; ++zeros)
  {
    vc<mint> c(d + 1), a(150);
    for (int j = 1; j <= d - zeros; ++j) c[j] = rng() % 17;
    for (int i = 0; i < d; ++i) a[i] = rng() % 17;
    for (int i = d; i < int(a.size()); ++i)
      for (int j = 1; j <= d; ++j) a[i] += c[j] * a[i - j];
    const auto [num, den] = linear_recurrence_gf(vc<mint>(a.begin(), a.begin() + d), c);
    vc<mint> prefix(a.begin(), a.begin() + 2 * d + 1);
    for (int k : {0, d - 1, d, 2 * d, 2 * d + 1, 149})
    {
      assert(bmbm(prefix, k, false) == a[k]);
      assert(bostan_mori(num, den, k) == a[k]);
    }
  }
}

int main()
{
  test<modint998244353>();
  test<modint1000000007>();
  PRINT("Hello World");
}
