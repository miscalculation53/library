#pragma once

#include "template/template_all_but_modint.hpp"
#include "algebra/from_integer.hpp"
#include "math/prime/sieve/linear_sieve.hpp"

/**
 * @brief 乗法的関数のテーブル
 * @docs docs/math/prime/sieve/enumerate_multiplicative.md
 */

namespace internal
{
  template <class R>
  typename R::S multiplicative_from_integer(ll n)
  { return from_integer<R>(n); }

  // q は const な参照で渡す。通常の f(q) と、環を受け取る f(q, R{}) に対応する。
  template <class R, class F, class Q>
  decltype(auto) eval_primepower(const F &f, const Q &q)
  {
    if constexpr (is_invocable_v<const F &, const Q &>)
      return f(q);
    else
      return f(q, R{});
  }
}

// R: 可換環
// f_primepower: f(p^e) を計算する関数 (引数は .p, .e, .pe を持つ)
// f(1), ..., f(n) を返す
// f(p^e) と環の演算が O(1) 時間で計算できるとき O(n) 時間
template <class R, class F>
vc<typename R::S> enumerate_multiplicative(int n, const F &f_primepower)
{
  assert(0 <= n);
  LinearSieve::reserve(n);
  vc<typename R::S> res(n + 1, R::e0());
  if (n >= 1)
    res[1] = R::e1();
  for (int d = 2; d <= n; d++)
  {
    const auto q = LinearSieve::lpf_[d];
    if (d == q.pe)
      res[d] = internal::eval_primepower<R>(f_primepower, q);
    else
      res[d] = R::mul(res[d / q.pe], res[q.pe]);
  }
  return res;
}

// f_primepower: f(p^e) を計算する関数 (引数は .p, .e, .pe を持つ)
// f(1), ..., f(n) を返す
// f(p^e) が O(1) 時間のとき、O(n) 時間
template <class F>
auto enumerate_multiplicative(int n, const F &f_primepower)
{
  using S = decay_t<invoke_result_t<const F &, const PrimePower<int> &>>;
  return enumerate_multiplicative<RingAddSubMul<S>>(n, f_primepower);
}

// 完全乗法的関数
// f_primepower: f(p^e) を計算する関数 (引数は .p, .e, .pe を持つ)
// ただし f_primepower は f(p) に対してのみ呼ばれる
// f(p) が O(t) 時間で計算できるとき O(n + π(n)t) 時間
template <class R, class F>
vc<typename R::S> enumerate_completely_multiplicative(int n, const F &f_primepower)
{
  assert(0 <= n);
  LinearSieve::reserve(n);
  vc<typename R::S> res(n + 1, R::e0());
  if (n >= 1)
    res[1] = R::e1();
  for (int d = 2; d <= n; d++)
  {
    const int p = LinearSieve::lpf_[d].p;
    if (d == p)
      res[d] = internal::eval_primepower<R>(f_primepower, PrimePower<int>(p, 1, p));
    else
      res[d] = R::mul(res[p], res[d / p]);
  }
  return res;
}

template <class F>
auto enumerate_completely_multiplicative(int n, const F &f_primepower)
{
  using S = decay_t<invoke_result_t<const F &, const PrimePower<int> &>>;
  return enumerate_completely_multiplicative<RingAddSubMul<S>>(n, f_primepower);
}

// 用意済みのラムダは第 2 引数で環 R{} を受け取る
// enumerate_multiplicative<R>(n, totient_primepower) のように渡す

// ε(1) = 1, ε(n) = 0 (n >= 2)
inline constexpr auto e_primepower = [](const auto &q, auto ring)
{ return q.e == 0 ? decltype(ring)::e1() : decltype(ring)::e0(); };

// ζ: f(n) = 1
inline constexpr auto zeta_primepower = [](const auto &, auto ring)
{ return decltype(ring)::e1(); };

// id(n) = n * 1_R
inline constexpr auto id_primepower = [](const auto &q, auto ring)
{ return from_integer<decltype(ring)>(q.pe); };

// f(n) = n^k (k >= 0)
inline constexpr auto pow_primepower = [](ll k)
{
  assert(k >= 0);
  return [k](const auto &q, auto ring)
  {
    using R = decltype(ring);
    return pow_monoid<MonoidOfSemiRingMul<R>>(
      from_integer<R>(q.pe), k);
  };
};

// modint を要素とし R::inv を持つ環用。
// k = 0 なら常に 1。k > 0 で逆元がないときは 0 とする。
inline constexpr auto pow_inv_primepower = [](ll k)
{
  assert(k >= 0);
  return [k](const auto &q, auto ring)
  {
    using R = decltype(ring);
    using S = typename R::S;
    if (k == 0 || q.e == 0)
      return R::e1();
    if (S::mod() % q.p == 0)
      return R::e0();
    return pow_monoid<MonoidOfSemiRingMul<R>>(
      R::inv(from_integer<R>(q.pe)), k);
  };
};

// メビウス関数 μ(p^e): e==0 で 1, e==1 で -1, e>=2 で 0
inline constexpr auto mobius_primepower = [](const auto &q, auto ring)
{
  using R = decltype(ring);
  return q.e == 0 ? R::e1() : q.e == 1 ? R::minus(R::e1()) : R::e0();
};

// 約数個数関数 τ(p^e) = 1 + e
inline constexpr auto divisor_count_primepower = [](const auto &q, auto ring)
{ return from_integer<decltype(ring)>(ll(q.e) + 1); };

// 約数総和関数 σ(p^e) = 1 + p + ... + p^e = (p^{e+1}-1)/(p-1) = (p^e-1)/(p-1) + p^e
inline constexpr auto divisor_sum_primepower = [](const auto &q, auto ring)
{
  using R = decltype(ring);
  return R::add(from_integer<R>(q.pe),
                from_integer<R>((q.pe - 1) / (q.p - 1)));
};

// 約数の k 乗和 σ_k(p^e) = 1 + p^k + ... + (p^k)^e
inline constexpr auto divisor_k_primepower = [](ll k)
{
  assert(k >= 0);
  return [k](const auto &q, auto ring)
  {
    using R = decltype(ring);
    const auto pk = pow_monoid<MonoidOfSemiRingMul<R>>(
      from_integer<R>(q.p), k);
    auto res = R::e1();
    for (int i = 0; i < q.e; i++)
      res = R::add(R::mul(res, pk), R::e1());
    return res;
  };
};

// オイラーのファイ関数 φ(p^e) = (p-1)p^e (e>=1)
inline constexpr auto totient_primepower = [](const auto &q, auto ring)
{ return from_integer<decltype(ring)>(q.pe - q.pe / q.p); };
