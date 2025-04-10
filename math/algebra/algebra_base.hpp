#pragma once

#include "../../template/template_all_but_modint.hpp"

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

template <class S_, auto add_, auto e0_, auto minus_, auto mul_, auto e1_, auto inv_>
struct Field
{
  using S = S_;
  static constexpr auto add = add_;
  static constexpr auto e0 = e0_;
  static constexpr auto minus = minus_;
  static constexpr auto mul = mul_;
  static constexpr auto e1 = e1_;
  static constexpr auto inv = inv_;
};

template <class SR>
using MonoidOfSemiRingAdd = Monoid<typename SR::S, SR::add, SR::e0>;
template <class SR>
using MonoidOfSemiRingMul = Monoid<typename SR::S, SR::mul, SR::e1>;
template <class R>
using GroupOfRingAdd = Group<typename R::S, R::add, R::e0, R::minus>;
template <class K>
using GroupOfFieldMul = Group<typename K::S, K::mul, K::e1, K::inv>;

// Madd は可換
template <class Madd, class Mmul>
struct SemiRingFromMonoidMonoid
{
  static_assert(is_same_v<typename Madd::S, typename Mmul::S>, "Madd::S and Mmul::S must be identical");
  using S = typename Madd::S;
  static constexpr auto add = Madd::op;
  static constexpr auto e0 = Madd::e;
  static constexpr auto mul = Mmul::op;
  static constexpr auto e1 = Mmul::e;
};

// Gadd は可換
template <class Gadd, class Mmul>
struct RingFromGroupMonoid
{
  static_assert(is_same_v<typename Gadd::S, typename Mmul::S>, "Gadd::S and Mmul::S must be identical");
  using S = typename Gadd::S;
  static constexpr auto add = Gadd::op;
  static constexpr auto e0 = Gadd::e;
  static constexpr auto minus = Gadd::inv;
  static constexpr auto mul = Mmul::op;
  static constexpr auto e1 = Mmul::e;
};

// Gadd, Gmul は可換
template <class Gadd, class Gmul>
struct FieldFromGroupGroup
{
  static_assert(is_same_v<typename Gadd::S, typename Gmul::S>, "Gadd::S and Gmul::S must be identical");
  using S = typename Gadd::S;
  static constexpr auto add = Gadd::op;
  static constexpr auto e0 = Gadd::e;
  static constexpr auto minus = Gadd::inv;
  static constexpr auto mul = Gmul::op;
  static constexpr auto e1 = Gmul::e;
  static constexpr auto inv = Gmul::inv;
};
