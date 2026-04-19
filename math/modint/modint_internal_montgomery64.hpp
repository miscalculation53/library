#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "modint_internal_isprime.hpp"

namespace internal
{

inline constexpr ull inv64(ull a)
{
  ull x = a;
  while (a * x != 1) x *= 2 - a * x;
  return x;
}

struct montgomery64odd
{
  ull m, im, sq;
  explicit montgomery64odd(ull m) : m(m), im(inv64(m)), sq(-u128(m) % m) {}
  ull umod() const { return m; }
  ull reduce(u128 x) const
  {
    auto t = (x + u128(m) * (-im * ull(x))) >> 64;
    if (t >= m) t -= m;
    return (ull)t;
  }
  ull inv_reduce(i128 v) const { return reduce(u128(v % m + m) * sq); }
};

// https://www.mathenachia.blog/even-mod-montgomery-impl/
struct montgomery64
{
  ull m, mx, imx, d, q;
  uint b;
  explicit montgomery64(ull m) : m(m)
  {
    b = countr_zero(m), mx = m >> b; 
    imx = inv64(mx);
    d = powmod_constexpr((mx + 1) / 2, b, mx); 
    u128 sq = -u128(mx) % mx; 
    q = (1 + (((sq - 1) * d) << b)) % m;
  }
  ull umod() const { return m; }
  ull reduce(u128 x) const
  {
    ull p = x & MASK(b); 
    x = (x >> b) + p * d;
    ull y = p << (64 - b);
    auto t = (x + u128(mx) * (imx * (y - ull(x)))) >> (64 - b);
    if (t >= m) { t -= m; if (t >= m) t -= m; }
    return (ull)t;
  }
  ull inv_reduce(i128 v) const { return reduce(u128(v % m + m) * q); }
};

template <int id>
struct policy_montgomery64_odd
{
  using value_type = ull;
  using calc_type = u128;
  using mod_type = ll;

  static constexpr bool is_prime = false;
  static inline montgomery64odd reducer{(1LL << 61) - 1};
  static void set_mod(mod_type m) { reducer = montgomery64odd(m); }
  static mod_type mod() { return reducer.umod(); }
  static value_type umod() { return reducer.umod(); }
  static value_type init(value_type v) { return reducer.inv_reduce(v); }
  static mod_type val(value_type v) { return reducer.reduce(v); }
  static value_type mul(value_type a, value_type b) { return reducer.reduce((calc_type)a * b); }
};

template <int id>
struct policy_montgomery64
{
  using value_type = ull;
  using calc_type = u128;
  using mod_type = ll;

  static constexpr bool is_prime = false;
  static inline montgomery64 reducer{(1LL << 61) - 1};
  static void set_mod(mod_type m) { reducer = montgomery64(m); }
  static mod_type mod() { return reducer.umod(); }
  static value_type umod() { return reducer.umod(); }
  static value_type init(value_type v) { return reducer.inv_reduce(v); }
  static mod_type val(value_type v) { return reducer.reduce(v); }
  static value_type mul(value_type a, value_type b) { return reducer.reduce((calc_type)a * b); }
};

};
