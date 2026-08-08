#pragma once

/**
 * @brief テンプレート（型）
 * @docs docs/template/template_types.md
 */

#include <bits/stdc++.h>
using namespace std;

#ifndef EPS
#define EPS 1e-11
#endif
using ld = decltype(EPS);

using ll = long long;
using uint = unsigned int;
using ull = unsigned long long;
using pll = pair<ll, ll>;
using tlll = tuple<ll, ll, ll>;
using tllll = tuple<ll, ll, ll, ll>;

#define vc vector
template <class T>
using vvc = vc<vc<T>>;
template <class T>
using vvvc = vc<vc<vc<T>>>;

using vb = vc<bool>;
using vl = vc<ll>;
using vpll = vc<pll>;
using vtlll = vc<tlll>;
using vtllll = vc<tllll>;
using vstr = vc<string>;
using vvb = vvc<bool>;
using vvl = vvc<ll>;

template <class T>
using pql = priority_queue<T, vc<T>, greater<T>>;
template <class T>
using pqg = priority_queue<T>;

#ifdef __SIZEOF_INT128__
using i128 = __int128_t;
using u128 = __uint128_t;
i128 stoi128(const string &s)
{
  const bool neg = s.front() == '-';
  u128 res = 0;
  for (int i = neg; i < (int)s.size(); i++)
    res = 10 * res + s[i] - '0';
  if (neg)
    return -i128(res - 1) - 1;
  return i128(res);
}
string i128tos(i128 x)
{
  if (x == 0) return "0";
  string sign = "", res = "";
  u128 ux;
  if (x < 0)
    ux = u128(-(x + 1)) + 1, sign = "-";
  else
    ux = x;
  while (ux > 0)
  {
    res += '0' + ux % 10;
    ux /= 10;
  }
  reverse(res.begin(), res.end());
  return sign + res;
}
istream &operator>>(istream &is, i128 &a)
{
  string s;
  is >> s;
  a = stoi128(s);
  return is;
}
ostream &operator<<(ostream &os, const i128 &a)
{
  os << i128tos(a);
  return os;
}
#endif

#define cauto const auto
