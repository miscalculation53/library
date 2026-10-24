#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../utils/is_integral_ext.hpp"
#include "../utils/integer_arithmetic.hpp"
#include "../utils/larger_int.hpp"

/**
 * @brief 演算時に gcd を使わない有理数
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

  template <class U, enable_if_t<is_constructible_v<T, const U &>, int> = 0>
  explicit Rational(const Rational<U> &x) : num(T(x.num)), den(T(x.den))
  {
    normalize_sign();
  }

  static Rational infty()
  {
    static_assert(is_rational_ordered_v<T>);
    return {T(1), T(0)};
  }

  bool is_infinite() const { return den == T(0); }
  bool is_finite() const { return den != T(0); }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  int sgn() const { return int(T(0) < num) - int(num < T(0)); }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  Rational abs() const { return num < T(0) ? -*this : *this; }

  Rational inv() const
  {
    assert(num != T(0));
    return is_infinite() ? Rational(T(0)) : Rational(den, num);
  }

  template <class U = T, enable_if_t<is_rational_ordered_v<U> && rational_has_mod<U>::value, int> = 0>
  T floor() const
  {
    assert(is_finite());
    T q = num / den;
    return num < T(0) && num % den != T(0) ? T(q - T(1)) : q;
  }

  template <class U = T, enable_if_t<is_rational_ordered_v<U> && rational_has_mod<U>::value, int> = 0>
  T ceil() const
  {
    assert(is_finite());
    T q = num / den;
    return T(0) < num && num % den != T(0) ? T(q + T(1)) : q;
  }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  friend int sgn(const Rational &x) { return x.sgn(); }

  template <class U = T, enable_if_t<is_rational_ordered_v<U>, int> = 0>
  friend Rational abs(const Rational &x) { return x.abs(); }

  friend Rational inv(const Rational &x) { return x.inv(); }

  template <class U = T, enable_if_t<is_rational_ordered_v<U> && rational_has_mod<U>::value, int> = 0>
  friend T floor(const Rational &x) { return x.floor(); }

  template <class U = T, enable_if_t<is_rational_ordered_v<U> && rational_has_mod<U>::value, int> = 0>
  friend T ceil(const Rational &x) { return x.ceil(); }

  pair<T, T> reduced() const
  {
    if constexpr (is_integral_ext<T>)
    {
      T g = T(gcd(unsigned_abs(num), unsigned_abs(den)));
      return g == T(0) ? pair<T, T>{num, den} : pair<T, T>{num / g, den / g};
    }
    else if constexpr (rational_has_mod<T>::value)
    {
      T g = gcd(num, den);
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
    if (rhs.num == T(0))
      return *this;
    if (num == T(0))
      return *this = rhs;
    if (den == rhs.den)
    {
      num += rhs.num;
      return *this;
    }
    T rhs_num = rhs.num, rhs_den = rhs.den;
    if constexpr (is_integral_ext<T> && !is_same_v<T, bool>)
    {
      // 交差積が大きくても、最終的な分子が T に収まれば正確に復元できる。
      using C = unsigned_arithmetic_t<T>;
      num = wrapping_cast<T>(C(num) * C(rhs_den) + C(rhs_num) * C(den));
    }
    else
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
    if (rhs.num == T(0))
      return *this;
    if (num == T(0))
    {
      num = -rhs.num, den = rhs.den;
      return *this;
    }
    if (den == rhs.den)
    {
      num -= rhs.num;
      return *this;
    }
    T rhs_num = rhs.num, rhs_den = rhs.den;
    if constexpr (is_integral_ext<T> && !is_same_v<T, bool>)
    {
      using C = unsigned_arithmetic_t<T>;
      num = wrapping_cast<T>(C(num) * C(rhs_den) - C(rhs_num) * C(den));
    }
    else
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
    if (num == T(0) || rhs.num == T(0))
    {
      num = T(0), den = T(1);
      return *this;
    }
    T rhs_num = rhs.num, rhs_den = rhs.den;
    if (num == rhs_den)
      num = T(1), rhs_den = T(1);
    if (den == rhs_num)
      den = T(1), rhs_num = T(1);
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
    if (num == T(0))
    {
      den = T(1);
      return *this;
    }
    T rhs_num = rhs.num, rhs_den = rhs.den;
    if (num == rhs_num)
      num = T(1), rhs_num = T(1);
    if (den == rhs_den)
      den = T(1), rhs_den = T(1);
    if constexpr (is_integral_ext<T> && !is_same_v<T, bool>)
    {
      using C = unsigned_arithmetic_t<T>;
      C next_num = C(num) * C(rhs_den), next_den = C(den) * C(rhs_num);
      if constexpr (is_signed_ext<T>)
        if (rhs_num < T(0))
          next_num = C(0) - next_num, next_den = C(0) - next_den;
      num = wrapping_cast<T>(next_num), den = wrapping_cast<T>(next_den);
    }
    else
    {
      num *= rhs_den;
      den *= rhs_num;
      normalize_sign();
    }
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
    if (lhs.den == rhs.den)
      return lhs.num == rhs.num;
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
    if (lhs.den == rhs.den)
      return lhs.num < rhs.num;
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
    return ::Rational<T>::infty();
  }
  static constexpr bool is_signed = numeric_limits<T>::is_signed;
  static constexpr bool is_integer = false;
  static constexpr bool is_exact = numeric_limits<T>::is_exact;
};
} // namespace std

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(reduced());
#endif
