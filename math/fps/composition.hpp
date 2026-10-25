#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../bit/bit_reverse.hpp"
#include "fps.hpp"
#include "../convolution/middle_product.hpp"

/**
 * @brief FPS 合成
 * @docs docs/math/fps/composition.md
 */

// f(g(x)) mod x^n。g の定数項は 0。
// power_projection(N-1, N, g, w.rev()) の w に関する転置。
template <class mint>
FormalPowerSeries<mint> composition
(
  const FormalPowerSeries<mint> &f,
  const FormalPowerSeries<mint> &g,
  int n
)
{
  using F = FormalPowerSeries<mint>;
  assert(n >= 0);
  assert(g.get(0) == 0);
  if (n == 0 || f.empty())
    return F(n);
  if (n == 1)
    return {f[0]};

  const int N = bit_ceil(n);
  F ipw;
  mint iz = 1;
  if constexpr (is_static_modint_v<mint>)
  {
    if (ntt_ok<mint>(4 * N))
    {
      static const internal::fft_info<mint> info;
      iz = mint(4 * N).inv();
      const mint iroot = info.iroot[bit_width(2 * N)];
      ipw.resize(2 * N);
      mint pw = 1;
      repi(i, 2 * N) ipw[bitrev(2 * N, i)] = pw, pw *= iroot;
    }
  }
  const bool use_ntt = !ipw.empty();

  // power_projection と同じ配置：x 方向に l 項、y 方向に k 項。
  // y 方向を反転した分母の最高次 y^k は、配列の外で扱う。
  auto rec = [&](auto &&self, F q, int l, int k) -> F
  {
    // g(0)=0 より終端の分母は 1。p.rev() の転置も反転。
    if (l == 1)
      return f.resized(n).resized(N).rev();

    F q2(use_ntt ? 4 * N : 2 * N), r, qq;
    repi(j, k) repi(i, l) q2[j * (2 * l) + i] = q[j * l + i];
    if (use_ntt)
    {
      ntt(q2);
      qq.resize(2 * N);
      repi(i, 2 * N) qq[i] = 2 * q2[2 * i] * q2[2 * i + 1];
      intt(qq);
      fem(a : qq) a *= iz;
    }
    else
    {
      r = q2;
      repi(i, 1, 2 * N, 2) r[i] = -r[i];
      qq = q2 * r;
    }
    F next_q(N);
    repi(j, 2 * k) repi(i, l / 2)
      next_q[j * (l / 2) + i] = qq[j * (use_ntt ? l : 2 * l) + (use_ntt ? i : 2 * i)];
    repi(j, k) repi(i, l / 2)
      next_q[(j + k) * (l / 2) + i] += 2 * q[j * l + 2 * i];

    // 戻り道では固定した q2 / r だけを使う。
    F().swap(q), F().swap(qq);
    if (!use_ntt) F().swap(q2);
    F p = self(self, move(next_q), l / 2, 2 * k), res(N);
    // 省略した y^k による加算の転置。
    repi(j, k) repi(i, l / 2)
      res[j * l + 2 * i + 1] = p[(j + k) * (l / 2) + i];

    if (use_ntt)
    {
      F p2(2 * N), p3(4 * N);
      // 係数の取り出しの転置は、同じ位置への 0 埋め。
      repi(j, 2 * k) repi(i, l / 2) p2[j * l + i] = p[j * (l / 2) + i];
      // intt^T = ntt o reverse_except_zero（intt は正規化前）。
      reverse(p2.begin() + 1, p2.end());
      ntt(p2);
      repi(i, 2 * N)
      {
        const mint a = p2[i] * ipw[i] * iz;
        p3[2 * i] = a * q2[2 * i + 1];
        p3[2 * i + 1] = -a * q2[2 * i];
      }
      // ntt^T = reverse_except_zero o intt。
      intt(p3);
      reverse(p3.begin() + 1, p3.end());
      repi(j, k) repi(i, l) res[j * l + i] += p3[j * (2 * l) + i];
    }
    else
    {
      F p2(4 * N);
      repi(j, 2 * k) repi(i, l / 2)
        p2[j * (2 * l) + 2 * i + 1] = p[j * (l / 2) + i];
      // 畳み込みの転置：a_i = sum_j r_j p2_{i+j}。
      p2 = middle_product(p2, r);
      repi(j, k) repi(i, l) res[j * l + i] += p2[j * (2 * l) + i];
    }
    return res;
  };
  return rec(rec, -g.resized(N), N, 1).rev().resized(n);
}
