#pragma once

#include "binomial_prefix_sum_slider.hpp"

/**
 * @brief 二項係数の prefix sum（オンライン）
 * @docs docs/math/modint/binomial_prefix_sum.md
 */

template <class mint, int B = 512>
struct BinomialPrefixSum
{
  static_assert(B > 0);

private:
  vc<mint> pow2 = {1};
  vvc<mint> table = {{1}};

public:
  BinomialPrefixSum(int n = 0)
  {
    assert(mint::mod() > 2);
    reserve(n);
  }

  void reserve(int n)
  {
    assert(0 <= n && n < mint::mod());
    int s = pow2.size();
    if (n < s) return;
    Binomial<mint>::reserve(n);
    pow2.resize(n + 1);
    repi(i, s, n + 1) pow2[i] = pow2[i - 1] + pow2[i - 1];

    // table[i][j] = sum_{k=0}^{j*B} C(i*B, k)
    s = table.size();
    table.resize(n / B + 1);
    repi(i, s, table.size())
    {
      table[i].resize(i + 1);
      table[i][0] = 1;
      table[i][i] = pow2[i * B];
      BinomialPrefixSumSlider<mint> slider;
      slider.l = i * B;
      repi(j, 1, i)
      {
        slider.set(i * B, j * B);
        table[i][j] = slider.sum;
      }
    }
  }

  mint sum(int n, int k)
  {
    assert(0 <= n && n < mint::mod());
    if (k < 0) return 0;
    reserve(n);
    if (k >= n) return pow2[n];
    BinomialPrefixSumSlider<mint> slider;
    slider.l = n / B * B;
    slider.r = k / B * B;
    slider.sum = table[n / B][k / B];
    slider.set(n, k);
    return slider.sum;
  }
};
