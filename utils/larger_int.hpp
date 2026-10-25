#pragma once

#include "../template/template_types.hpp"

/**
 * @brief 積の計算に使う拡張型
 * @docs docs/utils/larger_int.md
 */

namespace larger_int_detail
{
  template <class T>
  [[deprecated("larger_int: 128-bit integer is not widened; intermediate arithmetic may overflow")]]
  constexpr bool warn_no_wider_integer()
  {
    return true;
  }
}

template <class T>
struct larger_int
{
private:
  static constexpr bool check()
  {
    // 型エイリアス経由でも、128 ビット整数を入力にしたときだけ警告する。
    if constexpr (is_same_v<T, i128> || is_same_v<T, u128>)
      return larger_int_detail::warn_no_wider_integer<T>();
    return true;
  }
  static_assert(check());

public:
  using type = T;
};

#define LARGER_INT(T, U) \
  template <>            \
  struct larger_int<T>   \
  {                      \
    using type = U;      \
  };

LARGER_INT(signed char, short)
LARGER_INT(short, int)
LARGER_INT(int, long long)
LARGER_INT(long, __int128_t)
LARGER_INT(long long, __int128_t)

LARGER_INT(unsigned char, unsigned short)
LARGER_INT(unsigned short, unsigned int)
LARGER_INT(unsigned int, unsigned long long)
LARGER_INT(unsigned long, __uint128_t)
LARGER_INT(unsigned long long, __uint128_t)

#undef LARGER_INT

template <class T>
struct Rational;

template <class T>
struct larger_int<Rational<T>>
{
  using type = Rational<typename larger_int<T>::type>;
};

template <class T>
using larger_int_t = typename larger_int<T>::type;
