#pragma once

#include "matrix.hpp"
#include "bbla_rank_solve.hpp"
#include "internal_rank_strategy.hpp"

/**
 * @brief 疎行列
 * @docs docs/math/linalg/sparse_matrix.md
 */
template <class F = FieldAddSubMulDiv<modint998244353>>
class SparseMatrix
{
public:
  using S = typename F::S;
  using V = vc<S>;
  using Entry = pair<int, S>;

private:
  int n_ = 0, m_ = 0;
  CSR<Entry> rows_;

  static CSR<Entry> canonical_rows(CSR<Entry> rows)
  {
    const int n = rows.template size<int>();
    vc<int> sizes(n);
    for (int i = 0; i < n; i++)
    {
      auto row = rows[i];
      auto less_index = [](const Entry &a, const Entry &b) { return a.first < b.first; };
      if (!is_sorted(row.begin(), row.end(), less_index)) sort(row.begin(), row.end(), less_index);
      int end = 0;
      for (const auto &[j, x] : row)
      {
        if (end && row[end - 1].first == j)
          row[end - 1].second = F::add(row[end - 1].second, x);
        else row[end++] = {j, x};
      }
      int count = 0;
      for (int k = 0; k < end; k++) if (row[k].second != F::e0())
      {
        if (count != k) row[count] = std::move(row[k]);
        count++;
      }
      sizes[i] = count;
    }
    CSR<Entry> result(sizes);
    for (int i = 0; i < n; i++) copy_n(rows[i].begin(), sizes[i], result[i].begin());
    return result;
  }

public:
  SparseMatrix() = default;
  SparseMatrix(int n, int m) : n_(n), m_(m)
  {
    assert(n >= 0 && m >= 0);
    rows_ = CSR<Entry>(vc<int>(n));
  }

  // (行, 列, 係数) から構築する。同じ位置の係数は加算する。
  template <class I = int, class J = int>
  SparseMatrix(int n, int m, const vc<tuple<I, J, S>> &entries) : SparseMatrix(n, m)
  {
    static_assert(is_integral_ext<I> && is_integral_ext<J>);
    vc<int> sizes(n);
    for (const auto &[i, j, x] : entries)
    {
      assert(u128(i) < unsigned(n) && u128(j) < unsigned(m));
      sizes[int(i)]++;
    }
    CSR<Entry> rows(sizes);
    fill(sizes.begin(), sizes.end(), 0);
    for (const auto &[i, j, x] : entries) rows[int(i)][sizes[int(i)]++] = {int(j), x};
    rows_ = canonical_rows(std::move(rows));
  }
  SparseMatrix(int n, int m, initializer_list<tuple<int, int, S>> entries)
      : SparseMatrix(n, m, vc<tuple<int, int, S>>(entries)) {}

  // rows[i] は (列, 係数) の列。重複する位置の係数は加算する。
  template <class I, bool Erasable>
  SparseMatrix(int columns, const CSR<pair<I, S>, Erasable> &rows)
      : SparseMatrix(rows.template size<int>(), columns)
  {
    static_assert(is_integral_ext<I>);
    vc<int> sizes(n_);
    for (int i = 0; i < n_; i++) sizes[i] = rows[i].template size<int>();
    CSR<Entry> converted(sizes);
    for (int i = 0; i < n_; i++)
    {
      auto row = rows[i];
      for (int k = 0; k < sizes[i]; k++)
      {
        const auto &[j, x] = row[k];
        assert(u128(j) < unsigned(m_));
        converted[i][k] = {int(j), x};
      }
    }
    rows_ = canonical_rows(std::move(converted));
  }

  template <int BS>
  explicit SparseMatrix(const Matrix<F, BS> &matrix)
  {
    tie(n_, m_) = matrix.template shape<int>();
    vc<tuple<int, int, S>> entries;
    for (int i = 0; i < n_; i++) for (int j = 0; j < m_; j++)
      if (matrix[i][j] != F::e0()) entries.emplace_back(i, j, matrix[i][j]);
    *this = SparseMatrix(n_, m_, entries);
  }

  template <class I = ll> pair<I, I> shape() const { return {n_, m_}; }
  template <class I = ll> I size() const { return n_; }
  template <class I = ll> I nnz() const { return rows_.get_elist().size(); }
  auto operator[](int i) const { assert(0 <= i && i < n_); return rows_[i]; }
  const CSR<Entry> &rows() const { return rows_; }

  // ((行数, 列数), 非零成分の (行, 列, 係数) の列) を返す。O(n+z) 時間（n=行数、z=非零数）。
  auto content() const
  {
    vc<tuple<int, int, S>> entries;
    entries.reserve(nnz());
    for (int i = 0; i < n_; i++) for (const auto &[j, x] : rows_[i])
      entries.emplace_back(i, j, x);
    return pair{shape<int>(), std::move(entries)};
  }

  S operator()(int i, int j) const
  {
    assert(0 <= i && i < n_ && 0 <= j && j < m_);
    auto row = rows_[i];
    auto it = lower_bound(row.begin(), row.end(), j,
                          [](const Entry &entry, int column) { return entry.first < column; });
    return it != row.end() && it->first == j ? it->second : F::e0();
  }

  Matrix<F> to_dense() const
  {
    Matrix<F> result(n_, m_);
    for (int i = 0; i < n_; i++) for (const auto &[j, x] : rows_[i]) result[i][j] = x;
    return result;
  }

  SparseMatrix transpose() const
  {
    vc<int> sizes(m_);
    for (const auto &[j, x] : rows_.get_elist()) sizes[j]++;
    SparseMatrix result(m_, n_);
    result.rows_ = CSR<Entry>(sizes);
    fill(sizes.begin(), sizes.end(), 0);
    for (int i = 0; i < n_; i++) for (const auto &[j, x] : rows_[i])
      result.rows_[j][sizes[j]++] = {i, x};
    return result;
  }

  // 指定した行を順に取り出す。行番号の重複にも対応する。O(k+z) 時間（k=指定行数,z=出力非零数）。
  template <class I = int>
  SparseMatrix select_rows(const vc<I> &indices) const
  {
    static_assert(is_integral_ext<I>);
    vc<int> sizes(indices.size());
    for (int i = 0; i < int(indices.size()); i++)
    {
      assert(u128(indices[i]) < unsigned(n_));
      sizes[i] = rows_[int(indices[i])].template size<int>();
    }
    SparseMatrix result(indices.size(), m_);
    result.rows_ = CSR<Entry>(sizes);
    for (int i = 0; i < int(indices.size()); i++)
      copy(rows_[int(indices[i])].begin(), rows_[int(indices[i])].end(), result.rows_[i].begin());
    return result;
  }

  V operator*(const V &v) const
  {
    assert(int(v.size()) == m_);
    V result(n_, F::e0());
    for (int i = 0; i < n_; i++) for (const auto &[j, x] : rows_[i])
      result[i] = F::add(result[i], F::mul(x, v[j]));
    return result;
  }
  friend V operator*(const V &v, const SparseMatrix &a)
  {
    assert(int(v.size()) == a.n_);
    V result(a.m_, F::e0());
    for (int i = 0; i < a.n_; i++) for (const auto &[j, x] : a.rows_[i])
      result[j] = F::add(result[j], F::mul(v[i], x));
    return result;
  }

  /** scale を対角成分とする対角行列 D を左から掛ける（A ← DA）。 */
  SparseMatrix &scale_rows(const V &scale)
  {
    assert(int(scale.size()) == n_);
    for (int i = 0; i < n_; i++) for (auto &[j, x] : rows_[i]) x = F::mul(scale[i], x);
    rows_ = canonical_rows(std::move(rows_));
    return *this;
  }
  /** scale を対角成分とする対角行列 D を右から掛ける（A ← AD）。 */
  SparseMatrix &scale_columns(const V &scale)
  {
    assert(int(scale.size()) == m_);
    for (auto &[j, x] : rows_.get_elist()) x = F::mul(x, scale[j]);
    rows_ = canonical_rows(std::move(rows_));
    return *this;
  }
  SparseMatrix &operator*=(const S &scale)
  { return scale_rows(V(n_, scale)); }
  SparseMatrix operator*(const S &scale) const { auto result = *this; return result *= scale; }

  template <int BS>
  Matrix<F, BS> operator*(const Matrix<F, BS> &b) const
  {
    const auto [m, p] = b.template shape<int>();
    assert(m_ == m);
    Matrix<F, BS> result(n_, p);
    for (int i = 0; i < n_; i++) for (const auto &[k, x] : rows_[i])
      for (int j = 0; j < p; j++) result[i][j] = F::add(result[i][j], F::mul(x, b[k][j]));
    return result;
  }
  template <int BS>
  friend Matrix<F, BS> operator*(const Matrix<F, BS> &a, const SparseMatrix &b)
  {
    const auto [n, m] = a.template shape<int>();
    assert(m == b.n_);
    Matrix<F, BS> result(n, b.m_);
    for (int i = 0; i < n; i++) for (int k = 0; k < m; k++) if (a[i][k] != F::e0())
      for (const auto &[j, x] : b.rows_[k]) result[i][j] = F::add(result[i][j], F::mul(a[i][k], x));
    return result;
  }

  // 積を密行列として返す。
  Matrix<F> multiply_dense(const SparseMatrix &b) const
  {
    assert(m_ == b.n_);
    Matrix<F> result(n_, b.m_);
    for (int i = 0; i < n_; i++) for (const auto &[k, x] : rows_[i])
      for (const auto &[j, y] : b.rows_[k]) result[i][j] = F::add(result[i][j], F::mul(x, y));
    return result;
  }
  SparseMatrix operator*(const SparseMatrix &b) const
  {
    assert(m_ == b.n_);
    V value(b.m_, F::e0());
    vc<int> seen(b.m_, -1), touched;
    vc<tuple<int, int, S>> entries;
    for (int i = 0; i < n_; i++)
    {
      touched.clear();
      for (const auto &[k, x] : rows_[i]) for (const auto &[j, y] : b.rows_[k])
      {
        if (seen[j] != i) seen[j] = i, value[j] = F::e0(), touched.push_back(j);
        value[j] = F::add(value[j], F::mul(x, y));
      }
      for (int j : touched) if (value[j] != F::e0()) entries.emplace_back(i, j, value[j]);
    }
    return SparseMatrix(n_, b.m_, entries);
  }

  /**
   * rank。密評価と BBLA を自動選択する。BBLA は十分大きい体を仮定し、失敗時は過小評価。
   * n=行数,m=列数,s=非零数,d=min(n,m),t=trials。
   * 時間 O(min(n*m*d,t*d*(n+m+s)))、領域は密評価 O(n*m)、BBLA O(n+m)。
   */
  template <class I = ll, class Sample = decltype(bbla::random_sample_mint<S>) &>
  I rank(int trials = 2, Sample &&sample = bbla::random_sample_mint<S>) const
  {
    assert(trials > 0);
    if (n_ == 0 || m_ == 0 || nnz<u128>() == 0) return I(0);
    if (internal::prefer_dense_rank(n_, m_, u128(n_) * m_ + nnz<u128>(), nnz<u128>(), trials))
      return to_dense().template rank<I>();
    auto apply = [&](V &v) { v = *this * v; };
    auto transpose_apply = [&](V &v) { v = v * *this; };
    auto random = [&]() -> S { return sample(); };
    return I(bbla::rank<F>(n_, m_, apply, transpose_apply, trials, random));
  }

  /**
   * 正方行列の行列式。密評価と BBLA を自動選択する。BBLA は十分大きい体を仮定。
   * n=行数,s=非零数。密評価 O(n^3)、BBLA は1試行 O(n*(n+s)) 時間。
   * 領域は密評価 O(n^2)、BBLA O(n)。返り値は常に正しい。
   */
  template <class Sample = decltype(bbla::random_sample_mint<S>) &>
  S det(Sample &&sample = bbla::random_sample_mint<S>) const
  {
    assert(n_ == m_);
    if (n_ == 0) return F::e1();
    if (nnz<u128>() == 0) return F::e0();
    if (internal::prefer_dense_rank(n_, m_, u128(n_) * m_ + nnz<u128>(), nnz<u128>()))
      return get<2>(to_dense().template row_reduction<int>());
    auto apply = [&](V &v) { v = *this * v; };
    auto random = [&]() -> S { return sample(); };
    return bbla::det<F>(n_, apply, random);
  }

  /**
   * 解があれば一つ、なければ不整合の証明ベクトルを返す。BBLA を使い、十分大きい体を仮定。
   * n=行数,m=列数,s=非零数,d=min(n,m)。1試行 O(d*(n+m+s)) 時間、O(n+m) 領域。
   */
  template <class Sample = decltype(bbla::random_sample_mint<S>) &>
  bbla::LinearSolution<S> solve(const V &b, Sample &&sample = bbla::random_sample_mint<S>) const
  {
    auto apply = [&](V &v) { v = *this * v; };
    auto transpose_apply = [&](V &v) { v = v * *this; };
    auto random = [&]() -> S { return sample(); };
    return bbla::solve_general<F>(n_, m_, apply, transpose_apply, b, random);
  }
};
