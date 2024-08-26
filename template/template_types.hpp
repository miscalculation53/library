#pragma once

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using dbl = double;
using ld = long double;
using uint = unsigned int;
using ull = unsigned long long;
using pll = pair<ll, ll>;
using tlll = tuple<ll, ll, ll>;
using tllll = tuple<ll, ll, ll, ll>;

#define vc vector
using vl = vc<ll>;
using vpll = vc<pll>;
using vtlll = vc<tlll>;
using vtllll = vc<tllll>;
using vvl = vc<vc<ll>>;

#ifdef __SIZEOF_INT128__
  using i128 = __int128_t;
  i128 stoi128(const string &s)
  {
    i128 res = 0;
    if (s.front() == '-')
    {
      for (int i = 1; i < (int)s.size(); i++)
        res = 10 * res + s[i] - '0';
      res = -res;
    }
    else
    {
      for (auto &&c : s)
        res = 10 * res + c - '0';
    }
    return res;
  }
  string i128tos(i128 x)
  {
    if (x == 0) return "0";
    string sign = "", res = "";
    if (x < 0)
      x = -x, sign = "-";
    while (x > 0)
    {
      res += '0' + x % 10;
      x /= 10;
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