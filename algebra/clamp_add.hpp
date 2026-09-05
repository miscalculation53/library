#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief clamp・加算関数のモノイド
 * @docs docs/algebra/clamp_add.md
 */

// f(x) = clamp(x, lo, hi) + bias
// op(f, g) = f \circ g
template <class T, auto infty = INF>
struct MonoidClampAdd
{
  struct S
  {
    T lo, hi, bias;
    S() {}
    S(T lo, T hi, T bias = T{}) : lo(lo), hi(hi), bias(bias) {}
    constexpr T operator()(T x) const { return std::clamp(x, lo, hi) + bias; }
    static constexpr S chmin(T v) { return {-resolved_value<T, infty>(), v, T{}}; }
    static constexpr S chmax(T v) { return {v, resolved_value<T, infty>(), T{}}; }
    static constexpr S add(T v) { return {-resolved_value<T, infty>(), resolved_value<T, infty>(), v}; }
    static constexpr S clamp(T lo, T hi) { return {lo, hi, T{}}; }
    static constexpr S set(T v) { return {v, v, T{}}; }
  };
  static constexpr S op(const S &f, const S &g)
  {
    return {
        max(min(g.lo + g.bias, f.hi), f.lo) - g.bias,
        min(max(g.hi + g.bias, f.lo), f.hi) - g.bias,
        g.bias + f.bias};
  }
  static constexpr S e() { return {-resolved_value<T, infty>(), resolved_value<T, infty>(), T{}}; }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(lo, hi, bias);
#endif
