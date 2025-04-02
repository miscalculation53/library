#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief string を mint に
 * @docs docs/math/modint/stom.md
 */

template <class mint>
mint stom(string s)
{
  mint res = 0;
  fec(c : s)
  {
    res *= 10;
    res += c - '0';
  }
  return res;
}
