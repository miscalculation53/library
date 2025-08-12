#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"
#include "multipoint_evaluation_geo.hpp"
#include "../modint/inv_many.hpp"

/**
 * @brief 多項式補間（評価点が等比数列）
 * @docs docs/math/fps/interpolation_geo.md
 */

// i = 0, 1, ..., n-1 に対し f(ar^i) = y[i] であるとき f (n-1 次) を求める
// 制約: 1 <= i < n に対して r^i != 1
// https://noshi91.github.io/algorithm-encyclopedia/polynomial-interpolation-geometric#noredirect
template <class mint>
FormalPowerSeries<mint> interpolation_geo(const mint &a, const mint &r, const vc<mint> &ys)
{
  using F = FormalPowerSeries<mint>;
  const int n = ys.size();
  if (n == 0)
    return {};
  if (n == 1)
    return {ys[0]};
  assert(a != 0 && r != 0);
  vc<mint> pwr(n + 1, 1);
  repi(i, 1, n + 1)
  {
    pwr[i] = pwr[i - 1] * r;
    if (i < n)
      assert(pwr[i] != 1);
  }
  vc<mint> s(n + 1, 1), t(n + 1, 1), u(n, 1);
  repi(i, 1, n + 1) s[i] = s[i - 1] * (1 - pwr[i]);
  repi(i, n - 1) t[i + 1] = t[i] * pwr[n - i - 2];
  repi(i, n) u[i] = ipow(-1, i) * s[i] * s[n - 1 - i] * t[i];
  vc<mint> iu = inv_many(u);
  F w(n);
  repi(i, n) w[i] = ys[i] * iu[i];
  F prod(n + 1);
  t[0] = 1;
  repi(i, n) t[i + 1] = t[i] * pwr[i];
  if (s[n] == 0)
  {
    prod[0] = 1;
    prod[n] = ipow(-1, n) * t[n];
  }
  else
  {
    vc<mint> is = inv_many(s);
    repi(i, n + 1) prod[i] = ipow(-1, i) * t[i] * s[n] * is[i] * is[n - i];
  }
  F sum = multipoint_evaluation_geo(w, n, mint(1), r);
  F ans = (prod * sum).pre(n).rev();
  mint ia = a.inv();
  vc<mint> pwia(n, 1);
  repi(i, 1, n) pwia[i] = pwia[i - 1] * ia;
  repi(i, n) ans[i] *= pwia[i];
  return ans;
}
