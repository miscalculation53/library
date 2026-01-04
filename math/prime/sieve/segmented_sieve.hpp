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
  LinearSieve::reserve(sqrtl(r) + 1);
  for(ll p : LinearSieve::primes) rep(x, max(2 * p, divceil(l, p) * p), r + 1, p) f(p, x);
}
