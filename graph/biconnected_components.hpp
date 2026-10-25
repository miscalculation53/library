#pragma once
#include <cassert>
#include "lowlink.hpp"

/*! SPDX-License-Identifier: CC0-1.0 */

/**
 * @brief 二重連結成分分解
 * @docs docs/graph/biconnected_components.md
 */

class BiconnectedComponents {
    friend class BlockCutTree;

    int vertex_count_ = 0;
    CSR<int> vertex_groups_, edge_groups_;

    vc<int> build_vertices(const LowLink& precomputed) {
        vertex_count_ = precomputed.size<int>();
        edge_groups_ = CSR<int>();
        vc<int> membership(vertex_count_, -1), row_sizes, anchors;
        row_sizes.reserve(vertex_count_);
        anchors.reserve(vertex_count_);
        const auto& traversal = precomputed.dfs_order();
        for (int i = 0; i < vertex_count_; ++i) {
            const int v = traversal[i];
            const auto p = precomputed.parent(v);
            if (!p) {
                if (i + 1 == vertex_count_ ||
                    !precomputed.parent(traversal[i + 1])) {
                    row_sizes.push_back(1);
                    anchors.push_back(v);
                }
                continue;
            }
            if (precomputed.low(v) >= precomputed.order(p->first)) {
                membership[v] = static_cast<int>(row_sizes.size());
                row_sizes.push_back(1);
                anchors.push_back(p->first);
            } else {
                membership[v] = membership[p->first];
            }
            ++row_sizes[membership[v]];
        }
        vertex_groups_ = CSR<int>(row_sizes);
        auto& values = vertex_groups_.get_elist();
        for (int b = 0; b < static_cast<int>(row_sizes.size()); ++b) {
            row_sizes[b] = vertex_groups_.offset(b);
            values[row_sizes[b]++] = anchors[b];
        }
        for (int v = 0; v < vertex_count_; ++v) {
            if (membership[v] >= 0) {
                values[row_sizes[membership[v]]++] = v;
            }
        }
        return membership;
    }

    template <class Cost, bool is_erasable>
    void build(const GraphUndirected<Cost, is_erasable>& g) {
        vertex_count_ = g.template size<int>();
        const int m = g.template num_of_edges<int>();
        vc<int> order(vertex_count_, -1), parent_edge(vertex_count_);
        struct Frame {
            int vertex, next, low, back_begin;
        };
        vc<Frame> path;
        vc<int> pending_vertices, back_edges;
        vc<int> vertex_values, edge_values, vertex_sizes, edge_sizes;
        path.reserve(vertex_count_);
        pending_vertices.reserve(vertex_count_);
        vertex_values.reserve(static_cast<std::size_t>(vertex_count_) * 2);
        edge_values.reserve(m);
        vertex_sizes.reserve(vertex_count_);
        edge_sizes.reserve(vertex_count_);
        int timer = 0;
        for (int root = 0; root < vertex_count_; ++root) {
            if (order[root] >= 0) continue;
            order[root] = timer++;
            parent_edge[root] = -1;
            pending_vertices.push_back(root);
            path.push_back({root, 0, order[root],
                            static_cast<int>(back_edges.size())});
            while (!path.empty()) {
                Frame& frame = path.back();
                const int v = frame.vertex;
                const auto& row = g.out_edges(v);
                if (frame.next < static_cast<int>(row.size())) {
                    const auto& edge = row[frame.next++];
                    const int to = edge.to, id = edge.index;
                    if (id == parent_edge[v]) continue;
                    if (order[to] < 0) {
                        order[to] = timer++;
                        parent_edge[to] = id;
                        pending_vertices.push_back(to);
                        path.push_back({to, 0, order[to],
                                        static_cast<int>(back_edges.size())});
                    } else if (order[to] < order[v]) {
                        back_edges.push_back(id);
                        if (order[to] < frame.low) frame.low = order[to];
                    }
                    continue;
                }
                const int child_low = frame.low;
                const int back_begin = frame.back_begin;
                path.pop_back();
                if (path.empty()) {
                    pending_vertices.pop_back();
                    if (timer == order[v] + 1) {
                        vertex_values.push_back(v);
                        vertex_sizes.push_back(1);
                        edge_sizes.push_back(0);
                    }
                    continue;
                }
                Frame& parent = path.back();
                if (child_low >= order[parent.vertex]) {
                    int vertices = 1, edges = 0;
                    vertex_values.push_back(parent.vertex);
                    int x;
                    do {
                        x = pending_vertices.back();
                        pending_vertices.pop_back();
                        vertex_values.push_back(x);
                        edge_values.push_back(parent_edge[x]);
                        ++vertices;
                        ++edges;
                    } while (x != v);
                    for (int i = back_begin;
                         i < static_cast<int>(back_edges.size()); ++i) {
                        edge_values.push_back(back_edges[i]);
                        ++edges;
                    }
                    back_edges.resize(back_begin);
                    vertex_sizes.push_back(vertices);
                    edge_sizes.push_back(edges);
                } else if (child_low < parent.low) {
                    parent.low = child_low;
                }
            }
        }
        vertex_groups_ = CSR<int>(vertex_sizes);
        edge_groups_ = CSR<int>(edge_sizes);
        vertex_groups_.get_elist().swap(vertex_values);
        edge_groups_.get_elist().swap(edge_values);
    }

public:
    BiconnectedComponents() = default;

    template <class Cost, bool is_erasable>
    explicit BiconnectedComponents(
        const GraphUndirected<Cost, is_erasable>& g) {
        build(g);
    }

    template <class Cost, bool is_erasable>
    BiconnectedComponents(const GraphUndirected<Cost, is_erasable>& g,
                         const LowLink& precomputed) {
        assert(g.template size<int>() == precomputed.size<int>());
        const vc<int> membership = build_vertices(precomputed);
        const int blocks = vertex_groups_.template size<int>();
        vc<int> row_sizes(blocks);
        int tree_edges = 0;
        for (int b = 0; b < blocks; ++b) {
            row_sizes[b] = static_cast<int>(vertex_groups_[b].size()) - 1;
            tree_edges += row_sizes[b];
        }
        const int m = g.template num_of_edges<int>();
        vc<std::pair<int, int>> additional;
        additional.reserve(m - tree_edges);
        if (m != tree_edges) {
            for (int v = 0; v < vertex_count_; ++v) {
                for (const auto& edge : g.out_edges(v)) {
                    if (precomputed.order(v) >= precomputed.order(edge.to)) {
                        continue;
                    }
                    if (edge.index == precomputed.parent(edge.to)->second) {
                        continue;
                    }
                    const int b = membership[edge.to];
                    ++row_sizes[b];
                    additional.emplace_back(b, edge.index);
                }
            }
        }
        edge_groups_ = CSR<int>(row_sizes);
        for (int b = 0; b < blocks; ++b) {
            row_sizes[b] = edge_groups_.offset(b);
        }
        auto& values = edge_groups_.get_elist();
        for (int v = 0; v < vertex_count_; ++v) {
            if (membership[v] >= 0) {
                values[row_sizes[membership[v]]++] =
                    precomputed.parent(v)->second;
            }
        }
        for (const auto& item : additional) {
            values[row_sizes[item.first]++] = item.second;
        }
    }

    template <class I = ll> I size() const {
        return vertex_groups_.template size<I>();
    }
    template <class I = ll> I num_of_vertices() const {
        return static_cast<I>(vertex_count_);
    }
    auto vertices(int block) const {
        assert(0 <= block && block < size<int>());
        return vertex_groups_[block];
    }
    auto edges(int block) const {
        assert(0 <= block && block < size<int>());
        return edge_groups_[block];
    }
    const CSR<int>& vertex_groups() const { return vertex_groups_; }
    const CSR<int>& edge_groups() const { return edge_groups_; }
};
