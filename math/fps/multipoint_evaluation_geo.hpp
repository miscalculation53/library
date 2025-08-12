#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"

/**
 * @brief 多項式の多点評価（評価点が等比数列）
 * @docs docs/math/fps/multipoint_evaluation_geo.md
 */

// i = 0, ..., m - 1 に対する f(ar^i)
template <class mint>
vc<mint> multipoint_evaluation_geo(const FormalPowerSeries<mint> &f, int m, const mint &a, const mint &r)
{
  using F = FormalPowerSeries<mint>;
  assert(m >= 0);
  if (m == 0)
    return {};
  if (r == 0)
  {
    vc<mint> res(m, f.eval(0));
    res[0] = f.eval(a);
    return res;
  }
  const int n = f.size();
  auto calc_pw = [&](int k, const mint &x) -> F
  {
    F res(k, 1);
    repi(i, 1, k) res[i] = res[i - 1] * x;
    return res;
  };
  auto calc_tri = [&](int k, const F &pw) -> F
  {
    F res(k, 1);
    repi(i, k - 1) res[i + 1] = res[i] * pw[i];
    return res;
  };
  F pwa = calc_pw(n, a), pwr = calc_pw(n + m, r), pwir = calc_pw(max(n, m), r.inv());
  F trir = calc_tri(n + m, pwr), triir = calc_tri(max(n, m), pwir);
  F s(n);
  repi(i, n) s[n - 1 - i] = f[i] * pwa[i] * triir[i];
  F u = (s * trir).pre(n + m - 1) >> (n - 1);
  repi(i, m) u[i] *= triir[i];
  return u;
}
