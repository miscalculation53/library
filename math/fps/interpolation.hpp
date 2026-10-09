#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/algebra_basic_ops.hpp"

#include "fps.hpp"
#include "multipoint_evaluation.hpp"
#include "../modint/inv_many.hpp"

/**
 * @brief 多項式補間
 * @docs docs/math/fps/interpolation.md
 */

template <class mint> FormalPowerSeries<mint> interpolation(const vc<mint> &xs, const vc<mint> &ys)
{
  using F = FormalPowerSeries<mint>;
  assert(xs.size() == ys.size());
  int n = xs.size();
  if (!n)
    return {};
  internal::FPSMultipointTree<mint> tree(xs);
  F monic = tree.product[1].resized(n + 1).rev();
  auto weights = tree.evaluate(monic.diff());
  auto inverses = inv_many<FieldAddSubMulDiv<mint>>(weights);
  vc<F> value(2 * tree.base);
  repi(i, tree.base) value[tree.base + i] = {i < n ? ys[i] * inverses[i] : mint(0)};
  for (int i = tree.base - 1; i > 0; --i)
  {
    int k = tree.product[2 * i].sz() - 1, z = 2 * k;
    if (!tree.spectrum[2 * i].empty())
    {
      F a = value[2 * i].resized(z), b = value[2 * i + 1].resized(z);
      ntt(a), ntt(b);
      repi(j, z) a[j] = a[j] * tree.spectrum[2 * i + 1][j] + b[j] * tree.spectrum[2 * i][j];
      intt(a);
      mint iz = mint(z).inv();
      for (auto &x : a)
        x *= iz;
      value[i] = std::move(a);
    }
    else
      value[i] = value[2 * i] * tree.product[2 * i + 1] + value[2 * i + 1] * tree.product[2 * i];
    F().swap(value[2 * i]);
    F().swap(value[2 * i + 1]);
  }
  return value[1].resized(n).rev();
}
