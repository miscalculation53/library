#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（四則演算と min, max）
 * @docs docs/algebra/algebra_basic_ops.md
 */

template <class T, class = void>
struct has_e1 : false_type {};
template <class T>
struct has_e1<T, void_t<decltype(T::e1())>> : true_type {};
template <class T>
inline constexpr bool has_e1_v = has_e1<T>::value;

template <class T>
struct MonoidMul
{
  using S = T;
  static constexpr S op(S a, S b) { return a * b; }
  static constexpr S e()
  {
    if constexpr (has_e1_v<S>)
      return S::e1();
    else
      return 1;
  }
};

template <class T>
struct GroupAddSub
{
  using S = T;
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S e()
  {
    if constexpr (has_e0_v<S>)
      return S::e0();
    else
      return S{};
  }
  static constexpr S inv(S a) { return -a; }
  template <class I, class = decltype(declval<S>() * declval<I>())>
  static constexpr S pow(const S &a, I k) { return a * k; }
};
template <class T>
struct GroupMulDiv
{
  using S = T;
  static constexpr S op(S a, S b) { return a * b; }
  static constexpr S e()
  {
    if constexpr (has_e1_v<S>)
      return S::e1();
    else
      return S(1);
  }
  static constexpr S inv(S a) { return e() / a; }
};

template <class T, const T infty = INF>
using SemiRingMinPlus = SemiRingFromMonoidMonoid<MonoidMin<T, infty>, MonoidAdd<T>>;
template <class T, const T infty = INF>
using SemiRingMaxPlus = SemiRingFromMonoidMonoid<MonoidMax<T, infty>, MonoidAdd<T>>;
template <class T>
using RingAddSubMul = RingFromGroupMonoid<GroupAddSub<T>, MonoidMul<T>>;
template <class T>
using FieldAddSubMulDiv = FieldFromGroupGroup<GroupAddSub<T>, GroupMulDiv<T>>;
