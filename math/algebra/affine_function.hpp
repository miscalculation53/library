#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（一次関数）
 * @docs docs/math/algebra/affine_function.md
 */

// op(f, g) = f ∘ g
template <class mint>
struct GroupAffineFunction
{
  struct S
  {
    mint a, b;
    S() {}
    S(mint a, mint b) : a(a), b(b) {}
    S(const pair<mint, mint> &ab) : a(ab.first), b(ab.second) {}
  };
  static constexpr S op(const S &f, const S &g)
  {
    // f.a (g.a x + g.b) + f.b
    return {f.a * g.a, f.a * g.b + f.b};
  }
  static constexpr S e() { return {1, 0}; }
  static constexpr S inv(const S &f)
  {
    // y = ax + b <=> x = (y-b)/a
    mint ainv = 1 / f.a;
    return {ainv, -f.b * ainv};
  }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(a, b);
#endif
