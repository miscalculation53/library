#pragma once

#include "graph.hpp"

/**
 * @brief LowLink・橋・関節点
 * @docs docs/graph/lowlink.md
 */

struct LowLink
{
private:
  friend struct BiconnectedComponents;
  vc<int> ord_, low_, parent_, parent_edge_, dfs_order_, articulations_, bridges_;
  vc<unsigned char> articulation_;

  template <class Cost, bool is_erasable, class CountLowerEdges>
  void build(const GraphUndirected<Cost, is_erasable> &g, CountLowerEdges count_lower_edges)
  {
    const int n = g.template size<int>();
    ord_.assign(n, -1), low_.resize(n);
    parent_.assign(n, -1), parent_edge_.assign(n, -1);
    articulation_.assign(n, 0);
    dfs_order_.clear(), articulations_.clear(), bridges_.clear();
    dfs_order_.reserve(n);
    vc<int> cursor(n);

    for (int root = 0; root < n; ++root) if (ord_[root] < 0)
    {
      int root_children = 0, v = root;
      ord_[v] = low_[v] = dfs_order_.size();
      dfs_order_.push_back(v);
      while (v >= 0)
      {
        const auto row = g.out_edges(v);
        const int end = row.size(), pe = parent_edge_[v], discovery = ord_[v];
        // 隣接辺の走査中は局所変数を使い、配列への書き戻しをまとめる。
        int at = cursor[v], best = low_[v], lower_degree = 0;
        int child = -1, edge = -1;
        while (at < end)
        {
          const auto &e = row[at++];
          const int to_order = ord_[e.to];
          if (to_order < 0)
          {
            // 葉は親辺だけで探索が完了するので、その場で結果を確定する。
            if (g.out_edges(e.to).size() == 1)
            {
              const int leaf = e.to;
              parent_[leaf] = v, parent_edge_[leaf] = e.index;
              ord_[leaf] = low_[leaf] = dfs_order_.size();
              dfs_order_.push_back(leaf);
              root_children += v == root;
              if (v != root) articulation_[v] = 1;
              bridges_.push_back(e.index);
              count_lower_edges(leaf, 1);
              continue;
            }
            child = e.to, edge = e.index;
            break;
          }
          lower_degree += to_order < discovery;
          if (e.index != pe) best = min(best, to_order);
        }
        cursor[v] = at;
        if (best < low_[v]) low_[v] = best;
        count_lower_edges(v, lower_degree);
        if (child >= 0)
        {
          root_children += v == root;
          parent_[child] = v, parent_edge_[child] = edge;
          ord_[child] = low_[child] = dfs_order_.size();
          dfs_order_.push_back(child);
          v = child;
        }
        else
        {
          const int p = parent_[v];
          if (p < 0) articulation_[v] = root_children >= 2;
          else
          {
            if (best < low_[p]) low_[p] = best;
            if (p != root && best >= ord_[p]) articulation_[p] = 1;
            if (best > ord_[p]) bridges_.push_back(pe);
          }
          // 親配列そのものを探索スタックとして使う。
          v = p;
        }
      }
    }
    for (int v = 0; v < n; ++v) if (articulation_[v]) articulations_.push_back(v);
  }

public:
  LowLink() = default;
  template <class Cost, bool is_erasable>
  explicit LowLink(const GraphUndirected<Cost, is_erasable> &g)
  {
    build(g, [](int, int) {});
  }

  template <class I = ll>
  I size() const { return ord_.size(); }
  int order(int v) const
  {
    assert(0 <= v && v < size());
    return ord_[v];
  }
  int low(int v) const
  {
    assert(0 <= v && v < size());
    return low_[v];
  }
  optional<pair<int, int>> parent(int v) const
  {
    assert(0 <= v && v < size());
    if (parent_[v] < 0) return nullopt;
    return pair<int, int>(parent_[v], parent_edge_[v]);
  }
  const vc<int> &dfs_order() const { return dfs_order_; }
  const vc<int> &articulations() const { return articulations_; }
  const vc<int> &bridges() const { return bridges_; }
  bool is_articulation(int v) const
  {
    assert(0 <= v && v < size());
    return articulation_[v] != 0;
  }
  bool is_bridge(int u, int v) const
  {
    assert(0 <= u && u < size() && 0 <= v && v < size());
    if (parent_[v] == u) return low_[v] > ord_[u];
    if (parent_[u] == v) return low_[u] > ord_[v];
    return false;
  }
  template <class Cost>
  bool is_bridge(const Edge<Cost> &e) const { return is_bridge(e.from, e.to); }
};
