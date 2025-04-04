#pragma once

#include "../template/template_all_but_modint.hpp"

#include "extgcd.hpp"
#include "modint/modint32_internal.hpp"

/**
 * @brief 中国剰余定理 (CRT)
 * @docs docs/math/crt.md
 */

// (解が存在するか, r, m)
template <class T = ll, class R0, class R1, class M0, class M1>
constexpr tuple<bool, T, T> crt2(R0 r0, R1 r1, M0 m0, M1 m1)
{
  assert(m0 >= 1 && m1 >= 1);
  r0 = safemod(r0, m0), r1 = safemod(r1, m1);
  if (m0 < m1)
    swap(r0, r1), swap(m0, m1);
  if (m0 % m1 == 0)
  {
    if (r0 % m1 != r1)
      return {false, 0, 0};
    return {true, r0, m0};
  }
  auto [g, im, _] = extgcd<T>(m0, m1);
  T u1 = m1 / g;
  if ((r1 - r0) % g)
    return {false, 0, 0};
  T x = (r1 - r0) / g % u1 * im % u1;
  r0 += x * m0;
  m0 *= u1;
  if (r0 < 0)
    r0 += m0;
  return {true, r0, m0};
}

// (解が存在するか, r, m)
template <class T = ll, class V1, class V2>
constexpr tuple<bool, T, T> crt(const V1 &rs, const V2 &ms)
{
  assert(rs.size() == ms.size());
  const int n = rs.size();
  T r = 0, m = 1;
  repi(i, n)
  {
    auto [ok, nr, nm] = crt2<T>(r, rs[i], m, ms[i]);
    if (!ok)
      return {false, 0, 0};
    r = nr, m = nm;
  }
  return {true, r, m};
}

// (r, m)
// ms[i] たちは pairwise coprime
// T は ms[i] の型の 2 乗が収まる (符号つき)
template <class mint, class T = ll, class V1, class V2>
pair<mint, mint> crt_mod(const V1 &rs, const V2 &ms)
{
  assert(rs.size() == ms.size());
  const int n = rs.size();
  mint r = 0, m = 1;
  vc<T> rr(n, 0), mm(n, 1);
  repi(i, n)
  {
    // r = t[0] + t[1]m[0] + ... + t[i-1]m[0]...m[i-2] mod mint::mod
    // m = m[0]...m[i-1] mod mint::mod
    // rr[i] = t[0] + t[1]m[0] + ... + t[i-1]m[0]...m[i-2] mod m[i]
    // mm[i] = m[0]...m[i-1] mod m[i]
    assert(ms[i] >= 1);
    auto [g, im, _] = extgcd<T>(mm[i], ms[i]);
    assert(g == 1);
    T t = safemod((rs[i] % ms[i] - rr[i]) * im, ms[i]);
    r += t * m, m *= ms[i];
    repi(j, i + 1, n)
    {
      rr[j] += t * mm[j] % ms[j];
      if (rr[j] >= ms[j])
        rr[j] -= ms[j];
      mm[j] *= ms[i], mm[j] %= ms[j];
    }
  }
  return {r, m};
}

// (r, m)
// 引数は array
// ms[i] たちがコンパイル時定数であることを仮定
// ms[i] たちは pairwise coprime
// 0 <= rs[i] < ms[i]
// T は ms[i] の型の 2 乗が収まる (符号つき)
template <class mint, class T = ll, class U1, class U2, size_t n>
constexpr pair<mint, mint> crt_mod_constexpr(const array<U1, n> &rs, const array<U2, n> &ms)
{
  assert(rs.size() == ms.size());
  mint r = 0, m = 1;
  array<T, n> rr{}, mm;
  fill(ALL(mm), 1);
  repi(i, n)
  {
    assert(ms[i] >= U2(1));
    assert(U1(0) <= rs[i] && U2(rs[i]) < ms[i]);
    auto [g, im, _] = extgcd<T>(mm[i], ms[i]);
    assert(g == 1);
    T t = safemod((rs[i] - rr[i]) * im, ms[i]);
    r += t * m, m *= ms[i];
    repi(j, i + 1, n)
    {
      rr[j] += t * mm[j] % ms[j];
      if (rr[j] >= ms[j])
        rr[j] -= ms[j];
      mm[j] *= ms[i], mm[j] %= ms[j];
    }
  }
  return {r, m};
}

// (r, m)
// ms[i] たちは pairwise coprime
// ms[i] たちが dynamic 32 bit の場合に高速化したもの
template <class mint, class V1, class V2>
pair<mint, mint> crt_mod_dynamic_32(const V1 &rs, const V2 &ms)
{
  assert(rs.size() == ms.size());
  const int n = rs.size();
  mint r = 0, m = 1;
  vc<internal::barrett32> ba;
  ba.reserve(n);
  repi(i, n) ba.eb(ms[i]);
  vc<ll> rr(n, 0), mm(n, 1);
  repi(i, n)
  {
    assert(ms[i] >= 1);
    auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
    assert(g == 1);
    if (im < 0)
      im += ms[i];
    ll t = ba[i].mul(safemod(rs[i] - rr[i], ms[i]), im);
    r += t * m, m *= ms[i];
    repi(j, i + 1, n)
    {
      rr[j] += ba[j].mul(t, mm[j]);
      if (rr[j] >= ms[j])
        rr[j] -= ms[j];
      mm[j] = ba[j].mul(mm[j], ms[i]);
    }
  }
  return {r, m};
}

// 破壊的に変更する
// 解が存在しないなら false を返す
// 解が存在するなら true を返し、ms[i] たちが pairwise coprime であるような等価な方程式に変換する
template <class V1, class V2>
bool pre_crt(const V1 &rs, V2 &ms)
{
  using T = typename V2::value_type;
  assert(rs.size() == ms.size());
  const int n = rs.size();
  repi(i, n) repi(j, i + 1, n)
  {
    T g = gcd(ms[i], ms[j]);
    if ((rs[i] - rs[j]) % g)
      return false;
    ms[i] /= g, ms[j] /= g;
    T gi = gcd(ms[i], g), gj = g / gi;
    do
    {
      g = gcd(gi, gj);
      gi *= g, gj /= g;
    } while (g > 1);
    ms[i] *= gi, ms[j] *= gj;
  }
  return true;
}
