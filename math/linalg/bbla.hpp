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

// u^T A^i v (0 <= i < count)。乱択しない。行列作用は max(0, count-1) 回。
template <class F, class LinearMap>
vc<typename F::S> krylov_sequence(const LinearMap &linear_map, const vc<typename F::S> &u,
                                 vc<typename F::S> v, int count)
{
  using S = typename F::S;
  assert(count >= 0 && u.size() == v.size());
  vc<S> a(count, F::e0());
  repi(i, count)
  {
    a[i] = dot_product<F>(u.size(), u.begin(), v.begin());
    if (i + 1 < count) linear_map(v);
  }
  return a;
}

// p[0] + p[1] A + ... を v に作用させる。p は昇冪順。
template <class F, class LinearMap>
vc<typename F::S> apply_polynomial(const LinearMap &linear_map,
                                  const vc<typename F::S> &v, const vc<typename F::S> &p)
{
  using S = typename F::S;
  const int n = v.size();
  vc<S> res(n, F::e0());
  int d = int(p.size()) - 1;
  while (d >= 0 && p[d] == F::e0()) d--;
  if (d < 0) return res;
  repi(j, n) res[j] = F::mul(p[d], v[j]);
  while (d-- > 0)
  {
    linear_map(res);
    repi(j, n) res[j] = F::add(res[j], F::mul(p[d], v[j]));
  }
  return res;
}

// m(A)v = 0 を満たす最小多項式を高確率で返す（最高次係数は -1）。
// 射影に失敗すると真の最小多項式の約数を返す。
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<typename F::S> minimal_polynomial_vector(const LinearMap &linear_map, vc<typename F::S> v,
                                            const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  if (all_of(v.begin(), v.end(), [](const S &x) { return x == F::e0(); }))
    return {F::minus(F::e1())};
  const int n = v.size();
  vc<S> u(n);
  repi(j, n) u[j] = random_sample();
  return reversed(berlekamp_massey<F>(krylov_sequence<F>(linear_map, u, move(v), 2 * n)));
}

// 線型写像 linear_map: S^n → S^n の最小多項式を求める
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<typename F::S> minimal_polynomial(int n, const LinearMap &linear_map, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  assert(n >= 0);
  vc<S> u(n, F::e0()), v(n, F::e0());
  repi(j, n) u[j] = random_sample(), v[j] = random_sample();
  return reversed(berlekamp_massey<F>(krylov_sequence<F>(linear_map, u, move(v), 2 * n)));
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
  if (all_of(y.begin(), y.end(), [](const S &x) { return x == F::e0(); }))
    return y;
  while (true)
  {
    vc<S> u(n), v = y;
    repi(j, n) u[j] = random_sample();
    BerlekampMassey<F> bm;
    bm.a.reserve(2 * n);
    int checkpoint = min(32, 2 * n);
    repi(i, 2 * n)
    {
      bm.append(dot_product<F>(n, u.begin(), v.begin()));
      if (i + 1 == checkpoint)
      {
        // 低次数の候補だけ途中で検査する。一般の場合は最後に一度だけ。
        if (i + 1 == 2 * n || bm.degree() <= (i + 1) / 4)
        {
          auto m = reversed(bm.coefficients());
          if (m[0] != F::e0())
          {
            S scale = F::minus(F::inv(m[0]));
            m.erase(m.begin());
            for (S &x : m) x = F::mul(x, scale);
            auto res = apply_polynomial<F>(linear_map, y, m);
            auto check = res;
            linear_map(check);
            if (check == y) return res;
          }
        }
        checkpoint = min(2 * n, 2 * checkpoint);
      }
      if (i + 1 < 2 * n) linear_map(v);
    }
    // 射影の取りこぼしは検算で検出し、新しい射影で再試行する。
  }
}

// 線型写像 linear_map: S^n → S^n の表現行列の行列式を求める
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
typename F::S det(int n, const LinearMap &linear_map, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  assert(n >= 0);
  if (n == 0)
    return F::e1();
  while (true)
  {
    vc<S> d(n);
    repi(i, n)
    {
      do { d[i] = random_sample(); } while (d[i] == F::e0());
    }
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
