#pragma once

#include "modint.hpp"
#include "../../ds/hash_map.hpp"
#include "../prime/large/order_primitive_root.hpp"

/**
 * @brief 前計算による O(1) 累乗・逆元 modint
 * @docs docs/math/modint/modint_fast.md
 */

// https://maspypy.com/o1-mod-inv-mod-pow
namespace internal
{

template <int p>
struct modint_fast_table
{
  static_assert(isprime<p>, "fast_modint requires a prime modulus");

private:
  static constexpr int frac_shift = []
  {
    int s = 0;
    while ((1LL << (3 * s)) < p) ++s;
    return s;
  }();
  static constexpr int width = 1 << frac_shift;
  static constexpr int pow_shift = []
  {
    int s = 0;
    while ((1LL << (2 * s)) < p) ++s;
    return s;
  }();
  static constexpr int pow_width = 1 << pow_shift;

  vc<pair<uint16_t, uint16_t>> frac;
  vc<uint> inverse, logarithm, low, high;

  // 各区間の中央を挟む Farey 分数のうち、分母が小さい方を記録する。
  int build_frac()
  {
    const int n = min(width, p - 1);
    int a = 0, b = 1, c = 1, d = n, limit = n;
    frac.resize((ll(p) + width - 1) / width);
    for (int i = 0; i < int(frac.size()); ++i)
    {
      const ll l = ll(i) * width, r = min<ll>(p - 1, l + width - 1);
      while (2LL * c * p < (l + r) * d)
      {
        const int k = (n + b) / d;
        const int e = k * c - a, f = k * d - b;
        a = c, b = d, c = e, d = f;
      }
      const int num = b <= d ? a : c, den = min(b, d);
      frac[i] = {num, den};
      limit = max<ll>(limit, max(abs(den * l - ll(num) * p), abs(den * r - ll(num) * p)));
    }
    assert(limit < p);
    return limit;
  }

  uint root_pow(uint e) const
  {
    return ull(low[e & (pow_width - 1)]) * high[e >> pow_shift] % p;
  }

  void build_pow(uint root)
  {
    low.resize(pow_width);
    high.resize((p - 2) / pow_width + 1);
    low[0] = high[0] = 1;
    for (int i = 1; i < pow_width; ++i)
      low[i] = ull(low[i - 1]) * root % p;
    const uint step = ull(low.back()) * root % p;
    for (int i = 1; i < int(high.size()); ++i)
      high[i] = ull(high[i - 1]) * step % p;
  }

  void build_log(int limit, uint root)
  {
    logarithm.resize(limit + 1);
    int small = 1;
    while (small < limit && ll(small + 1) * (small + 1) < p) ++small;
    vc<int> lpf(small + 1), primes;
    for (int i = 2; i <= small; ++i)
    {
      if (lpf[i] == 0) lpf[i] = i, primes.push_back(i);
      for (int q : primes)
      {
        if (q > lpf[i] || q > small / i) break;
        lpf[q * i] = q;
      }
    }

    // 小さい素数の離散対数を、baby step を共用した BSGS で求める。
    const uint step = max(1, min(2 * limit, int(ceil(sqrt(double(p - 1) * primes.size())))));
    HashMap<uint, uint> baby;
    baby.reserve(step);
    uint x = 1;
    for (uint i = 0; i < step; ++i)
    {
      baby[x] = i;
      x = ull(x) * root % p;
    }
    const uint backward = static_modint32<p>(x).inv().val();
    for (int q : primes)
    {
      x = q;
      for (ull e = 0; ; e += step, x = ull(x) * backward % p)
      {
        if (const uint *v = baby.find_ptr(x))
        {
          logarithm[q] = (e + *v) % (p - 1);
          break;
        }
      }
    }
    for (int i = 2; i <= small; ++i)
      if (lpf[i] != i)
        logarithm[i] = (ull(logarithm[lpf[i]]) + logarithm[i / lpf[i]]) % (p - 1);

    // i > sqrt(p) なら p / i, p % i < i。i = -(p % i) / (p / i) を使う。
    for (int i = small + 1; i <= limit; ++i)
      logarithm[i] = (ull(logarithm[p % i]) + (p - 1) / 2 + (p - 1) - logarithm[p / i]) % (p - 1);
  }

  modint_fast_table()
  {
    const int limit = build_frac();
    const uint root = primitive_root(p, factorize(p - 1));
    build_pow(root);
    build_log(limit, root);
    inverse.resize(limit + 1);
    inverse[1] = 1;
    for (int i = 2; i <= limit; ++i)
      inverse[i] = p - ull(p / i) * inverse[p % i] % p;
  }

  uint log(uint x) const
  {
    const auto [a, b] = frac[x >> frac_shift];
    const ll t = ll(x) * b - ll(a) * p;
    ll e = ll(logarithm[abs(t)]) - logarithm[b];
    if (t < 0) e += (p - 1) / 2;
    if (e < 0) e += p - 1;
    if (e >= p - 1) e -= p - 1;
    return e;
  }

public:
  static const modint_fast_table &get()
  {
    static const modint_fast_table table;
    return table;
  }

  template <class T>
  uint pow(uint x, T e) const
  {
    assert(e >= 0);
    if (x == 0) return e == 0 ? 1 : 0;
    const uint reduced = e % (p - 1);
    return root_pow(ull(log(x)) * reduced % (p - 1));
  }

  uint inv(uint x) const
  {
    assert(x != 0);
    const auto [a, b] = frac[x >> frac_shift];
    const ll t = ll(x) * b - ll(a) * p;
    const uint y = ull(inverse[abs(t)]) * b % p;
    return t < 0 ? p - y : y;
  }
};

} // namespace internal

template <int p>
using fast_modint = internal::modint_impl<internal::policy_static<p>, internal::modint_fast_table<p>>;
