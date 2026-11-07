#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 一次式の剰余の最小値
 * @docs docs/math/mod_of_linear/min_of_mod_of_linear.md
 */

namespace internal
{
template <class W>
ll min_of_mod_of_linear_impl(ll n, ll m, ll a, ll b)
{
  ll ans = b;
  for (;;)
  {
    if (a > m / 2)
    {
      b = (W(a) * (n - 1) + b) % m;
      a = m - a;
    }
    chmin(ans, b);
    const ll wraps = (W(a) * (n - 1) + b) / m;
    if (wraps == 0 || ans == 0) return ans;
    n = wraps;
    b = (b - m) % a;
    if (b < 0) b += a;
    const ll next_a = (-m) % a;
    m = a;
    a = next_a < 0 ? next_a + m : next_a;
  }
}
}

// min_{0 <= i < n} (a*i+b) mod m
inline ll min_of_mod_of_linear(ll n, ll m, ll a, ll b)
{
  assert(n >= 1 && m >= 1);
  a %= m;
  b %= m;
  if (a < 0) a += m;
  if (b < 0) b += m;
  if ((ull(n) | ull(m)) <= numeric_limits<uint>::max())
    return internal::min_of_mod_of_linear_impl<ull>(n, m, a, b);
  return internal::min_of_mod_of_linear_impl<u128>(n, m, a, b);
}
