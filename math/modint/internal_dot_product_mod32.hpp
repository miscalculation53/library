#pragma once

#include "internal_mod32_value.hpp"
#include "internal_block_dot32.hpp"

namespace internal
{
template <class S>
using dot_product_mod32_value = ordinary_mod32<S>;

template <int block, class S, class It1, class It2>
S dot_product_mod32_impl(int n, It1 a, It2 b)
{
  return S::raw(block_dot32(n, S::mod(), block, 0, [&]()
  {
    return ull((a++)->val()) * (b++)->val();
  }));
}

template <class S, class It1, class It2>
S dot_product_mod32_values(int n, It1 a, It2 b)
{
  static_assert(ordinary_mod32<S>::value);
  assert(n >= 0);
  if (S::mod() <= (1 << 30)) return dot_product_mod32_impl<16, S>(n, a, b);
  return dot_product_mod32_impl<4, S>(n, a, b);
}
}
