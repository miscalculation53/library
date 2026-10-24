#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../utils/make_unsigned_ext.hpp"
#include "algebra_base.hpp"

/**
 * @brief 区間の長さ・和・最小値・最大値を持つモノイド
 * @docs docs/algebra/min_max_sum.md
 */

template <class T, class L = ll>
struct MonoidLenSum
{
  using UL = make_unsigned_ext_t<common_type_t<L, ull>>;
  struct S
  {
    L len;
    T val;
    S() {}
    S(T val) : len(1), val(val) {}
    S(L len, T val) : len(len), val(val) {}
    CPP_DUMP_DEFINE_DATA(len, val);
  };
  static constexpr S op(const S &a, const S &b) { return {L(a.len + b.len), a.val + b.val}; }
  static constexpr S e() { return {0, T{}}; }
  static constexpr S pow(const S &x, ll k)
  {
    assert(k >= 0);
    return k == 0 ? e() : S(L(x.len * UL(k)), T(x.val * UL(k)));
  }
};

template <class T, auto infty = nullptr>
struct MonoidMinMax
{
  struct S
  {
    T mn = resolved_infty<T, infty>(), mx = -resolved_infty<T, infty>();
    S() {}
    S(T x) : mn(x), mx(x) {}
    S(T mn, T mx) : mn(mn), mx(mx) {}
    CPP_DUMP_DEFINE_DATA(mn, mx);
  };
  static constexpr S op(const S &a, const S &b) { return {min(a.mn, b.mn), max(a.mx, b.mx)}; }
  static constexpr S e() { return {}; }
};

template <class T, auto infty = nullptr, class L = ll>
struct MonoidMinSum
{
  using UL = make_unsigned_ext_t<common_type_t<L, ull>>;
  struct S
  {
    T mn = resolved_infty<T, infty>(), sum = T{};
    L len = 0;
    S() {}
    S(T x, L len = 1) : mn(x), sum(x * UL(len)), len(len) {}
    S(T mn, T sum, L len) : mn(mn), sum(sum), len(len) {}
    CPP_DUMP_DEFINE_DATA(mn, sum, len);
  };
  static constexpr S op(const S &a, const S &b)
  {
    return {min(a.mn, b.mn), a.sum + b.sum, L(a.len + b.len)};
  }
  static constexpr S e() { return {}; }
};

template <class T, auto infty = nullptr, class L = ll>
struct MonoidMaxSum
{
  using UL = make_unsigned_ext_t<common_type_t<L, ull>>;
  struct S
  {
    T mx = -resolved_infty<T, infty>(), sum = T{};
    L len = 0;
    S() {}
    S(T x, L len = 1) : mx(x), sum(x * UL(len)), len(len) {}
    S(T mx, T sum, L len) : mx(mx), sum(sum), len(len) {}
    CPP_DUMP_DEFINE_DATA(mx, sum, len);
  };
  static constexpr S op(const S &a, const S &b)
  {
    return {max(a.mx, b.mx), a.sum + b.sum, L(a.len + b.len)};
  }
  static constexpr S e() { return {}; }
};

template <class T, auto infty = nullptr, class L = ll>
struct MonoidMinMaxSum
{
  using UL = make_unsigned_ext_t<common_type_t<L, ull>>;
  struct S
  {
    T mn = resolved_infty<T, infty>(), mx = -resolved_infty<T, infty>(), sum = T{};
    L len = 0;
    S() {}
    S(T x, L len = 1) : mn(x), mx(x), sum(x * UL(len)), len(len) {}
    S(T mn, T mx, T sum, L len) : mn(mn), mx(mx), sum(sum), len(len) {}
    CPP_DUMP_DEFINE_DATA(mn, mx, sum, len);
  };
  static constexpr S op(const S &a, const S &b)
  {
    return {min(a.mn, b.mn), max(a.mx, b.mx), a.sum + b.sum, L(a.len + b.len)};
  }
  static constexpr S e() { return {}; }
};
