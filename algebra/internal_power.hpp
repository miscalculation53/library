#pragma once

//! SPDX-License-Identifier: CC0-1.0

#include "../template/template_all_but_modint.hpp"
#include "../utils/make_unsigned_ext.hpp"

namespace internal_monoid_power
{
  template <class I>
  constexpr void check_exponent(I k)
  {
    static_assert(is_integral_ext<I>);
    if constexpr (is_signed_ext<I>) assert(k >= 0);
  }

  // 波括弧で引数を渡し、乗算の引数や係数のコンストラクタでの縮小変換を除く。
  template <class T, class I, class = void>
  struct HasMemberScalarProduct : false_type {};
  template <class T, class I>
  struct HasMemberScalarProduct<T, I, void_t<decltype(T(declval<const T &>().operator*({declval<I>()})))>> : true_type {};
  template <class T, class I, class = void>
  struct HasFreeScalarProduct : false_type {};
  template <class T, class I>
  struct HasFreeScalarProduct<T, I, void_t<decltype(T(operator*(declval<const T &>(), {declval<I>()})))>> : true_type {};
  template <class T, class I>
  struct HasScalarProduct : bool_constant<is_arithmetic_v<T> || is_same_v<T, f128> ||
                                         HasMemberScalarProduct<T, I>::value || HasFreeScalarProduct<T, I>::value> {};

  template <class T, class I>
  constexpr T scalar_product(const T &x, I k)
  {
    if constexpr (is_arithmetic_v<T> || is_same_v<T, f128>) return T(x * k);
    else if constexpr (HasMemberScalarProduct<T, I>::value) return T(x.operator*({k}));
    else return T(operator*(x, {k}));
  }

  // x を k 回加算する。整数型では T の幅で符号なし計算を行う。
  template <class T, class I>
  constexpr T scale(const T &x, I k)
  {
    check_exponent(k);
    if (k == 0) return T{};
    if constexpr (is_integral_ext<T> && !is_same_v<T, bool>)
    {
      using U = decltype(0U + make_unsigned_ext_t<T>{});
      return T(U(x) * U(k));
    }
    else if constexpr (HasScalarProduct<T, I>::value)
      return scalar_product(x, k);
    else
    {
      T sum{}, value = x;
      while (true)
      {
        if (k & 1) sum = sum + value;
        k >>= 1;
        if (k == 0) return sum;
        value = value + value;
      }
    }
  }

  // x, cap >= 0 として min(k*x, cap) を返す。
  template <class T, class I>
  constexpr T bounded_scale(const T &x, I k, const T &cap)
  {
    check_exponent(k);
    if (k == 0 || x == T{}) return T{};
    if constexpr (is_integral_ext<T>)
    {
      if (u128(k) > u128(cap / x)) return cap;
      return scale(x, k);
    }
    else if constexpr (HasScalarProduct<T, I>::value)
      return min(scalar_product(x, k), cap);
    else
    {
      T sum{}, value = min(x, cap);
      while (true)
      {
        if (k & 1) sum = cap - sum < value ? cap : sum + value;
        k >>= 1;
        if (k == 0) return sum;
        value = cap - value < value ? cap : value + value;
      }
    }
  }

  template <class M, class I>
  constexpr typename M::S binary(typename M::S x, I k)
  {
    check_exponent(k);
    typename M::S res = M::e();
    while (k)
    {
      if (k & 1) res = M::op(res, x);
      k >>= 1;
      if (k) x = M::op(x, x);
    }
    return res;
  }

  template <class M, class I>
  using ConstRefPower = typename M::S (*)(const typename M::S &, I);
  template <class M, class I>
  using ValuePower = typename M::S (*)(typename M::S, I);
  template <class M, class I, class = void>
  struct HasConstRefPower : false_type {};
  template <class M, class I>
  struct HasConstRefPower<M, I, void_t<decltype(static_cast<ConstRefPower<M, I>>(M::pow))>> : true_type {};
  template <class M, class I, class = void>
  struct HasValuePower : false_type {};
  template <class M, class I>
  struct HasValuePower<M, I, void_t<decltype(static_cast<ValuePower<M, I>>(M::pow))>> : true_type {};

  template <class M, class I>
  constexpr typename M::S power(const typename M::S &x, I k)
  {
    if constexpr (is_signed_ext<I>) assert(k >= 0);
    // 転送先で指数が狭い整数型へ変換される場合は、元の指数型で二分累乗する。
    if constexpr (HasConstRefPower<M, I>::value || HasValuePower<M, I>::value ||
                  (!is_integral_ext<I> && internal::HasMonoidPow<M, I>::value))
      return M::pow(x, k);
    else
      return binary<M>(x, k);
  }
}
