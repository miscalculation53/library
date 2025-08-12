#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"

/**
 * @brief Taylor Shift
 * @docs docs/math/fps/taylor_shift.md
 */

// f(x + c) を返す
template <class mint>
FormalPowerSeries<mint> taylor_shift(const FormalPowerSeries<mint> &f, const mint &c)
{
  const int n = f.size();
  FormalPowerSeries<mint> g(n);
  g[0] = 1;
  repi(i, 1, f.sz()) g[i] = c * g[i - 1];
  return (f.ogf().rev() * g.egf()).pre(n).rev().egf();
}
