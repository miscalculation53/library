#pragma once

#include "algebra_basic_ops.hpp"

/**
 * @brief 整数の環への埋め込み
 * @docs docs/algebra/from_integer.md
 */

namespace internal
{
  template <class R>
  struct RingIntegerEmbedding
  {
    static constexpr typename R::S get(ll n)
    { return pow_group<GroupOfRingAdd<R>>(R::e1(), n); }
  };
  template <class G, class M>
  struct RingIntegerEmbedding<RingFromGroupMonoid<G, M>>
  {
    static constexpr typename G::S get(ll n)
    { return pow_group<G>(M::e(), n); }
  };
  template <class G, class H>
  struct RingIntegerEmbedding<FieldFromGroupGroup<G, H>>
  {
    static constexpr typename G::S get(ll n)
    { return pow_group<G>(H::e(), n); }
  };
}

// 環 R の単位元を n 倍した要素を返す。負の n には加法逆元を使う。
template <class R>
constexpr typename R::S from_integer(ll n)
{
  return internal::RingIntegerEmbedding<R>::get(n);
}
