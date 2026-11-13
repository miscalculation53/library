#pragma once

#include "internal_linear_matroid.hpp"

/**
 * @brief 線形マトロイド交叉（乱択）
 * @docs docs/matroid/linear_matroid_intersection.md
 */

// 復元なし
// d₁, d₂: 行数、n: 列数 (要素数)、r₁, r₂: ランク として
// 前処理: O(d₁ n min(d₁, n) + d₂ n min(d₂, n)) 時間
// 実際の計算 (trials 回): O(r₁ r₂ (n + min(r₁, r₂))) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_intersection_size(const LinearMatroid<F> &first,
                                     const LinearMatroid<F> &second,
                                     Sample &&sample, int trials = 1)
{
  assert(first.size() == second.size() && trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto a = compressed_columns(first), b = compressed_columns(second);
  if (a.empty() || a[0].empty() || b[0].empty()) return 0;
  int best = 0;
  for (int trial = 0; trial < trials; trial++)
    best = max(best, intersection_rank<F>(a, b, 0, sample));
  return best;
}

// 復元あり (要素番号)
// d₁, d₂: 行数、n: 列数 (要素数)、r₁, r₂: ランク として
// 前処理: O(d₁ n min(d₁, n) + d₂ n min(d₂, n)) 時間
// 実際の計算 (trials 回): O(n * r₁ r₂ (n + min(r₁, r₂))) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_intersection_restore(const LinearMatroid<F> &first,
                                    const LinearMatroid<F> &second,
                                    Sample &&sample, int trials = 1)
{
  assert(first.size() == second.size() && trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto original_a = compressed_columns(first), original_b = compressed_columns(second);
  if (original_a.empty() || original_a[0].empty() || original_b[0].empty()) return {};
  const int n = first.size();
  vc<int> best;
  for (int trial = 0; trial < trials; trial++)
  {
    auto a = original_a, b = original_b;
    int need = intersection_rank<F>(a, b, 0, sample);
    vc<int> chosen;
    for (int e = 0; e < n && need > 0; e++)
    {
      auto next_a = a, next_b = b;
      if (!contract<F>(next_a, e) || !contract<F>(next_b, e)) continue;
      if (need > 1 && intersection_rank<F>(next_a, next_b, e + 1, sample) < need - 1)
        continue;
      a = std::move(next_a), b = std::move(next_b);
      chosen.push_back(e);
      need--;
    }
    if (chosen.size() > best.size()) best = std::move(chosen);
  }
  return best;
}


// 復元なし
// d₁, d₂: 行数、n: 列数 (要素数)、r₁, r₂: ランク として
// 前処理: O(d₁ n min(d₁, n) + d₂ n min(d₂, n)) 時間
// 実際の計算 (trials 回): O(r₁ r₂ (n + min(r₁, r₂))) 時間
template <class F>
int linear_matroid_intersection_size(const LinearMatroid<F> &first,
                                     const LinearMatroid<F> &second, int trials = 1)
{
  return linear_matroid_intersection_size(first, second, random_modint<typename F::S>, trials);
}


// 復元あり (要素番号)
// d₁, d₂: 行数、n: 列数 (要素数)、r₁, r₂: ランク として
// 前処理: O(d₁ n min(d₁, n) + d₂ n min(d₂, n)) 時間
// 実際の計算 (trials 回): O(n * r₁ r₂ (n + min(r₁, r₂))) 時間
template <class F>
vc<int> linear_matroid_intersection_restore(const LinearMatroid<F> &first,
                                     const LinearMatroid<F> &second, int trials = 1)
{
  return linear_matroid_intersection_restore(first, second, random_modint<typename F::S>, trials);
}

// 復元なし、疎行列用
// d₁, d₂: 行数、n: 列数 (要素数)、w₁, w₂: 列の非零要素の個数の上限 として
// 前処理: O(d₁ + d₂ + n(w₁ + w₂)) 時間
// 実際の計算 (trials 回):
//   O(min(
//     d₁ d₂ min(d₁, d₂) + n w₁ w₂,
//     min(d₁, d₂) (d₁ + d₂ + n(w₁ + w₂))
//   )) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_intersection_size(const SparseMatrix<F> &first,
                                     const SparseMatrix<F> &second,
                                     Sample &&sample, int trials = 1)
{
  const int n = first.template shape<int>().second;
  assert(n == second.template shape<int>().second && trials >= 1);
  internal::linear_matroid_detail::SparseIntersectionRank<F> rank(first, second);
  vc<unsigned char> active(n, 1);
  int best = 0;
  for (int trial = 0; trial < trials; trial++) best = max(best, rank(active, sample));
  return best;
}

// 復元あり (要素番号)、疎行列用
// d₁, d₂: 行数、n: 列数 (要素数)、w₁, w₂: 列の非零要素の個数の上限 として
// 前処理: O(d₁ + d₂ + n(w₁ + w₂)) 時間
// 実際の計算 (trials 回):
//   O(n * min(
//     d₁ d₂ min(d₁, d₂) + n w₁ w₂,
//     min(d₁, d₂) (d₁ + d₂ + n(w₁ + w₂))
//   )) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_intersection_restore(const SparseMatrix<F> &first,
                                    const SparseMatrix<F> &second,
                                    Sample &&sample, int trials = 1)
{
  const int n = first.template shape<int>().second;
  assert(n == second.template shape<int>().second && trials >= 1);
  internal::linear_matroid_detail::SparseIntersectionRank<F> evaluate(first, second);
  auto rank = [&](const vc<unsigned char> &active) { return evaluate(active, sample); };
  return internal::linear_matroid_detail::recover_by_deletion(n, rank, trials);
}

// 復元なし、疎行列用
// d₁, d₂: 行数、n: 列数 (要素数)、w₁, w₂: 列の非零要素の個数の上限 として
// 前処理: O(d₁ + d₂ + n(w₁ + w₂)) 時間
// 実際の計算 (trials 回):
//   O(min(
//     d₁ d₂ min(d₁, d₂) + n w₁ w₂,
//     min(d₁, d₂) (d₁ + d₂ + n(w₁ + w₂))
//   )) 時間
template <class F>
int linear_matroid_intersection_size(const SparseMatrix<F> &first,
                                     const SparseMatrix<F> &second, int trials = 1)
{
  return linear_matroid_intersection_size(first, second, random_modint<typename F::S>, trials);
}

// 復元あり (要素番号)、疎行列用
// d₁, d₂: 行数、n: 列数 (要素数)、w₁, w₂: 列の非零要素の個数の上限 として
// 前処理: O(d₁ + d₂ + n(w₁ + w₂)) 時間
// 実際の計算 (trials 回):
//   O(n * min(
//     d₁ d₂ min(d₁, d₂) + n w₁ w₂,
//     min(d₁, d₂) (d₁ + d₂ + n(w₁ + w₂))
//   )) 時間
template <class F>
vc<int> linear_matroid_intersection_restore(const SparseMatrix<F> &first,
                                    const SparseMatrix<F> &second, int trials = 1)
{
  return linear_matroid_intersection_restore(first, second, random_modint<typename F::S>, trials);
}
