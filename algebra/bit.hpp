#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（ビット演算）
 * @docs docs/algebra/bit.md
 */

template <class T>
struct GroupXor
{
  using S = T;
  static constexpr S op(S x, S y) { return x ^ y; }
  static constexpr S e() { return 0; }
  static constexpr S inv(S x) { return x; }
  template <class I>
  static constexpr S pow(S x, I k) { return k & 1 ? x : e(); }
};

template <class T>
struct MonoidAnd
{
  using S = T;
  static constexpr S op(S x, S y) { return x & y; }
  static constexpr S e() { return numeric_limits<S>::max(); }
  template <class I>
  static constexpr S pow(S x, I k) { return k == 0 ? e() : x; }
};

template <class T>
struct MonoidOr
{
  using S = T;
  static constexpr S op(S x, S y) { return x | y; }
  static constexpr S e() { return 0; }
  template <class I>
  static constexpr S pow(S x, I k) { return k == 0 ? e() : x; }
};

template <class T>
using RingXorAnd = RingFromGroupMonoid<GroupXor<T>, MonoidAnd<T>>;
template <class T>
using SemiRingOrAnd = SemiRingFromMonoidMonoid<MonoidOr<T>, MonoidAnd<T>>;
template <class T>
using SemiRingAndOr = SemiRingFromMonoidMonoid<MonoidAnd<T>, MonoidOr<T>>;
