#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 区間の長さ・和・最小値・最大値を持つモノイド
 * @docs docs/algebra/min_max_sum.md
 */

template <class T>
struct MonoidLenSum
{
  struct S
  {
    int len;
    T val;
    S() {}
    S(T val) : len(1), val(val) {}
    S(int len, T val) : len(len), val(val) {}
    CPP_DUMP_DEFINE_DATA(len, val);
  };
  static constexpr S op(const S &a, const S &b) { return {a.len + b.len, a.val + b.val}; }
  static constexpr S e() { return {0, T{}}; }
};

template <class T, auto infty = INF>
struct MonoidMinMax
{
  struct S
  {
    T mn = resolved_value<T, infty>(), mx = -resolved_value<T, infty>();
    S() {}
    S(T x) : mn(x), mx(x) {}
    S(T mn, T mx) : mn(mn), mx(mx) {}
    CPP_DUMP_DEFINE_DATA(mn, mx);
  };
  static constexpr S op(const S &a, const S &b) { return {min(a.mn, b.mn), max(a.mx, b.mx)}; }
  static constexpr S e() { return {}; }
};

template <class T, auto infty = INF>
struct MonoidMinSum
{
  struct S
  {
    T mn = resolved_value<T, infty>(), sum = T{};
    int len = 0;
    S() {}
    S(T x, int len = 1) : mn(x), sum(x * len), len(len) {}
    S(T mn, T sum, int len) : mn(mn), sum(sum), len(len) {}
    CPP_DUMP_DEFINE_DATA(mn, sum, len);
  };
  static constexpr S op(const S &a, const S &b)
  {
    return {min(a.mn, b.mn), a.sum + b.sum, a.len + b.len};
  }
  static constexpr S e() { return {}; }
};

template <class T, auto infty = INF>
struct MonoidMaxSum
{
  struct S
  {
    T mx = -resolved_value<T, infty>(), sum = T{};
    int len = 0;
    S() {}
    S(T x, int len = 1) : mx(x), sum(x * len), len(len) {}
    S(T mx, T sum, int len) : mx(mx), sum(sum), len(len) {}
    CPP_DUMP_DEFINE_DATA(mx, sum, len);
  };
  static constexpr S op(const S &a, const S &b)
  {
    return {max(a.mx, b.mx), a.sum + b.sum, a.len + b.len};
  }
  static constexpr S e() { return {}; }
};

template <class T, auto infty = INF>
struct MonoidMinMaxSum
{
  struct S
  {
    T mn = resolved_value<T, infty>(), mx = -resolved_value<T, infty>(), sum = T{};
    int len = 0;
    S() {}
    S(T x, int len = 1) : mn(x), mx(x), sum(x * len), len(len) {}
    S(T mn, T mx, T sum, int len) : mn(mn), mx(mx), sum(sum), len(len) {}
    CPP_DUMP_DEFINE_DATA(mn, mx, sum, len);
  };
  static constexpr S op(const S &a, const S &b)
  {
    return {min(a.mn, b.mn), max(a.mx, b.mx), a.sum + b.sum, a.len + b.len};
  }
  static constexpr S e() { return {}; }
};
