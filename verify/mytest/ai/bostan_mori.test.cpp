#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/fps/bostan_mori.hpp"

// Test focus: dense recurrence evaluation under NTT-friendly and arbitrary moduli.
template <class mint>
void test()
{
  using F = FormalPowerSeries<mint>;
  constexpr int d = 64;
  constexpr int k = 1000;

  F p(d), q(d + 1);
  repi(i, d) p[i] = mint((ll)(i + 1) * (i + 4) + 3);
  q[0] = 1;
  repi(i, 1, d + 1) q[i] = mint((ll)i * (i + 2) + 1);

  vc<mint> expected(k + 1);
  repi(i, k + 1)
  {
    expected[i] = p.get(i);
    repi(j, 1, min(i, d) + 1) expected[i] -= q[j] * expected[i - j];
  }
  assert(bostan_mori(p, q, k) == expected[k]);
}

// Test focus: a constant denominator and an index beyond 32 bits.
template <class mint>
void test_constant_denominator()
{
  using F = FormalPowerSeries<mint>;
  const F p = {2, 3, 5};
  const F q = {2};
  repi(k, 6) assert(bostan_mori(p, q, k) == p.get(k) / 2);
  assert(bostan_mori(p, q, 1LL << 32) == 0);
}

int main()
{
  test<modint998244353>();
  test<modint1000000007>();
  test_constant_denominator<modint998244353>();
  test_constant_denominator<modint1000000007>();

  cout << "Hello World" << endl;
}
