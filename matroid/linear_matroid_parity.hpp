#pragma once

#include "internal_linear_matroid.hpp"

/**
 * @brief 線形マトロイドパリティ（乱択）
 * @docs docs/matroid/linear_matroid_parity.md
 */

template <class F, class Sample>
int linear_matroid_parity_size(const LinearMatroid<F> &matroid,
                               const vc<pair<int, int>> &pairs,
                               Sample &&sample, int trials = 1)
{
  assert(trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto [columns, work_pairs] = parity_representation(matroid, pairs);
  int best = 0;
  for (int trial = 0; trial < trials; trial++)
    best = max(best, parity_rank<F>(columns, work_pairs, 0, sample));
  return best;
}

template <class F, class Sample>
int linear_matroid_parity_size(const LinearMatroid<F> &matroid,
                               Sample &&sample, int trials = 1)
{
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(matroid.size());
  return linear_matroid_parity_size(matroid, pairs, std::forward<Sample>(sample), trials);
}

template <class F, class Sample>
vc<int> linear_matroid_parity(const LinearMatroid<F> &matroid,
                              const vc<pair<int, int>> &pairs,
                              Sample &&sample, int trials = 1)
{
  assert(trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto [original, work_pairs] = parity_representation(matroid, pairs);
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

template <class F, class Sample>
vc<int> linear_matroid_parity(const LinearMatroid<F> &matroid,
                              Sample &&sample, int trials = 1)
{
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(matroid.size());
  return linear_matroid_parity(matroid, pairs, std::forward<Sample>(sample), trials);
}
