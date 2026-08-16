#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../utils/is_integral_ext.hpp"
#include "modint_internal_static.hpp"
#include "modint_internal_barrett32.hpp"
#include "modint_internal_montgomery64.hpp"
#include "../extgcd.hpp"

namespace internal
{

  template <class Policy>
  struct modint_impl
  {
    using V = typename Policy::value_type;
    using M = typename Policy::mod_type;
    using mint = modint_impl;

  private:
    V _v;

  public:
    static constexpr M mod() { return Policy::mod(); }

    template <class T = Policy>
    static auto set_mod(M m) -> decltype(T::set_mod(m)) { return T::set_mod(m); }

    static mint raw(V v)
    {
      mint x;
      x._v = v;
      return x;
    }

    modint_impl() : _v(0) {}

    template <class T, typename = enable_if_t<is_integral_ext<T>>>
    modint_impl(T v)
    {
      V rem;
      if constexpr (is_signed_ext<T>)
      {
        using S = make_signed_t<V>;
        S x = v % S(Policy::umod());
        if (x < 0)
          x += Policy::umod();
        rem = x;
      }
      else
        rem = V(v % Policy::umod());
      _v = Policy::init(rem);
    };

    M val() const { return Policy::val(_v); }

    mint &operator+=(const mint &rhs)
    {
      _v += rhs._v;
      if (_v >= Policy::umod())
        _v -= Policy::umod();
      return *this;
    }
    mint &operator-=(const mint &rhs)
    {
      _v -= rhs._v;
      if (_v >= Policy::umod())
        _v += Policy::umod();
      return *this;
    }
    mint &operator*=(const mint &rhs)
    {
      _v = Policy::mul(_v, rhs._v);
      return *this;
    }
    mint &operator/=(const mint &rhs)
    {
      return *this *= rhs.inv();
    }

    mint &operator++()
    {
      _v++;
      if (_v == Policy::umod())
        _v = 0;
      return *this;
    }
    mint &operator--()
    {
      if (_v == 0)
        _v = Policy::umod();
      _v--;
      return *this;
    }
    mint operator++(int)
    {
      mint res = *this;
      ++(*this);
      return res;
    }
    mint operator--(int)
    {
      mint res = *this;
      --(*this);
      return res;
    }
    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }

    template <class T>
    mint pow(T n) const
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
      if constexpr (Policy::is_prime)
      {
        return pow(mod() - 2);
      }
      else
      {
        auto [g, x, y] = extgcd<M>(val(), mod());
        assert(g == 1);
        return mint(x);
      }
    }

    friend mint operator+(const mint &lhs, const mint &rhs) { return mint(lhs) += rhs; }
    friend mint operator-(const mint &lhs, const mint &rhs) { return mint(lhs) -= rhs; }
    friend mint operator*(const mint &lhs, const mint &rhs) { return mint(lhs) *= rhs; }
    friend mint operator/(const mint &lhs, const mint &rhs) { return mint(lhs) /= rhs; }
    friend bool operator==(const mint &lhs, const mint &rhs) { return lhs._v == rhs._v; }
    friend bool operator!=(const mint &lhs, const mint &rhs) { return lhs._v != rhs._v; }
    friend M safe_hash_key(const mint &x) { return x.val(); }
  
#if defined LOCAL or not defined FAST_IO
    friend std::istream &operator>>(std::istream &is, mint &x)
    {
      long long a;
      is >> a;
      x = a;
      return is;
    }
    friend std::ostream &operator<<(std::ostream &os, const mint &x)
    {
      os << x.val();
      return os;
    }
#else
    friend void rd1(mint &x)
    {
      long long a;
      fastio::rd1(a);
      x = a;
    }
    friend void wt1(const mint &x)
    {
      fastio::wt1(x.val());
    }
#endif
  };

};

template <int mod>
using static_modint32 = internal::modint_impl<internal::policy_static<mod>>;
template <int id>
using dynamic_modint32 = internal::modint_impl<internal::policy_barrett32<id>>;
template <ll mod>
using static_modint64 = internal::modint_impl<internal::policy_static<mod>>;
template <int id>
using dynamic_modint64_odd = internal::modint_impl<internal::policy_montgomery64_odd<id>>;
template <int id>
using dynamic_modint64 = internal::modint_impl<internal::policy_montgomery64<id>>;

using modint998244353 = static_modint32<998244353>;
using modint1000000007 = static_modint32<1000000007>;
using modint = dynamic_modint32<-1>;
using modint61 = static_modint64<(1LL << 61) - 1>;
using modint64 = dynamic_modint64<-1>;

template <class T>
struct is_modint : std::false_type
{
};
template <class Policy>
struct is_modint<internal::modint_impl<Policy>> : std::true_type
{
};
template <class T>
inline constexpr bool is_modint_v = is_modint<T>::value;

template <class T>
struct is_static_modint : false_type {};
template <int m>
struct is_static_modint<static_modint32<m>> : true_type {};
template <ll m>
struct is_static_modint<static_modint64<m>> : true_type {};
template <class T>
inline constexpr bool is_static_modint_v = is_static_modint<T>::value;

template <class T>
struct is_dynamic_modint : false_type {};
template <int id>
struct is_dynamic_modint<dynamic_modint32<id>> : true_type {};
template <int id>
struct is_dynamic_modint<dynamic_modint64_odd<id>> : true_type {};
template <int id>
struct is_dynamic_modint<dynamic_modint64<id>> : true_type {};
template <class T>
inline constexpr bool is_dynamic_modint_v = is_dynamic_modint<T>::value;

template <typename, typename = void>
struct has_mod : std::false_type
{
};
template <typename T>
struct has_mod<T, std::void_t<decltype(T::mod())>> : std::true_type
{
};

template <class mint>
struct modint_less
{
  bool operator()(const mint &a, const mint &b) const
  {
    if constexpr (is_modint_v<mint>)
      return a.val() < b.val();
    else
      return a < b;
  }
};

template <class mint>
struct modint_hash
{
  auto operator()(const mint &x) const
  {
    if constexpr (is_modint_v<mint>)
      return std::hash<decltype(x.val())>{}(x.val());
    else
      return std::hash<mint>{}(x);
  }
};
