#pragma once

#include "../../template/template_all.hpp"
#include "modint_internal.hpp"
#include "../extgcd.hpp"

/**
 * @brief modint (32 bit)
 * @docs docs/math/modint/modint.md
 */

template <int m>
struct static_modint
{
  using mint = static_modint;
private:
  uint _v;
  static constexpr uint umod() { return m; }
  static constexpr bool prime = internal::isprime32<m>;

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
      auto [g, x, y] = extgcd<int>(_v, m);
      assert(g == 1);
      return x;
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
};

template <int id>
struct dynamic_modint
{
  using mint = dynamic_modint;
private:
  uint _v;
  static internal::barrett32 bt;
  static uint umod() { return bt.umod(); }

public:
  static int mod() { return (int)(bt.umod()); }
  static void set_mod(int m)
  {
    assert(m >= 1);
    bt = internal::barrett32(m);
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
    auto [g, x, y] = extgcd<int>(_v, mod());
    assert(g == 1);
    return x;
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
};
template <int id>
internal::barrett32 dynamic_modint<id>::bt(998244353);

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<-1>;

template <int m>
istream &operator>>(istream &is, static_modint<m> &x)
{
  ll a;
  is >> a;
  x = a;
  return is;
}
template <int m>
ostream &operator<<(ostream &os, const static_modint<m> &x)
{
  os << x.val();
  return os;
}
template <int id>
istream &operator>>(istream &is, dynamic_modint<id> &x)
{
  ll a;
  is >> a;
  x = a;
  return is;
}
template <int id>
ostream &operator<<(ostream &os, const dynamic_modint<id> &x)
{
  os << x.val();
  return os;
}

template <int m>
void rd1(static_modint<m> &x)
{
  ll a;
  fastio::rd1(a);
  x = a;
}
template <int m>
void wt1(const static_modint<m> &x)
{
  fastio::wt1(x.val());
}
template <int id>
void rd1(dynamic_modint<id> &x)
{
  ll a;
  fastio::rd1(a);
  x = a;
}
template <int id>
void wt1(const dynamic_modint<id> &x)
{
  fastio::wt1(x.val());
}