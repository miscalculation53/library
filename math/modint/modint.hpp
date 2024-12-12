#pragma once

#include "../../template/template_all.hpp"

/**
 * @brief modint (32 bit)
 * @docs docs/math/modint/modint.md
 */

namespace internal
{
  
struct barrett
{
  uint m;
  ull im;

  explicit barrett(uint m) : m(m), im((ull)(-1) / m + 1) {}
  uint umod() const { return m; }
  uint mul(uint a, uint b) const
  {
    ull z = a;
    z *= b;
    ull x = (ull)(((unsigned __int128)(z)*im) >> 64);
    ull y = x * m;
    return (uint)(z - y + (z < y ? m : 0));
  }
};

constexpr ll pow_mod_constexpr(ll x, ll n, int m)
{
  if (m == 1)
    return 0;
  uint _m = (uint)m;
  ull r = 1;
  ull y = safemod(x, m);
  while (n)
  {
    if (n & 1)
      r = (r * y) % _m;
    y = (y * y) % _m;
    n >>= 1;
  }
  return r;
}

constexpr bool is_prime_constexpr(int n)
{
  if (n <= 1)
    return false;
  if (n == 2 || n == 7 || n == 61)
    return true;
  if (n % 2 == 0)
    return false;
  ll d = n - 1;
  while (d % 2 == 0)
    d /= 2;
  constexpr ll bases[3] = {2, 7, 61};
  for (ll a : bases)
  {
    ll t = d;
    ll y = pow_mod_constexpr(a, t, n);
    while (t != n - 1 && y != 1 && y != n - 1)
    {
      y = y * y % n;
      t <<= 1;
    }
    if (y != n - 1 && t % 2 == 0)
      return false;
  }
  return true;
}
template <int n>
constexpr bool is_prime = is_prime_constexpr(n);

constexpr pll inv_gcd(ll a, ll b)
{
  a = safemod(a, b);
  if (a == 0)
    return {b, 0};
  ll s = b, t = a;
  ll m0 = 0, m1 = 1;
  while (t)
  {
    ll u = s / t;
    s -= t * u;
    m0 -= m1 * u;
    auto tmp = s;
    s = t, t = tmp, tmp = m0, m0 = m1, m1 = tmp;
  }
  if (m0 < 0)
    m0 += b / s;
  return {s, m0};
}

} // namespace internal

template <int m>
struct static_modint
{
  using mint = static_modint;
private:
  uint _v;
  static constexpr uint umod() { return m; }
  static constexpr bool prime = internal::is_prime<m>;

public:
  static constexpr int mod() { return m; }
  static mint raw(int v)
  {
    mint x;
    x._v = v;
    return x;
  }

  static_modint() : _v(0) {}
  template <class T>
  static_modint(T v)
  {
    if constexpr (is_signed_v<T>)
    {
      ll x = (ll)(v % (ll)(umod()));
      if (x < 0)
        x += umod();
      _v = (uint)x;
    }
    else if constexpr (is_unsigned_v<T>)
    {
      _v = (uint)(v % umod());
    }
    else
    {
      static_assert(is_signed_v<T> || is_unsigned_v<T>, "Unsupported Type");
    }
  }

  int val() const { return (int)_v; }

  mint& operator++()
  {
    _v++;
    if (_v == umod())
      _v = 0;
    return *this;
  }
  mint& operator--()
  {
    if (_v == 0)
      _v = umod();
    _v--;
    return *this;
  }
  mint operator++(int)
  {
    mint res = *this;
    ++*this;
    return res;
  }
  mint operator--(int)
  {
    mint res = *this;
    --*this;
    return res;
  }

  mint& operator+=(const mint& rhs)
  {
    _v += rhs._v;
    if (_v >= umod())
      _v -= umod();
    return *this;
  }
  mint& operator-=(const mint &rhs)
  {
    _v -= rhs._v;
    if (_v >= umod())
      _v += umod();
    return *this;
  }
  mint& operator*=(const mint &rhs)
  {
    ull z = _v;
    z *= rhs._v;
    _v = (uint)(z % umod());
    return *this;
  }
  mint& operator/=(const mint &rhs) { return *this = *this * rhs.inv(); }

  mint operator+() const { return *this; }
  mint operator-() const { return mint() - *this; }

  mint pow(ll n) const
  {
    assert(n >= 0);
    mint x = *this, r = 1;
    while (n)
    {
      if (n & 1)
        r *= x;
      x *= x;
      n >>= 1;
    }
    return r;
  }
  mint inv() const
  {
    if (prime)
    {
      assert(_v != 0);
      return pow(umod() - 2);
    }
    else
    {
      auto eg = internal::inv_gcd(_v, m);
      assert(eg.first == 1);
      return eg.second;
    }
  }

  friend mint operator+(const mint &lhs, const mint &rhs)
  { return mint(lhs) += rhs; }
  friend mint operator-(const mint &lhs, const mint &rhs)
  { return mint(lhs) -= rhs; }
  friend mint operator*(const mint &lhs, const mint &rhs)
  { return mint(lhs) *= rhs; }
  friend mint operator/(const mint &lhs, const mint &rhs)
  { return mint(lhs) /= rhs; }
  friend bool operator==(const mint &lhs, const mint &rhs)
  { return lhs._v == rhs._v; }
  friend bool operator!=(const mint &lhs, const mint &rhs)
  { return lhs._v != rhs._v; }

  friend istream &operator>>(istream &is, mint &x)
  {
    ll a;
    is >> a;
    x = a;
    return is;
  }
  friend ostream &operator<<(ostream &os, const mint &x)
  {
    os << x.val();
    return os;
  }
};

template <int id>
struct dynamic_modint
{
  using mint = dynamic_modint;
private:
  uint _v;
  static internal::barrett bt;
  static uint umod() { return bt.umod(); }

public:
  static int mod() { return (int)(bt.umod()); }
  static void set_mod(int m)
  {
    assert(m >= 1);
    bt = internal::barrett(m);
  }
  static mint raw(int v)
  {
    mint x;
    x._v = v;
    return x;
  }

  dynamic_modint() : _v(0) {}
  template <class T>
  dynamic_modint(T v)
  {
    if constexpr (is_signed_v<T>)
    {
      ll x = (ll)(v % (ll)(umod()));
      if (x < 0)
        x += umod();
      _v = (uint)x;
    }
    else if constexpr (is_unsigned_v<T>)
    {
      _v = (uint)(v % umod());
    }
    else
    {
      static_assert(is_signed_v<T> || is_unsigned_v<T>, "Unsupported Type");
    }
  }

  int val() const { return (int)_v; }

  mint& operator++()
  {
    _v++;
    if (_v == umod())
      _v = 0;
    return *this;
  }
  mint& operator--()
  {
    if (_v == 0)
      _v = umod();
    _v--;
    return *this;
  }
  mint operator++(int)
  {
    mint res = *this;
    ++*this;
    return res;
  }
  mint operator--(int)
  {
    mint res = *this;
    --*this;
    return res;
  }

  mint& operator+=(const mint& rhs)
  {
    _v += rhs._v;
    if (_v >= umod())
      _v -= umod();
    return *this;
  }
  mint& operator-=(const mint &rhs)
  {
    _v -= rhs._v;
    if (_v >= umod())
      _v += umod();
    return *this;
  }
  mint& operator*=(const mint &rhs)
  {
    _v = bt.mul(_v, rhs._v);
    return *this;
  }
  mint& operator/=(const mint &rhs) { return *this = *this * rhs.inv(); }

  mint operator+() const { return *this; }
  mint operator-() const { return mint() - *this; }

  mint pow(ll n) const
  {
    assert(n >= 0);
    mint x = *this, r = 1;
    while (n)
    {
      if (n & 1)
        r *= x;
      x *= x;
      n >>= 1;
    }
    return r;
  }
  mint inv() const
  {
    auto eg = internal::inv_gcd(_v, mod());
    assert(eg.first == 1);
    return eg.second;
  }

  friend mint operator+(const mint &lhs, const mint &rhs)
  { return mint(lhs) += rhs; }
  friend mint operator-(const mint &lhs, const mint &rhs)
  { return mint(lhs) -= rhs; }
  friend mint operator*(const mint &lhs, const mint &rhs)
  { return mint(lhs) *= rhs; }
  friend mint operator/(const mint &lhs, const mint &rhs)
  { return mint(lhs) /= rhs; }
  friend bool operator==(const mint &lhs, const mint &rhs)
  { return lhs._v == rhs._v; }
  friend bool operator!=(const mint &lhs, const mint &rhs)
  { return lhs._v != rhs._v; }

  friend istream &operator>>(istream &is, mint &x)
  {
    ll a;
    is >> a;
    x = a;
    return is;
  }
  friend ostream &operator<<(ostream &os, const mint &x)
  {
    os << x.val();
    return os;
  }
};
template <int id>
internal::barrett dynamic_modint<id>::bt(998244353);

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<-1>;