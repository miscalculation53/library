#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fps.hpp"

/**
 * @brief 多項式の評価点シフト
 * @docs docs/math/fps/shift_of_sampling_points.md
 */

// f(i) = ys[i] (0 <= i < n) で定まる n 次未満の f(x) について f(c) を求める
// O(n)
template <class mint, class mint2>
mint shift_of_sampling_points_one(const vc<mint> &ys, const mint2 &c)
{
  const int n = ys.size();
  assert(n > 0);
  vc<mint> prodr(n + 1);
  prodr[n] = 1;
  repi(i, n - 1, -1, -1) prodr[i] = prodr[i + 1] * (c - i);
  mint res = 0, prodl = 1;
  repi(i, n)
  {
    res += prodl * prodr[i + 1] * ys[i] * ipow(-1, n - 1 - i) * Binomial<mint>::finv(i) * Binomial<mint>::finv(n - 1 - i);
    prodl *= c - i;
  }
  return res;
}

// f(i) = ys[i] (0 <= i < n) で定まる n 次未満の f(x) について f(c), ..., f(c + m - 1) を求める
// O((n+m) log (n+m))
template <class mint, class mint2>
vc<mint> shift_of_sampling_points_many(const vc<mint> &ys, const mint2 &c, int m)
{
  if (m <= 16)
  {
    vc<mint> res(m);
    rep(i, m) res[i] = shift_of_sampling_points_one(ys, c + i);
    return res;
  }
  using F = FormalPowerSeries<mint>;
  const int n = ys.size();
  assert(n > 0);
  assert(m >= 0);
  F p(n);
  repi(i, n) p[i] = i % 2 == 0 ? 1 : -1;
  F a = (F(ys).egf() * p.egf()).pre(n);
  F q(n);
  q[0] = 1;
  repi(i, 1, n) q[i] = q[i - 1] * (c - i + 1);
  F b = (a.ogf() * q.egf().rev()) >> (n - 1);
  F r(m, 1);
  return (b.egf() * r.egf()).pre(m).ogf();
}
