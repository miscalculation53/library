#pragma once

#include "default_infty.hpp"
#include "resolved_value.hpp"

/**
 * @brief 既定値・値・関数から無限大を取得
 * @docs docs/utils/resolved_infty.md
 */

template <class T, auto x = nullptr>
constexpr decltype(auto) resolved_infty()
{
  if constexpr (is_same_v<decltype(x), nullptr_t>)
    return default_infty<T>();
  else
    return resolved_value<T, x>();
}
