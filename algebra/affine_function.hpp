#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 代数的構造（一次関数）
 * @docs docs/algebra/affine_function.md
 */

template <class T>
struct MonoidAffineFunction
{
  struct S
  {
    T b, c;
    S() {}
    S(T b, T c) : b(b), c(c) {}
#ifdef LOCAL
    auto dump_data() const { return tie(b, c); }
#endif
  };
  static constexpr S op(const S &f, const S &g)
  {
    return {f.b * g.b, f.b * g.c + f.c};
  }
  static constexpr S e() { return {1, 0}; }
};

// op(f, g) = f ∘ g
template <class mint>
struct GroupAffineFunction
{
  struct S
  {
    mint a, b;
    S() {}
    S(mint a, mint b) : a(a), b(b) {}
    template <class T, class U>
    S(const pair<T, U> &ab) : a(ab.first), b(ab.second) {}
#ifdef LOCAL
    auto dump_data() const { return tie(a, b); }
#endif
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
