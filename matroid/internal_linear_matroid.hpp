#pragma once

#include "linear_matroid.hpp"

namespace internal::linear_matroid_detail
{
template <class F>
vvc<typename F::S> compressed_columns(const LinearMatroid<F> &matroid)
{
  const int d = matroid.dimension(), n = matroid.size();
  Matrix<F> matrix(d, n);
  for (int e = 0; e < n; e++) for (int i = 0; i < d; i++)
    matrix[i][e] = matroid.columns()[e][i];
  auto [reduced, rank, determinant] = matrix.template row_reduction<int>();
  (void)determinant;
  vvc<typename F::S> result(n, vc<typename F::S>(rank));
  for (int e = 0; e < n; e++) for (int i = 0; i < rank; i++)
    result[e][i] = reduced[i][e];
  return result;
}

template <class F>
pair<vvc<typename F::S>, vc<pair<int, int>>>
parity_representation(const LinearMatroid<F> &matroid, const vc<pair<int, int>> &pairs)
{
  vc<unsigned char> used(matroid.size());
  for (auto [a, b] : pairs)
  {
    assert(0 <= a && a < matroid.size() && 0 <= b && b < matroid.size());
    assert(a != b && !used[a] && !used[b]);
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
