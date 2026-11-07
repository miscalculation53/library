#pragma once

#include "modint.hpp"

namespace internal
{
template <class S>
struct ordinary_mod32 : false_type {};
template <int mod, class PowInv>
struct ordinary_mod32<modint_impl<policy_static<mod>, PowInv>> : true_type {};
template <int id>
struct ordinary_mod32<dynamic_modint32<id>> : true_type {};

}
