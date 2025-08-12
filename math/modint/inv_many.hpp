#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 逆元をまとめて求める
 * @docs docs/math/modint/inv_many.md
 */

template <class mint>
vc<mint> inv_many(const vc<mint> &a)
{
  const int n = a.size();
  vc<mint> p(n + 1, 1);
  repi(i, n) p[i + 1] = p[i] * a[i];
  mint ip = p.back().inv();
  vc<mint> res(n);
  repi(i, n - 1, -1, -1)
  {
    res[i] = ip * p[i];
    ip *= a[i];
  }
  return res;
}
