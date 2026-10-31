#pragma once

#include "../internal_power.hpp"
#include "../multivariate_power_sum.hpp"
#include "affine_power_sum.hpp"

/**
 * @brief 作用つきモノイド：各数列への一次関数作用・混合累乗和取得
 * @docs docs/algebra/acted_monoid/affine_multivariate_power_sum.md
 */

template <class T, int... Ds>
struct ActedMonoidAffineMultivariatePowerSum
{
  using M = MonoidMultivariatePowerSum<T, Ds...>;
  using S = typename M::S;
  struct F
  {
    array<T, M::dim> b, c{};
    F() { b.fill(T(1)); }
    F(const array<T, M::dim> &b, const array<T, M::dim> &c) : b(b), c(c) {}
    CPP_DUMP_DEFINE_DATA(b, c);
  };
  static constexpr auto op = M::op;
  static constexpr auto e = M::e;
  template <class I>
  static constexpr S pow(const S &x, I k) { return internal_monoid_power::power<M>(x, k); }

private:
  template <int d, int D, int... Rest>
  static constexpr void mapping_axes(const F &f, S &x)
  {
    if (!(f.b[d] == T(1) && f.c[d] == T{}))
    {
      using AM = ActedMonoidAffinePowerSum<T, D>;
      constexpr int stride = M::stride[d], block = stride * (D + 1);
      repi(base, 0, M::size, block) repi(offset, stride)
      {
        typename AM::S fiber;
        repi(k, D + 1) fiber.power_sum[k] = x.power_sum[base + k * stride + offset];
        fiber = AM::mapping({f.b[d], f.c[d]}, fiber);
        repi(k, D + 1) x.power_sum[base + k * stride + offset] = fiber.power_sum[k];
      }
    }
    if constexpr (sizeof...(Rest) > 0) mapping_axes<d + 1, Rest...>(f, x);
  }

public:
  static constexpr S mapping(const F &f, S x)
  {
    mapping_axes<0, Ds...>(f, x);
    return x;
  }
  static constexpr F composition(const F &f, const F &g)
  {
    F res;
    repi(d, M::dim)
    {
      const auto h = ActedMonoidAffinePowerSum<T, 0>::composition({f.b[d], f.c[d]}, {g.b[d], g.c[d]});
      res.b[d] = h.b, res.c[d] = h.c;
    }
    return res;
  }
  static constexpr F id() { return {}; }
};
