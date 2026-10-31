#pragma once

#include "internal_power.hpp"

#include "../template/template_all_but_modint.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief clamp・加算関数のモノイド
 * @docs docs/algebra/clamp_add.md
 */

// f(x) = clamp(x, lo, hi) + bias
// op(f, g) = f \circ g
template <class T, auto infty = nullptr>
struct MonoidClampAdd
{
  using W = larger_int_t<T>;
  struct S
  {
    T lo, hi, bias;
    S() {}
    // f(x) = clamp(x, lo, hi) + bias
    constexpr S(T lo, T hi, T bias = T{}) : lo(lo), hi(hi), bias(bias) {}
    constexpr T operator()(T x) const { return T(W(std::clamp(x, lo, hi)) + W(bias)); }
    static constexpr S chmin(T v) { return {-resolved_infty<T, infty>(), v, T{}}; }
    static constexpr S chmax(T v) { return {v, resolved_infty<T, infty>(), T{}}; }
    static constexpr S add(T v) { return {-resolved_infty<T, infty>(), resolved_infty<T, infty>(), v}; }
    static constexpr S clamp(T lo, T hi) { return {lo, hi, T{}}; }
    static constexpr S set(T v) { return {v, v, T{}}; }
    // f(x) = clamp(x + v, lo, hi)
    static constexpr S shift_then_clamp(T v, T lo, T hi)
    {
      const T inf = resolved_infty<T, infty>();
      return {lo == -inf ? lo : T(W(lo) - W(v)),
              hi == inf ? hi : T(W(hi) - W(v)), v};
    }
  };
  static constexpr S op(const S &f, const S &g)
  {
    const T inf = resolved_infty<T, infty>();
    const T lo = f.lo == -inf ? g.lo : T(std::clamp(W(f.lo) - W(g.bias), W(g.lo), W(g.hi)));
    const T hi = f.hi == inf ? g.hi : T(std::clamp(W(f.hi) - W(g.bias), W(g.lo), W(g.hi)));
    if (g.hi != inf && f.lo != -inf && W(g.hi) + W(g.bias) < W(f.lo))
      return {lo, hi, T(W(f.lo) + W(f.bias) - W(g.hi))};
    if (g.lo != -inf && f.hi != inf && W(f.hi) < W(g.lo) + W(g.bias))
      return {lo, hi, T(W(f.hi) + W(f.bias) - W(g.lo))};
    return {lo, hi, T(W(g.bias) + W(f.bias))};
  }
  static constexpr S e() { return {-resolved_infty<T, infty>(), resolved_infty<T, infty>(), T{}}; }

  template <class I>
  static constexpr S pow(const S &x, I k)
  {
    internal_monoid_power::check_exponent(k);
    if (k == 0) return e();
    if (k == 1) return x;
    const T inf = resolved_infty<T, infty>();
    S res = x;
    const W d = W(x.bias);
    if (x.lo != -inf && x.hi != inf)
    {
      const W width = W(x.hi) - W(x.lo);
      const W step = d < W{} ? -d : d;
      const W shift = internal_monoid_power::bounded_scale(step, I(k - 1), width);
      if (d < W{})
        res.lo = T(W(x.lo) + shift), res.bias = T(d - shift);
      else
        res.hi = T(W(x.hi) - shift), res.bias = T(d + shift);
    }
    else
    {
      const W shift = internal_monoid_power::scale(d, I(k - 1));
      if (d < W{} && x.lo != -inf) res.lo = T(W(x.lo) - shift);
      if (W{} < d && x.hi != inf) res.hi = T(W(x.hi) - shift);
      res.bias = T(d + shift);
    }
    return res;
  }
};

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(lo, hi, bias);
#endif
