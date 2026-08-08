#pragma once

#include "../template/template_types.hpp"

/**
 * @brief 積の計算に使う拡張型
 * @docs docs/utils/larger_int.md
 */

template <class T>
struct larger_int
{
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
using larger_int_t = typename larger_int<T>::type;
