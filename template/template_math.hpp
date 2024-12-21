#pragma once

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif
#ifndef EPS
#define EPS 1e-11
#endif

#include "template_types.hpp"
#include "template_rep.hpp"

/**
 * @brief テンプレート（演算）
 * @docs docs/template/template_math.md
 */

inline bool chmin(auto &a, cauto &b) { return a > b ? a = b, true : false; }
inline bool chmax(auto &a, cauto &b) { return a < b ? a = b, true : false; }

template <class T = ll>
inline constexpr T divfloor(cauto &a, cauto &b) { return T(a) / T(b) - (T(a) % T(b) && (T(a) ^ T(b)) < 0); }
template <class T = ll>
inline constexpr T divceil(cauto &a, cauto &b) { return T(a) / T(b) + (T(a) % T(b) && (T(a) ^ T(b)) >= 0); }
template <class T = ll>
inline constexpr T divround(cauto &a, cauto &b) { return divfloor<T>(2 * T(a) + T(b), 2 * T(b)); }
template <class T = ll>
inline constexpr T safemod(cauto &a, cauto &b) { return T(a) - T(b) * divfloor<T>(a, b); }

template <class T = ll>
constexpr T ipow(auto a, auto b)
{
  assert(b >= 0);
  if (b == 0) return 1;
  if (a == 0 || a == 1) return a;
  if (a == -1) return b & 1 ? -1 : 1;

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
template <class T = ll>
T mul_limited(cauto &a, cauto &b, cauto &m = INF)
{
  assert(a >= 0 && b >= 0 && m >= 0);
  if (b == 0)
    return 0;
  return T(a) > T(m) / T(b) ? T(m) : T(a) * T(b);
}
template <class T = ll>
T pow_limited(cauto &a, cauto &b, cauto &m = INF)
{
  assert(a >= 0 && b >= 0 && m >= 0);
  if (a <= 1 || b == 0)
    return min(ipow<T>(a, b), T(m));
  
  T res = 1;
  repi(_, b)
  {
    if (res > T(m) / T(a))
      return T(m);
    res *= T(a);
  }
  return res;
}

template <class T = ll>
constexpr T iroot(cauto &a, cauto &k)
{
  assert(a >= 0 && k >= 1);
  if (a <= 1 || k == 1)
    return a;

  auto isok = [&](const T &x) -> bool
  {
    if (x == 0)
      return true;
    T tmp = 1;
    repi(_, k)
    {
      if (tmp > T(a) / x)
        return false;
      tmp *= x;
    }
    return tmp <= T(a);
  };

  T ok = 0, ng = 1;
  while (isok(ng))
    ok = ng, ng <<= 1;
  while (ng - ok > 1)
  {
    T mid = ((ng - ok) >> 1) + ok;
    if (isok(mid))
      ok = mid;
    else
      ng = mid;
  }
  return ok;
}

// https://misawa.github.io/others/avoid_errors/techniques_to_avoid_errors.html
template <class D = decltype(EPS)>
int sgn(cauto &a, const D &eps = EPS) { return int(a > eps) - int(a < -eps); }

// 位取り記数法と同じ順番（下位桁が後ろ）
template <class T = ll>
vc<T> b_ary(cauto &x, const int &b)
{
  vc<T> a;
  while (x > 0)
  {
    a.emplace_back(x % b);
    x /= b;
  }
  reverse(a.begin(), a.end());
  return a;
}
// 位取り記数法と同じ順番（下位桁が後ろ）
template <class T>
vc<T> b_ary(cauto &x, const int &b, const int &n)
{
  vc<T> a(n);
  repi(i, n)
  {
    a[i] = x % b;
    x /= b;
  }
  reverse(a.begin(), a.end());
  return a;
}
string b_ary_str(cauto &x, const int &b, bool use_upper = true)
{
  auto a = b_ary(x, b);
  string s = "";
  for (cauto &ai : a)
    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));
  return s;
}
string b_ary_str(cauto &x, const int &b, const int &n, bool use_upper = true)
{
  auto a = b_ary(x, b, n);
  string s = "";
  for (cauto &ai : a)
    s += (ai < 10 ? '0' + ai : (use_upper ? 'A' : 'a') + (ai - 10));
  return s;
}