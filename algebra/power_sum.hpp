#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../utils/make_unsigned_ext.hpp"

/**
 * @brief $k$ 乗和を持つモノイド
 * @docs docs/algebra/power_sum.md
 */

// 0, ..., D 乗和
template <class T, int D>
struct MonoidPowerSum
{
  static_assert(D >= 0);
  struct S
  {
    array<T, D + 1> power_sum{};
    S() {}
    S(T x)
    {
      power_sum[0] = T(1);
      if constexpr (is_integral_ext<T>)
      {
        using U = decltype(0U + make_unsigned_ext_t<T>{});
        U p = 1;
        repi(k, 1, D + 1) p *= U(x), power_sum[k] = T(p);
      }
      else
        repi(k, 1, D + 1) power_sum[k] = power_sum[k - 1] * x;
    }
    CPP_DUMP_DEFINE_DATA(power_sum);
  };
  static constexpr S op(const S &a, const S &b)
  {
    S res;
    repi(k, D + 1) res.power_sum[k] = a.power_sum[k] + b.power_sum[k];
    return res;
  }
  static constexpr S e() { return {}; }
};
