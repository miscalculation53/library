#pragma once

#include "../template/template_types.hpp"

/**
 * @brief 値または関数から値を取得
 * @docs docs/utils/resolved_value.md
 */

template <class T, auto x, enable_if_t<!is_invocable_v<decltype(x)>, int> = 0>
constexpr T resolved_value()
{
  return T(x);
}

template <class T, auto x, enable_if_t<is_invocable_v<decltype(x)>, long> = 0>
const T &resolved_value()
{
  static const T value = T(x());
  return value;
}
