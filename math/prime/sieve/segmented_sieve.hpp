#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/prime/sieve/linear_sieve.hpp"

/**
 * @brief 区間篩
 * @docs docs/math/prime/sieve/segmented_sieve.md
 */

// l <= x <= r, p | x, p != x を満たす素数 p と整数 x の組すべてに対して f(p, x) を実行
// m = max(r - l, √r) として O(m loglog m)
template <class F>
void segmented_sieve(ll l, ll r, const F &f)
{
  assert(1 <= l && l <= r);
  LinearSieve::reserve(sqrtl(r) + 1);
  vc<ll> rem(r - l + 1);
  repi(i, rem.size()) rem[i] = l + i;
  for (ll p : LinearSieve::primes)
  {
    if (p > r / p)
      break;
    rep(x, max(2 * p, divceil(l, p) * p), r + 1, p)
    {
      f(p, x);
      while (rem[x - l] % p == 0)
        rem[x - l] /= p;
    }
  }
  repi(i, rem.size())
  {
    const ll x = l + i;
    if (rem[i] > 1 && rem[i] != x)
      f(rem[i], x);
  }
}
