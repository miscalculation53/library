#pragma once
#include "convolution.hpp"

/**
 * @brief 中間積（畳み込みの転置）
 * @docs docs/math/convolution/middle_product.md
 */

namespace internal
{
template <int prime, class mint>
vc<static_modint32<prime>> middle_product_prime(const vc<mint> &a, const vc<mint> &b, int z)
{
  using M = static_modint32<prime>;
  vc<M> x(z), y(z);
  repi(i, int(a.size())) x[i] = a[i].val();
  repi(i, int(b.size())) y[i] = b[int(b.size()) - 1 - i].val();
  ntt(x), ntt(y);
  repi(i, z) x[i] *= y[i];
  intt(x);
  int out = int(a.size()) - int(b.size()) + 1;
  vc<M> result(out);
  M iz = M(z).inv();
  repi(i, out) result[i] = x[int(b.size()) - 1 + i] * iz;
  return result;
}

} // namespace internal

// y[i] = sum_j a[i+j]*b[j]. Cyclic wrap lands strictly below b.size()-1.
template <class mint> vc<mint> middle_product(const vc<mint> &a, const vc<mint> &b)
{
  using F = vc<mint>;
  assert(!b.empty() && a.size() >= b.size());
  int out = a.size() - b.size() + 1;
  int cutoff = ntt_ok<mint>(int(a.size())) ? 32 : 128;
  if (min(out, int(b.size())) <= cutoff)
  {
    F result(out);
    repi(i, out) result[i] = dot_product<RingAddSubMul<mint>>(int(b.size()), a.begin() + i, b.begin());
    return result;
  }
  vc<pair<int, mint>> nz;
  repi(j, int(b.size())) if (b[j] != 0)
  {
    nz.emplace_back(j, b[j]);
    if (int(nz.size()) > cutoff)
      break;
  }
  if (int(nz.size()) <= cutoff)
  {
    F result(out);
    for (auto [j, v] : nz)
      repi(i, out) result[i] += a[i + j] * v;
    return result;
  }
  if (ntt_ok<mint>(int(a.size())))
  {
    int z = bit_ceil(int(a.size()));
    F x = a, y(b.rbegin(), b.rend());
    x.resize(z), y.resize(z);
    ntt(x), ntt(y);
    repi(i, z) x[i] *= y[i];
    intt(x);
    mint iz = mint(z).inv();
    F result(out);
    repi(i, out) result[i] = x[int(b.size()) - 1 + i] * iz;
    return result;
  }
  if constexpr (internal::ordinary_mod32<mint>::value)
  {
    int z = bit_ceil(int(a.size()));
    auto x = internal::middle_product_prime<469762049>(a, b, z);
    auto y = internal::middle_product_prime<1811939329>(a, b, z);
    auto w = internal::middle_product_prime<2013265921>(a, b, z);
    constexpr array<int, 3> primes{469762049, 1811939329, 2013265921};
    F result(out);
    repi(i, out) result[i] =
        crt_mod_constexpr<mint>(array<ll, 3>{x[i].val(), y[i].val(), w[i].val()}, primes).first;
    return result;
  }
  F product = convolution(a, F(b.rbegin(), b.rend()));
  return F(product.begin() + int(b.size()) - 1, product.begin() + int(a.size()));
}
