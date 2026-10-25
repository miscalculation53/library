#pragma once

#include "is_integral_ext.hpp"
#include "make_unsigned_ext.hpp"

/**
 * @brief 整数の符号・絶対値・剰余を保つ型変換
 * @docs docs/utils/integer_arithmetic.md
 */

template <class T>
constexpr bool is_negative(T x)
{
  static_assert(is_integral_ext<T>);
  if constexpr (is_signed_ext<T>) return x < 0;
  return false;
}

template <class T>
constexpr make_unsigned_ext_t<T> unsigned_abs(T x)
{
  static_assert(is_integral_ext<T> && !is_same_v<T, bool>);
  using U = make_unsigned_ext_t<T>;
  return is_negative(x) ? U(U(0) - U(x)) : U(x);
}

// 整数は整数昇格後も unsigned となる型、それ以外は元の型。
template <class T, bool = is_integral_ext<T>>
struct unsigned_arithmetic { using type = T; };

template <class T>
struct unsigned_arithmetic<T, true>
{
  static_assert(!is_same_v<T, bool>);
  using type = common_type_t<unsigned int, make_unsigned_ext_t<T>>;
};

template <class T>
using unsigned_arithmetic_t = typename unsigned_arithmetic<T>::type;

// mod 2^w (w は R の幅) で合同な値を返す。
template <class R, class T>
constexpr R wrapping_cast(T x)
{
  static_assert(is_integral_ext<R> && !is_same_v<R, bool>);
  static_assert(is_integral_ext<T> && !is_same_v<T, bool>);
  if constexpr (is_signed_ext<R>)
  {
    using U = make_unsigned_ext_t<R>;
    const U bits = U(x);
    if (bits <= U(numeric_limits<R>::max())) return R(bits);
    return R(-1) - R(U(~bits));
  }
  else return R(x);
}
