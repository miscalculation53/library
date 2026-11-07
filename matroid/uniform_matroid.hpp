#pragma once

#include "../template/template_all_but_modint.hpp"
#include "linear_matroid.hpp"

template <class F>
LinearMatroid<F> uniform_linear_representation(int rank, const vc<typename F::S> &distinct_points)
{
  using S = typename F::S;
  assert(rank >= 0);
  const int n = distinct_points.size(), dimension = min(rank, n);
  vvc<S> columns(n, vc<S>(dimension, F::e0()));
  for (int e = 0; e < n; e++)
  {
    S power = F::e1();
    for (int i = 0; i < dimension; i++)
      columns[e][i] = power, power = F::mul(power, distinct_points[e]);
  }
  return LinearMatroid<F>(dimension, columns);
}

/**
 * @brief 一様マトロイド
 * @docs docs/matroid/uniform_matroid.md
 */
class UniformMatroid
{
  int n, rank_;
  vc<int> independent;
  vc<bool> selected;

public:
  UniformMatroid(int n, int rank) : n(n), rank_(rank), selected(n)
  { assert(n >= 0 && rank >= 0); }

  int size() const { return n; }

  template <class F>
  LinearMatroid<F> linear_representation(const vc<typename F::S> &points) const
  {
    assert(int(points.size()) == n);
    return uniform_linear_representation<F>(rank_, points);
  }

  void build(const vc<int> &s)
  {
    assert(int(s.size()) <= rank_);
    fill(ALL(selected), false);
    independent = s;
    for (int e : s)
    {
      assert(0 <= e && e < n && !selected[e]);
      selected[e] = true;
    }
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < n && !selected[e]);
    if (int(independent.size()) < rank_) return nullopt;
    return independent;
  }
};
