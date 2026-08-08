#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../modint/modint.hpp"
#include "../crt.hpp"

/**
 * @brief 畳み込み
 * @docs docs/math/convolution/convolution.md
 */

template <class T>
T convolution_point_get(const vc<T> &a, const vc<T> &b, int p)
{
  const int n = a.size(), m = b.size();
  T res = 0;
  repi(i, max(0, p - m + 1), min(n, p + 1)) res += a[i] * b[p - i];
  return res;
}

namespace internal
{

constexpr int primitive_root_constexpr(int m)
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
  int divs[20] = {};
  divs[0] = 2;
  int cnt = 1;
  int x = (m - 1) / 2;
  while (x % 2 == 0)
    x /= 2;
  for (int i = 3; (long long)(i)*i <= x; i += 2)
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
  for (int g = 2;; g++)
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
template <int m>
constexpr int primitive_root_for_convolution = primitive_root_constexpr(m);

template <class mint, int g = internal::primitive_root_for_convolution<mint::mod()>>
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
bool ntt_ok(int n)
{
  if (n <= 0)
    return false;
  if constexpr (is_static_modint_v<mint>)
  {
    if constexpr (!internal::isprime<mint::mod()>)
      return false;
    static constexpr int rank2 = countr_zero(mint::mod() - 1);
    return n <= (1 << rank2);
  }
  else
    return false;
}

template <int id>
void ntt(vc<dynamic_modint32<id>> &) { assert(false); }
template <int id>
void intt(vc<dynamic_modint32<id>> &) { assert(false); }

// 破壊的に変更する
template <auto mod>
void ntt(vc<internal::modint_impl<internal::policy_static<mod>>> &a)
{
  using mint = internal::modint_impl<internal::policy_static<mod>>;
  int n = int(a.size());
  assert(n > 0);
  int h = countr_zero((unsigned int)n);
  assert(n == (1 << h));
  assert(ntt_ok<mint>(n));

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
          auto mod2 = 1ULL * mint::mod() * mint::mod();
          auto a0 = 1ULL * a[i + offset].val();
          auto a1 = 1ULL * a[i + offset + p].val() * rot.val();
          auto a2 = 1ULL * a[i + offset + 2 * p].val() * rot2.val();
          auto a3 = 1ULL * a[i + offset + 3 * p].val() * rot3.val();
          auto a1na3imag =
              1ULL * mint(a1 + mod2 - a3).val() * imag.val();
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
template <auto mod>
void intt(vc<internal::modint_impl<internal::policy_static<mod>>> &a)
{
  using mint = internal::modint_impl<internal::policy_static<mod>>;
  int n = int(a.size());
  assert(n > 0);
  int h = countr_zero((unsigned int)n);
  assert(n == (1 << h));
  assert(ntt_ok<mint>(n));

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
              ((unsigned long long)mint::mod() + l.val() - (uint)r.val()) *
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
          auto a0 = 1ULL * a[i + offset + 0 * p].val();
          auto a1 = 1ULL * a[i + offset + 1 * p].val();
          auto a2 = 1ULL * a[i + offset + 2 * p].val();
          auto a3 = 1ULL * a[i + offset + 3 * p].val();

          auto a2na3iimag =
              1ULL *
              mint((mint::mod() + a2 - a3) * iimag.val()).val();

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

namespace internal
{

template <class mint>
vc<mint> convolution_naive(const vc<mint> &a, const vc<mint> &b)
{
  const int n = a.size(), m = b.size();
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  vc<mint> c(n + m - 1);
  if ((ll)m * cnta > (ll)n * cntb)
  {
    repi(j, m)
    {
      if (b[j] == 0)
        continue;
      repi(i, n) c[i + j] += a[i] * b[j];
    }
  }
  else
  {
    repi(i, n)
    {
      if (a[i] == 0)
        continue;
      repi(j, m) c[i + j] += a[i] * b[j];
    }
  }
  return c;
}

// n == 0 or m == 0 はバグるので注意
template <class mint>
vc<mint> convolution_ntt(vc<mint> a, vc<mint> b)
{
  const int n = a.size(), m = b.size();
  const int z = bit_ceil(n + m - 1);
  if (a == b)
  {
    a.resize(z);
    ntt(a);
    repi(i, z) a[i] *= a[i];
  }
  else
  {
    a.resize(z), b.resize(z);
    ntt(a), ntt(b);
    repi(i, z) a[i] *= b[i];
  }
  intt(a);
  mint iz = mint(z).inv();
  fem(ai : a) ai *= iz;
  a.resize(n + m - 1);
  return a;
}

template <size_t j, int mod, class T, size_t k>
void convolution_crt_helper(const vc<T> &a, const vc<T> &b, vc<array<T, k>> &cs)
{
  using mint = static_modint32<mod>;
  const int n = a.size(), m = b.size();
  auto c = convolution_ntt(vc<mint>(ALL(a)), vc<mint>(ALL(b)));
  repi(i, n + m - 1) cs[i][j] = c[i].val();
}

// n == 0 or m == 0 はバグるので注意
template <int ...ms, class T>
vc<T> convolution_crt(const vc<T> &a, const vc<T> &b)
{
  const int n = a.size(), m = b.size();

  constexpr size_t k = sizeof...(ms);
  vc<array<T, k>> cs(n + m - 1);
  constexpr array<int, k> ms_arr = {ms...};
  [&]<size_t... Is>(index_sequence<Is...>)
  {
    (convolution_crt_helper<Is, ms_arr[Is], T, k>(a, b, cs), ...);
  }(make_index_sequence<k>{});

  vc<T> c(n + m - 1);
  repi(i, n + m - 1) c[i] = get<1>(crt(cs[i], ms_arr));
  return c;
}

// n == 0 or m == 0 はバグるので注意
template <class mint, int... ms, class T>
vc<mint> convolution_crt_mod(const vc<T> &a, const vc<T> &b)
{
  const int n = a.size(), m = b.size();

  constexpr size_t k = sizeof...(ms);
  vc<array<T, k>> cs(n + m - 1);
  constexpr array<int, k> ms_arr = {ms...};
  [&]<size_t... Is>(index_sequence<Is...>)
  {
    (convolution_crt_helper<Is, ms_arr[Is], T, k>(a, b, cs), ...);
  }(make_index_sequence<k>{});

  vc<mint> c(n + m - 1);
  repi(i, n + m - 1) c[i] = crt_mod_constexpr<mint>(cs[i], ms_arr).first;
  return c;
}

}  // namespace internal

// ntt-friendly なら普通、そうでなければ 3 つの mod
// mod は 10^9 程度を想定、列の長さは合計 2^26 程度
template <class mint, typename = std::enable_if_t<!std::is_integral<mint>::value>>
vc<mint> convolution(const vc<mint> &a, const vc<mint> &b)
{
  const int n = a.size(), m = b.size();
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  if (n == 0 || m == 0)
    return {};
  if (ntt_ok<mint>(n + m - 1))
  {
    if (min(cnta, cntb) <= 60)
      return internal::convolution_naive(a, b);
    return internal::convolution_ntt(a, b);
  }
  else
  {
    if (min(cnta, cntb) <= 300)
      return internal::convolution_naive(a, b);
    assert(ntt_ok<static_modint32<469762049>>(n + m - 1) && "|a| + |b| - 1 <= 2^26");
    vc<ll> a_(n), b_(m);
    repi(i, n) a_[i] = a[i].val();
    repi(j, m) b_[j] = b[j].val();
    return internal::convolution_crt_mod<mint, 469762049, 1811939329, 2013265921>(a_, b_);
  }
}

template <int mod = 998244353, class T, typename = enable_if_t<is_integral<T>::value>>
vc<T> convolution(const vc<T> &a, const vc<T> &b)
{
  using mint = static_modint32<mod>;
  auto c = convolution(vc<mint>(ALL(a)), vc<mint>(ALL(b)));
  vc<T> c_(c.size());
  repi(i, c.size()) c_[i] = c[i].val();
  return c_;
}
