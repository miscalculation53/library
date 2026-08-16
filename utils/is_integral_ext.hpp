#pragma once

#include "../template/template_types.hpp"

/**
 * @brief $128$ ビット整数を含めた整数判定
 * @docs docs/utils/is_integral_ext.md
 */

template <class T>
constexpr bool is_integral_ext = is_integral_v<T> || is_same_v<T, i128> || is_same_v<T, u128>;

template <class T>
constexpr bool is_signed_ext = is_signed_v<T> || is_same_v<T, i128>;

template <class T>
constexpr bool is_unsigned_ext = is_unsigned_v<T> || is_same_v<T, u128>;
