#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "convolution.hpp"

/**
 * @brief 畳み込み（$\bmod \ 2^{64}$）
 * @docs docs/math/convolution/convolution64.md
 */

// mod 2^64
// mod 5 つで計算
// 列の長さは合計 2^25 程度
vc<ull> convolution64(const vc<ull> &a, const vc<ull> &b)
{
  const int n = a.size(), m = b.size();
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  if (min(cnta, cntb) <= 400)
    return internal::convolution_naive(a, b);
  assert(ntt_ok<static_modint32<754974721>>(n + m - 1) && "|a| + |b| - 1 <= 2^25");
  return internal::convolution_crt_mod<ull, 167772161, 469762049, 1107296257, 1711276033, 1811939329>(a, b);
}
