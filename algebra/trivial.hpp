#pragma once

#include "internal_power.hpp"
#include "../template/template_types.hpp"

/**
 * @brief 自明群
 * @docs docs/algebra/trivial.md
 */

struct GroupTrivial
{
  using S = monostate;
  static constexpr S e() { return {}; }
  template <class I>
  static constexpr S pow(S, I k)
  {
    internal_monoid_power::check_exponent(k);
    return e();
  }

  static constexpr S op(S, S) { return {}; }
  static constexpr S inv(S) { return {}; }
};
