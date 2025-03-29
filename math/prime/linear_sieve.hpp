#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "prime_power.hpp"

/**
 * @brief 線形篩
 * @docs docs/math/prime/linear_sieve.md
 */

struct LinearSieve
{
public:
  static int n;
  static vc<PrimePower<int>> lpf_;
  static vc<int> primes;

  static void reserve(int n_)
  {
    if (n_ <= n)
      return;
    n = max(n_, 2 * n);
    lpf_.resize(n + 1);
    for (int d = 2; d <= n; d++)
    {
      if (lpf_[d].p == -1)
      {
        lpf_[d] = PrimePower<int>(d, 1, d);
        primes.eb(d);
      }
      fec(p : primes)
      {
        if (p > n / d || p > lpf_[d].p)
          break;
        if (lpf_[d].p == p)
          lpf_[p * d] = PrimePower<int>(p, lpf_[d].e + 1, lpf_[d].pe * p);
        else
          lpf_[p * d] = PrimePower<int>(p, 1, p);
      }
    }
  }

  template <class P = int>
  static PrimePower<P> lpf(int n)
  {
    assert(n >= 1);
    reserve(n);
    return lpf_[n];
  }

  static bool is_prime(int n)
  {
    if (n <= 1)
      return false;
    return lpf(n).p == n;
  }

  // 計算量: O(n の素因数の種類数) = O(log n / loglog n)
  template <class P = int>
  static vc<PrimePower<P>> factorize(int n)
  {
    assert(n >= 1);
    reserve(n);
    vc<PrimePower<P>> res;
    while (n > 1)
    {
      res.eb(lpf_[n]);
      n /= lpf_[n].pe;
    }
    return res;
  }
};
vc<PrimePower<int>> LinearSieve::lpf_{};
int LinearSieve::n{};
vc<int> LinearSieve::primes{};