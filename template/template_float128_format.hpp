#pragma once

#include "template_float128_math.hpp"

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
}
