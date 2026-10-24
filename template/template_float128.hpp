#pragma once

#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 四倍精度浮動小数点数
 * @docs docs/template/template_types.md
 */

#if __cplusplus > 202002L && defined(__STDCPP_FLOAT128_T__)
#include <stdfloat>
using f128 = std::float128_t;
#elif defined(__SIZEOF_FLOAT128__)
using f128 = __float128;
#else
#error "f128 requires a compiler with IEEE binary128 support, such as GCC on x86_64."
#endif
#define LIBRARY_HAS_FLOAT128 1

#if __cplusplus > 202002L && defined(__STDCPP_FLOAT128_T__) && \
    (defined(_GLIBCXX_HAVE_FLOAT128_MATH) || defined(_GLIBCXX_LDOUBLE_IS_IEEE_BINARY128))
#define LIBRARY_USE_QUADMATH 0
#else
#include <quadmath.h>
#define LIBRARY_USE_QUADMATH 1

//! https://gcc.gnu.org/onlinedocs/libquadmath/Math-Library-Routines.html
#define _p_F128_UNARY(name) \
  inline f128 name(f128 x) { return f128(name##q(__float128(x))); }
_p_F128_UNARY(acos)
_p_F128_UNARY(acosh)
_p_F128_UNARY(asin)
_p_F128_UNARY(asinh)
_p_F128_UNARY(atan)
_p_F128_UNARY(atanh)
_p_F128_UNARY(cbrt)
_p_F128_UNARY(ceil)
_p_F128_UNARY(cos)
_p_F128_UNARY(cosh)
_p_F128_UNARY(erf)
_p_F128_UNARY(erfc)
_p_F128_UNARY(exp)
_p_F128_UNARY(exp2)
_p_F128_UNARY(expm1)
_p_F128_UNARY(fabs)
_p_F128_UNARY(floor)
_p_F128_UNARY(lgamma)
_p_F128_UNARY(log)
_p_F128_UNARY(log10)
_p_F128_UNARY(log1p)
_p_F128_UNARY(log2)
_p_F128_UNARY(logb)
_p_F128_UNARY(nearbyint)
_p_F128_UNARY(rint)
_p_F128_UNARY(round)
_p_F128_UNARY(sin)
_p_F128_UNARY(sinh)
_p_F128_UNARY(sqrt)
_p_F128_UNARY(tan)
_p_F128_UNARY(tanh)
_p_F128_UNARY(tgamma)
_p_F128_UNARY(trunc)
#undef _p_F128_UNARY

template <class T, enable_if_t<is_same_v<T, f128>, int> = 0>
inline f128 abs(T x) { return f128(fabsq(__float128(x))); }

#define _p_F128_BINARY(name) \
  inline f128 name(f128 x, f128 y) { return f128(name##q(__float128(x), __float128(y))); } \
  template <class T, enable_if_t<is_arithmetic_v<T>, int> = 0> \
  inline f128 name(f128 x, T y) { return name(x, f128(y)); } \
  template <class T, enable_if_t<is_arithmetic_v<T>, int> = 0> \
  inline f128 name(T x, f128 y) { return name(f128(x), y); }
_p_F128_BINARY(atan2)
_p_F128_BINARY(copysign)
_p_F128_BINARY(fdim)
_p_F128_BINARY(fmax)
_p_F128_BINARY(fmin)
_p_F128_BINARY(fmod)
_p_F128_BINARY(hypot)
_p_F128_BINARY(nextafter)
_p_F128_BINARY(pow)
_p_F128_BINARY(remainder)
#undef _p_F128_BINARY

inline f128 fma(f128 x, f128 y, f128 z)
{ return f128(fmaq(__float128(x), __float128(y), __float128(z))); }
inline f128 hypot(f128 x, f128 y, f128 z)
{ return hypot(hypot(x, y), z); }
namespace template_float128_detail
{
template <class T>
constexpr bool arithmetic = is_arithmetic_v<T> || is_same_v<T, f128>;
}
#define _p_F128_TERNARY_MIXED(name) \
  template <class T, class U, enable_if_t<template_float128_detail::arithmetic<T> && template_float128_detail::arithmetic<U>, int> = 0> \
  inline f128 name(f128 x, T y, U z) { return name(x, f128(y), f128(z)); } \
  template <class T, class U, enable_if_t<template_float128_detail::arithmetic<T> && !is_same_v<T, f128> && template_float128_detail::arithmetic<U>, int> = 0> \
  inline f128 name(T x, f128 y, U z) { return name(f128(x), y, f128(z)); } \
  template <class T, class U, enable_if_t<template_float128_detail::arithmetic<T> && !is_same_v<T, f128> && template_float128_detail::arithmetic<U> && !is_same_v<U, f128>, int> = 0> \
  inline f128 name(T x, U y, f128 z) { return name(f128(x), f128(y), z); }
_p_F128_TERNARY_MIXED(fma)
_p_F128_TERNARY_MIXED(hypot)
#undef _p_F128_TERNARY_MIXED
inline f128 nexttoward(f128 x, long double y)
{ return f128(nextafterq(__float128(x), __float128(y))); }
inline f128 frexp(f128 x, int *exponent)
{ return f128(frexpq(__float128(x), exponent)); }
inline f128 ldexp(f128 x, int exponent)
{ return f128(ldexpq(__float128(x), exponent)); }
inline f128 scalbn(f128 x, int exponent)
{ return f128(scalbnq(__float128(x), exponent)); }
inline f128 scalbln(f128 x, long exponent)
{ return f128(scalblnq(__float128(x), exponent)); }
inline f128 modf(f128 x, f128 *integer)
{
  __float128 value;
  const auto fraction = modfq(__float128(x), &value);
  *integer = f128(value);
  return f128(fraction);
}
inline f128 remquo(f128 x, f128 y, int *quotient)
{ return f128(remquoq(__float128(x), __float128(y), quotient)); }
inline long lrint(f128 x) { return lrintq(__float128(x)); }
inline long lround(f128 x) { return lroundq(__float128(x)); }
inline long long llrint(f128 x) { return llrintq(__float128(x)); }
inline long long llround(f128 x) { return llroundq(__float128(x)); }
inline int ilogb(f128 x) { return ilogbq(__float128(x)); }
template <class T, enable_if_t<is_same_v<T, f128>, int> = 0>
inline bool isfinite(T x) { return finiteq(__float128(x)); }
template <class T, enable_if_t<is_same_v<T, f128>, int> = 0>
inline bool isinf(T x) { return isinfq(__float128(x)); }
template <class T, enable_if_t<is_same_v<T, f128>, int> = 0>
inline bool isnan(T x) { return isnanq(__float128(x)); }
template <class T, enable_if_t<is_same_v<T, f128>, int> = 0>
inline bool signbit(T x) { return signbitq(__float128(x)); }
template <class T, enable_if_t<is_same_v<T, f128>, int> = 0>
inline bool isnormal(T x)
{ return isfinite(x) && abs(x) >= numeric_limits<f128>::min(); }
template <class T, enable_if_t<is_same_v<T, f128>, int> = 0>
inline int fpclassify(T x)
{
  if (isnan(x)) return FP_NAN;
  if (isinf(x)) return FP_INFINITE;
  if (x == 0) return FP_ZERO;
  return isnormal(x) ? FP_NORMAL : FP_SUBNORMAL;
}
#endif

namespace template_float128_detail
{
inline bool parse(string_view s, f128 &x)
{
  if (!s.empty() && s.front() == '+')
  {
    s.remove_prefix(1);
    if (s.empty() || s.front() == '-') return false;
  }
#if LIBRARY_USE_QUADMATH
  string token(s);
  char *end;
  errno = 0;
  const __float128 value = strtoflt128(token.c_str(), &end);
  if (end == token.data() || end != token.data() + token.size()) return false;
  if (errno == ERANGE && (value == 0 || isinfq(value))) return false;
  x = f128(value);
  return true;
#else
  auto [p, ec] = from_chars(s.data(), s.data() + s.size(), x);
  return ec == errc{} && p == s.data() + s.size();
#endif
}

inline string format(f128 x, chars_format style, int precision)
{
  string s(128, '\0');
  while (true)
  {
#if LIBRARY_USE_QUADMATH
    const char *spec = style == chars_format::fixed ? "%.*Qf" :
                       style == chars_format::scientific ? "%.*Qe" : "%.*Qg";
    const int n = quadmath_snprintf(s.data(), s.size(), spec, precision, __float128(x));
    if (n < 0) abort();
    if (size_t(n) < s.size())
    {
      s.resize(size_t(n));
      return s;
    }
    s.resize(size_t(n) + 1);
#else
    auto [p, ec] = to_chars(s.data(), s.data() + s.size(), x, style, precision);
    if (ec == errc{})
    {
      s.resize(size_t(p - s.data()));
      return s;
    }
    s.resize(s.size() * 2);
#endif
  }
}
} // namespace template_float128_detail
