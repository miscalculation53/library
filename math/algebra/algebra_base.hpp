#pragma once

#include "../../template/template_all.hpp"

/**
 * @brief 代数的構造の struct（基本）
 * @docs docs/math/algebra/algebra_base.md
 */

template <class S_, auto op_, auto e_>
struct Monoid
{
  using S = S_;
  static constexpr auto op = op_;
  static constexpr auto e = e_;
};

template <class S_, auto op_, auto e_, auto inv_>
struct Group
{
  using S = S_;
  static constexpr auto op = op_;
  static constexpr auto e = e_;
  static constexpr auto inv = inv_;
};

template <class S_, auto add_, auto e0_, auto mul_, auto e1_>
struct SemiRing
{
  using S = S_;
  static constexpr auto add = add_;
  static constexpr auto e0 = e0_;
  static constexpr auto mul = mul_;
  static constexpr auto e1 = e1_;
};

template <class S_, auto add_, auto e0_, auto minus_, auto mul_, auto e1_>
struct Ring
{
  using S = S_;
  static constexpr auto add = add_;
  static constexpr auto e0 = e0_;
  static constexpr auto minus = minus_;
  static constexpr auto mul = mul_;
  static constexpr auto e1 = e1_;
};

template <class SR>
using MonoidOfSemiRingAdd = Monoid<typename SR::S, SR::add, SR::e0>;
template <class SR>
using MonoidOfSemiRingMul = Monoid<typename SR::S, SR::mul, SR::e1>;
template <class R>
using GroupOfRingAdd = Group<typename R::S, R::add, R::e0, R::minus>;