#pragma once

#include "../template/template_all_but_modint.hpp"
#include "linear_matroid.hpp"
#include "../algebra/from_integer.hpp"

/** 行数は min(rank, distinct_points.size())、列数は distinct_points.size()。各列の非零数は高々行数。 */
template <class F = FieldAddSubMulDiv<modint998244353>, bool Sparse = false>
auto uniform_linear_representation(int rank, const vc<typename F::S> &distinct_points)
{
  using S = typename F::S;
  assert(rank >= 0);
  const int n = distinct_points.size(), dimension = min(rank, n);
  internal::LinearRepresentationBuilder<F, Sparse> columns(dimension, n);
  for (int e = 0; e < n; e++)
  {
    S power = F::e1();
    for (int i = 0; i < dimension; i++)
      columns.set(e, i, power), power = F::mul(power, distinct_points[e]);
  }
  return std::move(columns).finish();
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

  CPP_DUMP_DEFINE_DATA(n, rank_, independent);

  int size() const { return n; }

  /** 行数は min(rank, n)、列数は n（rank と n はコンストラクタの引数）。各列の非零数は高々行数。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  Matrix<F> linear_representation(const vc<typename F::S> &points) const
  {
    assert(int(points.size()) == n);
    return uniform_linear_representation<F>(rank_, points);
  }

  /** 行数は min(rank, n)、列数は n（rank と n はコンストラクタの引数）。各列の非零数は高々行数。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  Matrix<F> linear_representation() const
  {
    auto points = gen_vec(n, [&](int e)
    {
      auto point = from_integer<F>(e);
      assert(rank_ < 2 || e == 0 || point != F::e0());
      return point;
    });
    return linear_representation<F>(points);
  }

  /** 行数は min(rank, n)、列数は n（rank と n はコンストラクタの引数）。各列の非零数は高々行数。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  SparseMatrix<F> sparse_linear_representation(const vc<typename F::S> &points) const
  {
    assert(int(points.size()) == n);
    return uniform_linear_representation<F, true>(rank_, points);
  }

  /** 行数は min(rank, n)、列数は n（rank と n はコンストラクタの引数）。各列の非零数は高々行数。 */
  template <class F = FieldAddSubMulDiv<modint998244353>>
  SparseMatrix<F> sparse_linear_representation() const
  {
    auto points = gen_vec(n, [&](int e)
    {
      auto point = from_integer<F>(e);
      assert(rank_ < 2 || e == 0 || point != F::e0());
      return point;
    });
    return sparse_linear_representation<F>(points);
  }

  template <class I = int>
  void build(const vc<I> &s)
  {
    assert(int(s.size()) <= rank_);
    fill(ALL(selected), false);
    independent.assign(ALL(s));
    for (auto value : s)
    {
      assert(u128(value) < unsigned(n));
      const int e = int(value);
      assert(!selected[e]);
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
