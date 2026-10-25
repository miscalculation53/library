#pragma once

#include "is_integral_ext.hpp"

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

/**
 * @brief 型ごとの既定の無限大
 * @docs docs/utils/default_infty.md
 */

namespace default_infty_detail
{
  template <class T, class = void>
  struct has_infty : false_type {};

  template <class T>
  struct has_infty<T, void_t<decltype(T::infty())>> : true_type {};

  template <class T>
  const T &custom_value()
  {
    static const T value = T::infty();
    return value;
  }

  template <class T>
  inline constexpr bool unsupported = false;
}

template <class T>
constexpr decltype(auto) default_infty()
{
  if constexpr (default_infty_detail::has_infty<T>::value)
    return default_infty_detail::custom_value<T>();
  else if constexpr (is_same_v<T, i128> || is_same_v<T, u128>)
    return T(INF) * T(INF);
  else if constexpr (is_integral_ext<T> && !is_same_v<T, bool>)
  {
    if constexpr (sizeof(T) >= sizeof(ll))
      return T(INF);
    else if constexpr (numeric_limits<T>::digits >= 31)
      return (T(1) << 30) - 1;
    else
      return T(numeric_limits<T>::max() / 2);
  }
  else if constexpr (numeric_limits<T>::has_infinity)
    return numeric_limits<T>::infinity();
  else
    static_assert(default_infty_detail::unsupported<T>, "No default infinity for this type; specify infty explicitly or define T::infty().");
}
