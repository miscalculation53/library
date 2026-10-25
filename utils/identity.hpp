#pragma once

#include "../template/template_types.hpp"

/**
 * @brief 引数をそのまま返す関数オブジェクト
 * @docs docs/utils/identity.md
 */

struct Identity
{
  template <class T>
  constexpr T &&operator()(T &&x) const noexcept { return std::forward<T>(x); }
};
