#pragma once

#include "../graph/biconnected_components.hpp"

/**
 * @brief Block-Cut Tree
 * @docs docs/tree/block_cut_tree.md
 */

struct BlockCutTree
{
private:
  int blocks_ = 0;
  GraphUndirected<> forest_;
  vc<int> vertex_ids_, articulations_;

  void build(const BiconnectedComponents &bcc)
  {
    blocks_ = bcc.size<int>();
    const int n = bcc.num_of_vertices<int>();
    vertex_ids_.assign(n, -1);
    articulations_.clear();
    vc<unsigned char> shared(n);
    int repeated = 0;
    for (int b = 0; b < blocks_; ++b) for (int v : bcc.vertices(b))
    {
      if (vertex_ids_[v] < 0) vertex_ids_[v] = b;
      else shared[v] = 1, ++repeated;
    }
    for (int v = 0; v < n; ++v) if (shared[v])
    {
      vertex_ids_[v] = blocks_ + articulations_.size();
      articulations_.push_back(v);
    }
    vc<pair<int, int>> incidence;
    incidence.reserve(repeated + articulations_.size());
    if (!articulations_.empty())
      for (int b = 0; b < blocks_; ++b) for (int v : bcc.vertices(b))
        if (shared[v]) incidence.emplace_back(b, vertex_ids_[v]);
    forest_ = GraphUndirected<>(blocks_ + articulations_.size(), incidence);
  }

public:
  BlockCutTree() = default;
  explicit BlockCutTree(const BiconnectedComponents &bcc) { build(bcc); }
  template <class Cost, bool is_erasable>
  explicit BlockCutTree(const GraphUndirected<Cost, is_erasable> &g)
  {
    BiconnectedComponents blocks;
    blocks.build_vertices(LowLink(g));
    build(blocks);
  }

  template <class I = ll>
  I size() const { return forest_.template size<I>(); }
  template <class I = ll>
  I num_of_blocks() const { return blocks_; }
  const GraphUndirected<> &forest() const { return forest_; }
  int node_id(int v) const
  {
    assert(0 <= v && v < SZ(vertex_ids_));
    return vertex_ids_[v];
  }
  const vc<int> &vertex_ids() const { return vertex_ids_; }
  bool is_articulation_node(int node) const
  {
    assert(0 <= node && node < size());
    return node >= blocks_;
  }
  optional<int> articulation_vertex(int node) const
  {
    assert(0 <= node && node < size());
    if (node >= blocks_) return articulations_[node - blocks_];
    return nullopt;
  }
};
