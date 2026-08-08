#pragma once

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif
#ifndef EPS
#define EPS 1e-11
#endif

#include "template_types.hpp"
#include "template_rep.hpp"

#include "../utils/is_integral_ext.hpp"

/**
 * @brief テンプレート（演算）
 * @docs docs/template/template_math.md
 */

template <class T, class U>
inline bool chmin(T &a, U b) { return a > b ? a = b, true : false; }
template <class T, class U>
inline bool chmax(T &a, U b) { return a < b ? a = b, true : false; }

template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T divfloor(U a, V b) { return T(a) / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }
template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T divceil(U a, V b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }
template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T divround(U a, V b) { return divfloor<T>(2 * T(a) + T(b), 2 * T(b)); }
template <class T = ll, class U, class V, typename = enable_if_t<is_integral_ext<U> && is_integral_ext<V>>>
inline constexpr T safemod(U a, V b) { return T(a) - T(b) * divfloor<T>(a, b); }

template <class T = ll, class U, class V>
constexpr T ipow(U a, V b)
{
  assert(b >= 0);
  if (b == 0)
    return 1;
  if (a == 0 || a == 1)
    return a;
  if (a < 0 && a == -1)
    return b & 1 ? -1 : 1;

  T res = 1, tmp = a;
  while (true)
  {
    if (b & 1)
      res *= tmp;
    b >>= 1;
    if (b == 0)
      break;
    tmp *= tmp;
  }
  return res;
}
template <class T = ll, class A, class B, class M>
T mul_limited(A a, B b, M m)
{
  assert(a >= 0 && b >= 0 && m >= 0);
  if (b == 0)
    return 0;
  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);
}
template <class T = ll, class A, class B>
T mul_limited(A a, B b) { return mul_limited<T>(a, b, INF); }
template <class T = ll, class A, class B, class M>
T pow_limited(A a, B b, M m)
{
  assert(a >= 0 && b >= 0 && m >= 0);
  if (a <= 1 || b == 0)
    return min(ipow<T>(a, b), T(m));
  
  T res = 1, tmp = a;
  while (true)
  {
    if (b & 1)
    {
      if (res > T(m) / tmp)
        return m;
      res *= tmp;
    }
    b >>= 1;
    if (b == 0)
      break;
    if (tmp > T(m) / tmp)
      return m;
    tmp *= tmp;
  }
  return res;
}
template <class T = ll, class A, class B>
T pow_limited(A a, B b) { return pow_limited<T>(a, b, INF); }

template <class T = ll, class A, class K>
constexpr T iroot(A a, K k)
{
  assert(a >= 0 && k >= 1);
  if (a <= 1 || k == 1)
    return a;
  if (k == 2)
  {
    const T aa = T(a);
    T x = T(sqrtl((long double)a));
    while (x > aa / x)
      x--;
    while (x < numeric_limits<T>::max())
    {
      const T y = x + 1;
      if (y > aa / y)
        break;
      x = y;
    }
    return x;
  }

  auto isok = [&](T x) -> bool
  {
    if (x == 0)
      return true;
    T res = 1, k2 = k;
    while (true)
    {
      if (k2 & 1)
      {
        if (res > T(a) / x)
          return false;
        res *= x;
      }
      k2 >>= 1;
      if (k2 == 0)
        break;
      if (x > T(a) / x)
        return false;
      x *= x;
    }
    return res <= T(a);
  };

  T x = pow(a, 1.0 / k);
  bool up = true;
  while (!isok(x))
    up = false, x--;
  if (up)
  {
    while (x < numeric_limits<T>::max() && isok(x + 1))
      x++;
  }
  return x;
}
template <class T = ll, class A, class K>
constexpr T iroot_ceil(A a, K k)
{
  T x = iroot<T>(a, k);
  return ipow<T>(x, k) == a ? x : x + 1;
}

// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html
template <class D = decltype(EPS), class A>
int SGN(A a, D eps = EPS) { return int(a > eps) - int(a < -eps); }

// 位取り記数法と同じ順番（下位桁が後ろ）
// 0 に対しては {0} が返る
template <class T = ll, class U, class V>
vc<T> base_repr(U val, V base)
{
  assert(val >= 0);
  assert(base >= 2);
  if (val == 0)
    return {0};
  vc<T> a;
  while (val > 0)
  {
    a.emplace_back(val % base);
    val /= base;
  }
  reverse(a.begin(), a.end());
  return a;
}
// 位取り記数法と同じ順番（下位桁が後ろ）
template <class T = ll, class U, class V>
vc<T> base_repr(U val, V base, int n)
{
  assert(val >= 0);
  assert(base >= 2);
  assert(n >= 0);
  vc<T> a(n);
  repi(i, n)
  {
    a[i] = val % base;
    val /= base;
  }
  reverse(a.begin(), a.end());
  return a;
}
template <const bool use_upper = true, class U>
string base_repr_str(U val, int base)
{
  assert(val >= 0);
  assert(2 <= base && base <= 36);
  auto a = base_repr(val, base);
  string s = "";
  for (cauto &ai : a)
    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));
  return s;
}
template <const bool use_upper = true, class U>
string base_repr_str(U val, int base, int n)
{
  assert(val >= 0);
  assert(2 <= base && base <= 36);
  assert(n >= 0);
  auto a = base_repr(val, base, n);
  string s = "";
  for (cauto &ai : a)
    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));
  return s;
}
