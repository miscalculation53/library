#pragma once

#include "../../template/template_all.hpp"
#include "modint_internal.hpp"
#include "../extgcd.hpp"

/**
 * @brief modint (64 bit)
 * @docs docs/math/modint/modint64.md
 */

template <ll m>
struct static_modint64
{
  using mint = static_modint64;
private:
  ull _v;
  static constexpr ull umod() { return m; }
  static constexpr bool prime = internal::isprime64<m>;

public:
  static constexpr ll mod() { return m; }
  static mint raw(ll v)
  {
    mint x;
    x._v = v;
    return x;
  }

  static_modint64() : _v(0) {}
  template <class T>
  static_modint64(T v)
  {
    if constexpr (is_unsigned_v<T>)
    {
      _v = (ull)(v % umod());
    }
    else
    {
      ll x = (ll)(v % (ll)(umod()));
      if (x < 0)
        x += umod();
      _v = (ull)x;
    }
  }

  ll val() const { return (ll)_v; }

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
    u128 z = _v;
    z *= rhs._v;
    _v = (ull)(z % umod());
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
      auto [g, x, y] = extgcd<ll>(_v, m);
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
struct dynamic_modint64_odd
{
  using mint = dynamic_modint64_odd;
private:
  ull _v;  // montgomery expression
  static internal::montgomery64odd mg;
  static ull umod() { return mg.umod(); }

public:
  static ll mod() { return (ll)(mg.umod()); }
  static void set_mod(ll m)
  {
    assert(m >= 1 && m % 2 == 1);
    mg = internal::montgomery64odd(m);
  }

  dynamic_modint64_odd() : _v(0) {}
  dynamic_modint64_odd(i128 v)
  { _v = mg.inv_reduce(v); }

  ll val() const { return (ll)mg.reduce(_v); }

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
    _v = mg.reduce(u128(_v) * rhs._v);
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
    auto [g, x, y] = extgcd<ll>(val(), mod());
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
internal::montgomery64odd dynamic_modint64_odd<id>::mg((1LL << 61) - 1);

template <int id>
struct dynamic_modint64
{
  using mint = dynamic_modint64;
private:
  ull _v;  // montgomery expression
  static internal::montgomery64 mg;
  static ull umod() { return mg.umod(); }

public:
  static ll mod() { return (ll)(mg.umod()); }
  static void set_mod(ll m)
  {
    assert(m >= 1);
    mg = internal::montgomery64(m);
  }

  dynamic_modint64() : _v(0) {}
  dynamic_modint64(i128 v)
  { _v = mg.inv_reduce(v); }

  ll val() const { return (ll)mg.reduce(_v); }

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
    _v = mg.reduce(u128(_v) * rhs._v);
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
    auto [g, x, y] = extgcd<ll>(val(), mod());
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
internal::montgomery64 dynamic_modint64<id>::mg((1LL << 61) - 1);

using modint61 = static_modint64<(1LL << 61) - 1>;
using modint64_odd = dynamic_modint64_odd<-1>;
using modint64 = dynamic_modint64<-1>;

template <int m>
istream &operator>>(istream &is, static_modint64<m> &x)
{
  ll a;
  is >> a;
  x = a;
  return is;
}
template <int m>
ostream &operator<<(ostream &os, const static_modint64<m> &x)
{
  os << x.val();
  return os;
}
template <int id>
istream &operator>>(istream &is, dynamic_modint64_odd<id> &x)
{
  ll a;
  is >> a;
  x = a;
  return is;
}
template <int id>
ostream &operator<<(ostream &os, const dynamic_modint64_odd<id> &x)
{
  os << x.val();
  return os;
}
template <int id>
istream &operator>>(istream &is, dynamic_modint64<id> &x)
{
  ll a;
  is >> a;
  x = a;
  return is;
}
template <int id>
ostream &operator<<(ostream &os, const dynamic_modint64<id> &x)
{
  os << x.val();
  return os;
}

template <int m>
void rd1(static_modint64<m> &x)
{
  ll a;
  fastio::rd1(a);
  x = a;
}
template <int m>
void wt1(const static_modint64<m> &x)
{
  fastio::wt1(x.val());
}
template <int id>
void rd1(dynamic_modint64_odd<id> &x)
{
  ll a;
  fastio::rd1(a);
  x = a;
}
template <int id>
void wt1(const dynamic_modint64_odd<id> &x)
{
  fastio::wt1(x.val());
}
template <int id>
void rd1(dynamic_modint64<id> &x)
{
  ll a;
  fastio::rd1(a);
  x = a;
}
template <int id>
void wt1(const dynamic_modint64<id> &x)
{
  fastio::wt1(x.val());
}