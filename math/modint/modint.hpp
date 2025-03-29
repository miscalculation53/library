#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "modint32_internal.hpp"
#include "modint_base.hpp"
#include "../extgcd.hpp"

/**
 * @brief modint (32 bit)
 * @docs docs/math/modint/modint.md
 */

template <int m>
struct static_modint : internal::modint_base<static_modint<m>>
{
  using mint = static_modint;
private:
  friend struct internal::modint_base<static_modint<m>>;
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

  mint& operator*=(const mint &rhs)
  {
    ull z = _v;
    z *= rhs._v;
    _v = (uint)(z % umod());
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
      auto [g, x, y] = extgcd<int>(_v, m);
      assert(g == 1);
      return x;
    }
  }
};

template <int id>
struct dynamic_modint : internal::modint_base<dynamic_modint<id>>
{
  using mint = dynamic_modint;
private:
  friend struct internal::modint_base<dynamic_modint<id>>;
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

  mint& operator*=(const mint &rhs)
  {
    _v = bt.mul(_v, rhs._v);
    return *this;
  }

  mint inv() const
  {
    auto [g, x, y] = extgcd<int>(_v, mod());
    assert(g == 1);
    return x;
  }
};
template <int id>
internal::barrett32 dynamic_modint<id>::bt(998244353);

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<-1>;