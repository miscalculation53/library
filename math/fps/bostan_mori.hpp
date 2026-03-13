#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../bit/bit_reverse.hpp"
#include "fps.hpp"

/**
 * @brief Bostan–Mori
 * @docs docs/math/fps/bostan_mori.md
 */

// [x^k] p(x)/q(x) を求める
// 制約: [x^0] q(x) != 0
// 計算量: q(x) の次数を d として O(d log d log k) 時間
template <class mint>
mint bostan_mori(const FormalPowerSeries<mint> &p, const FormalPowerSeries<mint> &q, ll k)
{
  using F = FormalPowerSeries<mint>;
  static const internal::fft_info<mint> info;
  auto [r, u] = p.divmod(q);
  mint res = r.get(k);
  const int d = SZ(q) - 1;
  if (ntt_ok<mint>(2 * d + 1))
  {
    const int z = bit_ceil(2 * d + 1);
    const mint i2 = mint(2).inv();
    const mint iz = mint(z / 2).inv();
    const mint root = info.root[bit_width(z / 2)];
    const mint iroot = info.iroot[bit_width(z / 2)];
    vc<mint> ipw(z / 2);
    {
      mint itmp = 1;
      repi(i, z / 2)
      {
        ipw[bitrev(z / 2, i)] = itmp;
        itmp *= iroot;
      }
    }
    F v = q;
    u.resize(z / 2), v.resize(z / 2);
    F u2 = u, v2 = v;
    ntt(u), ntt(v);
    while (k > 0)
    {
      {
        mint tmp = 1;
        repi(i, z / 2)
        {
          u2[i] = u2[i] * tmp;
          v2[i] = v2[i] * tmp;
          tmp *= root;
        }
      }
      ntt(u2), ntt(v2);
      if (k & 1)
      {
        repi(i, z / 4)
        {
          const mint x = v[2 * i], y = v[2 * i + 1];
          u[i] = (u[2 * i] * y - u[2 * i + 1] * x) * ipw[i] * i2;
          v[i] = x * y;
        }
        repi(i, z / 4)
        {
          const mint x = v2[2 * i], y = v2[2 * i + 1];
          u[i + z / 4] = (u2[2 * i] * y - u2[2 * i + 1] * x) * ipw[i + z / 4] * i2;
          v[i + z / 4] = x * y;
        }
      }
      else
      {
        repi(i, z / 4)
        {
          const mint x = v[2 * i], y = v[2 * i + 1];
          u[i] = (u[2 * i] * y + u[2 * i + 1] * x) * i2;
          v[i] = x * y;
        }
        repi(i, z / 4)
        {
          const mint x = v2[2 * i], y = v2[2 * i + 1];
          u[i + z / 4] = (u2[2 * i] * y + u2[2 * i + 1] * x) * i2;
          v[i + z / 4] = x * y;
        }
      }
      u2 = u, v2 = v;
      intt(u2), intt(v2);
      repi(i, z / 2) u2[i] *= iz, v2[i] *= iz;
      k >>= 1;
    }
    return res + u2[0] / v2[0];
  }
  else
  {
    F v = q;
    u.resize(d + 1), v.resize(d + 1);
    while (k > 0)
    {
      F w = v;
      repi(i, 1, d + 1, 2) w[i] = -w[i];
      F u2 = u * w, v2 = v * w;
      repi(i, d + 1)
      {
        if (2 * i + (k & 1) < SZ(u2))
          u[i] = u2[2 * i + (k & 1)];
        if (2 * i < SZ(v2))
          v[i] = v2[2 * i];
      }
      k >>= 1;
    }
    return res + u[0] / v[0];
  }
}
