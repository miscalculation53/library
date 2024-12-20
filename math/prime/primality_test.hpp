#pragma once

#include "../../template/template_all.hpp"
#include "../modint/modint.hpp"
#include "../modint/modint64.hpp"

/**
 * @brief 素数判定
 * @docs docs/math/prime/primality_test.md
 */

bool is_prime_int(int n)
{
  if (n <= 1)
    return false;
  if (n == 2 || n == 7 || n == 61)
    return true;
  if (n % 2 == 0)
    return false;
  ll d = (n - 1) >> countr_zero(n - 1);
  static const ll bases[3] = {2, 7, 61};
  using mint = dynamic_modint<-2>;
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

bool is_prime(ll n)
{
  if (n <= INT_MAX)
    return is_prime_int(n);
  if (n % 2 == 0)
    return false;
  ll d = (n - 1) >> countr_zero(n - 1);
  static const ll bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  using mint = dynamic_modint64_odd<-2>;
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