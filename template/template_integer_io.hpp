#pragma once

#include "template_types_base.hpp"

#ifdef __SIZEOF_INT128__
i128 stoi128(const string &s)
{
  const bool neg = s.front() == '-';
  u128 res = 0;
  for (size_t i = size_t(neg); i < s.size(); i++)
    res = 10 * res + unsigned(s[i] - '0');
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
    ux = u128(x);
  while (ux > 0)
  {
    res += char('0' + ux % 10);
    ux /= 10;
  }
  reverse(res.begin(), res.end());
  return sign + res;
}
istream &operator>>(istream &is, i128 &a)
{
  string s;
  if (!(is >> s)) return is;
  a = stoi128(s);
  return is;
}
ostream &operator<<(ostream &os, const i128 &a)
{
  os << i128tos(a);
  return os;
}
#endif
