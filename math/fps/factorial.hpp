#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "shift_of_sampling_points.hpp"

/**
 * @brief 高速階乗・等差数列の積
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
    assert(k_ >= 0);
    // 評価点シフトで使う階乗の添字を法未満に保つ。
    int max_k = 0;
    while ((2LL << max_k) <= mint::mod() / 2) ++max_k;
    k = min(k_, max_k);
    y = {1};
    repi(i, k)
    {
      z = shift_of_sampling_points_many(y, 1 << i, (1 << (i + 2)) - (1 << i));
      z.insert(z.begin(), ALL(y));
      y.resize(1 << (i + 1));
      repi(j, 1 << (i + 1)) y[j] = z[2 * j] * z[2 * j + 1] * mint((1LL << i) * (2 * j + 1));
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
  mint factorial(ll n) const
  {
    assert(n >= 0);
    if (n >= mint::mod())
      return 0;
    mint res = fac[n >> k];
    repi(j, (n >> k << k) + 1, n + 1) res *= mint::raw(j);
    return res;
  }

  // prod_{i=0}^{n-1} (a*i+b)
  mint arithmetic_product(ll n, mint a, mint b) const
  {
    assert(n >= 0);
    if (n == 0) return 1;
    if (a == 0) return b.pow(n);
    auto direct = [&]()
    {
      mint res = 1, x = b;
      for (ll i = 0; i < n; ++i, x += a) res *= x;
      return res;
    };
    if (n <= 32) return direct();
    const ll p = mint::mod(), c = (b / a).val();
    if (c == 0 || n > p - c) return 0;
    const ll l = c - 1, r = c + n - 1, block = 1LL << k;
    const ll last = (p - 1) >> k;
    // R と p-1-L を B の倍数にする。既存の階乗表だけで R!/L! を得る。
    const ll L = p - 1 - (min(last, (p - c + block / 2) >> k) << k);
    const ll R = min(last, (r + block / 2) >> k) << k;
    if (n <= abs(L - l) + abs(R - r)) return direct();
    mint res = fac[R >> k] * fac[(p - 1 - L) >> k], den = 1;
    if (L % 2 == 0) res = -res;
    // 内側の区切りなら掛け足し、外側なら余分な因子を分母にまとめる。
    mint tail = 1;
    for (ll j = min(L, l) + 1; j <= max(L, l); ++j) tail *= mint::raw(j);
    if (L < l) den *= tail;
    else res *= tail;
    tail = 1;
    for (ll j = min(R, r) + 1; j <= max(R, r); ++j) tail *= mint::raw(j);
    if (R > r) den *= tail;
    else res *= tail;
    if (L < l || R > r) res /= den;
    return a.pow(n) * res;
  }
};
