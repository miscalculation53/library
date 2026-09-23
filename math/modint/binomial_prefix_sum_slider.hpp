#pragma once

#include "binomial.hpp"

/**
 * @brief 二項係数の prefix sum（Mo 用 slider）
 * @docs docs/math/modint/binomial_prefix_sum_slider.md
 */

template <class mint>
struct BinomialPrefixSumSlider
{
private:
  mint inv2;

public:
  int l = 0, r = 0;
  mint sum = 1;

  BinomialPrefixSumSlider(int max_n = 0)
  {
    assert(mint::mod() > 2);
    assert(0 <= max_n && max_n < mint::mod());
    Binomial<mint>::reserve(max_n);
    inv2 = mint(mint::mod() / 2 + 1);
  }

  // l = n, r = k, sum = C(l, 0) + ... + C(l, r)
  void lpp()
  {
    assert(l + 1 < mint::mod());
    sum += sum - Binomial<mint>::C(l, r);
    ++l;
  }
  void lmm()
  {
    assert(l > 0);
    --l;
    sum = (sum + Binomial<mint>::C(l, r)) * inv2;
  }
  void rpp()
  {
    ++r;
    sum += Binomial<mint>::C(l, r);
  }
  void rmm()
  {
    assert(r >= 0);
    sum -= Binomial<mint>::C(l, r);
    --r;
  }

  void set(int nl, int nr)
  {
    assert(0 <= nl && nl < mint::mod() && nr >= -1);
    while (nl < l) lmm();
    while (r < nr) rpp();
    while (l < nl) lpp();
    while (nr < r) rmm();
  }
};
