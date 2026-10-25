#pragma once

#include "is_integral_ext.hpp"
#include "larger_int.hpp"

/**
 * @brief 積と定数の和の商・剰余
 * @docs docs/utils/mul_add_divmod.md
 */

// (a*b+c) / m の商・剰余。a,c < m なら商は b 以下。
template <class U>
constexpr pair<U, U> mul_add_divmod(U a, U b, U c, U m)
{
  static_assert(is_unsigned_ext<U> && !is_same_v<U, bool>);
  assert(m > 0 && a < m && c < m);
  if constexpr (sizeof(U) < sizeof(u128))
  {
    using W = larger_int_t<U>;
    const W y = W(a) * b + c;
    return {U(y / m), U(y % m)};
  }
  else
  {
    if (a == 0 || b <= (numeric_limits<U>::max() - c) / a)
    {
      const U y = a * b + c;
      return {y / m, y % m};
    }
    // 商・剰余を保ちながら b の各 bit を読む。
    U q = 0, r = 0;
    for (int i = numeric_limits<U>::digits - 1; i >= 0; --i)
    {
      q *= 2;
      if (r >= m - r) r -= m - r, ++q;
      else r += r;
      if ((b >> i) & 1)
      {
        if (r >= m - a) r -= m - a, ++q;
        else r += a;
      }
    }
    if (r >= m - c) r -= m - c, ++q;
    else r += c;
    return {q, r};
  }
}
