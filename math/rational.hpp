#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../utils/is_integral_ext.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief 約分しない有理数
 * @docs docs/math/rational.md
 */

template <class T, class = void>
struct is_rational_ordered : false_type
{};

template <class T>
struct is_rational_ordered<T, void_t<decltype(declval<const T &>() < declval<const T &>())>> : true_type
{};

template <class T>
inline constexpr bool is_rational_ordered_v = is_rational_ordered<T>::value;

template <class T, class = void>
struct rational_has_mod : false_type
{};

template <class T>
struct rational_has_mod<T, void_t<decltype(declval<T>() % declval<T>())>> : true_type
{};

template <class T>
struct Rational
{
  using value_type = T;

  T num, den;

private:
  void normalize_sign()
  {
    if constexpr (is_rational_ordered_v<T>)
    {
      if (den == T(0))
      {
        assert(num != T(0));
        num = num < T(0) ? T(-1) : T(1);
      }
      else if (den < T(0))
        num = -num, den = -den;
    }
    else
      assert(den != T(0));
  }

  static T gcd_abs(T a, T b)
  {
    if constexpr (is_rational_ordered_v<T>)
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

  static T unit_sign(const T &x)
  {
    assert(x != T(0));
    return x < T(0) ? T(-1) : T(1);
  }

public:
  Rational() : num(0), den(1) {}

  Rational(const T &num) : num(num), den(1) {}

  template <class U, enable_if_t<is_integral_ext<U> && !is_same_v<decay_t<U>, T>, int> = 0>
  Rational(U num) : num(num), den(1) {}

  Rational(const T &num, const T &den) : num(num), den(den)
  {
    normalize_sign();
  }

  bool is_infinite() const { return den == T(0); }
  bool is_finite() const { return den != T(0); }

  pair<T, T> reduced() const
  {
    if constexpr (rational_has_mod<T>::value)
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
    if constexpr (is_rational_ordered_v<T>)
    {
      if (is_infinite() || rhs.is_infinite())
      {
        if (is_infinite() && rhs.is_infinite())
          assert(num == rhs.num);
        else if (rhs.is_infinite())
          num = rhs.num, den = rhs.den;
        return *this;
      }
    }
    T rhs_num = rhs.num, rhs_den = rhs.den;
    num = num * rhs_den + rhs_num * den;
    den *= rhs_den;
    normalize_sign();
    return *this;
  }

  Rational &operator-=(const Rational &rhs)
  {
    if constexpr (is_rational_ordered_v<T>)
    {
      if (is_infinite() || rhs.is_infinite())
      {
        if (is_infinite() && rhs.is_infinite())
          assert(num != rhs.num);
        else if (rhs.is_infinite())
          num = -rhs.num, den = rhs.den;
        return *this;
      }
    }
    T rhs_num = rhs.num, rhs_den = rhs.den;
    num = num * rhs_den - rhs_num * den;
    den *= rhs_den;
    normalize_sign();
    return *this;
  }

  Rational &operator*=(const Rational &rhs)
  {
    if constexpr (is_rational_ordered_v<T>)
    {
      if (is_infinite() || rhs.is_infinite())
      {
        if (is_infinite() && rhs.is_infinite())
          num *= rhs.num;
        else if (is_infinite())
        {
          assert(rhs.num != T(0));
          num *= unit_sign(rhs.num);
        }
        else
        {
          assert(num != T(0));
          num = unit_sign(num) * rhs.num;
          den = T(0);
        }
        return *this;
      }
    }
    T rhs_num = rhs.num, rhs_den = rhs.den;
    num *= rhs_num;
    den *= rhs_den;
    normalize_sign();
    return *this;
  }

  Rational &operator/=(const Rational &rhs)
  {
    assert(rhs.num != T(0));
    if constexpr (is_rational_ordered_v<T>)
    {
      if (is_infinite() || rhs.is_infinite())
      {
        if (is_infinite() && rhs.is_infinite())
        {
          assert(false && "infinity / infinity is undefined");
          return *this;
        }
        if (is_infinite())
          num *= unit_sign(rhs.num);
        else
          num = T(0), den = T(1);
        return *this;
      }
    }
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
    if (lhs.is_infinite() || rhs.is_infinite())
      return lhs.is_infinite() && rhs.is_infinite() && lhs.num == rhs.num;
    using C = larger_int_t<T>;
    return C(lhs.num) * C(rhs.den) == C(rhs.num) * C(lhs.den);
  }

  friend bool operator!=(const Rational &lhs, const Rational &rhs) { return !(lhs == rhs); }

  friend auto safe_hash_key(const Rational &x)
  {
    if constexpr (rational_has_mod<T>::value)
      return x.reduced();
    else
      return x.num / x.den;
  }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  friend bool operator<(const Rational &lhs, const Rational &rhs)
  {
    if (lhs.is_infinite() || rhs.is_infinite())
    {
      if (lhs.is_infinite() && rhs.is_infinite())
        return lhs.num < rhs.num;
      if (lhs.is_infinite())
        return lhs.num < T(0);
      return T(0) < rhs.num;
    }
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
  static constexpr bool has_infinity = ::is_rational_ordered_v<T>;
  static ::Rational<T> infinity() noexcept
  {
    static_assert(::is_rational_ordered_v<T>);
    return ::Rational<T>(T(1), T(0));
  }
  static constexpr bool is_signed = numeric_limits<T>::is_signed;
  static constexpr bool is_integer = false;
  static constexpr bool is_exact = numeric_limits<T>::is_exact;
};
} // namespace std

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(reduced());
#endif
