#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "math/prime/prime_power.hpp"
#include "math/prime/large/primality_test.hpp"

/**
 * @brief 素因数分解
 * @docs docs/math/prime/large/factorize.md
 */

namespace internal
{

template <class mint>
ll get_prime_factor_impl(ll n)
{
  mint::set_mod(n);
  int m = pow(n, .125);
  while (true)
  {
    int c = randrange(1, 100);
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
    using mint = dynamic_modint32<INT_MIN>;
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
    if (n % p == 0)
    {
      auto [e, pe, nn] = ord_pow_div(n, p);
      res.emplace_back(PrimePower<ll>(p, e, pe));
      n = nn;
    }
  }
  while (n > 1)
  {
    if (is_prime(n))
    {
      res.emplace_back(n);
      break;
    }
    ll p = internal::get_prime_factor(n);
    auto [e, pe, nn] = ord_pow_div(n, p);
    res.emplace_back(PrimePower<ll>(p, e, pe));
    n = nn;
  }
  sort(ALL(res), [&](const PrimePower<ll> &pp1, const PrimePower<ll> &pp2)
       { return pp1.p < pp2.p; });
  return res;
}
