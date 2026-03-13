#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"
#include "power_projection.hpp"

/**
 * @brief 逆関数
 * @docs docs/math/fps/compositional_inverse.md
 */

// 長さ n まで
template <class mint>
FormalPowerSeries<mint> compositional_inv
(
  const FormalPowerSeries<mint> &f,
  int n = -1
)
{
  using F = FormalPowerSeries<mint>;
  assert(f.get(0) == 0 && f.get(1) != 0);
  if (n < 0)
    n = f.size();
  if (n == 0)
    return {};
  if (n == 1)
    return {0};
  const mint c = f[1], ic = c.inv();
  F h = power_projection(n - 1, n, f * ic, F{1});
  dump(h);
  repi(i, 1, n) h[i] *= (n - 1) * Binomial<mint>::inv(i);
  reverse(ALL(h)), h.pop_back();
  dump(h);
  assert(h.get(0) == 1);
  F g = h.pow(-mint(n - 1).inv().val());
  g.insert(g.begin(), 0);
  mint pw = 1;
  repi(i, 1, n) pw = pw * ic, g[i] *= pw;
  return g;
}
