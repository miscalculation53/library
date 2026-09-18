#pragma once

#include "fps.hpp"

/**
 * @brief 多項式の Half-GCD・拡張 GCD・剰余環の逆元
 * @docs docs/math/fps/half_gcd.md
 */

namespace internal
{
template <class mint>
struct PolynomialHalfGCD
{
  using P = FormalPowerSeries<mint>;
  static constexpr int cutoff = 128;

  // 多項式は末尾の零を除く。零多項式は空配列。
  static pair<P, P> divmod(P a, const P &b)
  {
    assert(!b.empty());
    int k = a.sz() - b.sz() + 1;
    if (k <= 0) return {{}, std::move(a)};
    if (min(k, b.sz()) > cutoff) return a.divmod(b);
    P q(k);
    mint iv = b.back().inv();
    for (int i = k - 1; i >= 0; --i)
    {
      mint v = q[i] = a[i + b.sz() - 1] * iv;
      for (int j = 0; j < b.sz(); ++j) a[i + j] -= v * b[j];
    }
    a.shrink();
    return {std::move(q), std::move(a)};
  }

  struct Mat
  {
    P a{1}, b{}, c{}, d{1};

    pair<P, P> apply(const P &f, const P &g) const
    {
      int len = max({a.sz(), b.sz(), c.sz(), d.sz()}), other = max(f.sz(), g.sz());
      if (min(len, other) > 60 && ntt_ok<mint>(len + other - 1))
      {
        int z = bit_ceil(len + other - 1);
        array<P, 4> t{a, b, c, d};
        P u = f, v = g;
        for (auto &p : t) p.resize(z), ntt(p);
        u.resize(z), v.resize(z), ntt(u), ntt(v);
        for (int i = 0; i < z; ++i)
        {
          mint x = t[0][i] * u[i] + t[1][i] * v[i];
          mint y = t[2][i] * u[i] + t[3][i] * v[i];
          u[i] = x, v[i] = y;
        }
        intt(u), intt(v);
        mint iz = mint(z).inv();
        u *= iz, v *= iz;
        u.resize(len + other - 1), v.resize(len + other - 1);
        u.shrink(), v.shrink();
        return {std::move(u), std::move(v)};
      }
      P u = a * f + b * g, v = c * f + d * g;
      u.shrink(), v.shrink();
      return {std::move(u), std::move(v)};
    }

    Mat operator*(const Mat &r) const
    {
      int len = max({a.sz(), b.sz(), c.sz(), d.sz()});
      int other = max({r.a.sz(), r.b.sz(), r.c.sz(), r.d.sz()});
      if (min(len, other) > 60 && ntt_ok<mint>(len + other - 1))
      {
        int z = bit_ceil(len + other - 1);
        array<P, 4> x{a, b, c, d}, y{r.a, r.b, r.c, r.d};
        for (auto &p : x) p.resize(z), ntt(p);
        for (auto &p : y) p.resize(z), ntt(p);
        for (int i = 0; i < z; ++i)
        {
          mint aa = x[0][i] * y[0][i] + x[1][i] * y[2][i];
          mint bb = x[0][i] * y[1][i] + x[1][i] * y[3][i];
          mint cc = x[2][i] * y[0][i] + x[3][i] * y[2][i];
          mint dd = x[2][i] * y[1][i] + x[3][i] * y[3][i];
          x[0][i] = aa, x[1][i] = bb, x[2][i] = cc, x[3][i] = dd;
        }
        mint iz = mint(z).inv();
        for (auto &p : x)
        {
          intt(p), p *= iz, p.resize(len + other - 1), p.shrink();
        }
        return {std::move(x[0]), std::move(x[1]), std::move(x[2]), std::move(x[3])};
      }
      Mat t{a * r.a + b * r.c, a * r.b + b * r.d,
            c * r.a + d * r.c, c * r.b + d * r.d};
      t.a.shrink(), t.b.shrink(), t.c.shrink(), t.d.shrink();
      return t;
    }

    // [[0,1],[1,-q]] を左から掛ける。
    void step(const P &q)
    {
      P u = a - q * c, v = b - q * d;
      a = std::move(c), b = std::move(d);
      c = std::move(u), d = std::move(v);
      c.shrink(), d.shrink();
    }
  };

  // deg(a) > deg(b)。M(a,b)=(u,v) において
  // deg(u) >= ceil(deg(a)/2) > deg(v) となる互除法の接頭辞を返す。
  static Mat half(P a, P b)
  {
    assert(a.sz() > b.sz());
    const int m = a.sz() / 2;
    Mat r;
    if (b.sz() <= m) return r;
    if (a.sz() <= cutoff)
    {
      while (b.sz() > m)
      {
        auto [q, rem] = divmod(std::move(a), b);
        a = std::move(b), b = std::move(rem);
        r.step(q);
      }
      return r;
    }

    r = half(a >> m, b >> m);
    tie(a, b) = r.apply(a, b);
    if (b.sz() <= m) return r;
    auto [q, rem] = divmod(std::move(a), b);
    a = std::move(b), b = std::move(rem);
    r.step(q);
    if (b.sz() <= m) return r;
    const int shift = 2 * m - (a.sz() - 1);
    return half(a >> shift, b >> shift) * r;
  }

  // M(a,b)=(g,0)。g の定数倍はここでは正規化しない。
  static Mat full(P a, P b)
  {
    if (b.empty()) return {};
    Mat r;
    if (a.sz() > b.sz())
    {
      r = half(a, b);
      tie(a, b) = r.apply(a, b);
      if (b.empty()) return r;
    }
    auto [q, rem] = divmod(std::move(a), b);
    r.step(q);
    return full(std::move(b), std::move(rem)) * r;
  }
};
} // namespace internal

// (monic gcd, s, t)、s*a+t*b=gcd。gcd(0,0)=0。
template <class mint>
tuple<FormalPowerSeries<mint>, FormalPowerSeries<mint>, FormalPowerSeries<mint>>
polynomial_extgcd(FormalPowerSeries<mint> a, FormalPowerSeries<mint> b)
{
  using H = internal::PolynomialHalfGCD<mint>;
  using P = FormalPowerSeries<mint>;
  a.shrink(), b.shrink();
  if (a.empty() && b.empty()) return {P{}, P{}, P{}};
  auto m = H::full(a, b);
  P g = m.a * a + m.b * b;
  g.shrink();
  mint iv = g.back().inv();
  return {g * iv, m.a * iv, m.b * iv};
}

template <class mint>
FormalPowerSeries<mint> polynomial_gcd(FormalPowerSeries<mint> a, FormalPowerSeries<mint> b)
{
  return get<0>(polynomial_extgcd(std::move(a), std::move(b)));
}

// f*g == 1 (mod mod_poly)、deg(g) < deg(mod_poly)。存在しなければ false。
// mod_poly は非零。非零定数を法とするときは (true,{}) を返す。
// f(0)==0 でも互いに素ならよい。
template <class mint>
pair<bool, FormalPowerSeries<mint>> polynomial_inv_mod(
    FormalPowerSeries<mint> f, FormalPowerSeries<mint> mod_poly)
{
  mod_poly.shrink();
  assert(!mod_poly.empty());
  if (mod_poly.sz() == 1) return {true, {}};
  f.shrink();
  f = internal::PolynomialHalfGCD<mint>::divmod(std::move(f), mod_poly).second;
  auto [g, s, t] = polynomial_extgcd(std::move(f), mod_poly);
  if (g.sz() != 1) return {false, {}};
  s = internal::PolynomialHalfGCD<mint>::divmod(std::move(s), mod_poly).second;
  return {true, std::move(s)};
}
