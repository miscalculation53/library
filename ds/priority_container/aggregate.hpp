#pragma once

#include "../../algebra/algebra_base.hpp"
#include "../../algebra/trivial.hpp"
#include "../../utils/identity.hpp"

namespace priority_container_detail
{
// 集約型を省略したとき、基底の monoid_type を使う。通常のコンテナは集約なし。
template <class C, class = void>
struct NativeMonoid { using type = GroupTrivial; };
template <class C>
struct NativeMonoid<C, void_t<typename C::monoid_type>> { using type = typename C::monoid_type; };

// 射影を省略したとき、基底に設定した射影を引き継ぐ。通常のコンテナは恒等写像。
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

// 指定した集約を基底の all_prod() に任せられるかを判定する。
template <class C, class M, class Projection = typename NativeProjection<C>::type, class = void>
struct HasNativeAggregate : false_type {};
template <class C, class M, class Projection>
struct HasNativeAggregate<C, M, Projection, void_t<typename C::monoid_type,
    decltype(declval<const C &>().all_prod())>>
    : bool_constant<is_same_v<typename C::monoid_type, M> &&
                    is_same_v<typename NativeProjection<C>::type, Projection>> {};

// 基底に集約を任せる場合と集約なしの場合は空にし、それ以外は集約値を保存する。
template <class M, bool Native>
struct AggregateCache {};
template <class M>
struct AggregateCache<M, false> { typename M::S product = M::e(); };
template <>
struct AggregateCache<GroupTrivial, false> {};

} // namespace priority_container_detail
