#pragma once

#include "../math/linalg/xor_basis.hpp"
#include "linear_matroid.hpp"

/** 行数は整数型のビット幅、または DynamicBitset の長さ（DynamicBitset の入力が空なら 0）。
 *  列数は vectors.size()。各列の非零数は対応する入力ベクトルの 1 ビットの数。 */
template <class F = FieldAddSubMulDiv<static_modint32<2>>, class V, bool Sparse = false>
auto binary_linear_representation(const vc<V> &vectors)
{
  using Bits = internal::XorBasisVector<V>;
  const int width = [&]
  {
    if constexpr (is_same_v<V, DynamicBitset>) return vectors.empty() ? 0 : vectors[0].size();
    else return Bits::default_dimension;
  }();
  assert(F::add(F::e1(), F::e1()) == F::e0());
  internal::LinearRepresentationBuilder<F, Sparse> columns(width, vectors.size());
  for (int e = 0; e < int(vectors.size()); e++)
  {
    assert(Bits::valid(vectors[e], width));
    for (int b = 0; b < width; b++) if (Bits::test(vectors[e], b)) columns.set(e, b, F::e1());
  }
  return std::move(columns).finish();
}

/**
 * @brief binary matroid（XOR 基底）
 * @docs docs/matroid/binary_matroid.md
 */
template <class V = unsigned long long>
class BinaryMatroid
{
public:
  using UInt = V;
  using Value = V;

private:
  vc<V> vectors;
  XorBasis<V, true, void> basis;
  vc<int> independent;
  vc<bool> selected;

  static int infer_dimension(const vc<V> &values)
  {
    if constexpr (is_same_v<V, DynamicBitset>) return values.empty() ? 0 : values[0].size();
    else return internal::XorBasisVector<V>::default_dimension;
  }

public:
  explicit BinaryMatroid(vc<V> vectors)
      : vectors(std::move(vectors)), basis(infer_dimension(this->vectors)), selected(this->vectors.size())
  {
    for (const auto &value : this->vectors)
      assert(internal::XorBasisVector<V>::valid(value, basis.dimension()));
  }

  CPP_DUMP_DEFINE_DATA(vectors, independent, basis);

  int size() const { return int(vectors.size()); }

  /** 行数は整数型のビット幅、または DynamicBitset の長さ（DynamicBitset の入力が空なら 0）。
   *  列数は入力ベクトル数。各列の非零数は対応する入力ベクトルの 1 ビットの数。 */
  template <class F = FieldAddSubMulDiv<static_modint32<2>>>
  Matrix<F> linear_representation() const
  {
    return binary_linear_representation<F>(vectors);
  }
  /** 行数は整数型のビット幅、または DynamicBitset の長さ（DynamicBitset の入力が空なら 0）。
   *  列数は入力ベクトル数。各列の非零数は対応する入力ベクトルの 1 ビットの数。 */
  template <class F = FieldAddSubMulDiv<static_modint32<2>>>
  SparseMatrix<F> sparse_linear_representation() const
  {
    return binary_linear_representation<F, V, true>(vectors);
  }
  int dimension() const { return basis.dimension(); }
  const vc<V> &columns() const { return vectors; }

  template <class I = int>
  void build(const vc<I> &s)
  {
    independent.assign(ALL(s));
    fill(ALL(selected), false);
    basis.clear();
    for (auto value : s)
    {
      assert(u128(value) < unsigned(size()));
      const int e = int(value);
      assert(!selected[e]);
      selected[e] = true;
      bool inserted = basis.insert(vectors[e]);
      assert(inserted);
      (void)inserted;
    }
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < size() && !selected[e]);
    auto coefficient = basis.coordinates(vectors[e]);
    if (!coefficient) return nullopt;
    vc<int> result;
    for (int i = 0; i < int(independent.size()); i++)
      if (internal::XorBasisVector<V>::coefficient_test(*coefficient, i)) result.push_back(independent[i]);
    return result;
  }
};
