#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 代数的構造の struct（基本）
 * @docs docs/algebra/algebra_base.md
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

template <class M>
struct OppositeMonoid
{
  using S = typename M::S;
  static constexpr S op(const S &a, const S &b) { return M::op(b, a); }
  static constexpr auto e = M::e;
};
template <class G>
struct OppositeGroup
{
  using S = typename G::S;
  static constexpr S op(const S &a, const S &b) { return G::op(b, a); }
  static constexpr auto e = G::e;
  static constexpr auto inv = G::inv;
};
template <class M>
struct NormalAndOppositeMonoid
{
  struct S
  {
    typename M::S normal;
    typename M::S opposite;
    S() {}
    template <class... Args,
              std::enable_if_t<std::is_constructible_v<typename M::S, Args...>, std::nullptr_t> = nullptr>
    S(Args &&...args)
        : normal(std::forward<Args>(args)...), opposite(normal) {}
    S rev() { return {opposite, normal}; }
    S(const typename M::S &normal, const typename M::S &opposite) : normal(normal), opposite(opposite) {}
  };
  static constexpr S op(const S &a, const S &b) { return {M::op(a.normal, b.normal), M::op(b.opposite, a.opposite)}; }
  static constexpr S e() { return {M::e(), M::e()}; }
};
template <class G>
struct NormalAndOppositeGroup
{
  struct S
  {
    typename G::S normal;
    typename G::S opposite;
    S() {}
    template <class... Args,
              std::enable_if_t<std::is_constructible_v<typename G::S, Args...>, std::nullptr_t> = nullptr>
    S(Args &&...args)
        : normal(std::forward<Args>(args)...), opposite(normal) {}
    S rev() { return {opposite, normal}; }
    S(const typename G::S &normal, const typename G::S &opposite) : normal(normal), opposite(opposite) {}
  };
  static constexpr S op(const S &a, const S &b) { return {G::op(a.normal, b.normal), G::op(b.opposite, a.opposite)}; }
  static constexpr S e() { return {G::e(), G::e()}; }
  static constexpr S inv(const S &a) { return {G::inv(a.normal), G::inv(a.opposite)}; }
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

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(normal, opposite);
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(dump_data());
#endif
