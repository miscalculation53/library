#pragma once

#include "power_sum.hpp"

/**
 * @brief 複数の数列の混合累乗和を持つモノイド
 * @docs docs/algebra/multivariate_power_sum.md
 */

template <class T, int... Ds>
struct MonoidMultivariatePowerSum
{
  static_assert(sizeof...(Ds) > 0);
  static_assert(((Ds >= 0) && ...));
  static constexpr int dim = sizeof...(Ds);
  static constexpr array<int, dim> degree{Ds...};
  static constexpr int size = ((Ds + 1) * ...);
  static constexpr array<int, dim> stride = []
  {
    array<int, dim> res{};
    int s = 1;
    repi(d, dim - 1, -1, -1) res[d] = s, s *= degree[d] + 1;
    return res;
  }();

  static constexpr int index(const array<int, dim> &exponent)
  {
    int res = 0;
    repi(d, dim)
    {
      assert(0 <= exponent[d] && exponent[d] <= degree[d]);
      res += exponent[d] * stride[d];
    }
    return res;
  }

  struct S
  {
    array<T, size> power_sum{};
    S() {}
    S(const array<T, dim> &x)
    {
      if constexpr (is_integral_ext<T>)
        init<decltype(0U + make_unsigned_ext_t<T>{})>(x);
      else
        init<T>(x);
    }
    T &at(const array<int, dim> &exponent) { return power_sum[index(exponent)]; }
    const T &at(const array<int, dim> &exponent) const { return power_sum[index(exponent)]; }
    CPP_DUMP_DEFINE_DATA(power_sum);

  private:
    template <class U>
    void init(const array<T, dim> &x)
    {
      power_sum[0] = T(1);
      int len = 1;
      repi(d, dim)
      {
        if (degree[d] == 0) continue;
        // 既存の成分を末尾から広げ、変数 d の累乗を掛ける。
        repi(j, len - 1, -1, -1)
        {
          U p = U(power_sum[j]);
          repi(k, degree[d] + 1)
          {
            power_sum[j * (degree[d] + 1) + k] = T(p);
            if (k < degree[d]) p = p * U(x[d]);
          }
        }
        len *= degree[d] + 1;
      }
    }
  };
  static constexpr S op(const S &a, const S &b)
  {
    S res;
    repi(k, size) res.power_sum[k] = a.power_sum[k] + b.power_sum[k];
    return res;
  }
  static constexpr S e() { return {}; }
};
