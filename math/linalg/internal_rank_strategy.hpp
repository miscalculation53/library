#pragma once

#include "../../template/template_all_but_modint.hpp"

namespace internal
{
inline bool prefer_dense_rank(int rows, int columns, u128 formation_work,
                              u128 action_work, int trials = 1)
{
  const int d = min(rows, columns);
  const u128 dense = u128(rows) * columns * d + formation_work;
  const u128 black_box = u128(trials) * d * (u128(rows) + columns + action_work);
  return dense <= 8 * black_box;
}
}
