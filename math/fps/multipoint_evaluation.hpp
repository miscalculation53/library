#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"

/**
 * @brief 多項式の多点評価
 * @docs docs/math/fps/multipoint_evaluation.md
 */

template <class mint>
vc<mint> multipoint_evaluation(const FormalPowerSeries<mint> &f, const vc<mint> &xs)
{
  using F = FormalPowerSeries<mint>;
  const int m0 = xs.size(), m = bit_ceil(m0), h = min(6, (int)bit_width(m0) - 1);
  vc<F> node(2 * m, {1});
  repi(i, m0) node[m + i] = {-xs[i], 1};
  repi(i, m - 1, 0, -1) node[i] = convolution(node[2 * i], node[2 * i + 1]);
  node[1] = f % node[1];
  repi(i, 2, m >> (h - 1)) node[i] = node[i / 2] % node[i];
  vc<mint> res(m0);
  repi(i, m0) res[i] = node[(m + i) >> h].eval(xs[i]);
  return res;
}
