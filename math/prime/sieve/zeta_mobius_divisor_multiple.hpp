#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/prime/sieve/linear_sieve.hpp"
#include "algebra/algebra_basic_ops.hpp"

/**
 * @brief 約数・倍数 ゼータ・メビウス変換（$1$ 以上 $n$ 以下）
 * @docs docs/math/prime/sieve/zeta_mobius_divisor_multiple.md
 */

// ζa(n) = Σ[d | n] a(d)
// M は可換モノイド (Σ だと +)
// 1-indexed (a[0] は使わない)
template <class M>
vc<typename M::S> zeta_divisor(const vc<typename M::S> &a)
{
  const int n = SZ<int>(a) - 1;
  LinearSieve::reserve(n);
  auto b = a;
  fec(p : LinearSieve::primes)
  {
    if (p > n)
      break;
    for (int i = 1; i * p <= n; i++)
      b[i * p] = M::op(b[i * p], b[i]);
  }
  return b;
}

// μ は ζ の逆変換
// μa(n) = Σ{d | n} μ(n/d)a(d)  cf. メビウスの反転公式
// G は可換群 (Σ だと +)
// 1-indexed (a[0] は使わない)
template <class G>
vc<typename G::S> mobius_divisor(const vc<typename G::S> &a)
{
  const int n = SZ<int>(a) - 1;
  LinearSieve::reserve(n);
  auto b = a;
  fec(p : LinearSieve::primes)
  {
    if (p > n)
      break;
    for (int i = n / p; i >= 1; i--)
      b[i * p] = G::op(b[i * p], G::inv(b[i]));
  }
  return b;
}

// ζ'a(n) = Σ{n | m} a(m)
// M は可換モノイド (Σ だと +)
// 1-indexed (a[0] は使わない)
template <class M>
vc<typename M::S> zeta_multiple(const vc<typename M::S> &a)
{
  const int n = SZ<int>(a) - 1;
  LinearSieve::reserve(n);
  auto b = a;
  fec(p : LinearSieve::primes)
  {
    if (p > n)
      break;
    for (int i = n / p; i >= 1; i--)
      b[i] = M::op(b[i], b[i * p]);
  }
  return b;
}

// μ' は ζ' の逆変換
// μ'a(n) = Σ{n | m} μ(m/n)g(m)
// G は可換群 (Σ だと +)
// 1-indexed (a[0] は使わない)
template <class G>
vc<typename G::S> mobius_multiple(const vc<typename G::S> &a)
{
  const int n = SZ<int>(a) - 1;
  LinearSieve::reserve(n);
  auto b = a;
  fec(p : LinearSieve::primes)
  {
    if (p > n)
      break;
    for (int i = 1; i * p <= n; i++)
      b[i] = G::op(b[i], G::inv(b[i * p]));
  }
  return b;
}
