#pragma once

#include "linear_matroid.hpp"

namespace internal::linear_matroid_detail
{
template <class F, int BS>
int element_count(const Matrix<F, BS> &matrix)
{
  return matrix.template shape<int>().second;
}

template <class F>
int element_count(const LinearMatroid<F> &matroid)
{
  return matroid.size();
}

template <class F, int BS>
vvc<typename F::S> compressed_columns(const Matrix<F, BS> &matrix)
{
  const int n = element_count(matrix);
  auto [reduced, rank, determinant] = matrix.template row_reduction<int>();
  (void)determinant;
  vvc<typename F::S> result(n, vc<typename F::S>(rank));
  for (int e = 0; e < n; e++) for (int i = 0; i < rank; i++)
    result[e][i] = reduced[i][e];
  return result;
}

template <class F>
vvc<typename F::S> compressed_columns(const LinearMatroid<F> &matroid)
{
  return compressed_columns(matroid.matrix());
}

template <class F, class M, class I, class J>
pair<vvc<typename F::S>, vc<pair<int, int>>>
parity_representation(const M &matroid, const vc<pair<I, J>> &pairs)
{
  const int n = element_count(matroid);
  vc<unsigned char> used(n);
  for (auto [a, b] : pairs)
  {
    assert(u128(a) < unsigned(n) && u128(b) < unsigned(n));
    assert(u128(a) != u128(b) && !used[a] && !used[b]);
    used[a] = used[b] = 1;
  }
  auto compressed = compressed_columns(matroid);
  const int m = int(pairs.size());
  vvc<typename F::S> columns;
  vc<pair<int, int>> work_pairs;
  columns.reserve(2 * m);
  work_pairs.reserve(m);
  for (int e = 0; e < m; e++)
  {
    auto [a, b] = pairs[e];
    columns.push_back(std::move(compressed[a]));
    columns.push_back(std::move(compressed[b]));
    work_pairs.emplace_back(2 * e, 2 * e + 1);
  }
  return {std::move(columns), std::move(work_pairs)};
}

inline vc<pair<int, int>> consecutive_pairs(int size)
{
  assert(size % 2 == 0);
  vc<pair<int, int>> pairs;
  pairs.reserve(size / 2);
  for (int e = 0; e < size; e += 2) pairs.emplace_back(e, e + 1);
  return pairs;
}

template <class F>
bool contract(vvc<typename F::S> &columns, int e)
{
  using S = typename F::S;
  const int d = int(columns[e].size());
  int pivot = 0;
  while (pivot < d && columns[e][pivot] == F::e0()) pivot++;
  if (pivot == d) return false;
  vc<S> direction = columns[e];
  S inverse = F::inv(direction[pivot]);
  for (int i = 0; i < d; i++) direction[i] = F::mul(direction[i], inverse);
  for (auto &column : columns)
  {
    S factor = column[pivot];
    for (int i = 0; i < d; i++)
      column[i] = F::add(column[i], F::minus(F::mul(factor, direction[i])));
    column[pivot] = column.back();
    column.pop_back();
  }
  return true;
}

template <class F, class Sample>
int intersection_rank(const vvc<typename F::S> &a, const vvc<typename F::S> &b,
                      int begin, Sample &sample)
{
  const int n = int(a.size());
  if (begin == n || a[0].empty() || b[0].empty()) return 0;
  const int da = int(a[0].size()), db = int(b[0].size());
  Matrix<F> matrix(da, db);
  for (int e = begin; e < n; e++)
  {
    auto x = sample();
    for (int i = 0; i < da; i++)
    {
      auto coefficient = F::mul(x, a[e][i]);
      if (coefficient == F::e0()) continue;
      for (int j = 0; j < db; j++)
        matrix[i][j] = F::add(matrix[i][j], F::mul(coefficient, b[e][j]));
    }
  }
  return matrix.template rank<int>();
}

template <class F, class Sample>
int parity_rank(const vvc<typename F::S> &columns, const vc<pair<int, int>> &pairs,
                int begin, Sample &sample)
{
  const int m = int(pairs.size());
  if (begin == m || columns.empty() || columns[0].empty()) return 0;
  const int d = int(columns[0].size());
  Matrix<F> matrix(d, d);
  for (int e = begin; e < m; e++)
  {
    auto [a, b] = pairs[e];
    auto x = sample();
    for (int i = 0; i < d; i++) for (int j = i + 1; j < d; j++)
    {
      auto value = F::mul(x, F::add(F::mul(columns[a][i], columns[b][j]),
                                    F::minus(F::mul(columns[b][i], columns[a][j]))));
      matrix[i][j] = F::add(matrix[i][j], value);
    }
  }
  for (int i = 0; i < d; i++) for (int j = i + 1; j < d; j++)
    matrix[j][i] = F::minus(matrix[i][j]);
  int rank = matrix.template rank<int>();
  assert(rank % 2 == 0);
  return rank / 2;
}
}

namespace internal::linear_matroid_detail
{
template <class F, class Sample>
int sparse_intersection_rank(const SparseMatrix<F> &a, const SparseMatrix<F> &b,
                             const vc<unsigned char> &active, Sample &sample)
{
  using S = typename F::S;
  const auto sa = a.template shape<int>(), sb = b.template shape<int>();
  const int da = sa.first, db = sb.first, n = sa.second, nb = sb.second;
  assert(n == nb && int(active.size()) == n);
  if (!da || !db || !n) return 0;
  vc<S> diagonal(n, F::e0());
  for (int e = 0; e < n; e++) if (active[e]) diagonal[e] = sample();
  auto apply = [&](vc<S> &v)
  {
    v = v * b;
    for (int e = 0; e < n; e++) v[e] = F::mul(v[e], diagonal[e]);
    v = a * v;
  };
  auto transpose = [&](vc<S> &v)
  {
    v = v * a;
    for (int e = 0; e < n; e++) v[e] = F::mul(v[e], diagonal[e]);
    v = b * v;
  };
  auto random = [&]() -> S { return sample(); };
  return bbla::rank<F>(da, db, apply, transpose, 1, random);
}

template <class I, class J>
vc<pair<int, int>> sparse_parity_pairs(int size, const vc<pair<I, J>> &pairs)
{
  static_assert(is_integral_ext<I> && is_integral_ext<J>);
  vc<unsigned char> used(size);
  vc<pair<int, int>> result;
  result.reserve(pairs.size());
  for (auto [a, b] : pairs)
  {
    assert(u128(a) < unsigned(size) && u128(b) < unsigned(size));
    assert(u128(a) != u128(b) && !used[int(a)] && !used[int(b)]);
    used[int(a)] = used[int(b)] = 1;
    result.emplace_back(int(a), int(b));
  }
  return result;
}

template <class F, class Sample>
int sparse_parity_rank(const SparseMatrix<F> &matrix, const vc<pair<int, int>> &pairs,
                       const vc<unsigned char> &active, Sample &sample)
{
  using S = typename F::S;
  const auto shape = matrix.template shape<int>();
  const int d = shape.first, n = shape.second, m = pairs.size();
  if (!d || !m) return 0;
  vc<S> diagonal(m, F::e0());
  for (int e = 0; e < m; e++) if (active[e]) diagonal[e] = sample();
  auto apply = [&](vc<S> &v)
  {
    auto middle = v * matrix;
    vc<S> twisted(n, F::e0());
    for (int e = 0; e < m; e++)
    {
      const auto [a, b] = pairs[e];
      twisted[a] = F::mul(diagonal[e], middle[b]);
      twisted[b] = F::minus(F::mul(diagonal[e], middle[a]));
    }
    v = matrix * twisted;
  };
  auto transpose = [&](vc<S> &v)
  {
    apply(v);
    for (auto &x : v) x = F::minus(x);
  };
  auto random = [&]() -> S { return sample(); };
  return bbla::rank<F>(d, d, apply, transpose, 1, random) / 2;
}

template <class F>
optional<SparseMatrix<F>> compact_nonzero_rows(const SparseMatrix<F> &matrix)
{
  const int d = matrix.template shape<int>().first;
  vc<int> indices;
  for (int i = 0; i < d; i++) if (!matrix[i].empty()) indices.push_back(i);
  if (int(indices.size()) == d) return nullopt;
  return matrix.select_rows(indices);
}

template <class F>
class SparseIntersectionRank
{
  using S = typename F::S;
  const SparseMatrix<F> &original_a_, &original_b_;
  optional<SparseMatrix<F>> compact_a_, compact_b_, transpose_b_;

  const SparseMatrix<F> &a() const { return compact_a_ ? *compact_a_ : original_a_; }
  const SparseMatrix<F> &b() const { return compact_b_ ? *compact_b_ : original_b_; }

public:
  bool dense;

  SparseIntersectionRank(const SparseMatrix<F> &first, const SparseMatrix<F> &second)
      : original_a_(first), original_b_(second),
        compact_a_(compact_nonzero_rows(first)), compact_b_(compact_nonzero_rows(second))
  {
    const auto sa = a().template shape<int>(), sb = b().template shape<int>();
    const int n = sa.second;
    assert(n == sb.second);
    vc<int> ca(n), cb(n);
    for (const auto &[e, x] : a().rows().get_elist()) ca[e]++;
    for (const auto &[e, x] : b().rows().get_elist()) cb[e]++;
    u128 products = 0;
    for (int e = 0; e < n; e++) products += u128(ca[e]) * cb[e];
    const u128 work = u128(n) + a().template nnz<u128>() + b().template nnz<u128>();
    dense = internal::prefer_dense_rank(sa.first, sb.first,
                                         u128(sa.first) + sb.first + work + products, work);
    if (dense) transpose_b_.emplace(b().transpose());
  }

  template <class Sample>
  int operator()(const vc<unsigned char> &active, Sample &sample) const
  {
    const auto sa = a().template shape<int>(), sb = b().template shape<int>();
    if (!sa.first || !sb.first || !sa.second) return 0;
    if (!dense) return sparse_intersection_rank(a(), b(), active, sample);
    vc<S> diagonal(sa.second, F::e0());
    for (int e = 0; e < sa.second; e++) if (active[e]) diagonal[e] = sample();
    auto weighted = *transpose_b_;
    weighted.scale_rows(diagonal);
    return a().multiply_dense(weighted).template rank<int>();
  }
};

template <class F>
class SparseParityRank
{
  using S = typename F::S;
  const SparseMatrix<F> &original_;
  const vc<pair<int, int>> &pairs_;
  optional<SparseMatrix<F>> compact_, transpose_;
  u128 products_ = 0;

  const SparseMatrix<F> &matrix() const { return compact_ ? *compact_ : original_; }

public:
  bool dense;

  SparseParityRank(const SparseMatrix<F> &representation, const vc<pair<int, int>> &pairs)
      : original_(representation), pairs_(pairs), compact_(compact_nonzero_rows(representation))
  {
    const auto shape = matrix().template shape<int>();
    vc<int> count(shape.second);
    for (const auto &[e, x] : matrix().rows().get_elist()) count[e]++;
    for (auto [a, b] : pairs_) products_ += u128(count[a]) * count[b];
    const u128 work = u128(shape.second) + pairs_.size() + matrix().template nnz<u128>();
    dense = internal::prefer_dense_rank(shape.first, shape.first,
                                         shape.first + work + 2 * products_, work);
    if (dense) transpose_.emplace(matrix().transpose());
  }

  template <class Sample>
  int operator()(const vc<unsigned char> &active, Sample &sample) const
  {
    const int d = matrix().template shape<int>().first;
    if (!d || products_ == 0) return 0;
    if (!dense) return sparse_parity_rank(matrix(), pairs_, active, sample);
    const int n = matrix().template shape<int>().second;
    vc<tuple<int, int, S>> entries;
    entries.reserve(2 * pairs_.size());
    for (int e = 0; e < int(pairs_.size()); e++) if (active[e])
    {
      const auto [a, b] = pairs_[e];
      S x = sample();
      entries.emplace_back(a, b, x);
      entries.emplace_back(b, a, F::minus(x));
    }
    const SparseMatrix<F> alternating(n, n, entries);
    int rank = matrix().multiply_dense(alternating * *transpose_).template rank<int>();
    assert(rank % 2 == 0);
    return rank / 2;
  }
};

template <class Rank>
vc<int> recover_by_deletion(int size, Rank &&rank, int trials)
{
  vc<int> best;
  for (int trial = 0; trial < trials; trial++)
  {
    vc<unsigned char> active(size, 1);
    const int target = rank(active);
    if (target <= int(best.size())) continue;
    int count = size;
    auto erase = [&](auto &&self, int left, int right) -> void
    {
      if (count == target) return;
      if (count - (right - left) >= target)
      {
        fill(active.begin() + left, active.begin() + right, 0);
        if (rank(active) >= target)
        {
          count -= right - left;
          return;
        }
        fill(active.begin() + left, active.begin() + right, 1);
      }
      if (right - left <= 1) return;
      int middle = (left + right) / 2;
      self(self, left, middle);
      self(self, middle, right);
    };
    erase(erase, 0, size);
    if (count == target)
    {
      best.clear();
      for (int e = 0; e < size; e++) if (active[e]) best.push_back(e);
    }
  }
  return best;
}
}
