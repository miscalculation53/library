#pragma once

#include "../fps/fps.hpp"

/**
 * @brief 線形演算の転置（コード生成用）
 * @docs docs/math/linalg/linear_transpose.md
 */

namespace linear_transpose
{

// 入出力の長さと <F(x), y> = <x, F^T(y)> を検査する。
template <class T, class Forward, class Transpose>
bool check(int n, int m, const Forward &forward, const Transpose &transpose, int trials = 8)
{
  assert(n >= 0 && m >= 0 && trials >= 0);
  mt19937 rng(712367);
  repi(t, trials)
  {
    vc<T> x(n), y(m);
    fem(a : x) a = T(int(rng() % 7) - 3);
    fem(a : y) a = T(int(rng() % 7) - 3);
    const auto fx = forward(x), ty = transpose(y);
    if ((int)fx.size() != m || (int)ty.size() != n) return false;
    T lhs = 0, rhs = 0;
    repi(i, m) lhs += fx[i] * y[i];
    repi(i, n) rhs += x[i] * ty[i];
    if (lhs != rhs) return false;
  }
  return true;
}

template <class T>
void add_to(vc<T> &a, const vc<T> &b)
{
  assert(a.size() == b.size());
  repi(i, a.size()) a[i] += b[i];
}

template <class T>
vc<T> slice(const vc<T> &a, int l, int r)
{
  assert(0 <= l && l <= r && r <= (int)a.size());
  return vc<T>(a.begin() + l, a.begin() + r);
}

template <class T>
vc<T> resized(vc<T> a, int n)
{
  assert(n >= 0);
  a.resize(n);
  return a;
}

template <class T>
vc<T> reversed(vc<T> a)
{
  reverse(a.begin(), a.end());
  return a;
}

template <class T>
vc<T> ntt_forward(vc<T> a)
{
  if (!a.empty()) ntt(a);
  return a;
}

template <class T>
vc<T> ntt_transpose(vc<T> a)
{
  if (!a.empty())
  {
    intt(a);
    reverse(a.begin() + 1, a.end());
  }
  return a;
}

template <class T>
vc<T> intt_forward(vc<T> a)
{
  if (!a.empty()) intt(a);
  return a;
}

template <class T>
vc<T> intt_transpose(vc<T> a)
{
  if (!a.empty())
  {
    reverse(a.begin() + 1, a.end());
    ntt(a);
  }
  return a;
}

// a -> convolution(a, b) の転置。w の保存範囲外は 0。
template <class T>
vc<T> convolution_transpose(const vc<T> &w, const vc<T> &b, int n)
{
  assert(n >= 0);
  const int m = b.size();
  if (n == 0 || m == 0 || w.empty()) return vc<T>(n);
  const int len = n + m - 1, z = bit_ceil(len);
  vc<T> a(w.begin(), w.begin() + min(len, (int)w.size())), r(b.rbegin(), b.rend());
  if (min(a.size(), b.size()) > 60 && ntt_ok<T>(z))
  {
    // 巡回畳み込みの [m-1, m-1+n) には折り返しが入らない。
    a.resize(z), r.resize(z);
    ntt(a), ntt(r);
    repi(i, z) a[i] *= r[i];
    intt(a);
    const T iz = T(z).inv();
    vc<T> res(n);
    repi(i, n) res[i] = a[m - 1 + i] * iz;
    return res;
  }
  a = convolution(a, r);
  vc<T> res(n);
  repi(i, n) if (m - 1 + i < (int)a.size()) res[i] = a[m - 1 + i];
  return res;
}

// monic な g による剰余。結果は deg(g) 項にそろえる。
template <class T>
vc<T> polynomial_mod(const vc<T> &a, const vc<T> &g)
{
  assert(g.size() >= 2 && g.back() == T(1));
  using F = FormalPowerSeries<T>;
  return (F(a) % F(g)).resized(g.size() - 1);
}

template <class T>
vc<T> polynomial_mod_transpose(const vc<T> &w, const vc<T> &g, int n)
{
  assert(n >= 0);
  assert(g.size() >= 2 && g.back() == T(1));
  const int d = g.size() - 1;
  assert((int)w.size() == d);
  vc<T> res(n);
  repi(i, min(n, d)) res[i] = w[i];
  if (n <= d) return res;
  using F = FormalPowerSeries<T>;
  const int k = n - d;
  // r = a - q*g, q = rev(trunc_k(rev(a)) / rev(g))。
  auto t = convolution_transpose(w, g, k);
  reverse(t.begin(), t.end());
  F inv = F(g).rev().inv(k);
  t = convolution_transpose(t, inv, k);
  repi(i, k) res[n - 1 - i] -= t[i];
  return res;
}

} // namespace linear_transpose
