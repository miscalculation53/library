#pragma once

#include "../../template/template_all_but_modint.hpp"

namespace internal
{

constexpr ll powmod32_constexpr(ll x, ll n, int m)
{
  if (m == 1)
    return 0;
  uint _m = (uint)m;
  ull r = 1;
  ull y = safemod(x, m);
  while (n)
  {
    if (n & 1)
      r = (r * y) % _m;
    y = (y * y) % _m;
    n >>= 1;
  }
  return r;
}

constexpr bool isprime32_constexpr(int n)
{
  if (n <= 1)
    return false;
  if (n == 2 || n == 7 || n == 61)
    return true;
  if (n % 2 == 0)
    return false;
  ll d = n - 1;
  while (d % 2 == 0)
    d /= 2;
  constexpr ll bases[3] = {2, 7, 61};
  for (ll a : bases)
  {
    ll t = d;
    ll y = powmod32_constexpr(a, t, n);
    while (t != n - 1 && y != 1 && y != n - 1)
    {
      y = y * y % n;
      t <<= 1;
    }
    if (y != n - 1 && t % 2 == 0)
      return false;
  }
  return true;
}
template <int n>
constexpr bool isprime32 = isprime32_constexpr(n);

struct barrett32
{
  uint m;
  ull im;

  explicit barrett32(uint m) : m(m), im((ull)(-1) / m + 1) {}
  uint umod() const { return m; }
  uint mul(uint a, uint b) const
  {
    ull z = a;
    z *= b;
    ull x = (ull)((u128(z)*im) >> 64);
    ull y = x * m;
    return (uint)(z - y + (z < y ? m : 0));
  }
};

}