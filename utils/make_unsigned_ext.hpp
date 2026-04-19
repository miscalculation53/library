#pragma once

#include "../template/template_types.hpp"

/**
 * @brief $128$ ビット整数に対応した make_singed と make_unsigned
 * @docs docs/utils/make_unsigned_ext.md
 */

template <class T>
struct make_unsigned_ext
{
  using type = make_unsigned_t<T>;
};
template <>
struct make_unsigned_ext<i128>
{
  using type = u128;
};
template <class T>
using make_unsigned_ext_t = typename make_unsigned_ext<T>::type;

template <class T>
struct make_signed_ext
{
  using type = make_signed_t<T>;
};
template <>
struct make_signed_ext<u128>
{
  using type = i128;
};
template <class T>
using make_signed_ext_t = typename make_signed_ext<T>::type;
