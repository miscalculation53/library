#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"

/**
 * @brief 有理式の総和
 * @docs docs/math/fps/rational_sum.md
 */

template <class mint>
pair<FormalPowerSeries<mint>, FormalPowerSeries<mint>> rational_plus
(
  const pair<FormalPowerSeries<mint>, FormalPowerSeries<mint>> &f,
  const pair<FormalPowerSeries<mint>, FormalPowerSeries<mint>> &g
)
{
  using F = FormalPowerSeries<mint>;
  cauto &[p_, q_] = f;
  cauto &[r_, s_] = g;
  const int k = p_.size(), l = q_.size(), m = r_.size(), n = s_.size();
  const int pz = bit_ceil(k + n - 1), rz = bit_ceil(l + m - 1);
  const int z = bit_ceil(max({k + n - 1, l + m - 1, l + n - 1}));
  // (ps + qr) / qs
  if (!ntt_ok<mint>(z) || min({p_.cnt_nz(), q_.cnt_nz(), r_.cnt_nz(), s_.cnt_nz()}) <= 60)
    return {p_ * s_ + q_ * r_, q_ * s_};
  // NTT を使い回す
  F p = p_, q = q_, r = r_, s = s_;
  p.resize(pz), q.resize(z), r.resize(rz), s.resize(z);
  ntt(p), ntt(q), ntt(r), ntt(s);
  repi(i, pz) p[i] *= s[i];
  repi(i, rz) r[i] *= q[i];
  repi(i, z) q[i] *= s[i];
  intt(p), intt(r), intt(q);
  mint ipz = mint(pz).inv(), irz = mint(rz).inv(), iz = mint(z).inv();
  repi(i, pz) p[i] *= ipz;
  repi(i, rz) r[i] *= irz;
  repi(i, z) q[i] *= iz;
  return {p.pre(k + n - 1) + r.pre(l + m - 1), q.pre(l + n - 1)};
}

template <class mint>
pair<FormalPowerSeries<mint>, FormalPowerSeries<mint>> rational_sum
(const vc<pair<FormalPowerSeries<mint>, FormalPowerSeries<mint>>> &fs, int d = -1)
{
  using F = FormalPowerSeries<mint>;
  using R = pair<F, F>;
  auto dc = [&](auto dc, int l, int r) -> R
  {
    if (r - l == 0)
      return {{1}, {1}};
    if (r - l == 1)
      return fs[l];
    const int m = (l + r) / 2;
    R res = rational_plus(dc(dc, l, m), dc(dc, m, r));
    if (d < 0)
      return res;
    else
      return {res.first.pre(d), res.second.pre(d)};
  };
  return dc(dc, 0, fs.size());
}
