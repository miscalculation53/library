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

// 切り替え位置、NTT の長さ、疎な FPS 除算の境界を逐次計算と照合する。
template <class mint>
void test_switch()
{
  using F = FormalPowerSeries<mint>;
  mt19937_64 rng(20260926);
  for (int d : {1, 2, 3, 7, 8, 15, 16, 31, 32, 63, 64, 127, 128,
                199, 200, 201, 255, 256, 257, 511, 512, 513, 1024})
    for (int mode = 0; mode < 4; ++mode)
    {
      F p(mode == 0 ? 0 : mode == 1 ? d : 2 * d + 3), q(d + 1);
      for (auto &x : p) x = rng() % mint::mod();
      for (auto &x : q) x = 1 + rng() % (mint::mod() - 1);
      if (mode == 3)
        for (int i = 1; i < d; ++i) q[i] = 0;
      const int last = 4 * d + 3;
      F expected(last + 1);
      const mint iq0 = q[0].inv();
      for (int i = 0; i <= last; ++i)
      {
        expected[i] = p.get(i);
        for (int j = 1; j <= min(i, d); ++j) expected[i] -= q[j] * expected[i - j];
        expected[i] *= iq0;
      }
      assert(bostan_mori(p, q, -1) == 0);
      for (int k : {0, 1, d / 2, d - 1, d, d + 1, 2 * d - 1, 2 * d,
                    2 * d + 1, 4 * d - 1, 4 * d, 4 * d + 1, last})
        assert(bostan_mori(p, q, k) == expected[k]);
      for (int t = 0; t < 5; ++t)
      {
        const int k = rng() % (last + 1);
        assert(bostan_mori(p, q, k) == expected[k]);
      }
    }
}

// p / ((1 - ax) p) = 1 / (1 - ax) を使い、64 bit の添字を照合する。
template <class mint>
void test_large_index()
{
  using F = FormalPowerSeries<mint>;
  mt19937_64 rng(20260927);
  for (int d : {1, 2, 17, 64, 257, 1024})
  {
    F p(d), q(d + 1);
    for (auto &x : p) x = 1 + rng() % (mint::mod() - 1);
    const mint a = 1 + rng() % (mint::mod() - 1);
    for (int i = 0; i < d; ++i) q[i] += p[i], q[i + 1] -= a * p[i];
    for (ll k : {1LL << 32, (1LL << 32) + 1, 1000000000000000000LL,
                 numeric_limits<ll>::max()})
      assert(bostan_mori(p, q, k) == a.pow(k));
  }
}

int main()
{
  test<modint998244353>();
  test<modint1000000007>();
  test_constant_denominator<modint998244353>();
  test_constant_denominator<modint1000000007>();
  test_switch<modint998244353>();
  test_switch<modint1000000007>();
  test_switch<static_modint32<17>>();
  test_large_index<modint998244353>();
  test_large_index<modint1000000007>();
  test_large_index<static_modint32<17>>();

  cout << "Hello World" << endl;
}
