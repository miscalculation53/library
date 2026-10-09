#pragma once

#include "../template/template_types.hpp"

/**
 * @brief 自明群
 * @docs docs/algebra/trivial.md
 */

struct GroupTrivial
{
  using S = monostate;
  static constexpr S e() { return {}; }
  static constexpr S op(S, S) { return {}; }
  static constexpr S inv(S) { return {}; }
};
