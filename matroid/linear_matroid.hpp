#pragma once

#include "../math/linalg/matrix.hpp"
#include "../math/linalg/linear_basis.hpp"

/**
 * @brief 線形マトロイド
 * @docs docs/matroid/linear_matroid.md
 */

template <class F>
class LinearMatroid
{
public:
  using S = typename F::S;

private:
  int dimension_ = 0;
  vvc<S> columns_;
  vc<int> selected_;
  vc<unsigned char> used_;
  LinearBasis<F, true, void> basis_;

public:
  LinearMatroid() = default;
  LinearMatroid(int dimension, const vvc<S> &columns)
      : LinearMatroid(dimension, vvc<S>(columns)) {}
  LinearMatroid(int dimension, vvc<S> &&columns)
      : dimension_(dimension), columns_(std::move(columns)), used_(columns_.size()), basis_(dimension)
  {
    assert(dimension >= 0);
    for (const auto &column : columns_) assert(int(column.size()) == dimension_);
  }
  template <int BS>
  explicit LinearMatroid(const Matrix<F, BS> &matrix)
  {
    auto [d, n] = matrix.template shape<int>();
    dimension_ = d;
    basis_ = LinearBasis<F, true, void>(d);
    columns_.assign(n, vc<S>(d));
    used_.assign(n, 0);
    for (int e = 0; e < n; e++) for (int i = 0; i < d; i++)
      columns_[e][i] = matrix[i][e];
  }

  int size() const { return int(columns_.size()); }
  int dimension() const { return dimension_; }
  const vvc<S> &columns() const { return columns_; }

  void build(const vc<int> &independent)
  {
    selected_ = independent;
    fill(used_.begin(), used_.end(), 0);
    basis_.clear();
    for (int e : selected_)
    {
      assert(0 <= e && e < size() && !used_[e]);
      used_[e] = 1;
      bool inserted = basis_.insert(columns_[e]);
      assert(inserted);
      (void)inserted;
    }
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < size() && !used_[e]);
    auto coefficient = basis_.coordinates(columns_[e]);
    if (!coefficient) return nullopt;
    vc<int> result;
    for (int i = 0; i < int(selected_.size()); i++) if ((*coefficient)[i] != F::e0())
      result.push_back(selected_[i]);
    return result;
  }
};
