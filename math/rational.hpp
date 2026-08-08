#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../utils/is_integral_ext.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief 約分しない有理数
 * @docs docs/math/rational.md
 */

template <class T>
inline constexpr bool is_rational_ordered_v = is_integral_ext<T> || is_floating_point_v<T>;

template <class T>
struct Rational
{
  using value_type = T;

  T num, den;

private:
  void normalize_sign()
  {
    if constexpr (is_signed_ext<T> || is_floating_point_v<T>)
      if (den < T(0))
        num = -num, den = -den;
  }

  static T gcd_abs(T a, T b)
  {
    if constexpr (is_signed_ext<T>)
    {
      if (a < 0) a = -a;
      if (b < 0) b = -b;
    }
    while (b != T(0))
    {
      T r = a % b;
      a = b, b = r;
    }
    return a;
  }

public:
  Rational() : num(0), den(1) {}

  Rational(const T &num) : num(num), den(1) {}

  Rational(const T &num, const T &den) : num(num), den(den)
  {
    assert(den != T(0));
    normalize_sign();
  }

  pair<T, T> reduced() const
  {
    if constexpr (is_integral_ext<T>)
    {
      T g = gcd_abs(num, den);
      return g == T(0) ? pair<T, T>{num, den} : pair<T, T>{num / g, den / g};
    }
    else
      return {num, den};
  }

  Rational operator-() const { return {-num, den}; }

  Rational operator+() const { return *this; }

  Rational &operator+=(const Rational &rhs)
  {
    T rhs_num = rhs.num, rhs_den = rhs.den;
    num = num * rhs_den + rhs_num * den;
    den *= rhs_den;
    normalize_sign();
    return *this;
  }

  Rational &operator-=(const Rational &rhs)
  {
    T rhs_num = rhs.num, rhs_den = rhs.den;
    num = num * rhs_den - rhs_num * den;
    den *= rhs_den;
    normalize_sign();
    return *this;
  }

  Rational &operator*=(const Rational &rhs)
  {
    T rhs_num = rhs.num, rhs_den = rhs.den;
    num *= rhs_num;
    den *= rhs_den;
    normalize_sign();
    return *this;
  }

  Rational &operator/=(const Rational &rhs)
  {
    assert(rhs.num != T(0));
    T rhs_num = rhs.num, rhs_den = rhs.den;
    num *= rhs_den;
    den *= rhs_num;
    normalize_sign();
    return *this;
  }

  friend Rational operator+(Rational lhs, const Rational &rhs) { return lhs += rhs; }

  friend Rational operator-(Rational lhs, const Rational &rhs) { return lhs -= rhs; }

  friend Rational operator*(Rational lhs, const Rational &rhs) { return lhs *= rhs; }

  friend Rational operator/(Rational lhs, const Rational &rhs) { return lhs /= rhs; }

  friend bool operator==(const Rational &lhs, const Rational &rhs)
  {
    using C = larger_int_t<T>;
    return C(lhs.num) * C(rhs.den) == C(rhs.num) * C(lhs.den);
  }

  friend bool operator!=(const Rational &lhs, const Rational &rhs) { return !(lhs == rhs); }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  friend bool operator<(const Rational &lhs, const Rational &rhs)
  {
    using C = larger_int_t<T>;
    return C(lhs.num) * C(rhs.den) < C(rhs.num) * C(lhs.den);
  }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  friend bool operator>(const Rational &lhs, const Rational &rhs) { return rhs < lhs; }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  friend bool operator<=(const Rational &lhs, const Rational &rhs) { return !(rhs < lhs); }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  friend bool operator>=(const Rational &lhs, const Rational &rhs) { return !(lhs < rhs); }

  friend ostream &operator<<(ostream &os, const Rational &x)
  {
    auto [num, den] = x.reduced();
    return os << num << '/' << den;
  }

#if defined FAST_IO && !defined LOCAL
  friend void wt1(const Rational &x)
  {
    auto [num, den] = x.reduced();
    using fastio::wt1;
    wt1(num), wt1('/'), wt1(den);
  }
#endif
};

template <class T>
struct is_rational : false_type
{};

template <class T>
struct is_rational<Rational<T>> : true_type
{};

template <class T>
inline constexpr bool is_rational_v = is_rational<T>::value;

namespace std
{
template <class T>
struct numeric_limits<::Rational<T>> : numeric_limits<T>
{
  static constexpr bool is_specialized = numeric_limits<T>::is_specialized;
  static ::Rational<T> min() noexcept { return ::Rational<T>(numeric_limits<T>::min()); }
  static ::Rational<T> lowest() noexcept { return ::Rational<T>(numeric_limits<T>::lowest()); }
  static ::Rational<T> max() noexcept { return ::Rational<T>(numeric_limits<T>::max()); }
  static constexpr bool is_signed = numeric_limits<T>::is_signed;
  static constexpr bool is_integer = false;
  static constexpr bool is_exact = numeric_limits<T>::is_exact;
};
} // namespace std

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(reduced());
#endif
