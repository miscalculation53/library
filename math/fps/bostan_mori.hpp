#pragma once

#include "../../template/template_all_but_modint.hpp"

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
  auto [r, u] = p.divmod(q);
  mint res = r.get(k);
  const int d = SZ(q) - 1;
  if (ntt_ok<mint>(2 * d + 1))
  {
    const int z = bit_ceil(2 * d + 1);
    mint iz = mint(z).inv();
    F v = q;
    u.resize(z), v.resize(z);
    while (k > 0)
    {
      ntt(u), ntt(v);
      repi(i, 0, z, 2)
      {
        mint x = v[i + 1], y = v[i];
        u[i] *= x, v[i] *= x;
        u[i + 1] *= y, v[i + 1] *= y;
      }
      intt(u), intt(v);
      repi(i, z / 2)
      {
        u[i] = u[2 * i + (k & 1)] * iz;
        v[i] = v[2 * i] * iz;
      }
      repi(i, z / 2, z) u[i] = 0, v[i] = 0;
      k >>= 1;
    }
    return res + u[0] / v[0];
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
