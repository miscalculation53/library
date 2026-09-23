#pragma once

#include "polynomial_matrix_product.hpp"

/**
 * @brief P-recursive な漸化式の推定・項計算
 * @docs docs/math/fps/p_recursive.md
 */

// sum[j=0..r] c[j](n) * a[n+j] = 0。1 <= r <= max_order, deg(c[j]) <= max_degree。
// 初期項数に収まる範囲を探索し、位数、係数次数の順に小さい候補を返す。
// 見つからない場合（項数不足を含む）は空配列。
template <class mint>
vc<FormalPowerSeries<mint>> guess_p_recursive(
    const vc<mint> &a, int max_order = 10, int max_degree = 10)
{
  assert(max_order >= 1 && max_degree >= 0);
  using P = FormalPowerSeries<mint>;
  using M = Matrix<FieldAddSubMulDiv<mint>>;
  const int n = a.size();
  for (int r = 1; r <= max_order && r < n; ++r)
  {
    if (n - r < r + 1) break;
    const int degree = min<ll>({max_degree, (n - r) / (r + 1) - 1, mint::mod() - 1});
    const int cols = (r + 1) * (degree + 1);
    M mat(n - r, cols);
    repi(i, n - r)
    {
      mint pw = 1;
      repi(k, degree + 1)
      {
        // 次数の低い係数から列を並べ、同じ消去で低次数の候補も得る。
        repi(j, r + 1) mat[i][k * (r + 1) + j] = a[i + j] * pw;
        pw *= i;
      }
    }
    auto [ok, sol, basis] = mat.solve(vc<mint>(n - r));
    // 基底は自由変数の列順なので、先頭の有効な基底が最小次数になる。
    for (const auto &b : basis)
    {
      P last(degree + 1);
      repi(k, degree + 1) last[k] = b[k * (r + 1) + r];
      last.shrink();
      if (last.empty()) continue;
      vc<P> c(r + 1, P(degree + 1));
      mint iv = last.back().inv();
      repi(j, r + 1)
      {
        repi(k, degree + 1) c[j][k] = b[k * (r + 1) + j] * iv;
        c[j].shrink();
      }
      return c;
    }
  }
  return {};
}

// sum[j=0..r] c[j](n) * a[n+j] = 0 に従って先頭 size 項へ伸ばす。
// 制約: 必要な各遷移で c[r](n) != 0。
template <class mint>
vc<mint> p_recursive_extend(
    vc<mint> a, const vc<FormalPowerSeries<mint>> &c, int size)
{
  assert(size >= 0 && c.size() >= 2);
  const int r = c.size() - 1;
  if (size <= SZ(a))
  {
    a.resize(size);
    return a;
  }
  assert(SZ(a) >= r);
  a.reserve(size);
  while (SZ(a) < size)
  {
    const int n = a.size() - r;
    mint den = c[r].eval(mint(n)), num = 0;
    assert(den != 0 && "p_recursive_extend: zero denominator");
    repi(j, r) num -= c[j].eval(mint(n)) * a[n + j];
    a.eb(num / den);
  }
  return a;
}

// 第 k 項（0-indexed）。既知の接頭辞の末尾から計算する。
// 制約: 必要な各遷移で c[r](n) != 0。
template <class mint>
mint p_recursive_nth(
    const vc<mint> &a, const vc<FormalPowerSeries<mint>> &c, ll k)
{
  assert(k >= 0 && c.size() >= 2);
  if (k < ll(a.size())) return a[k];
  using P = FormalPowerSeries<mint>;
  const int r = c.size() - 1;
  assert(SZ(a) >= r);
  const ll l = ll(a.size()) - r, end = k - (r - 1);
  const mint den = polynomial_matrix_product<mint>({{c[r]}}, l, end)[0][0];
  assert(den != 0 && "p_recursive_nth: zero denominator");
  vvc<P> mat(r, vc<P>(r));
  repi(j, r - 1) mat[j][j + 1] = c[r];
  repi(j, r) mat[r - 1][j] = -c[j];
  auto prod = polynomial_matrix_product(mat, l, end);
  mint num = 0;
  repi(j, r) num += prod[r - 1][j] * a[l + j];
  return num / den;
}
