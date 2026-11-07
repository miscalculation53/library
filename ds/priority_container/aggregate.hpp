#pragma once

#include "../../algebra/algebra_base.hpp"
#include "../../algebra/trivial.hpp"
#include "../../utils/identity.hpp"

namespace priority_container_detail
{
template <class C, class = void>
struct NativeMonoid { using type = GroupTrivial; };
template <class C>
struct NativeMonoid<C, void_t<typename C::monoid_type>> { using type = typename C::monoid_type; };

template <class C, class = void>
struct NativeProjection
{
  using type = Identity;
  static type get(const C &) { return {}; }
};
template <class C>
struct NativeProjection<C, void_t<typename C::projection_type,
    decltype(declval<const C &>().projection())>>
{
  using type = typename C::projection_type;
  static type get(const C &c) { return c.projection(); }
};

template <class C, class M, class Projection = typename NativeProjection<C>::type, class = void>
struct HasNativeAggregate : false_type {};
template <class C, class M, class Projection>
struct HasNativeAggregate<C, M, Projection, void_t<typename C::monoid_type,
    decltype(declval<const C &>().all_prod())>>
    : bool_constant<is_same_v<typename C::monoid_type, M> &&
                    is_same_v<typename NativeProjection<C>::type, Projection>> {};

template <class M, bool Native>
struct AggregateCache {};
template <class M>
struct AggregateCache<M, false> { typename M::S product = M::e(); };
template <>
struct AggregateCache<GroupTrivial, false> {};

}
