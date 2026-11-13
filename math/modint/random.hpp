#pragma once

#include "modint.hpp"

/**
 * @brief modint の一様乱数
 * @docs docs/math/modint/random.md
 */

// 法の範囲から一様に選ぶ。nonzero=true では零を除く。
template <class mint = modint998244353, bool nonzero = false>
mint random_modint()
{
  return mint(randrange<ull>(nonzero ? 1 : 0, mint::mod()));
}
