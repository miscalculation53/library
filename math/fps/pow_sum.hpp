#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"
#include "rational_sum.hpp"

/**
 * @brief $\sum_{i} A_i^k$ の $k$ に関する列挙
 * @docs docs/math/fps/pow_sum.md
 */

// sum as[i]^k を k = 0, 1, ..., m-1 について列挙
template <class mint>
FormalPowerSeries<mint> pow_sum(const vc<mint> &as, int m)
{
  using F = FormalPowerSeries<mint>;
  const int n = as.size();
  vc<pair<F, F>> fs(n);
  rep(i, n) fs[i] = {{1}, {1, -as[i]}};
  auto [p, q] = rational_sum(fs);
  return (p.pre(m) * q.inv(m)).pre(m);
}
