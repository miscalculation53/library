#pragma once

#include "../../template/template_all.hpp"

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
constexpr ll powmod64_constexpr(ll x, ll n, ll m)
{
  if (m == 1)
    return 0;
  ull _m = (ull)m;
  ull r = 1;
  ull y = safemod(x, m);
  while (n)
  {
    u128 y128(y);
    if (n & 1)
      r = (y128 * r) % _m;
    y = (y128 * y) % _m;
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
constexpr bool isprime64_constexpr(ll n)
{
  if (n <= INT_MAX)
    return isprime32_constexpr(n);
  if (n % 2 == 0)
    return false;
  ll d = n - 1;
  while (d % 2 == 0)
    d /= 2;
  constexpr ll bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  for (ll a : bases)
  {
    ll t = d;
    ll y = powmod64_constexpr(a, t, n);
    while (t != n - 1 && y != 1 && y != n - 1)
    {
      y = (u128(y) * y) % n;
      t <<= 1;
    }
    if (y != n - 1 && t % 2 == 0)
      return false;
  }
  return true;
}
template <int n>
constexpr bool isprime32 = isprime32_constexpr(n);
template <ll n>
constexpr bool isprime64 = isprime64_constexpr(n);

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

inline constexpr ull inv64(ull a)
{
  ull x = a;
  while (a * x != 1) x *= 2 - a * x;
  return x;
}

struct montgomery64odd
{
  ull m, im, sq;
  // sq = (2^64)^2 % m = (2^128 - m) % m = (-m % 2^128) % m
  explicit montgomery64odd(ull m) : m(m), im(inv64(m)), sq(-u128(m) % m) {}
  ull umod() const { return m; }
  ull reduce(u128 x) const
  {
    auto t = (x + u128(m) * (-im * ull(x))) >> 64;
    if (t >= m)
      t -= m;
    return (ull)t;
  }
  ull inv_reduce(i128 v) const
  { return reduce(u128(v % m + m) * sq); }
};

// https://www.mathenachia.blog/even-mod-montgomery-impl/
struct montgomery64
{
  ull m, mx, imx, d, q;
  uint b;

  explicit montgomery64(ull m) : m(m)
  {
    b = countr_zero(m), mx = m >> b;  // m == 2^b * mx, mx is odd
    imx = inv64(mx);
    d = powmod64_constexpr((mx + 1) / 2, b, mx);  // 2^{-b} mod mx
    u128 sq = -u128(mx) % mx;  // 2^128 mod mx
    q = (1 + (((sq - 1) * d) << b)) % m;
  }
  ull umod() const { return m; }
  ull reduce(u128 x) const
  {
    ull p = x & MASK(b);  // x mod 2^b
    x = (x >> b) + p * d;
    ull y = p << (64 - b);
    auto t = (x + u128(mx) * (imx * (y - ull(x)))) >> (64 - b);
    if (t >= m)
    {
      t -= m;
      if (t >= m)
        t -= m;
    }
    return (ull)t;
  }
  ull inv_reduce(i128 v) const
  { return reduce(u128(v % m + m) * q); }
};

}