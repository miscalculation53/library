#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../modint/modint.hpp"
#include "../modint/modint64.hpp"

/**
 * @brief 素数判定
 * @docs docs/math/prime/primality_test.md
 */

namespace internal
{

template <class mint, class Array>
bool is_prime_impl(ll n, const Array &bases)
{
  if (n <= 1)
    return false;
  if (n == 2 || n == 7 || n == 61)
    return true;
  if (n % 2 == 0)
    return false;
  ll d = (n - 1) >> countr_zero(n - 1);
  mint::set_mod(n);
  for (ll a : bases)
  {
    ll t = d;
    mint y = mint(a).pow(t);
    while (t != n - 1 && y != 1 && y != n - 1)
    {
      y *= y;
      t <<= 1;
    }
    if (y != n - 1 && t % 2 == 0)
      return false;
  }
  return true;
}

}; // namespace internal

bool is_prime(ll n)
{
  static constexpr array<ll, 3> bases32 = {2, 7, 61};
  static constexpr array<ll, 7> bases64 = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  if (n <= INT_MAX)
  {
    using mint = dynamic_modint<INT_MIN>;
    return internal::is_prime_impl<mint>(n, bases32);
  }
  else
  {
    using mint = dynamic_modint64_odd<INT_MIN>;
    return internal::is_prime_impl<mint>(n, bases64);
  }
}