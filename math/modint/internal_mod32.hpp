#pragma once

#include "modint.hpp"
#include "../../algebra/algebra_basic_ops.hpp"

namespace internal
{
// val()/raw() が通常の [0, mod) 表現に対応し、mod < 2^31 である型。
template <class S>
struct ordinary_mod32 : false_type {};
template <int mod>
struct ordinary_mod32<static_modint32<mod>> : true_type {};
template <int id>
struct ordinary_mod32<dynamic_modint32<id>> : true_type {};

// 要素型だけでは判定しない。同じ modint 上の独自演算は汎用実装を使う。
template <class G>
struct ordinary_mod32_add_group : false_type {};
template <class S>
struct ordinary_mod32_add_group<GroupAddSub<S>> : ordinary_mod32<S> {};
template <class S>
struct ordinary_mod32_add_group<Group<S, GroupAddSub<S>::op, GroupAddSub<S>::e, GroupAddSub<S>::inv>>
    : ordinary_mod32<S> {};

template <class F>
struct ordinary_mod32_field : false_type {};
template <class S>
struct ordinary_mod32_field<FieldAddSubMulDiv<S>> : ordinary_mod32<S> {};
}
