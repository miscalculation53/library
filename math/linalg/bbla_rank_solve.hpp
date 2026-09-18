#pragma once

#include "bbla.hpp"

/**
 * @brief Black Box Linear Algebra（rank・特異な連立方程式）
 * @docs docs/math/linalg/bbla.md
 */

namespace bbla
{
namespace detail
{
template <class F>
bool is_zero(const vc<typename F::S> &v)
{
  return all_of(v.begin(), v.end(), [](const auto &x) { return x == F::e0(); });
}

template <class F, class RandomSample>
vc<typename F::S> random_diagonal(int n, const RandomSample &sample)
{
  vc<typename F::S> d(n);
  for (auto &x : d) do { x = sample(); } while (x == F::e0());
  return d;
}

template <class F>
void scale_diagonal(vc<typename F::S> &v, const vc<typename F::S> &d)
{
  assert(v.size() == d.size());
  repi(i, v.size()) v[i] = F::mul(v[i], d[i]);
}

// m(0) != 0 のとき、m(A)b = 0 から解候補を作る。呼出側で検算する。
template <class F, class LinearMap>
vc<typename F::S> inverse_action(const LinearMap &linear_map,
                                const vc<typename F::S> &b, const vc<typename F::S> &m)
{
  assert(!m.empty() && m[0] != F::e0());
  auto scale = F::minus(F::inv(m[0]));
  vc<typename F::S> p(m.begin() + 1, m.end());
  for (auto &x : p) x = F::mul(x, scale);
  return apply_polynomial<F>(linear_map, b, p);
}
} // namespace detail

// apply: S^cols -> S^rows, transpose: S^rows -> S^cols（ともに破壊的）。
// Monte Carlo: 過小評価することがある。trials 回の最大を返す。
// d=min(rows,cols), T=作用と転置作用の合計時間として O(trials*d*(rows+cols+T)) 時間。
// 作業領域 O(rows+cols)。サンプル集合は十分大きいこと。
template <class F, class LinearMap, class TransposeMap,
          class RandomSample = decltype(random_sample_mint<typename F::S>)>
int rank(int rows, int cols, const LinearMap &apply, const TransposeMap &transpose,
         int trials = 2, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  assert(rows >= 0 && cols >= 0 && trials > 0);
  const int d = min(rows, cols);
  if (d == 0) return 0;
  int result = 0;
  repi(trial, trials)
  {
    auto dr = detail::random_diagonal<F>(rows, random_sample);
    auto dc = detail::random_diagonal<F>(cols, random_sample);
    auto gram = [&](vc<S> &v)
    {
      if (cols <= rows)
      {
        apply(v);
        detail::scale_diagonal<F>(v, dr);
        transpose(v);
        detail::scale_diagonal<F>(v, dc);
      }
      else
      {
        transpose(v);
        detail::scale_diagonal<F>(v, dc);
        apply(v);
        detail::scale_diagonal<F>(v, dr);
      }
    };
    auto m = minimal_polynomial<F>(d, gram, random_sample);
    int valuation = 0;
    while (m[valuation] == F::e0()) valuation++;
    result = max(result, int(m.size()) - 1 - valuation);
    if (result == d) break;
  }
  return result;
}

template <class S>
struct LinearSolution
{
  bool solvable;
  vc<S> solution;    // solvable == true: A * solution == b
  vc<S> certificate; // solvable == false: A^T * certificate == 0, certificate^T * b != 0
};

// 特異・長方形も可。解があれば一つ、なければ不整合の証明ベクトルを返す。
// 解・証明は毎回検算する。失敗した乱択は再試行する。
// 1 試行 O(d*(rows+cols+T)) 時間、O(rows+cols) 空間、d=min(rows,cols)。
template <class F, class LinearMap, class TransposeMap,
          class RandomSample = decltype(random_sample_mint<typename F::S>)>
LinearSolution<typename F::S> solve_general(int rows, int cols, const LinearMap &apply,
                                          const TransposeMap &transpose, const vc<typename F::S> &b,
                                          const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  assert(rows >= 0 && cols >= 0 && int(b.size()) == rows);
  if (detail::is_zero<F>(b)) return {true, vc<S>(cols, F::e0()), {}};
  if (cols == 0)
  {
    vc<S> witness(rows, F::e0());
    repi(i, rows) if (b[i] != F::e0()) { witness[i] = F::e1(); break; }
    return {false, {}, witness};
  }
  while (true)
  {
    auto dr = detail::random_diagonal<F>(rows, random_sample);
    auto dc = detail::random_diagonal<F>(cols, random_sample);
    auto gram = [&](vc<S> &v)
    {
      if (cols <= rows) // D_c A^T D_r A
      {
        apply(v);
        detail::scale_diagonal<F>(v, dr);
        transpose(v);
        detail::scale_diagonal<F>(v, dc);
      }
      else // A D_c A^T D_r
      {
        detail::scale_diagonal<F>(v, dr);
        transpose(v);
        detail::scale_diagonal<F>(v, dc);
        apply(v);
        assert(int(v.size()) == rows);
      }
    };
    vc<S> rhs = b;
    if (cols <= rows)
    {
      detail::scale_diagonal<F>(rhs, dr);
      transpose(rhs);
      detail::scale_diagonal<F>(rhs, dc);
    }
    auto m = minimal_polynomial_vector<F>(gram, rhs, random_sample);
    vc<S> witness;
    if (m[0] != F::e0())
    {
      auto x = detail::inverse_action<F>(gram, rhs, m);
      if (cols > rows)
      {
        detail::scale_diagonal<F>(x, dr);
        transpose(x);
        detail::scale_diagonal<F>(x, dc);
      }
      auto ax = x;
      apply(ax);
      assert(int(ax.size()) == rows);
      if (ax == b) return {true, move(x), {}};
      witness = b;
      repi(i, rows) witness[i] = F::add(witness[i], F::minus(ax[i]));
    }
    else
    {
      // 行側 Gram の零固有成分から左核の候補を得る。
      if (cols <= rows) continue;
      vc<S> p(m.begin() + 1, m.end());
      witness = apply_polynomial<F>(gram, b, p);
    }
    detail::scale_diagonal<F>(witness, dr);
    if (dot_product<F>(rows, witness.begin(), b.begin()) == F::e0()) continue;
    auto atw = witness;
    transpose(atw);
    assert(int(atw.size()) == cols);
    if (detail::is_zero<F>(atw)) return {false, {}, move(witness)};
  }
}
} // namespace bbla
