#pragma once

#include "../../template/template_all.hpp"
#include "modint64_internal.hpp"
#include "modint_base.hpp"
#include "../extgcd.hpp"

/**
 * @brief modint (64 bit)
 * @docs docs/math/modint/modint64.md
 */

template <ll m>
struct static_modint64 : internal::modint_base<static_modint64<m>>
{
  using mint = static_modint64;
private:
  friend struct internal::modint_base<static_modint64<m>>;
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

  mint& operator*=(const mint &rhs)
  {
    u128 z = _v;
    z *= rhs._v;
    _v = (ull)(z % umod());
    return *this;
  }

  mint inv() const
  {
    if (prime)
    {
      assert(_v != 0);
      return CREF.pow(umod() - 2);
    }
    else
    {
      auto [g, x, y] = extgcd<ll>(_v, m);
      assert(g == 1);
      return x;
    }
  }
};

template <int id>
struct dynamic_modint64_odd : internal::modint_base<dynamic_modint64_odd<id>>
{
  using mint = dynamic_modint64_odd;
private:
  friend struct internal::modint_base<dynamic_modint64_odd<id>>;
  ull _v; // montgomery expression
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

  mint& operator*=(const mint &rhs)
  {
    _v = mg.reduce(u128(_v) * rhs._v);
    return *this;
  }

  mint inv() const
  {
    auto [g, x, y] = extgcd<ll>(val(), mod());
    assert(g == 1);
    return x;
  }
};
template <int id>
internal::montgomery64odd dynamic_modint64_odd<id>::mg((1LL << 61) - 1);

template <int id>
struct dynamic_modint64 : internal::modint_base<dynamic_modint64<id>>
{
  using mint = dynamic_modint64;
private:
  friend struct internal::modint_base<dynamic_modint64<id>>;
  ull _v; // montgomery expression
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

  mint& operator*=(const mint &rhs)
  {
    _v = mg.reduce(u128(_v) * rhs._v);
    return *this;
  }

  mint inv() const
  {
    auto [g, x, y] = extgcd<ll>(val(), mod());
    assert(g == 1);
    return x;
  }
};
template <int id>
internal::montgomery64 dynamic_modint64<id>::mg((1LL << 61) - 1);

using modint61 = static_modint64<(1LL << 61) - 1>;
using modint64_odd = dynamic_modint64_odd<-1>;
using modint64 = dynamic_modint64<-1>;