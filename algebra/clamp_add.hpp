#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief clamp・加算関数のモノイド
 * @docs docs/algebra/clamp_add.md
 */

// op(f, g) = f \circ g
template <class T, T infty = INF>
struct MonoidClampAdd
{
  struct S
  {
    T lo, hi, bias;
    S() {}
    // x -> clamp(x, lo, hi) + bias を構築する
    S(T lo, T hi, T bias = T{}) : lo(lo), hi(hi), bias(bias) {}

    // clamp(x, lo, hi) + bias を返す
    constexpr T operator()(T x) const { return std::clamp(x, lo, hi) + bias; }

    // x -> min(x, v) を返す
    static constexpr S chmin(T v) { return {-infty, v, T{}}; }
    // x -> max(x, v) を返す
    static constexpr S chmax(T v) { return {v, infty, T{}}; }
    // x -> x + v を返す
    static constexpr S add(T v) { return {-infty, infty, v}; }
    // x -> clamp(x, lo, hi) を返す
    static constexpr S clamp(T lo, T hi) { return {lo, hi, T{}}; }
    // x -> v を返す
    static constexpr S set(T v) { return {v, v, T{}}; }
  };

  // f \circ g を返す
  static constexpr S op(const S &f, const S &g)
  {
    return {
        max(min(g.lo + g.bias, f.hi), f.lo) - g.bias,
        min(max(g.hi + g.bias, f.lo), f.hi) - g.bias,
        g.bias + f.bias};
  }

  // 恒等関数を返す
  static constexpr S e() { return {-infty, infty, T{}}; }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(lo, hi, bias);
#endif
