#pragma once

#include "internal_power.hpp"
#include "../utils/resolved_infty.hpp"

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 総和と累積和の最小値・最大値を持つモノイド
 * @docs docs/algebra/prefix_min_max.md
 */

namespace internal_prefix_min_max
{
  template <class T, bool include_empty, auto infty>
  constexpr T min_identity()
  {
    if constexpr (include_empty) return T{};
    else return T(resolved_infty<T, infty>());
  }

  template <class T, bool include_empty, auto infty>
  constexpr T max_identity()
  {
    if constexpr (include_empty) return T{};
    else return T(-resolved_infty<T, infty>());
  }
}

template <class T, bool include_empty, auto infty = nullptr>
struct MonoidPrefixMin
{
  struct S
  {
    T sum{}, mn = internal_prefix_min_max::min_identity<T, include_empty, infty>();
    constexpr S() = default;
    constexpr S(T x) : sum(x), mn(include_empty ? min(T{}, x) : x) {}
    constexpr S(T sum, T mn) : sum(sum), mn(mn) {}
    CPP_DUMP_DEFINE_DATA(sum, mn);
  };
  static constexpr S op(const S &a, const S &b)
  {
    if constexpr (!include_empty)
    {
      if (a.mn == internal_prefix_min_max::min_identity<T, include_empty, infty>()) return b;
      if (b.mn == internal_prefix_min_max::min_identity<T, include_empty, infty>()) return a;
    }
    return {T(a.sum + b.sum), min(a.mn, T(a.sum + b.mn))};
  }
  static constexpr S e() { return {}; }
  template <class I>
  static constexpr S pow(const S &x, I k)
  {
    internal_monoid_power::check_exponent(k);
    if (k == 0) return e();
    if constexpr (!include_empty)
      if (x.mn == internal_prefix_min_max::min_identity<T, include_empty, infty>()) return e();
    const T shift = internal_monoid_power::scale(x.sum, I(k - 1));
    S res;
    res.sum = internal_monoid_power::scale(x.sum, k);
    if constexpr (include_empty) res.mn = min(T{}, T(x.mn + min(T{}, shift)));
    else res.mn = T(x.mn + min(T{}, shift));
    return res;
  }
};

template <class T, bool include_empty, auto infty = nullptr>
struct MonoidPrefixMax
{
  struct S
  {
    T sum{}, mx = internal_prefix_min_max::max_identity<T, include_empty, infty>();
    constexpr S() = default;
    constexpr S(T x) : sum(x), mx(include_empty ? max(T{}, x) : x) {}
    constexpr S(T sum, T mx) : sum(sum), mx(mx) {}
    CPP_DUMP_DEFINE_DATA(sum, mx);
  };
  static constexpr S op(const S &a, const S &b)
  {
    if constexpr (!include_empty)
    {
      if (a.mx == internal_prefix_min_max::max_identity<T, include_empty, infty>()) return b;
      if (b.mx == internal_prefix_min_max::max_identity<T, include_empty, infty>()) return a;
    }
    return {T(a.sum + b.sum), max(a.mx, T(a.sum + b.mx))};
  }
  static constexpr S e() { return {}; }
  template <class I>
  static constexpr S pow(const S &x, I k)
  {
    internal_monoid_power::check_exponent(k);
    if (k == 0) return e();
    if constexpr (!include_empty)
      if (x.mx == internal_prefix_min_max::max_identity<T, include_empty, infty>()) return e();
    const T shift = internal_monoid_power::scale(x.sum, I(k - 1));
    S res;
    res.sum = internal_monoid_power::scale(x.sum, k);
    if constexpr (include_empty) res.mx = max(T{}, T(x.mx + max(T{}, shift)));
    else res.mx = T(x.mx + max(T{}, shift));
    return res;
  }
};

template <class T, bool include_empty, auto infty = nullptr>
struct MonoidPrefixMinMax
{
  struct S
  {
    T sum{}, mn = internal_prefix_min_max::min_identity<T, include_empty, infty>(),
             mx = internal_prefix_min_max::max_identity<T, include_empty, infty>();
    constexpr S() = default;
    constexpr S(T x) : sum(x), mn(include_empty ? min(T{}, x) : x), mx(include_empty ? max(T{}, x) : x) {}
    constexpr S(T sum, T mn, T mx) : sum(sum), mn(mn), mx(mx) {}
    CPP_DUMP_DEFINE_DATA(sum, mn, mx);
  };
  static constexpr S op(const S &a, const S &b)
  {
    if constexpr (!include_empty)
    {
      if (a.mn == internal_prefix_min_max::min_identity<T, include_empty, infty>()) return b;
      if (b.mn == internal_prefix_min_max::min_identity<T, include_empty, infty>()) return a;
    }
    return {T(a.sum + b.sum), min(a.mn, T(a.sum + b.mn)), max(a.mx, T(a.sum + b.mx))};
  }
  static constexpr S e() { return {}; }
  template <class I>
  static constexpr S pow(const S &x, I k)
  {
    internal_monoid_power::check_exponent(k);
    if (k == 0) return e();
    if constexpr (!include_empty)
      if (x.mn == internal_prefix_min_max::min_identity<T, include_empty, infty>()) return e();
    const T shift = internal_monoid_power::scale(x.sum, I(k - 1));
    S res;
    res.sum = internal_monoid_power::scale(x.sum, k);
    if constexpr (include_empty)
    {
      res.mn = min(T{}, T(x.mn + min(T{}, shift)));
      res.mx = max(T{}, T(x.mx + max(T{}, shift)));
    }
    else
    {
      res.mn = T(x.mn + min(T{}, shift));
      res.mx = T(x.mx + max(T{}, shift));
    }
    return res;
  }
};
