#pragma once

/*!
 * Portions corresponding to AtCoder Library (CC0-1.0): primitive-root search and NTT butterflies.
 * Upstream: atcoder/convolution.hpp and atcoder/internal_math.hpp
 * https://github.com/atcoder/ac-library/tree/864245a00b00dd008d1abfdc239618fdb7d139da
 * License: https://github.com/atcoder/ac-library/blob/864245a00b00dd008d1abfdc239618fdb7d139da/LICENSE
 */

#include "../modint/modint.hpp"

namespace internal
{

template <class T>
constexpr T primitive_root_constexpr(T m)
{
  if (m == 2)
    return 1;
  if (m == 167772161)
    return 3;
  if (m == 469762049)
    return 3;
  if (m == 754974721)
    return 11;
  if (m == 998244353)
    return 3;
  if (m == 1107296257)
    return 10;
  if (m == 1711276033)
    return 29;
  if (m == 1811939329)
    return 13;
  if (m == 2013265921)
    return 31;
  if (m == 2113929217)
    return 5;
  T divs[20] = {};
  divs[0] = 2;
  int cnt = 1;
  T x = (m - 1) / 2;
  while (x % 2 == 0)
    x /= 2;
  for (T i = 3; i <= x / i; i += 2)
  {
    if (x % i == 0)
    {
      divs[cnt++] = i;
      while (x % i == 0)
      {
        x /= i;
      }
    }
  }
  if (x > 1)
  {
    divs[cnt++] = x;
  }
  for (T g = 2;; g++)
  {
    bool ok = true;
    for (int i = 0; i < cnt; i++)
    {
      if (powmod_constexpr(g, (m - 1) / divs[i], m) == 1)
      {
        ok = false;
        break;
      }
    }
    if (ok)
      return g;
  }
}
template <auto m>
constexpr auto primitive_root_for_convolution = primitive_root_constexpr(m);

template <class T>
constexpr T ntt_generator_constexpr(T m)
{
  if (m <= INT_MAX) return primitive_root_constexpr(m);
  if ((m - 1) % 4 == 2) return m - 1;
  for (T g = 2; ; ++g)
    if (powmod_constexpr(g, (m - 1) / 2, m) == m - 1) return g;
}
template <auto m>
constexpr auto ntt_generator_for_convolution = ntt_generator_constexpr(m);

template <class mint, auto g = internal::ntt_generator_for_convolution<mint::mod()>>
struct fft_info
{
  static constexpr int rank2 = countr_zero(mint::mod() - 1);
  std::array<mint, std::max(3, rank2 + 1)> root;  // root[i]^(2^i) == 1 (i <= rank2)
  std::array<mint, std::max(3, rank2 + 1)> iroot; // root[i] * iroot[i] == 1 (i <= rank2)

  std::array<mint, std::max(1, rank2 - 2 + 1)> rate2;
  std::array<mint, std::max(1, rank2 - 2 + 1)> irate2;

  std::array<mint, std::max(1, rank2 - 3 + 1)> rate3;
  std::array<mint, std::max(1, rank2 - 3 + 1)> irate3;

  fft_info()
  {
    root[rank2] = mint(g).pow((mint::mod() - 1) >> rank2);
    iroot[rank2] = root[rank2].inv();
    for (int i = rank2 - 1; i >= 0; i--)
    {
      root[i] = root[i + 1] * root[i + 1];
      iroot[i] = iroot[i + 1] * iroot[i + 1];
    }

    {
      mint prod = 1, iprod = 1;
      for (int i = 0; i <= rank2 - 2; i++)
      {
        rate2[i] = root[i + 2] * prod;
        irate2[i] = iroot[i + 2] * iprod;
        prod *= iroot[i + 2];
        iprod *= root[i + 2];
      }
    }
    {
      mint prod = 1, iprod = 1;
      for (int i = 0; i <= rank2 - 3; i++)
      {
        rate3[i] = root[i + 3] * prod;
        irate3[i] = iroot[i + 3] * iprod;
        prod *= iroot[i + 3];
        iprod *= root[i + 3];
      }
    }
  }
};

}  // namespace internal

// mint で長さ n の列の ntt ができるか判定
template <class mint>
constexpr bool ntt_ok(int n)
{
  if (n <= 0)
    return false;
  if constexpr (is_static_modint_v<mint>)
  {
    if constexpr (!internal::isprime<mint::mod()>)
      return false;
    constexpr int rank2 = countr_zero(mint::mod() - 1);
    return ull(n) <= (1ULL << rank2);
  }
  else
    return false;
}

template <int id>
void ntt(vc<dynamic_modint32<id>> &) { assert(false); }
template <int id>
void intt(vc<dynamic_modint32<id>> &) { assert(false); }

// 破壊的に変更する
template <auto mod, class PowInv>
void ntt(vc<internal::modint_impl<internal::policy_static<mod>, PowInv>> &a)
{
  using mint = internal::modint_impl<internal::policy_static<mod>, PowInv>;
  int n = int(a.size());
  assert(n > 0);
  int h = countr_zero((unsigned int)n);
  assert(n == (1 << h));
  assert(ntt_ok<mint>(n));

  using wide = conditional_t<(mint::mod() <= INT_MAX), ull, u128>;
  static const internal::fft_info<mint> info;

  int len = 0; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
  while (len < h)
  {
    if (h - len == 1)
    {
      int p = 1 << (h - len - 1);
      mint rot = 1;
      for (int s = 0; s < (1 << len); s++)
      {
        int offset = s << (h - len);
        for (int i = 0; i < p; i++)
        {
          auto l = a[i + offset];
          auto r = a[i + offset + p] * rot;
          a[i + offset] = l + r;
          a[i + offset + p] = l - r;
        }
        if (s + 1 != (1 << len))
          rot *= info.rate2[countr_zero(~(unsigned int)(s))];
      }
      len++;
    }
    else
    {
      // 4-base
      int p = 1 << (h - len - 2);
      mint rot = 1, imag = info.root[2];
      for (int s = 0; s < (1 << len); s++)
      {
        mint rot2 = rot * rot;
        mint rot3 = rot2 * rot;
        int offset = s << (h - len);
        for (int i = 0; i < p; i++)
        {
          auto mod2 = wide(mint::mod()) * mint::mod();
          auto a0 = wide(a[i + offset].val());
          auto a1 = wide(a[i + offset + p].val()) * rot.val();
          auto a2 = wide(a[i + offset + 2 * p].val()) * rot2.val();
          auto a3 = wide(a[i + offset + 3 * p].val()) * rot3.val();
          auto a1na3imag =
              wide(mint(a1 + mod2 - a3).val()) * imag.val();
          auto na2 = mod2 - a2;
          a[i + offset] = a0 + a2 + a1 + a3;
          a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
          a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
          a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
        }
        if (s + 1 != (1 << len))
          rot *= info.rate3[countr_zero(~(unsigned int)(s))];
      }
      len += 2;
    }
  }
}

// 破壊的に変更する
template <auto mod, class PowInv>
void intt(vc<internal::modint_impl<internal::policy_static<mod>, PowInv>> &a)
{
  using mint = internal::modint_impl<internal::policy_static<mod>, PowInv>;
  int n = int(a.size());
  assert(n > 0);
  int h = countr_zero((unsigned int)n);
  assert(n == (1 << h));
  assert(ntt_ok<mint>(n));

  using wide = conditional_t<(mint::mod() <= INT_MAX), ull, u128>;
  static const internal::fft_info<mint> info;

  int len = h; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
  while (len)
  {
    if (len == 1)
    {
      int p = 1 << (h - len);
      mint irot = 1;
      for (int s = 0; s < (1 << (len - 1)); s++)
      {
        int offset = s << (h - len + 1);
        for (int i = 0; i < p; i++)
        {
          auto l = a[i + offset];
          auto r = a[i + offset + p];
          a[i + offset] = l + r;
          a[i + offset + p] =
              (wide(mint::mod()) + l.val() - wide(r.val())) *
              irot.val();
          ;
        }
        if (s + 1 != (1 << (len - 1)))
          irot *= info.irate2[countr_zero(~(unsigned int)(s))];
      }
      len--;
    }
    else
    {
      // 4-base
      int p = 1 << (h - len);
      mint irot = 1, iimag = info.iroot[2];
      for (int s = 0; s < (1 << (len - 2)); s++)
      {
        mint irot2 = irot * irot;
        mint irot3 = irot2 * irot;
        int offset = s << (h - len + 2);
        for (int i = 0; i < p; i++)
        {
          auto a0 = wide(a[i + offset + 0 * p].val());
          auto a1 = wide(a[i + offset + 1 * p].val());
          auto a2 = wide(a[i + offset + 2 * p].val());
          auto a3 = wide(a[i + offset + 3 * p].val());

          auto a2na3iimag =
              wide(mint((wide(mint::mod()) + a2 - a3) * iimag.val()).val());

          a[i + offset] = a0 + a1 + a2 + a3;
          a[i + offset + 1 * p] =
              (a0 + (mint::mod() - a1) + a2na3iimag) * irot.val();
          a[i + offset + 2 * p] =
              (a0 + a1 + (mint::mod() - a2) + (mint::mod() - a3)) *
              irot2.val();
          a[i + offset + 3 * p] =
              (a0 + (mint::mod() - a1) + (mint::mod() - a2na3iimag)) *
              irot3.val();
        }
        if (s + 1 != (1 << (len - 2)))
          irot *= info.irate3[countr_zero(~(unsigned int)(s))];
      }
      len -= 2;
    }
  }
}

