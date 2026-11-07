#pragma once

#include "../flow/bipartite_matching_state.hpp"
#include "linear_matroid.hpp"

template <class F, class Cost, bool is_erasable, class Color, class Sample>
LinearMatroid<F> transversal_linear_representation(const GraphUndirected<Cost, is_erasable> &graph,
                                                   const vc<Color> &color, Sample &&sample)
{
  using S = typename F::S;
  internal::BipartiteGraphIndex indexed(graph, color);
  const int n = indexed.left.size(), dimension = indexed.right.size();
  vvc<S> columns(n, vc<S>(dimension, F::e0()));
  vc<int> seen(dimension, -1);
  for (int e = 0; e < n; e++) for (int v : indexed.adjacency[e])
  {
    if (seen[v] == e) continue;
    seen[v] = e;
    columns[e][v] = sample();
  }
  return LinearMatroid<F>(dimension, std::move(columns));
}

template <class F, class Sample>
LinearMatroid<F> transversal_linear_representation(int right_vertices, const vvc<int> &adjacency,
                                                   Sample &&sample)
{
  using S = typename F::S;
  assert(right_vertices >= 0);
  const int n = adjacency.size();
  vvc<S> columns(n, vc<S>(right_vertices, F::e0()));
  vc<int> seen(right_vertices, -1);
  for (int e = 0; e < n; e++) for (int v : adjacency[e])
  {
    assert(0 <= v && v < right_vertices);
    if (seen[v] == e) continue;
    seen[v] = e;
    columns[e][v] = sample();
  }
  return LinearMatroid<F>(right_vertices, std::move(columns));
}

template <class F, class Sample>
LinearMatroid<F> transversal_linear_representation(int right_vertices, const CSR<int> &adjacency,
                                                   Sample &&sample)
{
  using S = typename F::S;
  assert(right_vertices >= 0);
  const int n = adjacency.size();
  vvc<S> columns(n, vc<S>(right_vertices, F::e0()));
  vc<int> seen(right_vertices, -1);
  for (int e = 0; e < n; e++) for (int v : adjacency[e])
  {
    assert(0 <= v && v < right_vertices);
    if (seen[v] == e) continue;
    seen[v] = e;
    columns[e][v] = sample();
  }
  return LinearMatroid<F>(right_vertices, std::move(columns));
}

template <class F, class Sample>
LinearMatroid<F> transversal_linear_representation(int right_vertices, initializer_list<vc<int>> adjacency,
                                                   Sample &&sample)
{
  return transversal_linear_representation<F>(right_vertices, vvc<int>(adjacency),
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

  TransversalMatroid(int right_vertices, vvc<int> adjacency)
      : matching(right_vertices, adjacency), selected(adjacency.size()) {}

  int size() const { return matching.left_size(); }
  int vertex(int e) const { return matching.left_vertex(e); }
  optional<int> element(int vertex) const { return matching.left_index(vertex); }

  template <class F, class Sample>
  LinearMatroid<F> linear_representation(Sample &&sample) const
  {
    return transversal_linear_representation<F>(matching.right_size(), matching.adjacency(),
                                                std::forward<Sample>(sample));
  }

  void build(const vc<int> &s)
  {
    fill(ALL(selected), false);
    matching.clear();
    for (int e : s)
    {
      assert(0 <= e && e < size() && !selected[e]);
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
