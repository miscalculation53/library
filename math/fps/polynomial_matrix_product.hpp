#pragma once

#include "shift_of_sampling_points.hpp"
#include "../linalg/matrix.hpp"

/**
 * @brief 多項式行列の連続積
 * @docs docs/math/fps/polynomial_matrix_product.md
 */

namespace internal
{
// A(first+n-1) ... A(first)。d > 0 のとき 0 <= n <= mint::mod()。
template <class mint>
Matrix<FieldAddSubMulDiv<mint>> polynomial_matrix_product_short(
    const vvc<FormalPowerSeries<mint>> &a, mint first, ll n, int d)
{
  using M = Matrix<FieldAddSubMulDiv<mint>>;
  const int r = a.size();
  auto eval = [&](mint x)
  {
    M res(r, r);
    repi(i, r) repi(j, r) res[i][j] = a[i][j].eval(x);
    return res;
  };
  M res(r, r, 1);
  if (n == 0 || r == 0) return res;
  if (d == 0) return eval(first).pow(n);

  // d * block^2 >= n となる最小の 2 冪。比較は ll の積を避ける。
  ll block = 1;
  while (block <= (n - 1) / block / d) block *= 2;
  if (n <= 32 || d >= n || block > (mint::mod() - 1) / d)
  {
    for (ll i = 0; i < n; ++i) res = eval(first + mint(i)) * res;
    return res;
  }

  // values[i][j][t] = (A(first+block*t+s-1) ... A(first+block*t))[i][j]
  // 各成分は t の d*s 次以下の多項式。
  vvvc<mint> values(r, vvc<mint>(r, vc<mint>(d + 1)));
  repi(i, r) repi(j, r) repi(t, d + 1)
    values[i][j][t] = a[i][j].eval(first + mint(block) * t);
  const mint iblock = mint(block).inv();
  for (ll s = 1; s < block; s *= 2)
  {
    const int len = 2 * d * s + 1;
    vvvc<mint> left(r, vvc<mint>(r)), right(r, vvc<mint>(r));
    repi(i, r) repi(j, r)
    {
      left[i][j] = shift_of_sampling_points_many(values[i][j], mint(0), len);
      right[i][j] = shift_of_sampling_points_many(values[i][j], mint(s) * iblock, len);
    }
    values.assign(r, vvc<mint>(r, vc<mint>(len)));
    repi(i, r) repi(k, r) repi(j, r) repi(t, len)
      values[i][j][t] += right[i][k][t] * left[k][j][t];
  }
  for (ll t = 0; t < n / block; ++t)
  {
    M b(r, r);
    repi(i, r) repi(j, r) b[i][j] = values[i][j][t];
    res = b * res;
  }
  for (ll i = n / block * block; i < n; ++i)
    res = eval(first + mint(i)) * res;
  return res;
}
} // namespace internal

// A(r-1) ... A(l)。空区間は単位行列。mint の法は素数。
template <class mint>
Matrix<FieldAddSubMulDiv<mint>> polynomial_matrix_product(
    const vvc<FormalPowerSeries<mint>> &a, ll l, ll r)
{
  assert(0 <= l && l <= r);
  const int dim = a.size();
  int d = 0;
  for (const auto &row : a)
  {
    assert(SZ(row) == dim);
    for (const auto &f : row)
    {
      int len = f.sz();
      while (len > 0 && f[len - 1] == 0) --len;
      chmax(d, len - 1);
    }
  }
  using M = Matrix<FieldAddSubMulDiv<mint>>;
  const ll n = r - l, p = mint::mod();
  if (n == 0 || dim == 0) return M(dim, dim, 1);
  auto product = [&](ll len)
  {
    return internal::polynomial_matrix_product_short(a, mint(l), len, d);
  };
  if (d == 0 || n <= p) return product(n);
  // A(x+p)=A(x)。先頭から 1 周期分の積を繰り返し、端数を左に掛ける。
  return product(n % p) * product(p).pow(n / p);
}
