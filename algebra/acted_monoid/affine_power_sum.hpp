#pragma once

#include "../power_sum.hpp"
#include "../affine_function.hpp"

/**
 * @brief 作用つきモノイド：一次関数作用・累乗和取得
 * @docs docs/algebra/acted_monoid/affine_power_sum.md
 */

template <class T, int D>
struct ActedMonoidAffinePowerSum
{
  using M = MonoidPowerSum<T, D>;
  using MF = MonoidAffineFunction<T>;
  using S = typename M::S;
  using F = typename MF::S;
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;

private:
  template <class U>
  static constexpr S mapping_impl(const F &f, const S &x)
  {
    const U b = U(f.b), c = U(f.c);
    if (b == U(1) && c == U{}) return x;
    S res;
    res.power_sum[0] = x.power_sum[0];
    if (c == U{})
    {
      U p = U(1);
      repi(k, 1, D + 1) p = p * b, res.power_sum[k] = T(p * U(x.power_sum[k]));
      return res;
    }
    if (b == U{})
    {
      U p = U(x.power_sum[0]);
      repi(k, 1, D + 1) p = p * c, res.power_sum[k] = T(p);
      return res;
    }

    // coeff[j] は (bt+c)^k の t^j の係数。
    array<U, D + 1> coeff{};
    coeff[0] = U(1);
    repi(k, 1, D + 1)
    {
      coeff[k] = b * coeff[k - 1];
      repi(j, k - 1, 0, -1) coeff[j] = c * coeff[j] + b * coeff[j - 1];
      coeff[0] = coeff[0] * c;
      U sum{};
      repi(j, k + 1) sum = sum + coeff[j] * U(x.power_sum[j]);
      res.power_sum[k] = T(sum);
    }
    return res;
  }

public:
  static constexpr S mapping(const F &f, const S &x)
  {
    if constexpr (is_integral_ext<T>)
      return mapping_impl<decltype(0U + make_unsigned_ext_t<T>{})>(f, x);
    else
      return mapping_impl<T>(f, x);
  }
  static constexpr F composition(const F &f, const F &g)
  {
    if constexpr (is_integral_ext<T>)
    {
      using U = decltype(0U + make_unsigned_ext_t<T>{});
      return {T(U(f.b) * U(g.b)), T(U(f.b) * U(g.c) + U(f.c))};
    }
    else
      return MF::op(f, g);
  }
  static constexpr F id() { return {T(1), T{}}; }
};
