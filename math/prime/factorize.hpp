#pragma once

#include "../../template/template_all.hpp"
#include "../modint/modint.hpp"
#include "../modint/modint64.hpp"
#include "prime_power.hpp"
#include "primality_test.hpp"

/**
 * @brief 素因数分解
 * @docs docs/math/prime/factorize.md
 */

namespace internal
{

template <class mint>
ll get_prime_factor_impl(ll n)
{
  mint::set_mod(n);
  int m = pow(n, .125);
  mt19937 _mt;
  while (true)
  {
    int c = 1 + _mt() % 100;
    mint x = 2, y = 2, prod = 1;
    ll g = 1;
    while (g == 1)
    {
      repi(i, m)
      {
        x = x * x + c;
        y = y * y + c, y = y * y + c;
        prod *= x - y;
      }
      g = gcd(prod.val(), n);
    }
    if (g == n)
      continue;
    if (is_prime(g))
      return g;
    else if (is_prime(n / g))
      return n / g;
    else
      return get_prime_factor_impl<mint>(g);
  }
}

ll get_prime_factor(ll n)
{
  if (n <= INT_MAX)
  {
    using mint = dynamic_modint<INT_MIN>;
    return get_prime_factor_impl<mint>(n);
  }
  else
  {
    using mint = dynamic_modint64_odd<INT_MIN>;
    return get_prime_factor_impl<mint>(n);
  }
}

}; // namespace internal

vc<PrimePower<ll>> factorize(ll n)
{
  vc<PrimePower<ll>> res;
  repi(p, 2, 100)
  {
    PrimePower<ll> pp(p, 0);
    while (n % p == 0)
      n /= p, pp.mul_p();
    if (pp.e > 0)
      res.emplace_back(pp);
  }
  while (n > 1)
  {
    if (is_prime(n))
    {
      res.emplace_back(n);
      break;
    }
    ll p = internal::get_prime_factor(n);
    PrimePower<ll> pp(p, 0);
    while (n % p == 0)
      n /= p, pp.mul_p();
    res.emplace_back(pp);
  }
  sort(ALL(res), [&](cauto &pp1, cauto &pp2)
       { return pp1.p < pp2.p; });
  return res;
}