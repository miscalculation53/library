#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "convolution.hpp"

/**
 * @brief 畳み込み（long long）
 * @docs docs/math/convolution/convolution_ll.md
 */

// 最終的な要素が 4.2 × 10^18 程度に収まる場合
// mod 2 つで計算
// 列の長さは合計 2^25 程度
vc<ll> convolution_4e18(const vc<ll> &a, const vc<ll> &b)
{
  const int n = a.size(), m = b.size();
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  if (min(cnta, cntb) <= 150)
    return internal::convolution_naive(a, b);
  return internal::convolution_crt<2013265921, 2113929217>(a, b);
}
