#pragma once

#include "../flow/bipartite_matching_state.hpp"
#include "linear_matroid.hpp"

/** 行数は色 1 の頂点数、列数は色 0 の頂点数。各列の非零数は対応する頂点の異なる隣接先の数以下。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class Cost, bool is_erasable, class Color,
          class Sample = decltype(random_modint<typename F::S>) &, bool Sparse = false>
auto transversal_linear_representation(const GraphUndirected<Cost, is_erasable> &graph,
                                                   const vc<Color> &color, Sample &&sample = random_modint<typename F::S>)
{
  internal::BipartiteGraphIndex indexed(graph, color);
  const int n = indexed.left.size(), dimension = indexed.right.size();
  internal::LinearRepresentationBuilder<F, Sparse> columns(dimension, n);
  vc<int> seen(dimension, -1);
  for (int e = 0; e < n; e++) for (int v : indexed.adjacency[e])
  {
    if (seen[v] == e) continue;
    seen[v] = e;
    columns.set(e, v, sample());
  }
  return std::move(columns).finish();
}

/** 行数は right_vertices、列数は adjacency.size()。各列の非零数は対応する行の異なる隣接先の数以下。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &, class I = int, bool Sparse = false>
auto transversal_linear_representation(int right_vertices, const vvc<I> &adjacency,
                                                   Sample &&sample = random_modint<typename F::S>)
{
  assert(right_vertices >= 0);
  const int n = adjacency.size();
  internal::LinearRepresentationBuilder<F, Sparse> columns(right_vertices, n);
  vc<int> seen(right_vertices, -1);
  for (int e = 0; e < n; e++) for (auto value : adjacency[e])
  {
    assert(u128(value) < unsigned(right_vertices));
    const int v = int(value);
    if (seen[v] == e) continue;
    seen[v] = e;
    columns.set(e, v, sample());
  }
  return std::move(columns).finish();
}

/** 行数は right_vertices、列数は adjacency.size()。各列の非零数は対応する行の異なる隣接先の数以下。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &,
          class I = int, bool erasable = false, bool Sparse = false>
auto transversal_linear_representation(int right_vertices, const CSR<I, erasable> &adjacency,
                                                   Sample &&sample = random_modint<typename F::S>)
{
  assert(right_vertices >= 0);
  const int n = adjacency.size();
  internal::LinearRepresentationBuilder<F, Sparse> columns(right_vertices, n);
  vc<int> seen(right_vertices, -1);
  for (int e = 0; e < n; e++) for (auto value : adjacency[e])
  {
    assert(u128(value) < unsigned(right_vertices));
    const int v = int(value);
    if (seen[v] == e) continue;
    seen[v] = e;
    columns.set(e, v, sample());
  }
  return std::move(columns).finish();
}

/** 行数は right_vertices、列数は adjacency.size()。各列の非零数は対応する行の異なる隣接先の数以下。 */
template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &, class I = int, bool Sparse = false>
auto transversal_linear_representation(int right_vertices, initializer_list<vc<I>> adjacency,
                                                   Sample &&sample = random_modint<typename F::S>)
{
  return transversal_linear_representation<F, Sample, I, Sparse>(right_vertices, vvc<I>(adjacency),
                                               std::forward<Sample>(sample));
}

/**
 * @brief transversal matroid
 * @docs docs/matroid/transversal_matroid.md
 */
class TransversalMatroid
{
  BipartiteMatchingState matching;
  vc<bool> selected;

public:
  template <class Cost, bool is_erasable, class Color>
  TransversalMatroid(const GraphUndirected<Cost, is_erasable> &graph, const vc<Color> &color)
      : matching(graph, color), selected(matching.left_size()) {}

  template <class I = int>
  TransversalMatroid(int right_vertices, vvc<I> adjacency)
      : matching(right_vertices, adjacency), selected(adjacency.size()) {}

  CPP_DUMP_DEFINE_DATA(matching, selected);

  int size() const { return matching.left_size(); }
  int vertex(int e) const { return matching.left_vertex(e); }
  optional<int> element(int vertex) const { return matching.left_index(vertex); }

  /** 行数は右側の頂点数、列数は左側の頂点数。各列の非零数は対応する左頂点の異なる隣接先の数以下。 */
  template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &>
  Matrix<F> linear_representation(Sample &&sample = random_modint<typename F::S>) const
  {
    return transversal_linear_representation<F>(matching.right_size(), matching.adjacency(),
                                                std::forward<Sample>(sample));
  }

  /** 行数は右側の頂点数、列数は左側の頂点数。各列の非零数は対応する左頂点の異なる隣接先の数以下。 */
  template <class F = FieldAddSubMulDiv<modint998244353>, class Sample = decltype(random_modint<typename F::S>) &>
  SparseMatrix<F> sparse_linear_representation(Sample &&sample = random_modint<typename F::S>) const
  {
    return transversal_linear_representation<F, Sample, int, false, true>(matching.right_size(), matching.adjacency(),
                                                                         std::forward<Sample>(sample));
  }

  template <class I = int>
  void build(const vc<I> &s)
  {
    fill(ALL(selected), false);
    matching.clear();
    for (auto value : s)
    {
      assert(u128(value) < unsigned(size()));
      const int e = int(value);
      assert(!selected[e]);
      selected[e] = true;
      const bool added = matching.augment(e);
      assert(added);
      (void)added;
    }
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < size() && !selected[e]);
    auto result = matching.search<false>(e);
    if (result.free_right) return nullopt;
    result.left.erase(result.left.begin());
    return move(result.left);
  }
};
