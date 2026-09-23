#pragma once

#include "../linalg/matrix.hpp"

/**
 * @brief 2変数有理母関数の推定・係数計算
 * @docs docs/math/fps/rational_gf_2d.md
 */

// a[i][j] = [x^i y^j] P/Q となる P, Q を求める。Q[0][0] = 1。
// 分子・分母の全次数の上限を指定する。各単項式は入力の長方形内に限定する。
// 係数数が未知数より少ない場合、または整合する式がない場合は nullopt。
template <class T>
optional<pair<vvc<T>, vvc<T>>> pade_2d(
    const vvc<T> &a, int numerator_degree, int denominator_degree)
{
  assert(numerator_degree >= 0 && denominator_degree >= 0);
  const int h = a.size(), w = h ? a[0].size() : 0;
  for (const auto &row : a) assert(SZ(row) == w);
  if (h == 0 || w == 0) return nullopt;
  vc<pair<int, int>> ps, qs;
  repi(i, h) repi(j, w)
  {
    if (i + j <= numerator_degree) ps.eb(i, j);
    if (i + j <= denominator_degree && (i || j)) qs.eb(i, j);
  }
  const int rows = h * w - ps.size(), cols = qs.size();
  if (rows < cols) return nullopt;
  Matrix<FieldAddSubMulDiv<T>> mat(rows, cols);
  vc<T> rhs(rows);
  int row = 0;
  repi(i, h) repi(j, w) if (i + j > numerator_degree)
  {
    // [x^i y^j] QF = 0。負の添字の係数は 0。
    repi(k, cols)
    {
      auto [u, v] = qs[k];
      if (u <= i && v <= j) mat[row][k] = a[i - u][j - v];
    }
    rhs[row++] = -a[i][j];
  }
  auto [ok, sol, basis] = mat.solve(rhs);
  if (!ok) return nullopt;

  vvc<T> p, q(1, vc<T>{T(1)});
  auto set = [](vvc<T> &f, int i, int j, T value)
  {
    if (value == T(0)) return;
    if (i >= SZ(f)) f.resize(i + 1);
    if (j >= SZ(f[i])) f[i].resize(j + 1);
    f[i][j] = value;
  };
  repi(k, cols) set(q, qs[k].first, qs[k].second, sol[k]);
  for (auto [i, j] : ps)
  {
    T value = a[i][j];
    repi(k, cols)
    {
      auto [u, v] = qs[k];
      if (u <= i && v <= j) value += sol[k] * a[i - u][j - v];
    }
    set(p, i, j, value);
  }
  return pair{move(p), move(q)};
}

// 分子・分母の全次数の共通上限 d を 0, 1, ... の順に試す。
// 入力に整合する候補のうち最小の d のものを返す。
template <class T>
optional<pair<vvc<T>, vvc<T>>> guess_rational_gf_2d(
    const vvc<T> &a, int max_degree = 10)
{
  assert(max_degree >= 0);
  const int h = a.size(), w = h ? a[0].size() : 0;
  for (const auto &row : a) assert(SZ(row) == w);
  if (h == 0 || w == 0) return nullopt;
  const int last = min(max_degree, h + w - 2);
  vc<int> count(last + 1);
  repi(i, h) repi(j, w) if (i + j <= last) ++count[i + j];
  int terms = 0;
  for (int d = 0; d <= last; ++d)
  {
    terms += count[d];
    if (2LL * terms - 1 > ll(h) * w) break;
    auto res = pade_2d(a, d, d);
    if (res) return res;
  }
  return nullopt;
}

// P/Q の先頭 h 行 w 列の係数。p[i][j], q[i][j] は x^i y^j の係数。
// 行ごとの長さは任意。省略された係数は 0 とする。q[0][0] != 0 が必要。
template <class T>
vvc<T> rational_gf_2d_coefficients(const vvc<T> &p, const vvc<T> &q, int h, int w)
{
  assert(h >= 0 && w >= 0);
  assert(!q.empty() && !q[0].empty() && q[0][0] != T(0));
  vvc<T> a(h, vc<T>(w));
  if (h == 0 || w == 0) return a;
  vc<tuple<int, int, T>> terms;
  repi(i, min<int>(h, q.size())) repi(j, min<int>(w, q[i].size()))
    if ((i || j) && q[i][j] != T(0)) terms.eb(i, j, q[i][j]);
  const T inv = T(1) / q[0][0];
  repi(i, h) repi(j, w)
  {
    T value = i < SZ(p) && j < SZ(p[i]) ? p[i][j] : T(0);
    for (const auto &[u, v, c] : terms)
      if (u <= i && v <= j) value -= c * a[i - u][j - v];
    a[i][j] = value * inv;
  }
  return a;
}
