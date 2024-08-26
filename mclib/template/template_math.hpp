#pragma once

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

#include "template_types.hpp"
#include "template_rep.hpp"

bool chmin(auto &a, const auto &b) { return a > b ? a = b, true : false; }
bool chmax(auto &a, const auto &b) { return a < b ? a = b, true : false; }

template <class T1 = ll>
T1 safemod(auto a, auto m)
{
  T1 res = a % m;
  if (res < 0) res += m;
  return res;
}
template <class T1 = ll>
T1 divfloor(auto a, auto b)
{ 
  if (b < 0) a = -a, b = -b;
  return (a - safemod(a, b)) / b; 
}
template <class T1 = ll>
T1 divceil(auto a, auto b)
{ 
  if (b < 0) a = -a, b = -b;
  return divfloor(a + b - 1, b);
}

template <class T1 = ll>
T1 pow_ll(auto a, auto b)
{ 
  if (a == 0 || a == 1) return a;
  if (a == -1) return b & 1 ? -1 : 1;
  ll res = 1; repi(_, b) res *= a; return res;
}
template <class T1 = ll>
T1 mul_limited(auto a, auto b, T1 m = INF)
{
  if (b == 0) return 0;
  return a > m / b ? m : a * b;
}
template <class T1 = ll>
T1 pow_limited(auto a, auto b, T1 m = INF)
{
  if (a == 0 || a == 1) return a;
  ll res = 1;
  repi(_, b)
  { 
    if (res > m / a) return m;
    res *= a;
  }
  return res;
}