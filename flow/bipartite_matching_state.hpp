#pragma once

#include "../graph/graph.hpp"
#include "internal/bipartite_graph_index.hpp"

/**
 * @brief 状態を保持する二部マッチングと交互路探索
 * @docs docs/flow/bipartite_matching_state.md
 */

class BipartiteMatchingState
{
public:
  struct SearchResult
  {
    vc<int> left, right, parent_right;
    optional<int> free_right;
  };

private:
  int right_count = 0, matched = 0;
  CSR<int> adjacency_;
  vc<int> left_mate_, right_mate_;
  vc<int> left_vertices_, right_vertices_, vertex_index_;

  explicit BipartiteMatchingState(internal::BipartiteGraphIndex indexed)
      : right_count(indexed.right.size()), adjacency_(move(indexed.adjacency)),
        left_mate_(indexed.left.size(), -1), right_mate_(right_count, -1),
        left_vertices_(move(indexed.left)), right_vertices_(move(indexed.right)),
        vertex_index_(move(indexed.index)) {}

public:
  template <class Cost, bool is_erasable, class Color>
  BipartiteMatchingState(const GraphUndirected<Cost, is_erasable> &graph, const vc<Color> &color)
      : BipartiteMatchingState(internal::BipartiteGraphIndex(graph, color)) {}

  BipartiteMatchingState(int right_vertices, CSR<int> adjacency)
      : right_count(right_vertices), adjacency_(move(adjacency)),
        left_mate_(adjacency_.template size<int>(), -1), right_mate_(right_vertices, -1)
  {
    assert(right_vertices >= 0);
    for (int v : adjacency_.get_elist()) assert(0 <= v && v < right_count);
  }
  template <class I = int>
  BipartiteMatchingState(int right_vertices, const vvc<I> &adjacency)
      : BipartiteMatchingState(right_vertices, CSR<int>(adjacency))
  {
    for (const auto &row : adjacency) for (auto v : row)
      assert(u128(v) < unsigned(right_vertices));
  }
  template <class I = int>
  BipartiteMatchingState(int right_vertices, initializer_list<vc<I>> adjacency)
      : BipartiteMatchingState(right_vertices, vvc<I>(adjacency)) {}
  template <class I, bool erasable>
  BipartiteMatchingState(int right_vertices, const CSR<I, erasable> &adjacency)
      : BipartiteMatchingState(right_vertices, CSR<int>(adjacency))
  {
    for (int u = 0; u < adjacency.template size<int>(); u++) for (auto v : adjacency[u])
      assert(u128(v) < unsigned(right_vertices));
  }

  int left_size() const { return adjacency_.template size<int>(); }
  int right_size() const { return right_count; }
  int matching_size() const { return matched; }
  const CSR<int> &adjacency() const { return adjacency_; }
  const vc<int> &left_mate() const { return left_mate_; }
  const vc<int> &right_mate() const { return right_mate_; }

  int left_vertex(int u) const
  {
    assert(0 <= u && u < left_size());
    return left_vertices_.empty() ? u : left_vertices_[u];
  }
  int right_vertex(int v) const
  {
    assert(0 <= v && v < right_size());
    return right_vertices_.empty() ? v : right_vertices_[v];
  }
  optional<int> left_index(int vertex) const
  {
    if (vertex_index_.empty())
      return 0 <= vertex && vertex < left_size() ? optional<int>(vertex) : nullopt;
    if (vertex < 0 || vertex >= int(vertex_index_.size())) return nullopt;
    const int u = vertex_index_[vertex];
    return u < left_size() && left_vertices_[u] == vertex ? optional<int>(u) : nullopt;
  }
  optional<int> right_index(int vertex) const
  {
    if (vertex_index_.empty())
      return 0 <= vertex && vertex < right_size() ? optional<int>(vertex) : nullopt;
    if (vertex < 0 || vertex >= int(vertex_index_.size())) return nullopt;
    const int v = vertex_index_[vertex];
    return v < right_size() && right_vertices_[v] == vertex ? optional<int>(v) : nullopt;
  }
  vc<pair<int, int>> matching() const
  {
    vc<pair<int, int>> result;
    result.reserve(matched);
    for (int u = 0; u < left_size(); u++) if (left_mate_[u] != -1)
      result.emplace_back(left_vertex(u), right_vertex(left_mate_[u]));
    return result;
  }

  void clear()
  {
    fill(ALL(left_mate_), -1), fill(ALL(right_mate_), -1);
    matched = 0;
  }

  template <bool CollectRight = true>
  SearchResult search(int source) const
  {
    assert(0 <= source && source < left_size() && left_mate_[source] == -1);
    SearchResult result;
    result.parent_right.assign(right_count, -1);
    vc<bool> seen_left(left_size());
    seen_left[source] = true;
    result.left.eb(source);
    for (int head = 0; head < int(result.left.size()); head++)
    {
      const int u = result.left[head];
      for (int v : adjacency_[u]) if (result.parent_right[v] == -1)
      {
        result.parent_right[v] = u;
        if constexpr (CollectRight) result.right.eb(v);
        const int next = right_mate_[v];
        if (next == -1)
        {
          result.free_right = v;
          return result;
        }
        if (!seen_left[next]) seen_left[next] = true, result.left.eb(next);
      }
    }
    return result;
  }

  bool augment(int source)
  {
    auto result = search<false>(source);
    if (!result.free_right) return false;
    for (int v = *result.free_right; v != -1;)
    {
      const int u = result.parent_right[v], previous = left_mate_[u];
      left_mate_[u] = v, right_mate_[v] = u;
      v = previous;
    }
    matched++;
    return true;
  }

  int unmatch(int u)
  {
    assert(0 <= u && u < left_size());
    const int v = left_mate_[u];
    if (v == -1) return 0;
    left_mate_[u] = right_mate_[v] = -1;
    matched--;
    return 1;
  }

  int maximize()
  {
    for (int u = 0; u < left_size(); u++) if (left_mate_[u] == -1) augment(u);
    return matched;
  }

  GraphDirected<> alternating_graph() const
  {
    vc<pair<int, int>> edges;
    edges.reserve(adjacency_.get_elist().size());
    for (int u = 0; u < left_size(); u++) for (int v : adjacency_[u])
    {
      const int rv = left_size() + v;
      edges.emplace_back(u, rv);
    }
    for (int u = 0; u < left_size(); u++) if (left_mate_[u] != -1)
      edges.emplace_back(left_size() + left_mate_[u], u);
    return GraphDirected<>(left_size() + right_count, edges);
  }
};
