#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 値の和・添字の累乗和を持つモノイド
 * @docs docs/algebra/index_power_sum.md
 */

template <class T, int D>
struct MonoidIndexPowerSum
{
  static_assert(D >= 0);
  struct S
  {
    T val = T{};
    array<T, D + 1> power_sum{};
    S() {}
    S(int i, T val) : val(val)
    {
      power_sum[0] = T(1);
      repi(k, 1, D + 1) power_sum[k] = power_sum[k - 1] * T(i);
    }
    CPP_DUMP_DEFINE_DATA(val, power_sum);
  };
  static constexpr S op(const S &a, const S &b)
  {
    S res;
    res.val = a.val + b.val;
    repi(k, D + 1) res.power_sum[k] = a.power_sum[k] + b.power_sum[k];
    return res;
  }
  static constexpr S e() { return {}; }
};
