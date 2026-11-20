#pragma once

#include "internal_linear_matroid.hpp"

/**
 * @brief 線形マトロイドパリティ（乱択）
 * @docs docs/matroid/linear_matroid_parity.md
 */

namespace internal::linear_matroid_detail
{
template <class F, class M, class I, class J, class Sample>
int dense_parity_size(const M &matroid, const vc<pair<I, J>> &pairs, Sample &sample, int trials)
{
  assert(trials >= 1);
  auto [columns, work_pairs] = parity_representation<F>(matroid, pairs);
  if (columns.empty() || columns[0].empty()) return 0;
  int best = 0;
  for (int trial = 0; trial < trials; trial++)
    best = max(best, parity_rank<F>(columns, work_pairs, 0, sample));
  return best;
}

template <class F, class M, class I, class J, class Sample>
vc<int> dense_parity_restore(const M &matroid, const vc<pair<I, J>> &pairs, Sample &sample, int trials)
{
  assert(trials >= 1);
  auto [original, work_pairs] = parity_representation<F>(matroid, pairs);
  if (original.empty() || original[0].empty()) return {};
  const int m = int(pairs.size());
  vc<int> best;
  for (int trial = 0; trial < trials; trial++)
  {
    auto columns = original;
    int need = parity_rank<F>(columns, work_pairs, 0, sample);
    vc<int> chosen;
    for (int e = 0; e < m && need > 0; e++)
    {
      auto next = columns;
      auto [a, b] = work_pairs[e];
      if (!contract<F>(next, a) || !contract<F>(next, b)) continue;
      if (need > 1 && parity_rank<F>(next, work_pairs, e + 1, sample) < need - 1)
        continue;
      columns = std::move(next);
      chosen.push_back(e);
      need--;
    }
    if (chosen.size() > best.size()) best = std::move(chosen);
  }
  return best;
}
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, int BS, class Sample, class I = int, class J = int,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_parity_size(const Matrix<F, BS> &matroid,
                               const vc<pair<I, J>> &pairs,
                               Sample &&sample, int trials = 1)
{
  return internal::linear_matroid_detail::dense_parity_size<F>(matroid, pairs, sample, trials);
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, int BS, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_parity_size(const Matrix<F, BS> &matroid,
                               Sample &&sample, int trials = 1)
{
  const int n = matroid.template shape<int>().second;
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(n);
  return linear_matroid_parity_size(matroid, pairs, std::forward<Sample>(sample), trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, int BS, class Sample, class I = int, class J = int,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_parity_restore(const Matrix<F, BS> &matroid,
                              const vc<pair<I, J>> &pairs,
                              Sample &&sample, int trials = 1)
{
  return internal::linear_matroid_detail::dense_parity_restore<F>(matroid, pairs, sample, trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, int BS, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_parity_restore(const Matrix<F, BS> &matroid,
                              Sample &&sample, int trials = 1)
{
  const int n = matroid.template shape<int>().second;
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(n);
  return linear_matroid_parity_restore(matroid, pairs, std::forward<Sample>(sample), trials);
}


// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, int BS, class I = int, class J = int>
int linear_matroid_parity_size(const Matrix<F, BS> &matroid,
                               const vc<pair<I, J>> &pairs, int trials = 1)
{
  return linear_matroid_parity_size(matroid, pairs, random_modint<typename F::S>, trials);
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, int BS>
int linear_matroid_parity_size(const Matrix<F, BS> &matroid, int trials = 1)
{
  return linear_matroid_parity_size(matroid, random_modint<typename F::S>, trials);
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, int BS>
int linear_matroid_parity_size(const Matrix<F, BS> &matroid,
                               initializer_list<pair<int, int>> pairs, int trials = 1)
{
  return linear_matroid_parity_size(matroid, vc<pair<int, int>>(pairs), trials);
}


// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, int BS, class I = int, class J = int>
vc<int> linear_matroid_parity_restore(const Matrix<F, BS> &matroid,
                               const vc<pair<I, J>> &pairs, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, pairs, random_modint<typename F::S>, trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, int BS>
vc<int> linear_matroid_parity_restore(const Matrix<F, BS> &matroid, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, random_modint<typename F::S>, trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, int BS>
vc<int> linear_matroid_parity_restore(const Matrix<F, BS> &matroid,
                               initializer_list<pair<int, int>> pairs, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, vc<pair<int, int>>(pairs), trials);
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, class Sample, class I = int, class J = int,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_parity_size(const LinearMatroid<F> &matroid,
                               const vc<pair<I, J>> &pairs,
                               Sample &&sample, int trials = 1)
{
  return internal::linear_matroid_detail::dense_parity_size<F>(matroid, pairs, sample, trials);
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_parity_size(const LinearMatroid<F> &matroid,
                               Sample &&sample, int trials = 1)
{
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(matroid.size());
  return linear_matroid_parity_size(matroid, pairs, std::forward<Sample>(sample), trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, class Sample, class I = int, class J = int,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_parity_restore(const LinearMatroid<F> &matroid,
                              const vc<pair<I, J>> &pairs,
                              Sample &&sample, int trials = 1)
{
  return internal::linear_matroid_detail::dense_parity_restore<F>(matroid, pairs, sample, trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_parity_restore(const LinearMatroid<F> &matroid,
                              Sample &&sample, int trials = 1)
{
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(matroid.size());
  return linear_matroid_parity_restore(matroid, pairs, std::forward<Sample>(sample), trials);
}


// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F, class I = int, class J = int>
int linear_matroid_parity_size(const LinearMatroid<F> &matroid,
                               const vc<pair<I, J>> &pairs, int trials = 1)
{
  return linear_matroid_parity_size(matroid, pairs, random_modint<typename F::S>, trials);
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F>
int linear_matroid_parity_size(const LinearMatroid<F> &matroid, int trials = 1)
{
  return linear_matroid_parity_size(matroid, random_modint<typename F::S>, trials);
}

// 復元なし
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(r²(n+r)) 時間
template <class F>
int linear_matroid_parity_size(const LinearMatroid<F> &matroid,
                               initializer_list<pair<int, int>> pairs, int trials = 1)
{
  return linear_matroid_parity_size(matroid, vc<pair<int, int>>(pairs), trials);
}


// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F, class I = int, class J = int>
vc<int> linear_matroid_parity_restore(const LinearMatroid<F> &matroid,
                               const vc<pair<I, J>> &pairs, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, pairs, random_modint<typename F::S>, trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F>
vc<int> linear_matroid_parity_restore(const LinearMatroid<F> &matroid, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, random_modint<typename F::S>, trials);
}

// 復元あり (選んだペア番号)
// d: 行数、n: 列数 (要素数)、r: ランク として
// 前処理: O(dn min(d, n)) 時間
// 実際の計算 (trials 回): O(nr²(n+r)) 時間
template <class F>
vc<int> linear_matroid_parity_restore(const LinearMatroid<F> &matroid,
                               initializer_list<pair<int, int>> pairs, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, vc<pair<int, int>>(pairs), trials);
}

// 復元なし、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(min(d³ + nw², d(d + nw))) 時間
template <class F, class Sample, class I = int, class J = int,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_parity_size(const SparseMatrix<F> &matroid,
                               const vc<pair<I, J>> &pairs,
                               Sample &&sample, int trials = 1)
{
  assert(trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto work_pairs = sparse_parity_pairs(matroid.template shape<int>().second, pairs);
  SparseParityRank<F> rank(matroid, work_pairs);
  vc<unsigned char> active(pairs.size(), 1);
  int best = 0;
  for (int trial = 0; trial < trials; trial++)
    best = max(best, rank(active, sample));
  return best;
}

// 復元なし、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(min(d³ + nw², d(d + nw))) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
int linear_matroid_parity_size(const SparseMatrix<F> &matroid,
                               Sample &&sample, int trials = 1)
{
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(matroid.template shape<int>().second);
  return linear_matroid_parity_size(matroid, pairs, std::forward<Sample>(sample), trials);
}

// 復元あり (選んだペア番号)、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(n * min(d³ + nw², d(d + nw))) 時間
template <class F, class Sample, class I = int, class J = int,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_parity_restore(const SparseMatrix<F> &matroid,
                              const vc<pair<I, J>> &pairs,
                              Sample &&sample, int trials = 1)
{
  assert(trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto work_pairs = sparse_parity_pairs(matroid.template shape<int>().second, pairs);
  SparseParityRank<F> evaluate(matroid, work_pairs);
  auto rank = [&](const vc<unsigned char> &active) { return evaluate(active, sample); };
  return recover_by_deletion(int(pairs.size()), rank, trials);
}

// 復元あり (選んだペア番号)、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(n * min(d³ + nw², d(d + nw))) 時間
template <class F, class Sample,
          enable_if_t<is_invocable_r_v<typename F::S, Sample &>, int> = 0>
vc<int> linear_matroid_parity_restore(const SparseMatrix<F> &matroid,
                              Sample &&sample, int trials = 1)
{
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(matroid.template shape<int>().second);
  return linear_matroid_parity_restore(matroid, pairs, std::forward<Sample>(sample), trials);
}


// 復元なし、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(min(d³ + nw², d(d + nw))) 時間
template <class F, class I = int, class J = int>
int linear_matroid_parity_size(const SparseMatrix<F> &matroid,
                               const vc<pair<I, J>> &pairs, int trials = 1)
{
  return linear_matroid_parity_size(matroid, pairs, random_modint<typename F::S>, trials);
}

// 復元なし、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(min(d³ + nw², d(d + nw))) 時間
template <class F>
int linear_matroid_parity_size(const SparseMatrix<F> &matroid, int trials = 1)
{
  return linear_matroid_parity_size(matroid, random_modint<typename F::S>, trials);
}

// 復元なし、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(min(d³ + nw², d(d + nw))) 時間
template <class F>
int linear_matroid_parity_size(const SparseMatrix<F> &matroid,
                               initializer_list<pair<int, int>> pairs, int trials = 1)
{
  return linear_matroid_parity_size(matroid, vc<pair<int, int>>(pairs), trials);
}


// 復元あり (選んだペア番号)、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(n * min(d³ + nw², d(d + nw))) 時間
template <class F, class I = int, class J = int>
vc<int> linear_matroid_parity_restore(const SparseMatrix<F> &matroid,
                               const vc<pair<I, J>> &pairs, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, pairs, random_modint<typename F::S>, trials);
}

// 復元あり (選んだペア番号)、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(n * min(d³ + nw², d(d + nw))) 時間
template <class F>
vc<int> linear_matroid_parity_restore(const SparseMatrix<F> &matroid, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, random_modint<typename F::S>, trials);
}

// 復元あり (選んだペア番号)、大きい疎行列用
// d: 行数、n: 列数 (要素数)、w: 列の非零要素の個数の上限 として
// 前処理: O(d + nw) 時間
// 実際の計算 (trials 回): O(n * min(d³ + nw², d(d + nw))) 時間
template <class F>
vc<int> linear_matroid_parity_restore(const SparseMatrix<F> &matroid,
                               initializer_list<pair<int, int>> pairs, int trials = 1)
{
  return linear_matroid_parity_restore(matroid, vc<pair<int, int>>(pairs), trials);
}
