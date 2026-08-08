#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"
#include "../convolution/convolution_many.hpp"

/**
 * @brief $\sum_{i} A_i^k$ の $k$ に関する列挙
 * @docs docs/math/fps/pow_sum.md
 */

// sum as[i]^k を k = 0, 1, ..., m-1 について列挙
template <class mint>
FormalPowerSeries<mint> pow_sum(const vc<mint> &as, int m)
{
  using F = FormalPowerSeries<mint>;
  assert(m >= 0);
  if (m == 0)
    return {};
  const int n = as.size();
  vc<F> fs(n);
  repi(i, n) fs[i] = {1, -as[i]};
  F f = convolution_many(fs);
  f = -(f.log(m).diff());
  f.insert(f.begin(), n);
  return f;
}
