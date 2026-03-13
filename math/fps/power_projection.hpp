#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../bit/bit_reverse.hpp"
#include "fps.hpp"

/**
 * @brief Power Projection
 * @docs docs/math/fps/power_projection.md
 */

// [x^n] g(x)f(x)^i (i=0,...,m-1)
template <class mint>
FormalPowerSeries<mint> power_projection
(
  int n,
  int m,
  const FormalPowerSeries<mint> &f,
  const FormalPowerSeries<mint> &g
)
{
  using F = FormalPowerSeries<mint>;
  static const internal::fft_info<mint> info;
  if (!has_single_bit(n + 1))
  {
    const int n2 = bit_ceil(n + 1);
    auto g2 = g;
    g2.resize(n + 1);
    reverse(ALL(g2)), g2.resize(n2), reverse(ALL(g2));
    return power_projection(n2 - 1, m, f, g2);
  }
  n++;
  int k = 1;
  if (ntt_ok<mint>(4 * n))
  {
    const mint iz = mint(4 * n).inv();
    const mint iroot = info.iroot[bit_width(2 * n)];
    vc<mint> ipw(2 * n);
    {
      mint tmp = 1;
      repi(i, 2 * n) ipw[bitrev(2 * n, i)] = tmp, tmp *= iroot;
    }
    F p = g, q = -f, p2, q2;
    p.resize(n), q.resize(n);
    while (n > 1)
    {
      // x について長さ n で、y について長さ k
      p2.assign(4 * n * k, 0), q2.assign(4 * n * k, 0);
      repi(j, k) repi(i, n)
      {
        p2[j * (2 * n) + i] = p[j * n + i];
        q2[j * (2 * n) + i] = q[j * n + i];
      }
      ntt(p2), ntt(q2);
      repi(i, 2 * n * k)
      {
        const mint x = q2[2 * i], y = q2[2 * i + 1];
        p2[i] = (p2[2 * i] * y - p2[2 * i + 1] * x) * ipw[i];
        q2[i] = 2 * x * y;
      }
      p2.resize(2 * n * k), q2.resize(2 * n * k);
      intt(p2), intt(q2);
      repi(i, 2 * n * k) p2[i] *= iz, q2[i] *= iz;
      repi(j, k) repi(i, n / 2)
      {
        p2[(j + k) * n + i] += p[j * n + (2 * i + 1)];
        q2[(j + k) * n + i] += 2 * q[j * n + (2 * i)];
      }
      n >>= 1, k <<= 1;
      repi(j, k) repi(i, n)
      {
        p[j * n + i] = p2[j * (2 * n) + i];
        q[j * n + i] = q2[j * (2 * n) + i];
      }
    }
    return (p.rev().pre(m) * (F{1} + (q.rev() << 1)).inv(m)).pre(m);
  }
  else
  {
    F p = g, q = -f, p2(2 * n), q2(2 * n), r(2 * n);
    p.resize(n), q.resize(n);
    while (n > 1)
    {
      // x について長さ n で、y について長さ k
      p2.resize(2 * n * k), q2.resize(2 * n * k);
      fill(ALL(p2), 0), fill(ALL(q2), 0), fill(ALL(r), 0);
      repi(j, k) repi(i, n)
      {
        p2[j * (2 * n) + i] = p[j * n + i];
        q2[j * (2 * n) + i] = q[j * n + i];
        r[j * (2 * n) + i] = i % 2 == 0 ? q[j * n + i] : -q[j * n + i];
      }
      p2 *= r, q2 *= r;
      repi(j, k) repi(i, n)
      {
        p2[(j + k) * (2 * n) + i] += p[j * n + i];
        if (i % 2 == 0)
          q2[(j + k) * (2 * n) + i] += 2 * q[j * n + i];
      }
      n >>= 1, k <<= 1;
      repi(j, k) repi(i, n)
      {
        p[j * n + i] = p2[j * (4 * n) + (2 * i + 1)];
        q[j * n + i] = q2[j * (4 * n) + (2 * i)];
      }
    }
    return (p.rev().pre(m) * (F{1} + (q.rev() << 1)).inv(m)).pre(m);
  }
}
