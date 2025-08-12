#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"
#include "../convolution/convolution_many.hpp"
#include "multipoint_evaluation.hpp"
#include "rational_sum.hpp"
#include "../modint/inv_many.hpp"

/**
 * @brief 多項式補間
 * @docs docs/math/fps/interpolation.md
 */

template <class mint>
FormalPowerSeries<mint> interpolation(const vc<mint> &xs, const vc<mint> &ys)
{
  using F = FormalPowerSeries<mint>;
  assert(xs.size() == ys.size());
  const int n = xs.size();
  vc<F> fs(n);
  repi(i, n) fs[i] = {-xs[i], 1};
  F g = convolution_many(fs);
  vc<mint> a = multipoint_evaluation(g.diff(), xs);
  vc<mint> ia = inv_many(a);
  vc<pair<F, F>> rs(n);
  repi(i, n) rs[i] = {{ys[i] * ia[i]}, fs[i]};
  return rational_sum(rs, n).first;
}
