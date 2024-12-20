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

int get_prime_factor_int(int n)
{
  int m = pow(n, .125);
  using mint = dynamic_modint<-2>;
  mint::set_mod(n);
  for (int c = 1;; c++)
  {
    mint x = 2, y = 2, prod = 1;
    int g = 1;
    for (int t = 1; g == 1; t = min(2 * t, m))
    {
      repi(i, t)
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
      return get_prime_factor_int(g);
  }
}

ll get_prime_factor(ll n)
{
  if (n <= INT_MAX)
    return get_prime_factor_int(n);
  int m = pow(n, .125);
  using mint = dynamic_modint64_odd<-2>;
  mint::set_mod(n);
  for (int c = 1;; c++)
  {
    mint x = 2, y = 2, prod = 1;
    ll g = 1;
    for (int t = 1; g == 1; t = min(2 * t, m))
    {
      repi(i, t)
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
      return get_prime_factor(g);
  }
}

};

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

vc<ll> divisors(const vc<PrimePower<ll>> &pps)
{
  vc<ll> res;
  auto dfs = [&](auto dfs, ll d, int i) -> void
  {
    if (i == SZI(pps))
    {
      res.emplace_back(d);
      return;
    }
    auto &pp = pps[i];
    ull nd = d;
    repi(j, pp.e + 1)
    {
      dfs(dfs, nd, i + 1);
      nd *= pp.p;
    }
  };
  dfs(dfs, 1, 0);
  sort(ALL(res));
  return res;
}