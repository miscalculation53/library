#pragma once

#include "half_gcd.hpp"

/**
 * @brief Half-GCD による最短線形漸化式の復元
 * @docs docs/math/fps/berlekamp_massey_fast.md
 */

// berlekamp_massey<F> と同じ係数順・符号 (c[0] == -1)。
// 解が一意でない場合、通常版と係数は異なり得るが最小次数は同じ。
// NTT が使えるとき O(n log^2 n) 時間、O(n) 空間。全項を一括で渡す。
template <class mint>
vc<mint> berlekamp_massey_fast(const vc<mint> &seq)
{
  using H = internal::PolynomialHalfGCD<mint>;
  using P = FormalPowerSeries<mint>;
  const int n = seq.size();
  P a(n + 1), b(seq.rbegin(), seq.rend());
  a[n] = 1;
  b.shrink();
  if (b.empty()) return {mint(-1)};
  auto m = H::half(a, b);
  tie(a, b) = m.apply(a, b);
  // b == m.c*x^n + m.d*reverse(seq)。deg(b) < deg(m.d) で停止。
  while (b.sz() >= m.d.sz())
  {
    auto [q, rem] = H::divmod(std::move(a), b);
    a = std::move(b), b = std::move(rem);
    m.step(q);
  }
  return (m.d * (-m.d.back().inv())).rev();
}
