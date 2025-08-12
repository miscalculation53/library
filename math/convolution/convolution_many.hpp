#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "convolution.hpp"

/**
 * @brief 多項式の総積
 * @docs docs/math/convolution/convolution_many.md
 */

// F は vc<mint> または fps
// d は返ってくる vector の最大長さ
// 計算量: 次数の総和を n として O(n log^2 n)
template <class F>
F convolution_many(const vc<F> &fs, int d = -1)
{
  auto dc = [&](auto dc, int l, int r) -> F
  {
    if (r - l == 0)
      return {1};
    if (r - l == 1)
      return fs[l];
    const int m = (l + r) / 2;
    F f = convolution(dc(dc, l, m), dc(dc, m, r));
    if (d < 0 || d >= (int)f.size())
      return f;
    return F(f.begin(), f.begin() + d);
  };
  return dc(dc, 0, fs.size());
}
