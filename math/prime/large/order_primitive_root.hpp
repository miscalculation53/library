#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "math/modint/modint64.hpp"
#include "math/prime/prime_power.hpp"
#include "math/prime/large/primality_test.hpp"
#include "math/prime/large/factorize.hpp"
#include "math/prime/large/euler_phi_carmichael.hpp"

/**
 * @brief 元の位数と原始根
 * @docs docs/math/prime/large/order_primitive_root.md
 */

// https://37zigen.com/primitive-root/#i-4
// https://x.com/noshi91/status/1317025404700405760

namespace internal
{

// x^{ a[0]...a[i-1] a[i+1]...a[n-1] } for all i
template <class mint, class I>
vc<mint> lagrange_basis(const vc<I> &a, mint x)
{
  const int n = a.size();
  vc<mint> res(n);
  if (n == 0)
    return res;
  auto dfs = [&](auto dfs, mint v, int l, int r)
  {
    if (r - l == 1)
    {
      res[l] = v;
      return;
    }
    int m = (l + r) / 2;
    mint vl = v, vr = v;
    repi(i, l, m) vr = vr.pow(a[i]);
    repi(i, m, r) vl = vl.pow(a[i]);
    dfs(dfs, vl, l, m);
    dfs(dfs, vr, m, r);
  };
  dfs(dfs, x, 0, n);
  return res;
}

template <class mint, class P>
bool internal_is_primitive_root(ll x, ll p, const vc<PrimePower<P>> &fac)
{
  const int k = fac.size();
  mint::set_mod(p);
  vc<ll> a(k);
  repi(i, k) a[i] = fac[i].pe;
  auto b = lagrange_basis(a, mint(x));
  repi(i, k)
  {
    if (b[i].pow(fac[i].pe / fac[i].p) == 1)
      return false;
  }
  return true;
}

template <class mint, class P>
ll internal_primitive_root(ll p, const vc<PrimePower<P>> &fac)
{
  if (p == 2)
    return 1;
  while (true)
  {
    ll g = randrange(1, p);
    if (internal_is_primitive_root<mint>(g, p, fac))
      return g;
  }
}
template <class mint, class P>
ll internal_primitive_root_min(ll p, const vc<PrimePower<P>> &fac)
{
  if (p == 2)
    return 1;
  for (int g = 2; g < p; g++)
  {
    if (internal_is_primitive_root<mint>(g, p, fac))
      return g;
  }
  assert(false);
}

template <class mint, class P>
ll internal_order_mod(ll x, ll m, const vc<PrimePower<P>> &fac)
{
  const int k = fac.size();
  mint::set_mod(m);
  vc<ll> a(k);
  repi(i, k) a[i] = fac[i].pe;
  auto b = internal::lagrange_basis(a, mint(x));
  ll res = 1;
  repi(i, k)
  {
    mint tmp = b[i];
    repi(j, fac[i].e + 1)
    {
      if (tmp == 1)
        break;
      tmp = tmp.pow(fac[i].p);
      res *= fac[i].p;
    }
  }
  return res;
}

}; // namespace internal

// m >= 2
// mod m での x の位数
// fac にはオイラー関数 φ(m) かカーマイケル関数 λ(m) の素因数分解を渡す (素数なら p-1)
// O( log(m) loglog(m) )
template <class P>
ll order_mod(ll x, ll m, const vc<PrimePower<P>> &fac)
{
  assert(m >= 2);
  assert(gcd(x, m) == 1);
  if (m <= INT_MAX)
  {
    using mint = dynamic_modint<INT_MIN>;
    return internal::internal_order_mod<mint>(x, m, fac);
  }
  else if (m % 2 == 1)
  {
    using mint = dynamic_modint64_odd<INT_MIN>;
    return internal::internal_order_mod<mint>(x, m, fac);
  }
  else
  {
    using mint = dynamic_modint64<INT_MIN>;
    return internal::internal_order_mod<mint>(x, m, fac);
  }
}

// p (素数) の原始根
// fac には p-1 の素因数分解を渡す
// O( log(p) loglog(p)^2 )
template <class P>
ll primitive_root(ll p, const vc<PrimePower<P>> &fac)
{
  if (p <= INT_MAX)
  {
    using mint = dynamic_modint<INT_MIN>;
    return internal::internal_primitive_root<mint>(p, fac);
  }
  else
  {
    using mint = dynamic_modint64_odd<INT_MIN>;
    return internal::internal_primitive_root<mint>(p, fac);
  }
}

// p (素数) の最小の原始根
// fac には p-1 の素因数分解を渡す
// O( log(p) loglog(p)^2 )
template <class P>
ll primitive_root_min(ll p, const vc<PrimePower<P>> &fac)
{
  if (p <= INT_MAX)
  {
    using mint = dynamic_modint<INT_MIN>;
    return internal::internal_primitive_root_min<mint>(p, fac);
  }
  else
  {
    using mint = dynamic_modint64_odd<INT_MIN>;
    return internal::internal_primitive_root_min<mint>(p, fac);
  }
}
