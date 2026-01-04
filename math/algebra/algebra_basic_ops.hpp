#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（四則演算と min, max）
 * @docs docs/math/algebra/algebra_basic_ops.md
 */

template <class T>
struct MonoidMul
{
  using S = T;
  static constexpr S op(S a, S b) { return a * b; }
  static constexpr S e() { return 1; }
};

template <class T>
struct GroupAddSub
{
  using S = T;
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S e() { return S{}; }
  static constexpr S inv(S a) { return -a; }
};
template <class T>
struct GroupMulDiv
{
  using S = T;
  static constexpr S op(S a, S b) { return a * b; }
  static constexpr S e() { return S(1); }
  static constexpr S inv(S a) { return S(1) / a; }
};

template <class T, const T infty = INF>
using SemiRingMinPlus = SemiRingFromMonoidMonoid<MonoidMin<T>, MonoidAdd<T>>;
template <class T, const T infty = INF>
using SemiRingMaxPlus = SemiRingFromMonoidMonoid<MonoidMax<T>, MonoidAdd<T>>;
template <class T>
using RingAddSubMul = RingFromGroupMonoid<GroupAddSub<T>, MonoidMul<T>>;
template <class T>
using FieldAddSubMulDiv = FieldFromGroupGroup<GroupAddSub<T>, GroupMulDiv<T>>;

template <class M>
typename M::S pow_monoid(typename M::S a, ll k)
{
  typename M::S c = M::e();
  for (; k; k >>= 1)
  {
    if (k & 1)
      c = M::op(c, a);
    a = M::op(a, a);
  }
  return c;
}
