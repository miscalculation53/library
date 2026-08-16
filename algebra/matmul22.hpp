#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（$2 \times 2$ 行列の積）
 * @docs docs/algebra/matmul22.md
 */

template <class mint>
struct GroupMatMul22
{
  using S = array<mint, 4>;
  static constexpr S op(const S &l, const S &r)
  {
    cauto &[a, b, c, d] = l;
    cauto &[e, f, g, h] = r;
    return {{a * e + b * g, a * f + b * h, c * e + d * g, c * f + d * h}};
  }
  static constexpr S e() { return {{1, 0, 0, 1}}; }
  static constexpr S inv(const S &m)
  {
    cauto &[a, b, c, d] = m;
    mint detinv = (a * d - b * c).inv();
    return {{d * detinv, -b * detinv, -c * detinv, a * detinv}};
  }
};
