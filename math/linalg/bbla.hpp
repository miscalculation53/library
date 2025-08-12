#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../fps/berlekamp_massey.hpp"

/**
 * @brief Black Box Linear Algebra
 * @docs docs/math/linalg/bbla.md
 */

// https://yukicoder.me/wiki/black_box_linear_algebra

namespace bbla
{

template <class mint>
mint random_sample_mint() { return randrange(1, mint::mod()); }

// 線型写像 linear_map: S^n → S^n の最小多項式を求める
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<typename F::S> minimal_polynomial(int n, const LinearMap &linear_map, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  assert(n > 0);
  vc<S> u(n, F::e0()), v(n, F::e0());
  repi(j, n) u[j] = random_sample(), v[j] = random_sample();
  vc<S> a(2 * n + 1, F::e0());
  repi(i, 2 * n + 1)
  {
    S sm = F::e0();
    repi(j, n) sm = F::add(sm, F::mul(u[j], v[j]));
    a[i] = sm;
    linear_map(v);
  }
  return reversed(berlekamp_massey<F>(a));
}

// 線型写像 linear_map: S^n → S^n とベクトル y: S^n を受け取り、
// 線型方程式 y = linear_map(x) を解く
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 制約: linear_map は非特異
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<typename F::S> solve(const LinearMap &linear_map, vc<typename F::S> y, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  const int n = y.size();
  if (n == 0)
    return {};
  auto m = minimal_polynomial<F>(n, linear_map, random_sample);
  assert(m[0] != F::e0());
  S minus_inv_m0 = F::minus(F::inv(m[0]));
  vc<S> res(n, F::e0());
  repi(i, 1, m.size())
  {
    S coef = F::mul(m[i], minus_inv_m0);
    repi(j, n) res[j] = F::add(res[j], F::mul(coef, y[j]));
    linear_map(y);
  }
  return res;
}

// 線型写像 linear_map: S^n → S^n の表現行列の行列式を求める
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
typename F::S det(int n, const LinearMap &linear_map, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  if (n == 0)
    return 1;
  while (true)
  {
    vc<S> d(n);
    repi(i, n) d[i] = random_sample();
    auto linear_map_ad = [&](vc<S> &v)
    {
      repi(i, n) v[i] = F::mul(v[i], d[i]);
      linear_map(v);
    };
    auto m = minimal_polynomial<F>(n, linear_map_ad, random_sample);
    if (m[0] == F::e0())
      return F::e0();
    if (SZ(m) != n + 1)
      continue;
    S detd = F::e1();
    fec(di : d) detd = F::mul(detd, di);
    S res = F::mul(m[0], F::inv(detd));
    return n & 1 ? res : F::minus(res);
  }
}

} // namespace bbla
