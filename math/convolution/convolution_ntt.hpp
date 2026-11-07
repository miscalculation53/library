#pragma once

#include "ntt.hpp"
#include "../modint/internal_dot_product_mod32.hpp"

/**
 * @brief NTT 用の畳み込み
 * @docs docs/math/convolution/convolution.md
 */

template <class T>
T convolution_point_get(const vc<T> &a, const vc<T> &b, int p)
{
  const int n = a.size(), m = b.size();
  if constexpr (internal::dot_product_mod32_value<T>::value)
  {
    if (p < 0 || ll(p) >= ll(n) + m - 1 || n == 0 || m == 0) return T(0);
    const int l = max(0, p - m + 1), r = min(n, p + 1);
    return internal::dot_product_mod32_values<T>(r - l, a.begin() + l, b.rbegin() + (m - 1 - p + l));
  }
  T res = 0;
  repi(i, max(0, p - m + 1), min(n, p + 1)) res += a[i] * b[p - i];
  return res;
}

namespace internal
{

template <class mint>
vc<mint> convolution_naive(const vc<mint> &a, const vc<mint> &b)
{
  const int n = a.size(), m = b.size();
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  vc<mint> c(n + m - 1);
  if (ll(cnta) * 2 < n && ll(cntb) * 2 < m)
  {
    vc<pair<int, mint>> an, bn;
    repi(i, n) if (a[i] != 0) an.eb(i, a[i]);
    repi(j, m) if (b[j] != 0) bn.eb(j, b[j]);
    for (auto [i, x] : an) for (auto [j, y] : bn) c[i+j] += x*y;
    return c;
  }
  if constexpr (dot_product_mod32_value<mint>::value)
  {
    if (min(n, m) >= 16 && ll(cnta) * 2 >= n && ll(cntb) * 2 >= m)
    {
      repi(p, n + m - 1) c[p] = convolution_point_get(a, b, p);
      return c;
    }
  }
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

// 非零係数を含む最小の半開区間。
template <class mint>
pair<int, int> convolution_support(const vc<mint> &a)
{
  int l = 0, r = a.size();
  while (l < r && a[l] == 0) ++l;
  while (l < r && a[r - 1] == 0) --r;
  return {l, r};
}

inline int convolution_ntt_length(int n, int m)
{
  const int size = n + m - 1, z = bit_ceil(size), half = z / 2;
  return size - half <= 32 && max(n, m) <= half ? half : z;
}

template <class mint>
bool convolution_sparse_preferred(const vc<mint> &a, const vc<mint> &b, int ca, int cb, int moduli)
{
  const bool sparse_a = ll(ca) * 2 < int(a.size()), sparse_b = ll(cb) * 2 < int(b.size());
  if (!sparse_a && !sparse_b) return false;
  const ll work = sparse_a && sparse_b ? ll(ca) * cb : min(ll(a.size()) * cb, ll(b.size()) * ca);
  const auto [al, ar] = convolution_support(a);
  const auto [bl, br] = convolution_support(b);
  const int z = convolution_ntt_length(ar - al, br - bl);
  const ll budget = ll(moduli) * z * countr_zero(unsigned(z)) / 4;
  return work <= budget;
}

template <class mint>
vc<mint> convolution_ntt(vc<mint> a, vc<mint> b)
{
  if (a.empty() || b.empty()) return {};
  const int full_size = int(a.size()) + int(b.size()) - 1;
  const auto [al, ar] = convolution_support(a);
  const auto [bl, br] = convolution_support(b);
  if (al == ar || bl == br) return vc<mint>(full_size);

  int z = convolution_ntt_length(a.size(), b.size()), shift = 0;
  const int cropped = convolution_ntt_length(ar - al, br - bl);
  if (cropped < z)
  {
    shift = al + bl;
    a.resize(ar), b.resize(br);
    if (al) a.erase(a.begin(), a.begin() + al);
    if (bl) b.erase(b.begin(), b.begin() + bl);
    z = cropped;
  }
  const int size = int(a.size()) + int(b.size()) - 1;
  vc<mint> tail(max(0, size - z));
  repi(i, tail.size()) tail[i] = convolution_point_get(a, b, z + i);

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
  a.resize(min(z, size));
  const mint iz = mint(z).inv();
  for (auto &x : a) x *= iz;
  a.resize(size);
  repi(i, tail.size())
  {
    a[i] -= tail[i];
    a[z + i] = tail[i];
  }
  if (shift) a.insert(a.begin(), shift, mint(0));
  a.resize(full_size);
  return a;
}

}

// ntt_ok<mint>(a.size()+b.size()-1) が成り立つ係数列の畳み込みを返す。
template <class mint>
vc<mint> ntt_convolution(const vc<mint> &a, const vc<mint> &b)
{
  const int n = a.size(), m = b.size();
  if (n == 0 || m == 0) return {};
  assert(ntt_ok<mint>(n + m - 1));
  const int ca = n - count(ALL(a), 0), cb = m - count(ALL(b), 0);
  if (ca == 0 || cb == 0) return vc<mint>(n + m - 1);
  if (min(ca, cb) <= 60 || internal::convolution_sparse_preferred(a, b, ca, cb, 1))
    return internal::convolution_naive(a, b);
  return internal::convolution_ntt(a, b);
}
