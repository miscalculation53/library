#pragma once

#include "../algebra/algebra_basic_ops.hpp"
#include "modint/internal_mod32.hpp"
#include "modint/internal_dot_product_mod32.hpp"

/**
 * @brief 半環上の内積（32 bit modint は剰余をまとめて計算）
 * @docs docs/math/dot_product.md
 */

namespace internal
{
template <class SR>
struct dot_product_mod32 : false_type {};
template <class S>
struct dot_product_mod32<FieldAddSubMulDiv<S>> : dot_product_mod32_value<S> {};
template <class S>
struct dot_product_mod32<RingAddSubMul<S>> : dot_product_mod32_value<S> {};
template <class S>
struct dot_product_mod32<SemiRingFromMonoidMonoid<MonoidAdd<S>, MonoidMul<S>>>
    : dot_product_mod32_value<S> {};


}

// a, b からそれぞれ n 要素読む。逆向きの iterator にも対応する。
// SR::S, SR::e0, SR::add, SR::mul のみを要求する。e1・minus・inv は不要。
template <class SR, class It1, class It2>
typename SR::S dot_product(int n, It1 a, It2 b)
{
  using S = typename SR::S;
  assert(n >= 0);
  if constexpr (internal::dot_product_mod32<SR>::value)
  {
    return internal::dot_product_mod32_values<S>(n, a, b);
  }
  else
  {
    S sum = SR::e0();
    repi(i, n) sum = SR::add(sum, SR::mul(*a++, *b++));
    return sum;
  }
}
