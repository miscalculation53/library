#pragma once

#include "../math/linalg/matrix.hpp"
#include "../math/linalg/linear_basis.hpp"
#include "../math/linalg/sparse_matrix.hpp"

/**
 * @brief 線形マトロイド
 * @docs docs/matroid/linear_matroid.md
 */

template <class F = FieldAddSubMulDiv<modint998244353>>
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

  CPP_DUMP_DEFINE_DATA(dimension_, columns_, selected_);

  int size() const { return int(columns_.size()); }
  int dimension() const { return dimension_; }
  const vvc<S> &columns() const { return columns_; }

  Matrix<F> matrix() const
  {
    Matrix<F> result(dimension_, size());
    for (int e = 0; e < size(); e++) for (int i = 0; i < dimension_; i++)
      result[i][e] = columns_[e][i];
    return result;
  }

  template <class I = int>
  void build(const vc<I> &independent)
  {
    selected_.assign(ALL(independent));
    fill(used_.begin(), used_.end(), 0);
    basis_.clear();
    for (auto value : independent)
    {
      assert(u128(value) < unsigned(size()));
      const int e = int(value);
      assert(!used_[e]);
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

namespace internal
{
template <class F, bool Sparse>
class LinearRepresentationBuilder
{
  using S = typename F::S;
  int dimension_, size_;
  conditional_t<Sparse, vc<tuple<int, int, S>>, vvc<S>> data_;

public:
  LinearRepresentationBuilder(int dimension, int size)
      : dimension_(dimension), size_(size), data_([&]
        {
          if constexpr (Sparse) return vc<tuple<int, int, S>>{};
          else return vvc<S>(size, vc<S>(dimension, F::e0()));
        }()) {}
  void set(int element, int coordinate, const S &value)
  {
    if constexpr (Sparse)
    {
      if (value != F::e0()) data_.emplace_back(coordinate, element, value);
    }
    else data_[element][coordinate] = value;
  }
  auto finish() &&
  {
    if constexpr (Sparse) return SparseMatrix<F>(dimension_, size_, data_);
    else return LinearMatroid<F>(dimension_, std::move(data_));
  }
};
}
