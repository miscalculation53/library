#pragma once

#include "../template/template_all_but_modint.hpp"

#include "extgcd.hpp"
#include "../utils/larger_int.hpp"
#include "modint/modint_internal_barrett32.hpp"

/**
 * @brief 中国剰余定理 (CRT)
 * @docs docs/math/crt.md
 */

// (解が存在するか, r, m)
template <class T = ll, class R0, class R1, class M0, class M1>
constexpr tuple<bool, T, T> crt2(R0 r0_, R1 r1_, M0 m0_, M1 m1_)
{
  T m0 = m0_, m1 = m1_;
  assert(m0 >= 1 && m1 >= 1);
  T r0 = safemod(r0_, m0), r1 = safemod(r1_, m1);
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
template <class mint, class V1, class V2>
pair<mint, mint> crt_mod(const V1 &rs, const V2 &ms)
{
  using T = decay_t<decltype(ms[0])>;
  assert(rs.size() == ms.size());
  const int n = rs.size();
  mint r = 0, m = 1;

  // 小さい法は 4 本以上で計算幅を切り替え、16 本以上で Barrett を使う。
  bool small = sizeof(T) <= 4;
  if constexpr (sizeof(T) > 4)
    if (n >= 4)
    {
      small = true;
      for (auto mod : ms)
        if (mod > INT_MAX) { small = false; break; }
    }
  if (sizeof(T) <= 4 || (small && n >= 16))
  {
    vc<internal::barrett32> ba;
    ba.reserve(n);
    repi(i, n) ba.eb(ms[i]);
    vc<uint> rr(n, 0), mm(n, 1);
    repi(i, n)
    {
      assert(ms[i] >= 1);
      auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
      assert(g == 1);
      if (im < 0)
        im += ms[i];
      ll diff = rs[i] % ll(ms[i]);
      if (diff < 0) diff += ms[i];
      diff -= rr[i];
      if (diff < 0) diff += ms[i];
      uint t = ba[i].mul(diff, im);
      r += t * m, m *= ms[i];
      repi(j, i + 1, n)
      {
        rr[j] += ba[j].mul(t, mm[j]);
        if (rr[j] >= (uint)ms[j])
          rr[j] -= ms[j];
        mm[j] = ba[j].mul(mm[j], ms[i]);
      }
    }
  }
  else
  {
    auto solve = [&](auto word)
    {
      using W = decltype(word);
      vc<ull> rr(n, 0), mm(n, 1);
      repi(i, n)
      {
        assert(ms[i] >= 1);
        auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
        assert(g == 1);
        if (im < 0)
          im += ms[i];
        ll diff = rs[i] % ll(ms[i]);
        if (diff < 0) diff += ms[i];
        diff -= ll(rr[i]);
        if (diff < 0) diff += ms[i];
        ull t = (ull)(W(diff) * im % ms[i]);
        r += t * m, m *= ms[i];
        repi(j, i + 1, n)
        {
          rr[j] += (ull)(W(t) * mm[j] % ms[j]);
          if (rr[j] >= (ull)ms[j]) rr[j] -= ms[j];
          mm[j] = (ull)(W(mm[j]) * ms[i] % ms[j]);
        }
      }
    };
    if (small) solve(ull(0));
    else solve(u128(0));
  }
  return {r, m};
}

// (r, m)
// 引数は array
// ms[i] たちがコンパイル時定数であることを仮定
// ms[i] たちは pairwise coprime
// 0 <= rs[i] < ms[i]
template <class mint, class U1, class U2, size_t n>
constexpr pair<mint, mint> crt_mod_constexpr(const array<U1, n> &rs, const array<U2, n> &ms)
{
  if constexpr (sizeof(U2) > 4)
  {
    bool small = true;
    for (auto mod : ms)
      if (mod > INT_MAX) { small = false; break; }
    if (small)
    {
      array<int, n> residues{}, moduli{};
      repi(i, n) residues[i] = rs[i], moduli[i] = ms[i];
      return crt_mod_constexpr<mint>(residues, moduli);
    }
  }
  using T = larger_int_t<U2>;
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
    T t = safemod<T>((rs[i] - rr[i]) * im, ms[i]);
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
