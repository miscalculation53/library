#pragma once

#include "../utils/is_integral_ext.hpp"

/**
 * @brief モノイド・群のべき乗
 * @docs docs/algebra/monoid_power.md
 */

namespace internal
{
  template <class M, class I, class = void>
  struct HasMonoidPow : false_type {};
  template <class M, class I>
  struct HasMonoidPow<M, I, void_t<decltype(M::pow(declval<const typename M::S &>(), declval<I>()))>> : true_type {};
}

namespace internal_monoid_power
{
  template <class I>
  constexpr void check_exponent(I k)
  {
    static_assert(is_integral_ext<I>);
    if constexpr (is_signed_ext<I>) assert(k >= 0);
  }

  template <class M, class I>
  constexpr typename M::S binary_inplace(typename M::S &x, I k)
  {
    typename M::S res = M::e();
    if (k & 1) res = M::op(res, x);
    for (k >>= 1; k; k >>= 1)
    {
      x = M::op(x, x);
      if (k & 1) res = M::op(res, x);
    }
    return res;
  }

  template <class M, class I>
  constexpr typename M::S binary(typename M::S x, I k)
  {
    check_exponent(k);
    return binary_inplace<M>(x, k);
  }
}

// 非負の k に対して x を k 回結合する。M::pow が呼び出せればそれを使う。
template <class M, class I>
constexpr typename M::S pow_monoid(typename M::S x, I k)
{
  if constexpr (is_signed_ext<I>) assert(k >= 0);
  if constexpr (internal::HasMonoidPow<M, I>::value)
    return M::pow(x, k);
  else
    return internal_monoid_power::binary_inplace<M>(x, k);
}

// x の k 乗を返す。負の k には逆元を使う。
template <class G, class I>
constexpr typename G::S pow_group(typename G::S x, I k)
{
  if constexpr (is_signed_ext<I>)
    if (k < 0)
    {
      x = G::inv(x);
      return G::op(pow_monoid<G>(x, -(k + 1)), x);
    }
  return pow_monoid<G>(x, k);
}
