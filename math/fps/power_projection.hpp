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
  F p = g, q = -f, p2, q2, r;
  p.resize(n), q.resize(n);
  // x について長さ n で、y について長さ k
  auto pad = [&]<bool reflect>()
  {
    repi(j, k) repi(i, n)
    {
      const int from = j * n + i, to = from + j * n;
      p2[to] = p[from], q2[to] = q[from];
      if constexpr (reflect)
        r[to] = i % 2 == 0 ? q[from] : -q[from];
    }
  };
  auto halve = [&]<int stride>()
  {
    n >>= 1, k <<= 1;
    repi(j, k) repi(i, n)
    {
      const int to = j * n + i, from = stride * (to + j * n);
      p[to] = p2[from + stride - 1], q[to] = q2[from];
    }
  };
  if (ntt_ok<mint>(4 * n))
  {
    const mint iz = mint(4 * n).inv();
    const mint iroot = info.iroot[bit_width(2 * n)];
    vc<mint> ipw(2 * n);
    mint pw = 1;
    repi(i, 2 * n) ipw[bitrev(2 * n, i)] = pw, pw *= iroot;
    while (n > 1)
    {
      const int length = 2 * n * k;
      p2.assign(2 * length, 0), q2.assign(2 * length, 0);
      pad.template operator()<false>();
      ntt(p2), ntt(q2);
      repi(i, length)
      {
        const mint x = q2[2 * i], y = q2[2 * i + 1];
        p2[i] = (p2[2 * i] * y - p2[2 * i + 1] * x) * ipw[i];
        q2[i] = 2 * x * y;
      }
      p2.resize(length), q2.resize(length);
      intt(p2), intt(q2);
      repi(i, length) p2[i] *= iz, q2[i] *= iz;
      repi(j, k) repi(i, n / 2)
      {
        p2[(j + k) * n + i] += p[j * n + (2 * i + 1)];
        q2[(j + k) * n + i] += 2 * q[j * n + (2 * i)];
      }
      halve.template operator()<1>();
    }
  }
  else
  {
    r.resize(2 * n);
    while (n > 1)
    {
      p2.resize(2 * n * k), q2.resize(2 * n * k);
      fill(ALL(p2), 0), fill(ALL(q2), 0), fill(ALL(r), 0);
      pad.template operator()<true>();
      p2 *= r, q2 *= r;
      repi(j, k) repi(i, n)
      {
        p2[(j + k) * (2 * n) + i] += p[j * n + i];
        if (i % 2 == 0)
          q2[(j + k) * (2 * n) + i] += 2 * q[j * n + i];
      }
      halve.template operator()<2>();
    }
  }
  return (p.rev().resized(m) * (F{1} + (q.rev() << 1)).inv(m)).resized(m);
}
