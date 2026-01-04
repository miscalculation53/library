#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "shift_of_sampling_points.hpp"

/**
 * @brief 階乗
 * @docs docs/math/fps/factorial.md
 */

// https://suisen-kyopro.hatenablog.com/entry/2023/11/22/201600
// パラメータ B = 2^k をとって、
// 前計算 O(B log B + (p log p) / B)、クエリ O(B)
// パラメータの値 (デフォルト値はクエリ数 10^5 を想定):
// - NTT friendly のときデフォルトで k = 9 (B = 512)
// - NTT friendly でないときデフォルトで k = 11 (B = 2048)
// - クエリ数が非常に少ないなら k = 13 (B = 8192) とする
template <class mint>
struct Factorial
{
private:
  int k;
  vc<mint> y, z, fac;

public:
  Factorial(int k_ = -1)
  {
    if (k_ == -1)
      k_ = ntt_ok<mint>(1 << 22) ? 9 : 11;
    k = k_;
    y = {1};
    repi(i, k)
    {
      z = shift_of_sampling_points_many(y, 1 << i, (1 << (i + 2)) - (1 << i));
      z.insert(z.begin(), ALL(y));
      y.resize(1 << (i + 1));
      repi(j, 1 << (i + 1)) y[j] = z[2 * j] * z[2 * j + 1] * mint::raw((1 << i) * (2 * j + 1));
    }
    if ((1 << k) <= mint::mod() >> k)
    {
      z = shift_of_sampling_points_many(y, 1 << k, mint::mod() >> k);
      y.insert(y.end(), ALL(z));
    }
    fac.resize((mint::mod() >> k) + 1);
    fac[0] = 1;
    repi(i, mint::mod() >> k) fac[i + 1] = fac[i] * y[i] * mint::raw((1 + i) << k);
  }
  mint factorial(ll n)
  {
    if (n >= mint::mod())
      return 0;
    mint res = fac[n >> k];
    repi(j, (n >> k << k) + 1, n + 1) res *= mint::raw(j);
    return res;
  }
};
