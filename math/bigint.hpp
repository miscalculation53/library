#pragma once

#include "../template/template_all_but_modint.hpp"

#include "math/convolution/convolution_ll.hpp"
#include "utils/is_integral_ext.hpp"
#include "utils/make_unsigned_ext.hpp"

/**
 * @brief 多倍長整数
 * @docs docs/math/bigint.md
 */

// 桁数の制限
// FFT ができる: 2^24 ≒ 1.67 * 10^7 桁
// CRT が壊れない: (3 * 10^18) * d / b^{2d} 桁
// (b, d) = (10, 6) で 1.80 * 10^7
// (b, d) = (16, 5) で 1.36 * 10^7
// (b, d) = (2, 21) で 1.43 * 10^7
template <int base = 10, int digit = 6>
struct BigInteger
{
private:
  static constexpr int BASE = ipow(base, digit);
  vl vec;
  bool is_nega = false;

  static int char_to_digit(char c)
  {
    if ('0' <= c && c <= '9')
      return c - '0';
    if ('a' <= c && c <= 'z')
      return c - 'a' + 10;
    return c - 'A' + 10;
  }
  static char digit_to_char(int x) { return x < 10 ? char('0' + x) : char('A' + x - 10); }

  void zero_suppress()
  {
    while (!vec.empty() && vec.back() == 0)
      vec.pop_back();
    if (vec.empty())
      is_nega = false;
  }
  void carry(int d = -1)
  {
    const int n = vec.size();
    if (n == 0)
      return;
    if (d < 0)
      d = n - 1;
    repi(i, d)
    {
      vec[i + 1] += vec[i] / BASE;
      vec[i] = vec[i] % BASE;
    }
    for (int i = d; !(0 <= vec[i] && vec[i] < BASE); i++)
    {
      vec.eb(vec[i] / BASE);
      vec[i] = vec[i] % BASE;
    }
  }
  void borrow(int si, int mx)
  {
    ll bor = 0;
    repi(i, si, vec.size())
    {
      ll val = vec[i] - bor;
      if (val < 0)
      {
        bor = (-val + BASE - 1) / BASE;
        vec[i] = val + bor * BASE;
      }
      else
      {
        vec[i] = val;
        bor = 0;
        if (i >= mx)
          break;
      }
    }
    if (bor > 0)
    {
      is_nega ^= 1;
      ll car = 1;
      repi(i, vec.size())
      {
        ll val = (BASE - 1) - vec[i] + car;
        if (val >= BASE)
          vec[i] = val - BASE, car = 1;
        else
          vec[i] = val, car = 0;
      }
    }
    zero_suppress();
    if (vec.empty())
      is_nega = false;
  }

  BigInteger &operator<<=(size_t k)
  {
    vec.insert(vec.begin(), k, 0);
    return *this;
  }
  BigInteger &operator>>=(size_t k)
  {
    vec.erase(vec.begin(), vec.begin() + min(vec.size(), k));
    return *this;
  }
  BigInteger operator<<(size_t k) const { return BigInteger(*this) <<= k; }
  BigInteger operator>>(size_t k) const { return BigInteger(*this) >>= k; }

  pair<BigInteger, int> divmod_digit(int d) const
  {
    BigInteger q = *this;
    ll rem = 0;
    repi(i, SZ(q.vec) - 1, -1, -1)
    {
      ll cur = q.vec[i] + rem * BASE;
      q.vec[i] = cur / d;
      rem = cur % d;
    }
    q.zero_suppress();
    return {q, int(rem)};
  }

  pair<BigInteger, BigInteger> divmod_knuth(const BigInteger &b) const
  {
    BigInteger q, r;
    vl u = vec, v = b.vec;
    const ll d = BASE / (v.back() + 1);
    auto normalize = [&](vl &a)
    {
      ll car = 0;
      fem(x : a)
      {
        ll cur = x * d + car;
        x = cur % BASE, car = cur / BASE;
      }
      if (car) a.eb(car);
    };
    normalize(u), normalize(v);
    const int n = v.size();
    u.eb(0);
    const int m = SZ(u) - n - 1;
    q.vec.resize(m + 1);
    repi(j, m, -1, -1)
    {
      ll cur = u[j + n] * BASE + u[j + n - 1];
      ll x = cur / v[n - 1], rem = cur % v[n - 1];
      if (x == BASE)
        x--, rem += v[n - 1];
      while (n >= 2 && rem < BASE && x * v[n - 2] > rem * BASE + u[j + n - 2])
        x--, rem += v[n - 1];

      ll bor = 0;
      repi(i, n)
      {
        ll z = x * v[i] + bor;
        bor = z / BASE, z %= BASE;
        if (u[j + i] < z)
          u[j + i] += BASE, bor++;
        u[j + i] -= z;
      }
      u[j + n] -= bor;
      if (u[j + n] < 0)
      {
        x--;
        ll car = 0;
        repi(i, n)
        {
          ll z = u[j + i] + v[i] + car;
          u[j + i] = z % BASE, car = z / BASE;
        }
        u[j + n] += car;
      }
      q.vec[j] = x;
    }

    r.vec.assign(u.begin(), u.begin() + n);
    ll rem = 0;
    repi(i, n - 1, -1, -1)
    {
      ll cur = r.vec[i] + rem * BASE;
      r.vec[i] = cur / d, rem = cur % d;
    }
    q.zero_suppress(), r.zero_suppress();
    return {q, r};
  }

  pair<BigInteger, BigInteger> divmod_abs(const BigInteger &b) const
  {
    BigInteger a = abs(), bb = b.abs();
    if (a < bb)
      return {0, a};
    ll work = ll(bb.vec.size()) * (SZ(a.vec) - SZ(bb.vec) + 1);
    if (work <= 600'000)
      return a.divmod_knuth(bb);
    BigInteger ib = bb.inv(a.vec.size());
    BigInteger q = (a * ib) >> (SZ(a.vec) + SZ(bb.vec) - 1);
    BigInteger r = a - bb * q;
    assert(r >= 0);
    if (r >= bb)
      q += 1, r -= bb;
    return {q, r};
  }

  int cmp(const BigInteger &b) const
  {
    if (is_nega ^ b.is_nega)
      return is_nega ? -1 : 1;
    if (vec.size() != b.vec.size())
      return ((vec.size() < b.vec.size()) ^ is_nega) ? -1 : 1;
    repi(i, SZ(vec) - 1, -1, -1)
    {
      if (vec[i] != b.vec[i])
        return ((vec[i] < b.vec[i]) ^ is_nega) ? -1 : 1;
    }
    return 0;
  }

  template <class T>
  void add_abs(int q, T val)
  {
    if (q >= SZ(vec))
      vec.resize(q + 1);
    int i = q;
    while (val > 0)
    {
      if (i >= SZ(vec))
        vec.eb(0);
      val += vec[i];
      vec[i] = ll(val % BASE);
      val /= BASE;
      i++;
    }
  }
  template <class T>
  void sub_abs(int q, T val)
  {
    if (q >= SZ(vec))
      vec.resize(q + 1);
    int i = q;
    while (val > 0)
    {
      if (i >= SZ(vec))
        vec.eb(0);
      vec[i] -= ll(val % BASE);
      val /= BASE;
      i++;
    }
    borrow(q, i - 1);
  }

public:
  BigInteger() {}
  BigInteger(const string &s)
  {
    int len = s.size(), offs = 0;
    if (!s.empty() && s[0] == '-')
    {
      is_nega = true;
      len--;
      offs++;
    }
    else
      is_nega = false;
    vec.resize(divceil(len, digit));
    repi(i, vec.size())
    {
      int r = len - i * digit;
      int l = max(0, r - digit);
      ll x = 0;
      repi(j, l, r)
      {
        int d = char_to_digit(s[offs + j]);
        assert(0 <= d && d < base);
        x = x * base + d;
      }
      vec[i] = x;
    }
    zero_suppress();
  }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger(T x)
  {
    using U = make_unsigned_ext_t<T>;
    U ux = x;
    if constexpr (is_signed_ext<T>)
    {
      if (x < 0)
        is_nega = true, ux = -ux;
      else
        is_nega = false;
    }
    else
      is_nega = false;
    
    if (ux == 0)
      return;
    while (ux > 0)
    {
      vec.eb((ll)(ux % BASE));
      ux /= BASE;
    }
  }

  string to_string() const
  {
    const int n = vec.size();
    if (n == 0)
      return "0";
    string s;
    s.reserve(size_t(n) * digit + is_nega);
    if (is_nega)
      s += '-';
    ll x = vec.back();
    char buf[digit];
    int len = 0;
    do
    {
      buf[len++] = digit_to_char(x % base);
      x /= base;
    } while (x);
    repi(i, len - 1, -1, -1) s += buf[i];
    repi(i, SZ(vec) - 2, -1, -1)
    {
      size_t p = s.size();
      s.resize(p + digit);
      x = vec[i];
      repi(j, digit - 1, -1, -1)
      {
        s[p + j] = digit_to_char(x % base);
        x /= base;
      }
    }
    return s;
  }

  bool operator<(const BigInteger &b) const { return cmp(b) < 0; }
  bool operator>(const BigInteger &b) const { return cmp(b) > 0; }
  bool operator<=(const BigInteger &b) const { return cmp(b) <= 0; }
  bool operator>=(const BigInteger &b) const { return cmp(b) >= 0; }
  bool operator==(const BigInteger &b) const { return is_nega == b.is_nega && vec == b.vec; }
  bool operator!=(const BigInteger &b) const { return !(*this == b); }
  friend auto safe_hash_key(const BigInteger &x) { return tie(x.is_nega, x.vec); }

  BigInteger operator-() const
  {
    BigInteger res(*this);
    if (!res.vec.empty())
      res.is_nega ^= 1;
    return res;
  }
  BigInteger abs() const
  {
    BigInteger res(*this);
    res.is_nega = false;
    return res;
  }
  BigInteger &operator+=(const BigInteger &b)
  {
    if (b.vec.empty())
      return *this;
    if (vec.size() < b.vec.size())
      vec.resize(b.vec.size());
    if (is_nega == b.is_nega)
    {
      repi(i, b.vec.size()) vec[i] += b.vec[i];
      carry();
    }
    else
    {
      repi(i, b.vec.size()) vec[i] -= b.vec[i];
      borrow(0, SZ(b.vec) - 1);
    }
    return *this;
  }
  BigInteger &operator-=(const BigInteger &b)
  {
    if (b.vec.empty())
      return *this;
    if (vec.size() < b.vec.size())
      vec.resize(b.vec.size());
    if (is_nega != b.is_nega)
    {
      repi(i, b.vec.size()) vec[i] += b.vec[i];
      carry();
    }
    else
    {
      repi(i, b.vec.size()) vec[i] -= b.vec[i];
      borrow(0, SZ(b.vec) - 1);
    }
    return *this;
  }
  BigInteger &operator*=(const BigInteger &b)
  {
    if (this->vec.empty() || b.vec.empty())
    {
      this->vec.clear();
      this->is_nega = false;
      return *this;
    }
    bool neg = is_nega ^ b.is_nega;
    if (b.vec.size() == 1)
    {
      *this *= int(b.vec[0]);
      is_nega = neg;
      return *this;
    }
    if (vec.size() == 1)
    {
      int v = vec[0];
      *this = b;
      *this *= v;
      is_nega = neg;
      return *this;
    }
    vec = convolution_4e18(this->vec, b.vec);
    carry();
    zero_suppress();
    is_nega = neg;
    return *this;
  }
  BigInteger operator+(const BigInteger &b) const { return BigInteger(*this) += b; }
  BigInteger operator-(const BigInteger &b) const { return BigInteger(*this) -= b; }
  BigInteger operator*(const BigInteger &b) const { return BigInteger(*this) *= b; }
  BigInteger inv(int d) const
  {
    assert(!vec.empty());
    BigInteger a(abs()), c, c2;
    BigInteger b = binsearch(LMD(m, ((a * m).vec.size() <= a.vec.size())), 0, BASE + 1, false, false).first;
    const BigInteger ONE(1), TWO(2);
    for (int k = 1;; k = min(2 * k, d))
    {
      c = a * b;
      if (SZ(b.vec) >= d + 1 && SZ(c.vec) == SZ(a.vec) + SZ(b.vec) - 1)
      {
        c2 = c + a;
        if (c2.vec.size() == a.vec.size() + b.vec.size())
        {
          if (c2 == ONE << (SZ(c2.vec) - 1))
            b += 1;
          break;
        }
      }
      b *= (TWO << (SZ(a.vec) + SZ(b.vec) - 1)) - c;
      if (SZ(b.vec) >= k + 1)
        b >>= SZ(b.vec) - k - 1;
    }
    b >>= 1;
    if (is_nega)
      b = -b;
    return b;
  }
  BigInteger operator/(const BigInteger &b) const
  {
    assert(!b.vec.empty());
    if (b.vec.size() == 1)
    {
      BigInteger q = divmod_digit(int(b.vec[0])).first;
      q.is_nega = !q.vec.empty() && (is_nega ^ b.is_nega);
      return q;
    }
    BigInteger q = divmod_abs(b).first;
    q.is_nega = !q.vec.empty() && (is_nega ^ b.is_nega);
    return q;
  }
  pair<BigInteger, BigInteger> divmod(const BigInteger &b) const
  {
    assert(!b.vec.empty());
    if (b.vec.size() == 1)
    {
      auto [q, rem] = divmod_digit(int(b.vec[0]));
      q.is_nega = !q.vec.empty() && (is_nega ^ b.is_nega);
      BigInteger r(rem);
      r.is_nega = rem && is_nega;
      return {q, r};
    }
    auto [q, r] = divmod_abs(b);
    q.is_nega = !q.vec.empty() && (is_nega ^ b.is_nega);
    r.is_nega = !r.vec.empty() && is_nega;
    return {q, r};
  }
  BigInteger operator%(const BigInteger &b) const { return divmod(b).second; }
  BigInteger &operator/=(const BigInteger &b) { return *this = *this / b; }
  BigInteger &operator%=(const BigInteger &b) { return *this = *this % b; }

  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger &operator+=(T v)
  {
    if (v == 0)
      return *this;
    bool v_nega = false;
    using U = make_unsigned_ext_t<T>;
    U uv = v;
    if constexpr (is_signed_ext<T>)
    {
      if (v < 0)
        v_nega = true, uv = -uv;
    }
    using V = larger_int_t<U>;
    if (is_nega == v_nega)
      add_abs(0, (V)uv);
    else
      sub_abs(0, (V)uv);
    return *this;
  }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger &operator-=(T v)
  {
    if (v == 0)
      return *this;
    bool v_nega = false;
    using U = make_unsigned_ext_t<T>;
    U uv = v;
    if constexpr (is_signed_ext<T>)
    {
      if (v < 0)
        v_nega = true, uv = -uv;
    }
    using V = larger_int_t<U>;
    if (is_nega != v_nega)
      add_abs(0, (V)uv);
    else
      sub_abs(0, (V)uv);
    return *this;
  }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger &operator*=(T v)
  {
    if (vec.empty() || v == 0)
    {
      vec.clear();
      is_nega = false;
      return *this;
    }
    bool v_nega = false;
    using U = make_unsigned_ext_t<T>;
    U uv = v;
    if constexpr (is_signed_ext<T>)
    {
      if (v < 0)
        v_nega = true, uv = -uv;
    }
    is_nega ^= v_nega;
    using V = larger_int_t<U>;
    V car = 0;
    repi(i, SZ(vec))
    {
      car += (V)vec[i] * uv;
      vec[i] = (ll)(car % BASE);
      car /= BASE;
    }
    while (car > 0)
    {
      vec.eb((ll)(car % BASE));
      car /= BASE;
    }
    return *this;
  }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger &operator/=(T v)
  {
    assert(v != 0);
    if (vec.empty())
      return *this;
    bool v_nega = false;
    using U = make_unsigned_ext_t<T>;
    U uv = v;
    if constexpr (is_signed_ext<T>)
    {
      if (v < 0)
        v_nega = true, uv = -uv;
    }
    is_nega ^= v_nega;
    using V = larger_int_t<U>;
    V rem = 0;
    repi(i, SZ(vec) - 1, -1, -1)
    {
      V cur = vec[i] + rem * BASE;
      vec[i] = (ll)(cur / uv);
      rem = cur % uv;
    }
    zero_suppress();
    return *this;
  }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger &operator%=(T v)
  {
    assert(v != 0);
    if (vec.empty())
      return *this;
    using U = make_unsigned_ext_t<T>;
    U uv = v;
    if constexpr (is_signed_ext<T>)
    {
      if (v < 0)
        uv = -uv;
    }
    using V = larger_int_t<U>;
    V rem = 0;
    repi(i, SZ(vec) - 1, -1, -1)
    {
      V cur = vec[i] + rem * BASE;
      rem = cur % uv;
    }
    vec.clear();
    if (rem > 0)
    {
      while (rem > 0)
      {
        vec.eb((ll)(rem % BASE));
        rem /= BASE;
      }
    }
    else
    {
      is_nega = false;
    }
    return *this;
  }

  // base**i * coef を加算、ならし O(1) 時間
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger &add_term(long long i, T coef)
  {
    if (coef == 0)
      return *this;
    bool c_nega = false;
    using U = make_unsigned_ext_t<T>;
    U ucoef = coef;
    if constexpr (is_signed_ext<T>)
    {
      if (coef < 0)
        c_nega = true, ucoef = -ucoef;
    }
    int q = i / digit, r = i % digit;
    using V = larger_int_t<U>;
    V val = (V)ucoef * ipow(base, r);
    if (is_nega == c_nega)
      add_abs(q, val);
    else
      sub_abs(q, val);
    return *this;
  }
  // base**i * coef を減算、数が常に非負の場合はならし O(1) 時間
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger &sub_term(long long i, T coef)
  {
    if (coef == 0)
      return *this;
    bool c_nega = false;
    using U = make_unsigned_ext_t<T>;
    U ucoef = coef;
    if constexpr (is_signed_ext<T>)
    {
      if (coef < 0)
        c_nega = true, ucoef = -ucoef;
    }
    int q = i / digit, r = i % digit;
    using V = larger_int_t<U>;
    V val = (V)ucoef * ipow(base, r);
    if (is_nega != c_nega)
      add_abs(q, val);
    else
      sub_abs(q, val);
    return *this;
  }

  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger operator+(T v) const { return BigInteger(*this) += v; }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger operator-(T v) const { return BigInteger(*this) -= v; }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger operator*(T v) const { return BigInteger(*this) *= v; }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger operator/(T v) const { return BigInteger(*this) /= v; }
  template <class T, typename = enable_if_t<is_integral_ext<T>>>
  BigInteger operator%(T v) const { return BigInteger(*this) %= v; }
};

template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
BigInteger<base, digit> operator+(T a, const BigInteger<base, digit> &b)
{
  return b + a;
}
template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
BigInteger<base, digit> operator*(T a, const BigInteger<base, digit> &b)
{
  return b * a;
}
template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
BigInteger<base, digit> operator-(T a, const BigInteger<base, digit> &b)
{
  return BigInteger<base, digit>(a) - b;
}

#if defined LOCAL or not defined FAST_IO
template <int base, int digit>
istream &operator>>(istream &is, BigInteger<base, digit> &a)
{
  string s;
  is >> s;
  a = s;
  return is;
}
template <int base, int digit>
ostream &operator<<(ostream &os, const BigInteger<base, digit> &a)
{
  os << a.to_string();
  return os;
}
#else
template <int base, int digit>
void rd1(BigInteger<base, digit> &a)
{
  string s;
  fastio::rd1(s);
  a = s;
}
template <int base, int digit>
void wt1(const BigInteger<base, digit> &a)
{
  fastio::wt1(a.to_string());
}
#endif

template <int base, int digit>
inline BigInteger<base, digit> divfloor(const BigInteger<base, digit> &a, const BigInteger<base, digit> &b)
{
  auto [q, r] = a.divmod(b);
  if (r != 0 && (a < 0) != (b < 0))
    q -= 1;
  return q;
}
template <int base, int digit>
inline BigInteger<base, digit> divceil(const BigInteger<base, digit> &a, const BigInteger<base, digit> &b)
{
  auto [q, r] = a.divmod(b);
  if (r != 0 && (a < 0) == (b < 0))
    q += 1;
  return q;
}
template <int base, int digit>
inline BigInteger<base, digit> divround(const BigInteger<base, digit> &a, const BigInteger<base, digit> &b)
{
  return divfloor(a * 2 + b, b * 2);
}
template <int base, int digit>
inline BigInteger<base, digit> safemod(const BigInteger<base, digit> &a, const BigInteger<base, digit> &b)
{
  auto [q, r] = a.divmod(b);
  if (r != 0 && (a < 0) != (b < 0))
    r += b;
  return r;
}

template <int base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> divfloor(const BigInteger<base, digit> &a, T b)
{
  BigInteger<base, digit> q = a / b;
  BigInteger<base, digit> r = a % b;
  if (r != 0 && (a < 0) != (b < 0))
    q -= 1;
  return q;
}
template <int base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> divceil(const BigInteger<base, digit> &a, T b)
{
  BigInteger<base, digit> q = a / b;
  BigInteger<base, digit> r = a % b;
  if (r != 0 && (a < 0) == (b < 0))
    q += 1;
  return q;
}
template <int base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> divround(const BigInteger<base, digit> &a, T b)
{
  return divfloor(a * 2 + b, b * 2);
}
template <int base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> safemod(const BigInteger<base, digit> &a, T b)
{
  BigInteger<base, digit> r = a % b;
  if (r != 0 && (a < 0) != (b < 0))
    r += b;
  return r;
}

template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> divfloor(T a, const BigInteger<base, digit> &b)
{
  return divfloor(BigInteger<base, digit>(a), b);
}
template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> divceil(T a, const BigInteger<base, digit> &b)
{
  return divceil(BigInteger<base, digit>(a), b);
}
template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> divround(T a, const BigInteger<base, digit> &b)
{
  return divround(BigInteger<base, digit>(a), b);
}
template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
inline BigInteger<base, digit> safemod(T a, const BigInteger<base, digit> &b)
{
  return safemod(BigInteger<base, digit>(a), b);
}

template <int base, int digit>
inline pair<BigInteger<base, digit>, BigInteger<base, digit>> safedivmod(const BigInteger<base, digit> &a, const BigInteger<base, digit> &b)
{
  auto [q, r] = a.divmod(b);
  if (r != 0 && (a < 0) != (b < 0))
  {
    q -= 1;
    r += b;
  }
  return {q, r};
}
template <int base, int digit, class T, typename = enable_if_t<is_integral_ext<T>>>
inline pair<BigInteger<base, digit>, BigInteger<base, digit>> safedivmod(const BigInteger<base, digit> &a, T b)
{
  BigInteger<base, digit> q = a / b;
  BigInteger<base, digit> r = a % b;
  if (r != 0 && (a < 0) != (b < 0))
  {
    q -= 1;
    r += b;
  }
  return {q, r};
}
template <class T, int base, int digit, typename = enable_if_t<is_integral_ext<T>>>
inline pair<BigInteger<base, digit>, BigInteger<base, digit>> safedivmod(T a, const BigInteger<base, digit> &b)
{
  return safedivmod(BigInteger<base, digit>(a), b);
}
