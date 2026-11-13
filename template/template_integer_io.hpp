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
inline string u128tos(u128 x)
{
  string res;
  do
  {
    res += char('0' + x % 10);
    x /= 10;
  } while (x > 0);
  reverse(res.begin(), res.end());
  return res;
}
string i128tos(i128 x)
{
  return x < 0 ? "-" + u128tos(u128(-(x + 1)) + 1) : u128tos(u128(x));
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
