#pragma once

#include "internal_mod32_value.hpp"
#include "../../algebra/algebra_basic_ops.hpp"

namespace internal
{
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
