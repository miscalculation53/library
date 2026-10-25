#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../utils/larger_int.hpp"
#include "../utils/resolved_value.hpp"

/**
 * @brief clamp・加算と左右の補正距離に重みを付けた費用のモノイド
 * @docs docs/algebra/clamp_add_cost.md
 */

// position(x) = clamp(x, lo, hi) + bias
// score(x) = base + chmax_cost * max(lo-x, 0) + chmin_cost * max(x-hi, 0)
// op(f, g): g の後に f を適用し、score を足す。
template <class T, auto chmax_cost, auto chmin_cost, auto infty = nullptr, class Cost = T>
struct MonoidClampAddCost
{
  using P = larger_int_t<T>;
  using W = larger_int_t<common_type_t<T, Cost>>;
  static constexpr T inf() { return resolved_infty<T, infty>(); }
  static constexpr W chmax_weight() { return resolved_value<W, chmax_cost>(); }
  static constexpr W chmin_weight() { return resolved_value<W, chmin_cost>(); }

  struct S
  {
    T lo, hi, bias;
    Cost base;
    S() {}
    // position(x) = clamp(x, lo, hi) + bias
    // score(x) = base + chmax_cost * max(lo-x, 0) + chmin_cost * max(x-hi, 0)
    constexpr S(T lo, T hi, T bias = T{}, Cost base = Cost{})
        : lo(lo), hi(hi), bias(bias), base(base) {}
    constexpr pair<T, Cost> operator()(T x) const
    {
      const T y = std::clamp(x, lo, hi);
      W cost = W(base);
      if (x < y) cost += chmax_weight() * W(P(y) - P(x));
      if (y < x) cost += chmin_weight() * W(P(x) - P(y));
      return {T(P(y) + P(bias)), Cost(cost)};
    }
    static constexpr S add(T v, Cost base = Cost{}) { return {-inf(), inf(), v, base}; }
    static constexpr S chmin(T v, Cost base = Cost{}) { return {-inf(), v, T{}, base}; }
    static constexpr S chmax(T v, Cost base = Cost{}) { return {v, inf(), T{}, base}; }
    static constexpr S clamp(T lo, T hi, Cost base = Cost{}) { return {lo, hi, T{}, base}; }
    // x を v に移し、左右の補正距離に重みを付けて加算する。
    static constexpr S set(T v, Cost base = Cost{}) { return {v, v, T{}, base}; }
    // position(x) = clamp(x + v, lo, hi)
    // score(x) = base + chmax_cost * max(lo-x-v, 0) + chmin_cost * max(x+v-hi, 0)
    static constexpr S shift_then_clamp(T v, T lo, T hi, Cost base = Cost{})
    {
      return {lo == -inf() ? lo : T(P(lo) - P(v)),
              hi == inf() ? hi : T(P(hi) - P(v)), v, base};
    }
    CPP_DUMP_DEFINE_DATA(lo, hi, bias, base);
  };

  static constexpr S op(const S &f, const S &g)
  {
    // f の区間を g の入力側へ戻して clamp する。
    const T lo = f.lo == -inf() ? g.lo : T(std::clamp(P(f.lo) - P(g.bias), P(g.lo), P(g.hi)));
    const T hi = f.hi == inf() ? g.hi : T(std::clamp(P(f.hi) - P(g.bias), P(g.lo), P(g.hi)));
    P bias;
    W gap;
    if (g.hi != inf() && f.lo != -inf() && P(g.hi) + P(g.bias) < P(f.lo))
    {
      gap = chmax_weight() * W(P(f.lo) - P(g.hi) - P(g.bias));
      bias = P(f.lo) + P(f.bias) - P(g.hi);
    }
    else if (g.lo != -inf() && f.hi != inf() && P(f.hi) < P(g.lo) + P(g.bias))
    {
      gap = chmin_weight() * W(P(g.lo) + P(g.bias) - P(f.hi));
      bias = P(f.hi) + P(f.bias) - P(g.lo);
    }
    else
      bias = P(g.bias) + P(f.bias), gap = W{};
    return {lo, hi, T(bias), Cost(W(g.base) + W(f.base) + gap)};
  }
  static constexpr S e() { return {-inf(), inf(), T{}, Cost{}}; }
};
