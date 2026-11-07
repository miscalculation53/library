#pragma once

#include "internal_linear_matroid.hpp"

/**
 * @brief 線形マトロイド交叉（乱択）
 * @docs docs/matroid/linear_matroid_intersection.md
 */

template <class F, class Sample>
int linear_matroid_intersection_size(const LinearMatroid<F> &first,
                                     const LinearMatroid<F> &second,
                                     Sample &&sample, int trials = 1)
{
  assert(first.size() == second.size() && trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto a = compressed_columns(first), b = compressed_columns(second);
  int best = 0;
  for (int trial = 0; trial < trials; trial++)
    best = max(best, intersection_rank<F>(a, b, 0, sample));
  return best;
}

template <class F, class Sample>
vc<int> linear_matroid_intersection(const LinearMatroid<F> &first,
                                    const LinearMatroid<F> &second,
                                    Sample &&sample, int trials = 1)
{
  assert(first.size() == second.size() && trials >= 1);
  using namespace internal::linear_matroid_detail;
  auto original_a = compressed_columns(first), original_b = compressed_columns(second);
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
