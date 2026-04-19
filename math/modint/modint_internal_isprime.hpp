#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../utils/larger_int.hpp"

namespace internal
{

template <class T>
constexpr ll powmod_constexpr(ll x, ll n, T m)
{
  if (m == 1)
    return 0;
  using U = make_unsigned_t<T>;
  using L = larger_int_t<U>;

  U r = 1, y = safemod(x, m);
  while (n)
  {
    if (n & 1)
      r = L(r) * y % m;
    y = L(y) * y % m;
    n >>= 1;
  }
  return r;
}

template <class T>
constexpr bool isprime_constexpr(T n)
{
  if constexpr (sizeof(T) > 4)
  {
    if (n <= INT_MAX)
      return isprime_constexpr<int>(n);
  }

  if (n <= 1)
    return false;
  if (n == 2 || n == 7 || n == 61)
    return true;
  if (n % 2 == 0)
    return false;

  ll d = n - 1;
  while (d % 2 == 0)
    d /= 2;

  using U = make_unsigned_t<T>;
  using L = larger_int_t<U>;

  auto miller_rabin = [&](const auto &bases) constexpr
  {
    for (ll a : bases)
    {
      ll t = d, y = powmod_constexpr(a, t, n);
      while (t != n - 1 && y != 1 && y != n - 1)
      {
        y = L(y) * y % n;
        t <<= 1;
      }
      if (y != n - 1 && t % 2 == 0)
        return false;
    }
    return true;
  };

  if constexpr(sizeof(T) <= 4)
  {
    constexpr ll bases[3] = {2, 7, 61};
    return miller_rabin(bases);
  }
  else
  {
    constexpr ll bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    return miller_rabin(bases);
  }
}

template <auto n>
constexpr bool isprime = isprime_constexpr(n);

};
